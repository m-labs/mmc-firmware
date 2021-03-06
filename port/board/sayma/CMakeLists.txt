#Select which microcontroller and board are being used
#if (NOT TARGET_CONTROLLER)
set(TARGET_CONTROLLER "LPC1776" CACHE STRING "Target MMC Controller")
#endif()

if (NOT TARGET_BOARD_NAME)
  set(TARGET_BOARD_NAME "Sayma" CACHE STRING "Board Name")
endif()

#List all modules used by this board
set(TARGET_MODULES
  "FRU"
  "PAYLOAD"
  "SDR"
  "WATCHDOG"
  "SCANSTA112"
  #"SI5324"
  "HOTSWAP_SENSOR"
  #"EEPROM_AT24MAC"
  #"LM75"
  #"MAX6642"
  #"INA220_VOLTAGE"
  #"INA220_CURRENT"
  "XR77129"
  #"HPM"
  "RTM"
  "ETHERNET"
  "UART_DEBUG"
  "SENSORS"
  )

set(BOARD_PATH ${CMAKE_CURRENT_SOURCE_DIR})

#Include the modules sources
##Sensors SDR list
set( PROJ_SRCS ${PROJ_SRCS} ${BOARD_PATH}/sdr_list.c )
set( PROJ_SRCS ${PROJ_SRCS} ${BOARD_PATH}/i2c_mapping.c )
set( PROJ_SRCS ${PROJ_SRCS} ${BOARD_PATH}/ipmi_oem.c )

if (";${TARGET_MODULES};" MATCHES ";PAYLOAD;")
  set(PROJ_SRCS ${PROJ_SRCS} ${BOARD_PATH}/payload.c)
  set(MODULES_FLAGS "${MODULES_FLAGS} -DMODULE_PAYLOAD")
endif()


#Set the variables in the main scope
set(TARGET_MODULES ${TARGET_MODULES} PARENT_SCOPE)
set(MODULES_FLAGS ${MODULES_FLAGS} PARENT_SCOPE)
set(PROJ_SRCS ${PROJ_SRCS} PARENT_SCOPE)
set(PROJ_HDRS ${PROJ_HDRS} ${BOARD_PATH})
set(PROJ_HDRS ${PROJ_HDRS} PARENT_SCOPE)
