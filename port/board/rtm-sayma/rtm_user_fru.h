/*
 *   openMMC -- Open Source modular IPM Controller firmware
 *
 *   Copyright (C) 2015  Julian Mendez  <julian.mendez@cern.ch>
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

#include "fru_editor.h"

/*********************************************
 * Common defines
 *********************************************/
#define RTM_LANG_CODE               0
#define RTM_FRU_FILE_ID             "RTMFRU"       //Allows knowing the source of the FRU present in the memory

/*********************************************
 * Board information area
 *********************************************/
#define RTM_BOARD_MANUFACTURING_TIME 11121887      /* Amount of minutes since 0:00 1/1/96 */
#define RTM_BOARD_MANUFACTURER      "ISEPW"
#define RTM_BOARD_NAME              "RTM-SAYMA"
#define RTM_BOARD_SN                "1"
#define RTM_BOARD_PN                "RTM-SAYMA"

/*********************************************
 * Product information area
 *********************************************/
#define RTM_PRODUCT_MANUFACTURER    "ISEPW"
#define RTM_PRODUCT_NAME            "Sayma RTM"
#define RTM_PRODUCT_PN              "00001"
#define RTM_PRODUCT_VERSION         "v1.0"
#define RTM_PRODUCT_SN              "1"
#define RTM_PRODUCT_ASSET_TAG       "No tag"

/*********************************************
 * RTM Compatibility Record
 *********************************************/
#define RTM_COMPATIBILITY_CODE               0x11223344
