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

#include "port.h"
#include "scansta1101.h"

void scansta112_init( void )
{
    gpio_set_pin_state( PIN_PORT(GPIO_SCANSTA112_ADDR0), PIN_NUMBER(GPIO_SCANSTA112_ADDR0), GPIO_LEVEL_LOW );
    gpio_set_pin_state( PIN_PORT(GPIO_SCANSTA112_ADDR1), PIN_NUMBER(GPIO_SCANSTA112_ADDR1), GPIO_LEVEL_LOW );
    gpio_set_pin_state( PIN_PORT(GPIO_SCANSTA112_ADDR2), PIN_NUMBER(GPIO_SCANSTA112_ADDR2), GPIO_LEVEL_LOW );
    gpio_set_pin_state( PIN_PORT(GPIO_SCANSTA112_ADDR3), PIN_NUMBER(GPIO_SCANSTA112_ADDR3), GPIO_LEVEL_LOW );
    gpio_set_pin_state( PIN_PORT(GPIO_SCANSTA112_ADDR4), PIN_NUMBER(GPIO_SCANSTA112_ADDR4), GPIO_LEVEL_LOW );
    gpio_set_pin_state( PIN_PORT(GPIO_SCANSTA112_ADDR5), PIN_NUMBER(GPIO_SCANSTA112_ADDR5), GPIO_LEVEL_LOW );
    gpio_set_pin_state( PIN_PORT(GPIO_SCANSTA112_ADDR6), PIN_NUMBER(GPIO_SCANSTA112_ADDR6), GPIO_LEVEL_LOW );
    gpio_set_pin_state( PIN_PORT(GPIO_SCANSTA112_ADDR7), PIN_NUMBER(GPIO_SCANSTA112_ADDR7), GPIO_LEVEL_LOW );

    // keep scansta in reset
    gpio_set_pin_state( PIN_PORT(GPIO_SCANSTA112_RESET), PIN_NUMBER(GPIO_SCANSTA112_RESET), GPIO_LEVEL_LOW );

    // lpsel0 keep unseleted - inactive master port becomes LSP0
  	gpio_set_pin_state( PIN_PORT(GPIO_SCANSTA112_LPSEL0), PIN_NUMBER(GPIO_SCANSTA112_LPSEL0), GPIO_LEVEL_LOW );

  	// lpsel1 MMC JTAG
  	gpio_set_pin_state( PIN_PORT(GPIO_SCANSTA112_LPSEL1), PIN_NUMBER(GPIO_SCANSTA112_LPSEL1), GPIO_LEVEL_LOW );

  	// lpsel2 FPGA JTAG
  	gpio_set_pin_state( PIN_PORT(GPIO_SCANSTA112_LPSEL2), PIN_NUMBER(GPIO_SCANSTA112_LPSEL2), GPIO_LEVEL_HIGH );

  	// lpsel3 FMC1 JTAG
  	gpio_set_pin_state( PIN_PORT(GPIO_SCANSTA112_LPSEL3), PIN_NUMBER(GPIO_SCANSTA112_LPSEL3), GPIO_LEVEL_LOW );

  	// lpsel4 FMC2 JTAG
  	gpio_set_pin_state( PIN_PORT(GPIO_SCANSTA112_LPSEL4), PIN_NUMBER(GPIO_SCANSTA112_LPSEL4), GPIO_LEVEL_LOW );

  	// lpsel6 PS JTAG
  	gpio_set_pin_state( PIN_PORT(GPIO_SCANSTA112_LPSEL6), PIN_NUMBER(GPIO_SCANSTA112_LPSEL6), GPIO_LEVEL_LOW );

  	// stitcher mode
  	gpio_set_pin_state( PIN_PORT(GPIO_SCANSTA112_SB_S), PIN_NUMBER(GPIO_SCANSTA112_SB_S), GPIO_LEVEL_LOW );

  	asm("NOP");
  	asm("NOP");
  	asm("NOP");

  	// start scansta
  	gpio_set_pin_state( PIN_PORT(GPIO_SCANSTA112_RESET), PIN_NUMBER(GPIO_SCANSTA112_RESET), GPIO_LEVEL_HIGH );
}
