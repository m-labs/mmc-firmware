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
#include "pcf8574.h"
#include "rtm_user.h"

/* data for EXARS on AMC [0] and RTM [1] */
xr77129_data_t xr77129_data[2];

void xr77129_init()
{
	xr77129_data[0].chipid = CHIP_ID_XR77129;

	xr77129_data[0].status_regs_addr[HOST_STS_REG]	 = XR77129_GET_HOST_STS;
	xr77129_data[0].status_regs_addr[FAULT_STS_REG]  = XR77129_GET_FAULT_STS;
	xr77129_data[0].status_regs_addr[PWR_STATUS_REG] = XR77129_PWR_GET_STATUS;
	xr77129_data[0].status_regs_addr[PWR_CHIP_READY] = XR77129_CHIP_READY;
	xr77129_data[0].status_regs_addr[GPIO_STATE] 	 = XR77129_GPIO_READ_GPIO;

	xTaskCreate( vTaskXR77129, "XR77129", 256, (void *) NULL, tskXR77129_PRIORITY, &vTaskXR77129_Handle);

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
	uint8_t xr77129_flashverified = false;

    TickType_t xLastWakeTime;
    /* Task will run every 100ms */
    const TickType_t xFrequency = XR77129_UPDATE_RATE / portTICK_PERIOD_MS;

    /* Initialise the xLastWakeTime variable with the current time. */
    xLastWakeTime = xTaskGetTickCount();

    vTaskDelay(2000);
    while (!xr77129_flashverified) {
    	xr77129_flashverified = xr77129_check_flash(&xr77129_data[0], xr77129_amc_flash_cfg, sizeof(xr77129_amc_flash_cfg)/sizeof(xr77129_amc_flash_cfg[0]));
    	vTaskDelay(500);
    }

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

uint8_t xr77129_flash_verify( xr77129_data_t * data, const uint8_t * flash_data, uint32_t size )
{
	uint8_t rx[2] = { 0 };
	uint8_t tx[3] = { 0 };
	uint8_t reg;
	uint16_t value;
	uint8_t i2c_interf, i2c_addr;

	printf("Checking %d EXAR configuration\n", data->chipid);

	// ENABLE FLASH
	uint16_t command = 0x0001;
	reg = 0x4D;
	xr77129_write_value(data, reg, command);

	// 10msec delay
	vTaskDelay(100);

	// set FLASH address at 0x00
	command = 0x0000;
	xr77129_write_value(data, XR77129_FLASH_PROGRAM_ADDRESS, command);

	uint8_t data_address = XR77129_FLASH_PROGRAM_DATA;
	if( i2c_take_by_chipid(data->chipid, &i2c_addr, &i2c_interf, portMAX_DELAY) == pdTRUE ) {
		for (uint32_t i = 0x00 ; i < size; i += 2 )
		{
			tx[0] = XR77129_FLASH_PROGRAM_ADDRESS;
			tx[2] = i & 0xFF;
			tx[1] = (i & 0xFF00) >> 8;
			xI2CMasterWrite( i2c_interf, i2c_addr, tx, 3 );

			vTaskDelay(10);
			xI2CMasterWriteRead( i2c_interf, i2c_addr, data_address, rx, 2 );

			if (rx[0] != flash_data[i] || rx[1] != flash_data[i + 1]) {
				printf("\nDifference at byte %d for %d\n", i, data->chipid);
				i2c_give( i2c_interf );
				return false;
			}
		}
		i2c_give( i2c_interf );
	}

	return true;
}

uint8_t xr77129_flash_read( xr77129_data_t * data, uint16_t address, uint32_t size)
{
	uint8_t rx[2] = { 0 };
	uint8_t tx[3] = { 0 };
	uint8_t reg;
	uint16_t value;
	uint8_t i2c_interf, i2c_addr;

	printf("Read %d at %d\n", size, address);

	// ENABLE FLASH
	uint16_t command = 0x0001;
	reg = 0x4D;
	xr77129_write_value(data, reg, command);

	// 10msec delay
	vTaskDelay(100);

	// set FLASH address at 0x00
	command = address;
	xr77129_write_value(data, XR77129_FLASH_PROGRAM_ADDRESS, command);

	vTaskDelay(200);

	// Readout by using DATA INC each read
	uint8_t data_address = XR77129_FLASH_PROGRAM_DATA; //XR77129_FLASH_PROGRAM_DATA_INC_ADDRESS;

	if( i2c_take_by_chipid(data->chipid, &i2c_addr, &i2c_interf, portMAX_DELAY) == pdTRUE ) {
		for (int i = 0x00 ; i < size; i += 2 )
		{
			vTaskDelay(100);

			tx[0] = XR77129_FLASH_PROGRAM_ADDRESS;
			tx[2] = (address + i) & 0xFF;
			tx[1] = ((address + i) & 0xFF00) >> 8;
			xI2CMasterWrite( i2c_interf, i2c_addr, tx, 3 );

			vTaskDelay(10);

			xI2CMasterWriteRead( i2c_interf, i2c_addr, data_address, rx, 2 );
			printf("[%d] %02X %02X\n", i, rx[0], rx[1]);
		}
		i2c_give( i2c_interf );
	}
	return true;
}

uint8_t xr77129_check_flash( xr77129_data_t * data, const uint8_t * xr77129_config, uint32_t size )
{
	// EXAR PROGRAMMING ROUTINE
	// If new configuration is detected, image is
	// flashed & verified
	if (gpio_read_pin( PIN_PORT(GPIO_P12V0_OK), PIN_NUMBER(GPIO_P12V0_OK))) {
		if (!xr77129_flash_verify(data, xr77129_config, size))
		{
//			xr77129_set_ready(data, 0);
//			xr77129_flash_erase(data);
//			xr77129_flash_program(data, xr77129_config, size);
//			if (xr77129_flash_verify(data, xr77129_config, size))
//			{
//				printf("xr77129 flash programming successfull\n");
//				xr77129_reset(data);
//			}
			printf("xr77129 %d flash different\n", data->chipid);
		} else {
			printf("xr77129 %d flash verified\n", data->chipid);
		}

		return true;
	}

	return false;
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

	xTaskCreate( vTaskXR77129_RTM, "XR77129_RTM", 256, (void *) NULL, tskXR77129_PRIORITY, &vTaskXR77129_RTM_Handle);
}

void vTaskXR77129_RTM( void *Parameters )
{
	/* Power Good flags */
	uint8_t xr77129_PowerGood = 0;
	uint8_t i2c_interf, i2c_addr;
	uint8_t val[2] = { 0 };
	uint8_t xr77129_flashverified = false;

    TickType_t xLastWakeTime;
    /* Task will run every 100ms */
    const TickType_t xFrequency = XR77129_UPDATE_RATE / portTICK_PERIOD_MS;

    /* Initialise the xLastWakeTime variable with the current time. */
    xLastWakeTime = xTaskGetTickCount();

    vTaskDelay(5000);
    while (!xr77129_flashverified) {
    	uint8_t state = pcf8574_read_port();
    	if (state & RTM_GPIO_EN_DC_DC) {
    		xr77129_flashverified = xr77129_check_flash(&xr77129_data[1], xr77129_rtm_flash_cfg, sizeof(xr77129_rtm_flash_cfg)/sizeof(xr77129_rtm_flash_cfg[0]));
    	}
    	vTaskDelay(1000);
    }

    for (;;)
    {
//    	if (!gpio_read_pin( PIN_PORT(GPIO_RTM_PS), PIN_NUMBER(GPIO_RTM_PS) ))
//    	{
//    		if( i2c_take_by_chipid(xr77129_data[1].chipid , &i2c_addr, &i2c_interf, portMAX_DELAY) == pdTRUE )
//			{
//				xI2CMasterWriteRead( i2c_interf, i2c_addr, 0x0B, &val[0], sizeof(val)/sizeof(val[0]) );
//
//				i2c_give( i2c_interf );
//
//		        printf("Read reg %02X value %02X\n", 0x14, (val[0] << 8) | (val[1]));
//			}
//    	}

//    	vTaskDelayUntil( &xLastWakeTime, xFrequency );
    	vTaskDelay(10000);
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
	printf("Clearing EXAR flash pages....");

	for ( page=0 ; page <= 6 ; page++)
	{
		xr77129_flash_page_clear(data, page);
	}

	for ( page=0 ; page <= 6 ; page++)
	{
		xr77129_flash_page_erase(data, page);
	}

	printf("Done\n");

	return 0;
}

uint8_t xr77129_flash_page_erase ( xr77129_data_t * data, uint8_t page )
{
	if (page > 6) return 255;

	// Init flash for page clear
	uint16_t flash_enable_erase = 0x0005;
	xr77129_write_value( data, XR77129_FLASH_INIT, flash_enable_erase);

	vTaskDelay(200);

	// Issue page erase
	uint16_t page_to_clear = (uint16_t) page;
	xr77129_write_value( data, XR77129_FLASH_PAGE_ERASE, page_to_clear);

	vTaskDelay(800);

	uint16_t page_erase_status = 0x01;
	uint8_t erase_timeout = 0xFF;
	while ( page_erase_status & 0xFF )
	{
		xr77129_read_value( data, XR77129_FLASH_PAGE_ERASE, &page_erase_status );
		vTaskDelay(100);
		if (!erase_timeout--)
		{
			printf("Page erase timeout %d\n", page);
			return 0xFF;
		}
	}

	return 0;
}

uint8_t xr77129_flash_page_clear ( xr77129_data_t * data, uint8_t page )
{
	if (page > 6) return 255;

	// Init flash for page clear
	uint16_t flash_enable_clear = 0x0001;
	xr77129_write_value( data, XR77129_FLASH_INIT, flash_enable_clear);

	vTaskDelay(200);

	// Issue page erase
	uint16_t page_to_clear = (uint16_t) page;
	xr77129_write_value( data, XR77129_FLASH_PAGE_CLEAR, page_to_clear);

	vTaskDelay(800);

	uint16_t page_erase_status = 0x01;
	uint8_t erase_timeout = 0x20;

	while ( page_erase_status & 0xFF )
	{
		xr77129_read_value( data, XR77129_FLASH_PAGE_CLEAR, &page_erase_status );
		vTaskDelay(100);

		if (!erase_timeout--) {
			printf("Page clear timeout %d\n", page);
			return 0xFF;
		}
	}

	return 0;
}

uint8_t xr77129_flash_program ( xr77129_data_t * data, const uint8_t * flash_data, uint32_t size )
{
	printf("Programming EXAR flash...");

	/* Enable flash program */
	uint16_t page_clear = 0x0001;
	xr77129_write_value( data, XR77129_FLASH_INIT, page_clear);

	vTaskDelay(100);

	/* program data */
	uint32_t i;
	uint8_t i2c_interf, i2c_addr;
	uint8_t tx[3] = { 0 };

	if ( i2c_take_by_chipid( data->chipid, &i2c_addr, &i2c_interf, portMAX_DELAY) == pdTRUE )
	{
		for ( i = 0 ; i < size ; i += 2 ) {

			tx[0] = XR77129_FLASH_PROGRAM_ADDRESS;
			tx[2] = i & 0xFF;
			tx[1] = (i & 0xFF00) >> 8;
			xI2CMasterWrite( i2c_interf, i2c_addr, tx, 3 );

			vTaskDelay(10);

			tx[0] = XR77129_FLASH_PROGRAM_DATA;
			tx[1] = flash_data[i];
			tx[2] = flash_data[i + 1];
			xI2CMasterWrite( i2c_interf, i2c_addr, tx, 3 );

			vTaskDelay(200);
		}

		i2c_give( i2c_interf );
	}

	printf("Done\n");
	return 0;
}

uint8_t xr77129_flash_write ( xr77129_data_t * data, uint16_t address, uint8_t flash_data, uint8_t flash_data2)
{
	/* Enable flash program */
	uint16_t page_clear = 0x0001;
	xr77129_write_value( data, XR77129_FLASH_INIT, page_clear);

	vTaskDelay(100);

	printf("writing %d %d to %d\n", flash_data, flash_data2, address);

	/* program data */
	uint8_t i2c_interf, i2c_addr;
	uint8_t tx[3] = { 0 };

	if ( i2c_take_by_chipid( data->chipid, &i2c_addr, &i2c_interf, portMAX_DELAY) == pdTRUE )
	{
		tx[0] = XR77129_FLASH_PROGRAM_ADDRESS;
		tx[2] = address & 0xFF;
		tx[1] = (address & 0xFF00) >> 8;
		xI2CMasterWrite( i2c_interf, i2c_addr, tx, 3 );

		vTaskDelay(10);

		tx[0] = XR77129_FLASH_PROGRAM_DATA;
		tx[1] = flash_data;
		tx[2] = flash_data2;
		xI2CMasterWrite( i2c_interf, i2c_addr, tx, 3 );
		vTaskDelay(100);

		i2c_give( i2c_interf );
	}

	return 0;
}

void xr77129_reset( xr77129_data_t * data )
{
	uint16_t value = 0x0F00;
	xr77129_write_value( data, XR77129_PWR_RESTART, value);
}

void xr77129_set_ready( xr77129_data_t * data , uint16_t status)
{
	xr77129_write_value( data, XR77129_CHIP_READY, status);

	uint8_t reg = 0x0E;
	uint16_t value;

	vTaskDelay(100);

	xr77129_read_value(data, reg, &value);
	printf("EXAR STATUS %d\n", value);
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

//        printf("Read reg %02X value %02X\n", reg_address, (val[0] << 8) | (val[1]));

        return true;
    }

    return false;
}

uint8_t xr77129_write_value( xr77129_data_t * data, uint8_t reg_address, uint16_t value )
{
	uint8_t i2c_interf, i2c_addr;

	uint8_t tx[3] = {
			reg_address,
			(value & 0xFF00) >> 8,
			value & 0xFF
	};

	if ( i2c_take_by_chipid( data->chipid, &i2c_addr, &i2c_interf, portMAX_DELAY) == pdTRUE )
	{
		xI2CMasterWrite( i2c_interf, i2c_addr, tx, 3 );
		i2c_give( i2c_interf );

		return 0;
	}

	return -1;
};

void xr77129_dump_registers(void)
{
	uint16_t ret_val;
	uint8_t reg_addr = 0x00;

	printf("------------ Exar Dump ----------\n");
	reg_addr = 0x02;
	xr77129_read_value( &xr77129_data[1], reg_addr, &ret_val);
	printf("GET_HOST_STS 0x%x 0x%x 0x%x\n", 0x02, (ret_val >> 8) & 0xFF, ret_val & 0xFF);

	reg_addr = 0x05;
	xr77129_read_value( &xr77129_data[1], reg_addr, &ret_val);
	printf("GET_FAULT_STS 0x%x 0x%x 0x%x\n", 0x05, (ret_val >> 8) & 0xFF, ret_val & 0xFF);

	reg_addr = 0x09;
	xr77129_read_value( &xr77129_data[1], reg_addr, &ret_val);
	printf("PWR_GET_STATUS 0x%x 0x%x 0x%x\n", 0x09, (ret_val >> 8) & 0xFF, ret_val & 0xFF);

	reg_addr = 0x10;
	xr77129_read_value( &xr77129_data[1], reg_addr, &ret_val);
	printf("PWR_READ_VOLTAGE_CH1 0x%x %d %d V\n", 0x10, (((ret_val >> 8) & 0xFF), (ret_val & 0xFF)));

	reg_addr = 0x11;
	xr77129_read_value( &xr77129_data[1], reg_addr, &ret_val);
	printf("PWR_READ_VOLTAGE_CH2 0x%x %d %d V\n", 0x11, (((ret_val >> 8) & 0xFF), (ret_val & 0xFF)));

	reg_addr = 0x12;
	xr77129_read_value( &xr77129_data[1], reg_addr, &ret_val);
	printf("PWR_READ_VOLTAGE_CH3 0x%x %d %d V\n", 0x12, (((ret_val >> 8) & 0xFF), (ret_val & 0xFF)));

	reg_addr = 0x13;
	xr77129_read_value( &xr77129_data[1], reg_addr, &ret_val);
	printf("PWR_READ_VOLTAGE_CH4 0x%x %d %d V\n", 0x13, (((ret_val >> 8) & 0xFF), (ret_val & 0xFF)));

	reg_addr = 0x14;
	xr77129_read_value( &xr77129_data[1], reg_addr, &ret_val);
	printf("PWR_READ_VOLTAGE_IN 0x%x %d %d V\n", 0x14, (((ret_val >> 8) & 0xFF), (ret_val & 0xFF)));
}
