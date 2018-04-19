/*
 * eth_phy.c
 *
 *  Created on: Feb 8, 2018
 *      Author: wizath
 */

#include "eth_phy.h"
#include "chip_lpc177x_8x.h"
#include "enet_17xx_40xx.h"

void phy_init(void)
{
	// init pins
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0x1, 0, (IOCON_FUNC1 | IOCON_MODE_INACT));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0x1, 1, (IOCON_FUNC1 | IOCON_MODE_INACT));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0x1, 4, (IOCON_FUNC1 | IOCON_MODE_INACT));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0x1, 8, (IOCON_FUNC1 | IOCON_MODE_INACT));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0x1, 9, (IOCON_FUNC1 | IOCON_MODE_INACT));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0x1, 10, (IOCON_FUNC1 | IOCON_MODE_INACT));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0x1, 14, (IOCON_FUNC1 | IOCON_MODE_INACT));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0x1, 15, (IOCON_FUNC1 | IOCON_MODE_INACT));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0x1, 16, (IOCON_FUNC1 | IOCON_MODE_INACT));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0x1, 17, (IOCON_FUNC1 | IOCON_MODE_INACT));

	Chip_ENET_DeInit(LPC_ETHERNET);
	Chip_ENET_Init(LPC_ETHERNET, true);

	// init ETH PHY in RGMII mode
	// RESET PHY
	gpio_set_pin_state( PIN_PORT(GPIO_PHY_RESETn), PIN_NUMBER(GPIO_PHY_RESETn), GPIO_LEVEL_LOW );

	// SET RGMII = 1 -> RGMII mode
	gpio_set_pin_state( PIN_PORT(GPIO_PHY_RGMII_SEL), PIN_NUMBER(GPIO_PHY_RGMII_SEL), GPIO_LEVEL_LOW );

	// PHY SPD 0 = 0
	gpio_set_pin_state( PIN_PORT(GPIO_PHY_CFG_SPD0), PIN_NUMBER(GPIO_PHY_CFG_SPD0), GPIO_LEVEL_LOW );

	// PHY SPD 1 = 1
	gpio_set_pin_state( PIN_PORT(GPIO_PHY_CFG_SPD1), PIN_NUMBER(GPIO_PHY_CFG_SPD1), GPIO_LEVEL_HIGH );

	// CFG DDR = 1
	gpio_set_pin_state( PIN_PORT(GPIO_PHY_CFG_DDR), PIN_NUMBER(GPIO_PHY_CFG_DDR), GPIO_LEVEL_HIGH );

	// un-RESET PHY
	gpio_set_pin_state( PIN_PORT(GPIO_PHY_RESETn), PIN_NUMBER(GPIO_PHY_RESETn), GPIO_LEVEL_HIGH );

	// MII LED OFF
	gpio_set_pin_state( PIN_PORT(GPIO_PHY_MII_MODE_LED), PIN_NUMBER(GPIO_PHY_MII_MODE_LED), GPIO_LEVEL_LOW );

	vTaskDelay(100);

	// select page 2
	phy_write(0x04, 31, 0x12);
	printf("31 %d\n", 0, phy_read(0x4, 31));

	// power down Rx CDR
	phy_write(0x04,  16, 0x4004);

	vTaskDelay(100);

	// power up Rx CDR
	phy_write(0x04, 16, 0x4000);
	phy_write(0x04, 0, 0x8000); // reset the data path BMCR.DP_RST

	// select page 0
	phy_write(0x04,  31, 0x10);

	// force RGMII, TXCLKEN=0
	phy_write(0x04,  18, 0b1000100010000000 );

	printf("GMIICR mod %d 0x%x\n", 18, phy_read(0x4, 18));
	printf("PHY init done\n");

	// SET RGMII = 1 -> FPGA mode
	gpio_set_pin_state( PIN_PORT(GPIO_PHY_RGMII_SEL), PIN_NUMBER(GPIO_PHY_RGMII_SEL), GPIO_LEVEL_HIGH );
}

uint16_t phy_read(uint16_t bPhyAddr, uint8_t PhyReg)
{
	unsigned int tout;

	LPC_ETHERNET->MAC.MADR = ((bPhyAddr & 0x1F) << 8 ) | (PhyReg & 0x1F);
	LPC_ETHERNET->MAC.MCMD = ENET_MCMD_READ;

	/* Wait until operation completed */
	tout = 0;
	for (tout = 0; tout < ENET_MII_RD_TOUT; tout++)
	{
		if ((LPC_ETHERNET->MAC.MIND & ENET_MIND_BUSY) == 0)
		{
			break;
		}
	}

	LPC_ETHERNET->MAC.MCMD = 0;
	return (LPC_ETHERNET->MAC.MRDD);
}

void phy_write(uint16_t bPhyAddr,uint8_t PhyReg, uint16_t Value)
{
	unsigned int tout;

	LPC_ETHERNET->MAC.MADR = ((bPhyAddr & 0x1F) << 8 ) | (PhyReg & 0x1F);
	LPC_ETHERNET->MAC.MWTD = Value;

	/* Wait utill operation completed */
	tout = 0;

	for (tout = 0; tout < ENET_MII_WR_TOUT; tout++)
	{
		if ((LPC_ETHERNET->MAC.MIND & ENET_MIND_BUSY) == 0)
		{
			break;
		}
	}
}

void phy_delay_set(void)
{
	printf("Setting PHY Delay\n");

	gpio_set_pin_state( PIN_PORT(GPIO_PHY_RGMII_SEL), PIN_NUMBER(GPIO_PHY_RGMII_SEL), GPIO_LEVEL_LOW );

	uint16_t value = phy_read(0x4, 4);

	// set PAUSE to 11
	value |= 1UL << 8;
	value |= 1UL << 7;

	phy_write(0x4, 4, value);

	//SET RGMII = 1 -> RMII mode
	gpio_set_pin_state( PIN_PORT(GPIO_PHY_RGMII_SEL), PIN_NUMBER(GPIO_PHY_RGMII_SEL), GPIO_LEVEL_HIGH );
}

void phy_dump(void)
{
	//SET RGMII = 1 -> MII mode
	gpio_set_pin_state( PIN_PORT(GPIO_PHY_RGMII_SEL), PIN_NUMBER(GPIO_PHY_RGMII_SEL), GPIO_LEVEL_LOW );

	// EMAC_Configs.bPhyAddr   = 0x4;
	printf("------------ PHY Dump ----------\n");
	printf("Register, ADDR DATA\n");

	phy_write(0x04,  31, 0x10); //page 0
	printf("BMCR %d 0x%x\n", 0, phy_read(0x4, 0));
	printf("BMSR %d 0x%x\n", 1, phy_read(0x4, 1));
	printf("ID1 %d 0x%x\n", 2, phy_read(0x4, 2));
	printf("ID2 %d 0x%x\n", 3, phy_read(0x4, 3));
	printf("AN_ADV %d 0x%x\n", 4, phy_read(0x4, 4));
	printf("AN_RX %d 0x%x\n", 5, phy_read(0x4, 5));
	printf("AN_EXP %d 0x%x\n", 6, phy_read(0x4, 6));
	printf("EXT_STAT %d 0x%x\n", 15, phy_read(0x4, 15));
	printf("page0\n");
	printf("JIT_DIAG %d 0x%x\n", 16, phy_read(0x4, 16));
	printf("PCSCR %d 0x%x\n", 17, phy_read(0x4, 17));
	printf("GMIICR %d 0x%x\n", 18, phy_read(0x4, 18));
	printf("CR %d 0x%x\n", 19, phy_read(0x4, 19));
	printf("IR %d 0x%x\n", 20, phy_read(0x4, 20));
	printf("page1\n");
	phy_write(0x04,  31, 0x11); //page 1
	printf("ID %d 0x%x\n", 16, phy_read(0x4, 16));
	printf("GPIOCR1 %d 0x%x\n", 17, phy_read(0x4, 17));
	printf("GPIOCR2 %d 0x%x\n", 18, phy_read(0x4, 18));
	printf("GPIOSR %d 0x%x\n", 19, phy_read(0x4, 19));
	printf("PTPCR1 %d 0x%x\n", 20, phy_read(0x4, 20));
	printf("page2\n");
	phy_write(0x04,  31, 0x12); //page 1
	printf("PTPCR1 %d 0x%x\n", 16, phy_read(0x4, 16));
	printf("?? %d 0x%x\n", 17, phy_read(0x4, 17));
	printf("?? %d 0x%x\n", 18, phy_read(0x4, 18));
	printf("?? %d 0x%x\n", 19, phy_read(0x4, 19));
	printf("?? %d 0x%x\n", 20, phy_read(0x4, 20));
	printf("page3\n");
	phy_write(0x04,  31, 0x13); //page 1
	printf("?? %d 0x%x\n", 16, phy_read(0x4, 16));
	printf("?? %d 0x%x\n", 17, phy_read(0x4, 17));
	printf("?? %d 0x%x\n", 18, phy_read(0x4, 18));
	printf("?? %d 0x%x\n", 19, phy_read(0x4, 19));
	printf("?? %d 0x%x\n", 20, phy_read(0x4, 20));

	//SET RGMII = 1 -> RMII mode
	gpio_set_pin_state( PIN_PORT(GPIO_PHY_RGMII_SEL), PIN_NUMBER(GPIO_PHY_RGMII_SEL), GPIO_LEVEL_HIGH );
}

void phy_scan(void)
{
	uint16_t recv = 0x0;
	for (int i = 0; i < 32; i++)
	{
		phy_write(i,  31, 0x10);
		recv = phy_read(i, 18);
		if (recv != 0xFFFF) printf("PHY Scan [0x%02X GMIICR] = 0x%02X\n", i, recv);
	}
	printf("Scan done\n");
}
