/*
 * PCF8574.c
 *
 *  Created on: 28-07-2014
 *      Author: Bartek
 */

#include "FreeRTOS.h"

#include "port.h"
#include "pcf8574.h"
#include "i2c.h"
#include "i2c_mapping.h"
#include "pin_mapping.h"


void pcf8574_set_port_dir_output(uint8_t pin_mask)
{
	uint8_t i2c_addr, i2c_id;
	uint8_t state = pcf8574_read_port();

	if ( state == PCF8574_READ_ERROR ) {
		return;
	}

	uint8_t data = state & (~pin_mask) ;

    if( i2c_take_by_chipid( CHIP_ID_RTM_PCF8574A, &i2c_addr, &i2c_id, (TickType_t) 200) ) {
        xI2CMasterWrite(i2c_id, i2c_addr, &data, sizeof(data));
        i2c_give(i2c_id);
    }
}

void pcf8574_set_port_dir_input(uint8_t pin_mask)
{
	uint8_t i2c_addr, i2c_id;
	uint8_t state = pcf8574_read_port();

	if ( state == PCF8574_READ_ERROR ) {
		return;
	}

	uint8_t data = state | pin_mask ;

    if( i2c_take_by_chipid( CHIP_ID_RTM_PCF8574A, &i2c_addr, &i2c_id, (TickType_t) 200) ) {
        xI2CMasterWrite(i2c_id, i2c_addr, &data, sizeof(data));
        i2c_give(i2c_id);
    }
}

void pcf8574_set_port_high(uint8_t pin_mask)
{
	uint8_t i2c_addr, i2c_id;
	uint8_t state = pcf8574_read_port();

	if ( state == PCF8574_READ_ERROR ) {
		return;
	}

	uint8_t data = state | pin_mask ;

	if( i2c_take_by_chipid( CHIP_ID_RTM_PCF8574A, &i2c_addr, &i2c_id, (TickType_t) 200) ) {
		xI2CMasterWrite( i2c_id, i2c_addr, &data, sizeof(state) );
		i2c_give(i2c_id);
	}
}

void pcf8574_set_port_low(uint8_t pin_mask)
{
	uint8_t i2c_addr, i2c_id;
	uint8_t state = pcf8574_read_port();

	if ( state == PCF8574_READ_ERROR ) {
		return;
	}

	uint8_t data = state & (~pin_mask) ;

    if( i2c_take_by_chipid( CHIP_ID_RTM_PCF8574A, &i2c_addr, &i2c_id, (TickType_t) 200) ) {
        xI2CMasterWrite(i2c_id, i2c_addr, &data, sizeof(data));
        i2c_give(i2c_id);
    }
}

uint8_t pcf8574_read_port()
{
	uint8_t i2c_addr, i2c_id;
	uint8_t data_rx = PCF8574_READ_ERROR;

    if ( i2c_take_by_chipid( CHIP_ID_RTM_PCF8574A, &i2c_addr, &i2c_id, (TickType_t) 200) ) {

    	/* Check if read was successful */
        if (xI2CMasterRead(i2c_id, i2c_addr, &data_rx, 1) == 0 ) {
        	return PCF8574_READ_ERROR;
        }

        i2c_give(i2c_id);
    }

	return data_rx;
}

uint8_t pcf8574_read_pin(uint8_t pin_num)
{
	pcf8574_set_port_high(1 << pin_num);
	uint8_t state = pcf8574_read_port();

	return (state & ( 1 << pin_num ));
}
