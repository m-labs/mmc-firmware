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

/* power override variables */
extern void setDC_DC_ConvertersON();
bool pwr_override = false;
extern volatile uint8_t rtm_power_level;

#define DATA_BYTES 64

extern char _pvHeapStart;
extern char _vStackTop;
#define heap_low _pvHeapStart
#define heap_top _vStackTop

char *heap_end = 0;
caddr_t _sbrk(int incr) {
 char *prev_heap_end;

 if (heap_end == 0) {
  heap_end = &heap_low;
 }
 prev_heap_end = heap_end;

 if (heap_end + incr > &heap_top) {
  /* Heap and stack collision */
  return (caddr_t)0;
 }

 heap_end += incr;
 return (caddr_t) prev_heap_end;
 }

extern xr77129_data_t xr77129_data[2];

void api_handler(uint8_t * data, uint8_t len)
{
    char cmd[10], hex[64];
    int a1, a2, a3, a4;
    sscanf((char*) data, "%s %d %d %d %d", cmd, &a1, &a2, &a3, &a4);

    if (!strcmp(cmd, "help")) {
		printf("Help command\n");
	} else if (!strcmp(cmd, "clear")) {
		if (a1 >= 0 && a1 <= 6) xr77129_flash_page_clear(&xr77129_data[0], a1);
	} else if (!strcmp(cmd, "erase")) {
		if (a1 >= 0 && a1 <= 6) xr77129_flash_page_erase(&xr77129_data[0], a1);
 	} else if (!strcmp(cmd, "write")) {
 		xr77129_flash_write(&xr77129_data[0], a1, a2, a3);
 	} else if (!strcmp(cmd, "read")) {
 		if (a1 % 2 == 0) xr77129_flash_read(&xr77129_data[0], a1, a2);
 	} else if (!strcmp(cmd, "ready")) {
 		xr77129_set_ready(&xr77129_data[0], a1);
 	} else if (!strcmp(cmd, "reset")) {
 		xr77129_reset(&xr77129_data[0]);
 	} else if (!strcmp(cmd, "full")) {
 		xr77129_flashops();
	}
}

void vCommandTask(void *pvParameters)
{
	uint8_t data_array[DATA_BYTES];
	uint8_t index = 0;
	uint8_t chr;

	vTaskDelay(100);

	if (gpio_read_pin( PIN_PORT(GPIO_P12V0_OK), PIN_NUMBER(GPIO_P12V0_OK)))
	{
		printf("Power override\n");
		pwr_override = true;
		setDC_DC_ConvertersON();
//		rtm_power_level = 0x01;
	}

	for (;;)
	{
		if (Chip_UART_Read(LPC_UART3, &chr, 1))
		{
			data_array[index++] = chr;
			if ((data_array[index - 1] == '\r') || (index >= (DATA_BYTES)))
			{
				data_array[index] = '\0';
				api_handler(data_array, index);
				index = 0;
			}
		}

		vTaskDelay(50);
	}

//	uint8_t Rxbuf[2];
//	uint16_t addr = 0x00;
//	static uint8_t cnt = 0;
//
//	vTaskDelay(100);
//
//    if (gpio_read_pin( PIN_PORT(GPIO_P12V0_OK), PIN_NUMBER(GPIO_P12V0_OK)))
//	{
//    	printf("Power override\n");
//    	pwr_override = true;
//    	setDC_DC_ConvertersON();
//    	rtm_power_level = 0x01;
//	}
//
//	for (;;)
//	{
//		if( Chip_UART_Read(LPC_UART3, Rxbuf, 1) )
//		{
//			if (Rxbuf[0] == 'P')
//			{
//				xr77129_dump_registers();
//			}
//			else if (Rxbuf[0] == 'E')
//			{
//				phy_dump();
//			}
//			else if (Rxbuf[0] == 'I')
//			{
//				phy_init();
//			} else if (Rxbuf[0] == 'x')
//			{
//				xr77129_flashops();
//			} else if (Rxbuf[0] == 'b') {
//				uint8_t state = pcf8574_read_port();
//				printf("pcf8574 state %d\n", state);
//
//			} else if (Rxbuf[0] == 'n') {
//				if (cnt++ % 2 == 0) {
//					pcf8574_set_port_low( 1 << RTM_GPIO_LED_BLUE );
//				} else {
//					pcf8574_set_port_high( 1 << RTM_GPIO_LED_BLUE );
//				}
//			}
//		}
//
//		vTaskDelay(100);
//	}
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
    printf("build date %s %s\n", __DATE__, __TIME__);
    printf("CPU clock = %d MHz\n", (int) (SystemCoreClock / 1000000));
    printf("Selected board: %s\n", STR(TARGET_BOARD_NAME));

#ifdef BENCH_TEST
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
#ifdef MODULE_SCANSTA1101
    scansta1101_init();
#endif
#ifdef MODULE_SCANSTA112
    scansta112_init();
#endif
#ifdef MODULE_FPGA_SPI
    fpga_spi_init();
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
