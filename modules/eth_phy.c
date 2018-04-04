/*
 * eth_phy.c
 *
 *  Created on: Feb 8, 2018
 *      Author: wizath
 */

#include "eth_phy.h"
#include "chip_lpc177x_8x.h"

#include "enet_17xx_40xx.h"

/*------------- Ethernet Media Access Controller (EMAC) ----------------------*/
typedef struct
{
  __IO uint32_t MAC1;                   /* MAC Registers                      */
  __IO uint32_t MAC2;
  __IO uint32_t IPGT;
  __IO uint32_t IPGR;
  __IO uint32_t CLRT;
  __IO uint32_t MAXF;
  __IO uint32_t SUPP;
  __IO uint32_t TEST;
  __IO uint32_t MCFG;
  __IO uint32_t MCMD;
  __IO uint32_t MADR;
  __O  uint32_t MWTD;
  __I  uint32_t MRDD;
  __I  uint32_t MIND;
       uint32_t RESERVED0[2];
  __IO uint32_t SA0;
  __IO uint32_t SA1;
  __IO uint32_t SA2;
       uint32_t RESERVED1[45];
  __IO uint32_t Command;                /* Control Registers                  */
  __I  uint32_t Status;
  __IO uint32_t RxDescriptor;
  __IO uint32_t RxStatus;
  __IO uint32_t RxDescriptorNumber;
  __I  uint32_t RxProduceIndex;
  __IO uint32_t RxConsumeIndex;
  __IO uint32_t TxDescriptor;
  __IO uint32_t TxStatus;
  __IO uint32_t TxDescriptorNumber;
  __IO uint32_t TxProduceIndex;
  __I  uint32_t TxConsumeIndex;
       uint32_t RESERVED2[10];
  __I  uint32_t TSV0;
  __I  uint32_t TSV1;
  __I  uint32_t RSV;
       uint32_t RESERVED3[3];
  __IO uint32_t FlowControlCounter;
  __I  uint32_t FlowControlStatus;
       uint32_t RESERVED4[34];
  __IO uint32_t RxFilterCtrl;           /* Rx Filter Registers                */
  __I  uint32_t RxFilterWoLStatus;
  __O  uint32_t RxFilterWoLClear;
       uint32_t RESERVED5;
  __IO uint32_t HashFilterL;
  __IO uint32_t HashFilterH;
       uint32_t RESERVED6[882];
  __I  uint32_t IntStatus;              /* Module Control Registers           */
  __IO uint32_t IntEnable;
  __O  uint32_t IntClear;
  __O  uint32_t IntSet;
       uint32_t RESERVED7;
  __IO uint32_t PowerDown;
       uint32_t RESERVED8;
  __IO uint32_t Module_ID;
} LPC_EMAC_TypeDef;

#define LPC_EMAC              ((LPC_EMAC_TypeDef *) LPC_ENET_BASE )

void phy_init(void)
{
	printf("etyhernet %d\n", ((LPC_ENET_T *) LPC_ETHERNET));

	LPC_EMAC->TxDescriptorNumber;

//	pENET->CONTROL.COMMAND = ENET_COMMAND_REGRESET | ENET_COMMAND_TXRESET | ENET_COMMAND_RXRESET |
//							 ENET_COMMAND_PASSRUNTFRAME;

//	// init ETH PHY in RGMII mode
//	// RESET PHY
//	gpio_set_pin_state( PIN_PORT(GPIO_PHY_RESETn), PIN_NUMBER(GPIO_PHY_RESETn), GPIO_LEVEL_LOW );
//
//	// SET RGMII = 1 -> RGMII mode
//	gpio_set_pin_state( PIN_PORT(GPIO_PHY_RGMII_SEL), PIN_NUMBER(GPIO_PHY_RGMII_SEL), GPIO_LEVEL_LOW );
//
//	// PHY SPD 0 = 0
//	gpio_set_pin_state( PIN_PORT(GPIO_PHY_CFG_SPD0), PIN_NUMBER(GPIO_PHY_CFG_SPD0), GPIO_LEVEL_LOW );
//
//	// PHY SPD 1 = 1
//	gpio_set_pin_state( PIN_PORT(GPIO_PHY_CFG_SPD1), PIN_NUMBER(GPIO_PHY_CFG_SPD1), GPIO_LEVEL_HIGH );
//
//	// CFG DDR = 1
//	gpio_set_pin_state( PIN_PORT(GPIO_PHY_CFG_DDR), PIN_NUMBER(GPIO_PHY_CFG_DDR), GPIO_LEVEL_HIGH );
//
//	// un-RESET PHY
//	gpio_set_pin_state( PIN_PORT(GPIO_PHY_RESETn), PIN_NUMBER(GPIO_PHY_RESETn), GPIO_LEVEL_HIGH );
//
//	// MII LED OFF
//	gpio_set_pin_state( PIN_PORT(GPIO_PHY_MII_MODE_LED), PIN_NUMBER(GPIO_PHY_MII_MODE_LED), GPIO_LEVEL_LOW );
//
//	// configure mdio pins
//	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 16, (IOCON_FUNC1));
//	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 17, (IOCON_FUNC1));
//
//	Chip_ENET_DeInit(LPC_ETHERNET);
//	Chip_ENET_Init(LPC_ETHERNET, true);
//
//	// select page 2
//	phy_write(0x04, 31, 0x12);
//	printf("31 %d\n", 0, phy_read(0x4, 31));
//
//	// power down Rx CDR
//	phy_write(0x04,  16, 0x4004);
//
//	asm("NOP");
//	asm("NOP");
//	asm("NOP");
//
//	// power up Rx CDR
//	phy_write(0x04, 16, 0x4000);
//	phy_write(0x04, 0, 0x8000); // reset the data path BMCR.DP_RST
//
//	// select page 0
//	phy_write(0x04,  31, 0x10);
//
//	// force RGMII, TXCLKEN=0
//	phy_write(0x04,  18, 0b1000100010000000 );
//
//	printf("GMIICR mod %d 0x%x\n", 18, phy_read(0x4, 18));
//	printf("PHY init done\n");
//
//	// SET RGMII = 1 -> FPGA mode
//	gpio_set_pin_state( PIN_PORT(GPIO_PHY_RGMII_SEL), PIN_NUMBER(GPIO_PHY_RGMII_SEL), GPIO_LEVEL_HIGH );
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
