#ifndef emmc_h_INCLUDED
#define emmc_h_INCLUDED

#define EMMC_BASE PERIPHERAL_BASE + 300000

typedef struct{

	volatile uint32_t ARG2;				//0x0
	volatile uint32_t BLKSIZECNT;		//0x4
	volatile uint32_t ARG1;				//0x8
	volatile uint32_t CMDTM;			//0xc
	volatile uint32_t RESP0;			//0x10
	volatile uint32_t RESP1;			//0x14
	volatile uint32_t RESP2;			//0x18
	volatile uint32_t RESP3;			//0x1c
	volatile uint32_t DATA;				//0x20
	volatile uint32_t STATUS;			//0x24
	volatile uint32_t CONTROL0;			//0x28
	volatile uint32_t CONTROL1;			//0x2c
	volatile uint32_t INTERRUPT;		//0x30
	volatile uint32_t IRPT_MASK;		//0x34
	volatile uint32_t IRPT_EN;			//0x38
	volatile uint32_t CONTROL2;			//0x3c		//0x40
	volatile uint32_t NOT_USED[4];		//0x40
	volatile uint32_t FORCE_IRPT;		//0x50
	volatile uint32_t NOT_IN_USE[7];	//0x54
	volatile uint32_t BOOT_TIMEOUT;		//0x70
	volatile uint32_t DBG_SEL;			//0x74
	volatile uint32_t NO_USAGE[2]		//0x78
	volatile uint32_t EXRDFIFO_CFG;		//0x80
	volatile uint32_t EXRDFIFO_EN;		//0x84
	volatile uint32_t TUNE_STEP;		//0x88
	volatile uint32_t TUNE_STEPS_STD;	//0x8c
	volatile uint32_t TUNE_STEPS_DDR;	//0x90
	volatile uint32_t NO_USE[23];		//0x94
	volatile uint32_t SPI_INT_SPT;		//0xfo
	volatile uint32_t NOT_IN_USAGE[2]	//0xf4
	volatile uint32_t SLOTISR_VER;		//0xfc
										
}emmc_d;								



#endif // emmc_h_INCLUDED

