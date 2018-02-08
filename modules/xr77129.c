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

/**
 * @file xr77129.c
 *
 *
 * @brief XR77129 interface implementation
 *
 * @ingroup XR7129
 */

/* FreeRTOS Includes */
#include "FreeRTOS.h"
#include "task.h"

/* Project Includes */
#include "port.h"
#include "sdr.h"
#include "task_priorities.h"
#include "i2c.h"
#include "i2c_mapping.h"
#include "payload.h"
#include "xr77129.h"
#include "fpga_spi.h"
#include "fru.h"
#include "xr77129_flash_cfg.h"
#include "uart_debug.h"

/* data for EXARS on AMC [0] and RTM [1] */
static xr77129_data_t xr77129_data[2];

void xr77129_init ()
{
	xr77129_data[0].chipid = CHIP_ID_XR77129;

	xr77129_data[0].status_regs_addr[HOST_STS_REG]	 = XR77129_GET_HOST_STS;
	xr77129_data[0].status_regs_addr[FAULT_STS_REG]  = XR77129_GET_FAULT_STS;
	xr77129_data[0].status_regs_addr[PWR_STATUS_REG] = XR77129_PWR_GET_STATUS;
	xr77129_data[0].status_regs_addr[PWR_CHIP_READY] = XR77129_CHIP_READY;
	xr77129_data[0].status_regs_addr[GPIO_STATE] 	 = XR77129_GPIO_READ_GPIO;

	/* Check if there is valid configuration in xr77128 flash */
//	if ( ! xr77129_flash_verify( &xr77129_data[0], xr77129_flash_config, sizeof(xr77129_flash_config)/sizeof(xr77129_flash_config[0]) ) )
//	{
//		xr77129_flash_load ( &xr77129_data[0], xr77129_flash_config, sizeof(xr77129_flash_config)/sizeof(xr77129_flash_config[0]) );
//	}

	xTaskCreate( vTaskXR77129, "XR77129", 100, (void *) NULL, tskXR77129_PRIORITY, &vTaskXR77129_Handle);

#ifdef MODULE_RTM
	xr77129_RTM_init();
#endif

	sensor_t *temp_sensor;
	uint8_t i = 0;
    /* Iterate through the SDR Table to find all the XR77129 entries */
    for ( temp_sensor = sdr_head; temp_sensor != NULL; temp_sensor = temp_sensor->next) {

        if ( temp_sensor->task_handle == NULL ) {
            continue;
        }

        /* Check if this task should update the selected SDR */
        if ( *(temp_sensor->task_handle) != vTaskXR77129_Handle ) {
            continue;
        }

        /* Each XR77129 has 4 outputs hence 4 voltage sensors and 4 current sensors */
        if (i < XR77129_SENSORS_COUNT ) {
            xr77129_data[0].sensor[i] = temp_sensor;
            xr77129_data[0].sensor[i]->signed_flag = 0;

            if ((GET_SENSOR_TYPE(xr77129_data[0].sensor[i])) == SENSOR_TYPE_CURRENT ) {
            	xr77129_data[0].sensor[i]->signed_flag = 1;
            }
            i++;
        }
    }
}


void vTaskXR77129( void *Parameters )
{
	/* Power Good flags */
	uint8_t xr77129_PowerGood = 0;
	uint8_t P0V95_PowerGood = 0;
	uint8_t P1V2_PowerGood = 0;

    TickType_t xLastWakeTime;
    /* Task will run every 100ms */
    const TickType_t xFrequency = XR77129_UPDATE_RATE / portTICK_PERIOD_MS;

    /* Initialise the xLastWakeTime variable with the current time. */
    xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {
    	/* Payload power good flag */
    	if (gpio_read_pin( PIN_PORT(GPIO_P12V0_OK), PIN_NUMBER(GPIO_P12V0_OK)))
    	{
			/* Read power status of xr77129 outputs, P0V95 and P1V2 */
			xr77129_read_status( &xr77129_data[0] );

			if ( xr77129_data[0].status_regs[PWR_STATUS_REG] == XR77129_POWER_OK ) {
				xr77129_PowerGood = 1;
			} else {
				xr77129_PowerGood = 0;
			}


			if ( xr77129_data[0].status_regs[GPIO_STATE] & XR77129_GPIO0 ) {
				P0V95_PowerGood = 1;
			} else {
				P0V95_PowerGood = 0;
			}

			if ( xr77129_data[0].status_regs[GPIO_STATE] & XR77129_PSIO1 ) {
				P1V2_PowerGood = 1;
			} else {
				P1V2_PowerGood = 0;
			}

			if ( xr77129_PowerGood && P0V95_PowerGood && P1V2_PowerGood ) {
				/* Let Payload Task know that Power is OK */
				payload_send_message( FRU_AMC, PAYLOAD_MESSAGE_DCDC_PGOOD );
			} else {
				payload_send_message( FRU_AMC, PAYLOAD_MESSAGE_DCDC_PGOODn );
			}
    	}

    	vTaskDelayUntil( &xLastWakeTime, xFrequency );
    }
}

#ifdef MODULE_RTM

void xr77129_RTM_init()
{
	xr77129_data[1].chipid = CHIP_ID_RTM_XR77129;

	xr77129_data[1].status_regs_addr[HOST_STS_REG]	 = XR77129_GET_HOST_STS;
	xr77129_data[1].status_regs_addr[FAULT_STS_REG]	 = XR77129_GET_FAULT_STS;
	xr77129_data[1].status_regs_addr[PWR_STATUS_REG] = XR77129_PWR_GET_STATUS;
	xr77129_data[1].status_regs_addr[PWR_CHIP_READY] = XR77129_CHIP_READY;
	xr77129_data[1].status_regs_addr[GPIO_STATE] 	 = XR77129_GPIO_READ_GPIO;

//	xTaskCreate( vTaskXR77129_RTM, "XR77129_RTM", 100, (void *) NULL, tskXR77129_PRIORITY, &vTaskXR77129_RTM_Handle);
}

void vTaskXR77129_RTM( void *Parameters )
{
	/* Power Good flags */
	uint8_t xr77129_PowerGood = 0;

    TickType_t xLastWakeTime;
    /* Task will run every 100ms */
    const TickType_t xFrequency = XR77129_UPDATE_RATE / portTICK_PERIOD_MS;

    /* Initialise the xLastWakeTime variable with the current time. */
    xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {
    	if (!gpio_read_pin( PIN_PORT(GPIO_RTM_PS), PIN_NUMBER(GPIO_RTM_PS) ))
    	{
    		xr77129_read_status( &xr77129_data[1] );
    	}

    	vTaskDelayUntil( &xLastWakeTime, xFrequency );
    }
}

#endif

uint8_t xr77129_flash_load ( xr77129_data_t * data, const uint8_t * xr77129_config, uint32_t size )
{

	xr77129_flash_erase ( data );

	xr77129_flash_program ( data, xr77129_config, size );

	xr77129_flash_verify ( data, xr77129_config, size );

	return 0;
}


uint8_t xr77129_flash_erase ( xr77129_data_t * data )
{
	uint8_t page;

	for ( page=0 ; page <= 6 ; page++)
	{
		xr77129_flash_page_clear(data, page);
	}

	for ( page=0 ; page <= 6 ; page++)
	{
		xr77129_flash_page_erase(data, page);
	}

	return 0;
}

uint8_t xr77129_flash_page_clear ( xr77129_data_t * data, uint8_t page )
{
	uint16_t flash_enable_clear = 0x0001;
	xr77129_write_value( data, XR77129_FLASH_INIT, flash_enable_clear);

	uint16_t page_clear = page;
	xr77129_write_value( data, XR77129_FLASH_PAGE_CLEAR, page_clear);

	uint16_t page_clear_status = 0x01;
	while ( page_clear_status & 0xFF )
	{
		/* TODO: timeout */
		xr77129_read_value( data, XR77129_FLASH_PAGE_CLEAR, &page_clear_status );
	}

	return 0;
}

uint8_t xr77129_flash_page_erase (xr77129_data_t * data, uint8_t page )
{
	uint16_t init_page_erase = 0x0005;
	xr77129_write_value( data, XR77129_FLASH_INIT, init_page_erase);

	uint16_t page_erase = page;
	xr77129_write_value( data, XR77129_FLASH_PAGE_ERASE, page_erase );

	uint16_t page_erase_status = 0x01;
	while ( page_erase_status & 0xFF )
	{
		/* TODO: timeout */
		xr77129_read_value( data, XR77129_FLASH_PAGE_ERASE, &page_erase_status );
	}

	return 0;
}


uint8_t xr77129_flash_program ( xr77129_data_t * data, const uint8_t * flash_data, uint32_t size )
{
	/* Enable flash program */
	uint16_t page_clear = 0x0001;
	xr77129_write_value( data, XR77129_FLASH_INIT, page_clear);

	/* set flash address pointer */
	uint16_t flash_program_address = 0x0000;
	xr77129_write_value( data, XR77129_FLASH_PROGRAM_ADDRESS, flash_program_address);


	/* program data */
	uint32_t i;
	uint16_t val;

	for ( i=0 ; i < size - 1 ; i+=2 )
	{
		val = (flash_data[1] << 8) | flash_data[i+1] ;
		xr77129_write_value( data, XR77129_FLASH_PROGRAM_DATA_INC_ADDRESS, val);
	}

	return 0;
}

uint8_t xr77129_flash_verify ( xr77129_data_t * data, const uint8_t * flash_data, uint32_t size )
{
	uint8_t i2c_interf, i2c_addr;

	uint8_t temp_flash [ size ];
	uint8_t data_address = XR77129_FLASH_PROGRAM_DATA_INC_ADDRESS;

	uint8_t rx[2];
	uint8_t i = 0x00;
	for ( ; i < size-1 ; i+=2 )
	{
		if( i2c_take_by_chipid( data->chipid, &i2c_addr, &i2c_interf, portMAX_DELAY) == pdTRUE ) {

			xI2CMasterWriteRead( i2c_interf, i2c_addr, data_address, rx, sizeof(rx)/sizeof(rx[0]) );

			i2c_give( i2c_interf );
			temp_flash[i] = rx[0];
			temp_flash[i+1] = rx[1];
		}
	}

	for ( i=0 ; i < size ; i++)
	{
		if (temp_flash[i] != flash_data[i])
		{
			return 0;
		}
	}

	return 1;
}



void xr77129_read_status( xr77129_data_t * data )
{
	uint8_t reg;
	for (reg = HOST_STS_REG ; reg < XR77129_STATUS_REGISTERS_COUNT ; reg++)
	{
		xr77129_read_value( data, data->status_regs_addr[reg], &(data->status_regs[reg]) );
	}
}

Bool xr77129_read_value( xr77129_data_t * data, uint8_t reg_address, uint16_t *read )
{
    uint8_t i2c_interf, i2c_addr;
    uint8_t val[2] = {0};

    if( i2c_take_by_chipid( data->chipid, &i2c_addr, &i2c_interf, portMAX_DELAY) == pdTRUE )
    {
        xI2CMasterWriteRead( i2c_interf, i2c_addr, reg_address, &val[0], sizeof(val)/sizeof(val[0]) );

        i2c_give( i2c_interf );

        *read = (val[0] << 8) | (val[1]);

//        DEBUG_MSG("Read reg %02X value %02X\n", reg_address, (val[0] << 8) | (val[1]));

        return true;
    }

    return false;
}

uint8_t xr77129_write_value( xr77129_data_t * data, uint8_t reg_address, uint16_t value )
{
	uint8_t i2c_interf, i2c_addr;

	uint8_t tx[2] = {
			value >> 8,
			value & 0xFF
	};

	if ( i2c_take_by_chipid( data->chipid, &i2c_addr, &i2c_interf, portMAX_DELAY) == pdTRUE )
	{
		xI2CMasterWrite( i2c_interf, i2c_addr, tx, sizeof(tx)/sizeof(tx[0]) );
		i2c_give( i2c_interf );

		return 0;
	}

	return -1;
}

void xr77129_dump_registers(void)
{
	uint16_t ret_val;
	uint8_t reg_addr = 0x00;

	printf("------------ Exar Dump ----------\n");
	reg_addr = 0x02;
	xr77129_read_value( &xr77129_data[0], reg_addr, &ret_val);
	printf("GET_HOST_STS 0x%x 0x%x 0x%x\n", 0x02, (ret_val >> 8) & 0xFF, ret_val & 0xFF);

	reg_addr = 0x05;
	xr77129_read_value( &xr77129_data[0], reg_addr, &ret_val);
	printf("GET_FAULT_STS 0x%x 0x%x 0x%x\n", 0x05, (ret_val >> 8) & 0xFF, ret_val & 0xFF);

	reg_addr = 0x09;
	xr77129_read_value( &xr77129_data[0], reg_addr, &ret_val);
	printf("PWR_GET_STATUS 0x%x 0x%x 0x%x\n", 0x09, (ret_val >> 8) & 0xFF, ret_val & 0xFF);

	reg_addr = 0x10;
	xr77129_read_value( &xr77129_data[0], reg_addr, &ret_val);
	printf("PWR_READ_VOLTAGE_CH1 0x%x %d %d V\n", 0x10, (((ret_val >> 8) & 0xFF), (ret_val & 0xFF)));

	reg_addr = 0x11;
	xr77129_read_value( &xr77129_data[0], reg_addr, &ret_val);
	printf("PWR_READ_VOLTAGE_CH2 0x%x %d %d V\n", 0x11, (((ret_val >> 8) & 0xFF), (ret_val & 0xFF)));

	reg_addr = 0x12;
	xr77129_read_value( &xr77129_data[0], reg_addr, &ret_val);
	printf("PWR_READ_VOLTAGE_CH3 0x%x %d %d V\n", 0x12, (((ret_val >> 8) & 0xFF), (ret_val & 0xFF)));

	reg_addr = 0x13;
	xr77129_read_value( &xr77129_data[0], reg_addr, &ret_val);
	printf("PWR_READ_VOLTAGE_CH4 0x%x %d %d V\n", 0x13, (((ret_val >> 8) & 0xFF), (ret_val & 0xFF)));

	reg_addr = 0x14;
	xr77129_read_value( &xr77129_data[0], reg_addr, &ret_val);
	printf("PWR_READ_VOLTAGE_IN 0x%x %d %d V\n", 0x14, (((ret_val >> 8) & 0xFF), (ret_val & 0xFF)));
}
