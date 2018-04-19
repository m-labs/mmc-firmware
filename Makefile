PWD := $(shell pwd)
BUILD_DIR := $(PWD)/build
SOURCE_DIR := $(PWD)
RM := rm -rf

all: default
default:
	mkdir -p build
	cmake -B$(BUILD_DIR) -H$(SOURCE_DIR) -DBOARD=sayma -DBOARD_RTM=sayma -DTARGET_CONTROLLER=LPC1776 -DCMAKE_BUILD_TYPE=Debug
	make -C$(BUILD_DIR) -j8

clean:
	rm -r build/

flash:
	/opt/LPCXpresso/lpcxpresso/bin/crt_emu_cm_redlink -flash-load-exec "build/out/openMMC.axf" -g -2  -vendor=NXP -pLPC1776    -reset=vectreset -flash-driver=LPC177x_8x_407x_8x_256.cfx
