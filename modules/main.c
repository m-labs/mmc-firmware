/*
 *   openMMC -- Open Source modular IPM Controller firmware
 *
 *   Copyright (C) 2015  Henrique Silva <henrique.silva@lnls.br>
 *   Copyright (C) 2015  Piotr Miedzik  <P.Miedzik@gsi.de>
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

/* Kernel includes. */
#include "FreeRTOS.h"

/* Project includes */
#include "port.h"
#include "led.h"
#include "ipmi.h"
#include "payload.h"
#include "i2c.h"
#include "fru.h"
#include "scansta1101.h"
#include "scansta112.h"
#include "eth_phy.h"
#include "fpga_spi.h"
#include "watchdog.h"
#include "uart_debug.h"
#include "xr77129.h"
#ifdef MODULE_RTM
#include "rtm.h"
#endif

void vCommandTask(void *pvParameters)
{
	uint8_t Rxbuf[2];

	for (;;)
	{
		if( Chip_UART_Read(LPC_UART3, Rxbuf, 1) )
		{
			if (Rxbuf[0] == 'p')
			{
				xr77129_dump_registers();
			}
			else if (Rxbuf[0] == 'e')
			{
				phy_dump();
			}
			else if (Rxbuf[0] == 'i')
			{
				phy_init();
			}
		}
	}
}

/*-----------------------------------------------------------*/
int main( void )
{
    pin_init();

    // make sure address is valid
    gpio_set_pin_state( PIN_PORT(GPIO_I2C_MUX_ADDR1), PIN_NUMBER(GPIO_I2C_MUX_ADDR1), GPIO_LEVEL_LOW );
    gpio_set_pin_state( PIN_PORT(GPIO_I2C_MUX_ADDR2), PIN_NUMBER(GPIO_I2C_MUX_ADDR2), GPIO_LEVEL_LOW );
    gpio_set_pin_state( PIN_PORT(GPIO_SW_RESETn), PIN_NUMBER(GPIO_SW_RESETn), GPIO_LEVEL_HIGH );

    // I2C FIX for 1776
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 1, (IOCON_FUNC3 | IOCON_MODE_PULLUP | IOCON_OPENDRAIN_EN));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 0, (IOCON_FUNC3 | IOCON_MODE_PULLUP | IOCON_OPENDRAIN_EN));

#ifdef MODULE_UART_DEBUG
    uart_init( UART_DEBUG );
#endif

    printf("openMMC Starting!\n");
    printf("CPU clock = %d MHz\n", (int) (SystemCoreClock / 1000000));
    printf("Selected board: %s\n", STR(TARGET_BOARD_NAME));

#if BENCH_TEST
    printf("BENCH Test active\n");
#endif

#ifdef MODULE_WATCHDOG
    watchdog_init();
#endif

    LED_init();
    i2c_init();

#ifdef MODULE_FRU
    fru_init(FRU_AMC);
#endif

#ifdef MODULE_SDR
    sdr_init();
#endif
#ifdef MODULE_SENSORS
    sensor_init();
#endif
#ifdef MODULE_PAYLOAD
    payload_init();
#endif
#ifdef MODULE_ETHERNET
    phy_init();
#endif
#ifdef MODULE_SCANSTA1101
    scansta1101_init();
#endif
#ifdef MODULE_SCANSTA112
    scansta112_init();
#endif
#ifdef MODULE_FPGA_SPI
//    fpga_spi_init();
#endif
#ifdef MODULE_RTM
    rtm_manage_init();
#endif
    /*  Init IPMI interface */
    /* NOTE: ipmb_init() is called inside this function */
    ipmi_init();

	xTaskCreate(vCommandTask, (signed char *) "Commands", configMINIMAL_STACK_SIZE + 256UL, NULL, (tskIDLE_PRIORITY + 1UL), (xTaskHandle *) NULL);
    /* Start the tasks running. */
    vTaskStartScheduler();

    /* If all is well we will never reach here as the scheduler will now be
       running.  If we do reach here then it is likely that there was insufficient
       heap available for the idle task to be created. */
    for( ;; );
}

/*-----------------------------------------------------------*/
/* Put the MCU in sleep state when no task is running */
void vApplicationIdleHook (void)
{
    pm_sleep();
}

void HardFault_Handler(void)
{
	gpio_set_pin_low(1, 25);

	__asm volatile
	    (
	        " tst lr, #4                                                \n"
	        " ite eq                                                    \n"
	        " mrseq r0, msp                                             \n"
	        " mrsne r0, psp                                             \n"
	        " ldr r1, [r0, #24]                                         \n"
	        " ldr r2, handler2_address_const                            \n"
	        " bx r2                                                     \n"
	        " handler2_address_const: .word prvGetRegistersFromStack    \n"
	    );


	while(1) {}
}

void prvGetRegistersFromStack( uint32_t *pulFaultStackAddress )
{
	/* These are volatile to try and prevent the compiler/linker optimising them
	away as the variables never actually get used.  If the debugger won't show the
	values of the variables, make them global my moving their declaration outside
	of this function. */
	volatile uint32_t r0;
	volatile uint32_t r1;
	volatile uint32_t r2;
	volatile uint32_t r3;
	volatile uint32_t r12;
	volatile uint32_t lr; /* Link register. */
	volatile uint32_t pc; /* Program counter. */
	volatile uint32_t psr;/* Program status register. */

    r0 = pulFaultStackAddress[ 0 ];
    r1 = pulFaultStackAddress[ 1 ];
    r2 = pulFaultStackAddress[ 2 ];
    r3 = pulFaultStackAddress[ 3 ];

    r12 = pulFaultStackAddress[ 4 ];
    lr = pulFaultStackAddress[ 5 ];
    pc = pulFaultStackAddress[ 6 ];
    psr = pulFaultStackAddress[ 7 ];

    printf("Hardfault @ %d\n", pc);

    /* When the following line is hit, the variables contain the register values. */
    for( ;; );
}
