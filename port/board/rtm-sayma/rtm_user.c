/*
 *   openMMC -- Open Source modular IPM Controller firmware
 *
 *   Copyright (C) 2015-2016  Henrique Silva <henrique.silva@lnls.br>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 */

/* Project includes */
#include "port.h"
#include "rtm.h"
#include "pcf8574.h"
#include "pin_mapping.h"
#include "hotswap.h"
#include "i2c_mapping.h"
#include "fru.h"
#include "utils.h"
#include "led.h"
#include "uart_debug.h"

extern uint8_t rtm_pm_enabled;

/* RTM Management functions */
void rtm_enable_payload_power( void )
{
	printf("RTM PWR Enable\n");

	if (!gpio_read_pin( PIN_PORT(GPIO_RTM_PS), PIN_NUMBER(GPIO_RTM_PS) ))
	{
		pcf8574_set_port_high( 1 << RTM_GPIO_LED_BLUE );
		pcf8574_set_port_low( 1 << RTM_GPIO_LED_RED );
		pcf8574_set_port_low( 1 << RTM_GPIO_LED_GREEN );

		gpio_set_pin_state( PIN_PORT(GPIO_EN_RTM_PWR), PIN_NUMBER(GPIO_EN_RTM_PWR), 1 );

		rtm_enable_dcdc();
	}
}

void rtm_disable_payload_power( void )
{
	printf("RTM PWR Disable\n");

	if (!gpio_read_pin( PIN_PORT(GPIO_RTM_PS), PIN_NUMBER(GPIO_RTM_PS) ))
	{
		rtm_quiesce();

		gpio_set_pin_state( PIN_PORT(GPIO_EN_RTM_PWR), PIN_NUMBER(GPIO_EN_RTM_PWR), 0 );

		pcf8574_set_port_low( 1 << RTM_GPIO_LED_BLUE );
		pcf8574_set_port_high( 1 << RTM_GPIO_LED_RED );
		pcf8574_set_port_high( 1 << RTM_GPIO_LED_GREEN );
	}
}

void rtm_enable_dcdc( void )
{
	uint8_t i2c_addr, i2c_id;
	uint8_t state = pcf8574_read_port();

	if ( state == PCF8574_READ_ERROR ) {
		printf("pcf read error");
		return;
	}

	uint8_t data = state & (~(1 << RTM_GPIO_EN_DC_DC)) ;

	if( i2c_take_by_chipid( CHIP_ID_RTM_PCF8574A, &i2c_addr, &i2c_id, (TickType_t) portMAX_DELAY) ) {
		xI2CMasterWrite(i2c_id, i2c_addr, &data, sizeof(data));
		i2c_give(i2c_id);
	}

	state = pcf8574_read_port();
	printf("DCDC en %d\n", state);
}

uint8_t rtm_get_hotswap_handle_status( uint8_t *state )
{
	uint8_t result = pcf8574_read_pin( RTM_GPIO_HOTSWAP_HANDLE );

	if (result == PCF8574_READ_ERROR)
		return false;

	*state = 0;
	return true;
}

void rtm_check_presence( uint8_t *status )
{
	if ( gpio_read_pin( PIN_PORT(GPIO_RTM_PS), PIN_NUMBER(GPIO_RTM_PS) ) ) {
		*status = HOTSWAP_STATE_URTM_ABSENT;
	}
	else {
		*status = HOTSWAP_STATE_URTM_PRSENT;
	}
}

void rtm_hardware_init( void )
{
	printf("RTM Enable Hardware\n");

	gpio_set_pin_state( PIN_PORT(GPIO_EN_RTM_MP), PIN_NUMBER(GPIO_EN_RTM_MP), GPIO_LEVEL_HIGH );
	vTaskDelay(100);

	pcf8574_set_port_dir_input(1 << RTM_GPIO_HOTSWAP_HANDLE);
	pcf8574_set_port_dir_input(1 << RTM_GPIO_POWER_GOOD);

	pcf8574_set_port_low( 1 << RTM_GPIO_LED_BLUE );
	pcf8574_set_port_high( 1 << RTM_GPIO_LED_RED );
	pcf8574_set_port_high( 1 << RTM_GPIO_LED_GREEN );
}

bool rtm_compatibility_check( void )
{
    uint8_t i;
    size_t rec_sz[2];
    uint8_t *z3_compat_recs[2] = { NULL, NULL };
    uint8_t cmn_hdr[8], multirec_hdr[10];
    uint8_t multirec_off;
    bool z3rec_found;

    for ( i = 0; i < 2; i++ ) {
        /* Read FRU Common Header */
        fru_read( i, cmn_hdr, 0, 8 );
        /* The offsets are divided by 8 in the common header */
        multirec_off = cmn_hdr[5] * 8;

        do {
            /* Read Multirecord header */
            fru_read( i, multirec_hdr, multirec_off, 10 );

            if (multirec_hdr[8] == 0x30) {
                z3rec_found = true;
                break;
            }
            /* Advance the offset pointer, adding the record length field to it */
            multirec_off += multirec_hdr[2]+5;

        } while ( (multirec_hdr[1] >> 7) != 1 );

        if ( z3rec_found ) {
            /* Read the Zone3 Compatibility Record, including the Multirecord header */
            rec_sz[i] = multirec_hdr[2] + 5;
            z3_compat_recs[i] = pvPortMalloc( rec_sz[i] );
            fru_read( i, z3_compat_recs[i], multirec_off, rec_sz[i] );
        }
    }

    if (!cmpBuffs( z3_compat_recs[0], rec_sz[0], z3_compat_recs[1], rec_sz[1] ) ) {
        return true;
    }

    vPortFree(z3_compat_recs[0]);
    vPortFree(z3_compat_recs[1]);

	return false;
}

bool rtm_quiesce( void )
{
	if (!gpio_read_pin( PIN_PORT(GPIO_RTM_PS), PIN_NUMBER(GPIO_RTM_PS) ))
	{
		uint8_t power_good_n;
		pcf8574_set_port_high( 1 << RTM_GPIO_EN_DC_DC );

		power_good_n = pcf8574_read_pin( RTM_GPIO_POWER_GOOD );

		if ( power_good_n ) {
			return true;
		}
	}
	return false;
}

void rtm_ctrl_led( uint8_t id, uint8_t state )
{
	uint8_t pcf_pin;

	switch (id)
	{
		case LED_BLUE:
			pcf_pin = RTM_GPIO_LED_BLUE;
			break;
		case LED1:
			pcf_pin = RTM_GPIO_LED_RED;
			break;
		case LED2:
			pcf_pin = RTM_GPIO_LED_GREEN;
			break;
		default:
			return;
	}

	if (!gpio_read_pin( PIN_PORT(GPIO_RTM_PS), PIN_NUMBER(GPIO_RTM_PS) ))
	{
		if (state == 1) {
			pcf8574_set_port_high ( 1 << pcf_pin );
		}
		else if (state == 0) {
			pcf8574_set_port_low( 1 << pcf_pin );
		}
	}
}

uint8_t rtm_read_led( uint8_t id )
{
	uint8_t pcf_pin;

	switch (id) {
	case LED_BLUE:
		pcf_pin = RTM_GPIO_LED_BLUE;
		break;
	case LED1:
		pcf_pin = RTM_GPIO_LED_RED;
		break;
	case LED2:
		pcf_pin = RTM_GPIO_LED_GREEN;
		break;
	default:
		return 1;
	}

	if (!gpio_read_pin( PIN_PORT(GPIO_RTM_PS), PIN_NUMBER(GPIO_RTM_PS) ) )
	{
		return pcf8574_read_pin( pcf_pin );
	}

	return 0;
}
