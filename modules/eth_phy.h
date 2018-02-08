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

#ifndef MODULES_ETH_PHY_H_
#define MODULES_ETH_PHY_H_

#include "port.h"
#include "enet_17xx_40xx.h"

void phy_init(void);
uint16_t phy_read(uint16_t bPhyAddr, uint8_t PhyReg);
void phy_write(uint16_t bPhyAddr,uint8_t PhyReg, uint16_t Value);
void phy_dump(void);

#endif /* MODULES_ETH_PHY_H_ */
