/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* SPDX-License-Identifier: Apache-2.0                                                                     */
/* Copyright(c) 2020 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
/*  ms51_16k.h                                                                         */
/*  Header file for Nuvoton MS51FB9AE / MS51XB9AE / MS51XB9BE                           */
/*--------------------------------------------------------------------------------------*/

/******************************************************************************/
/*                      Macro define  header files                            */
/******************************************************************************/

sfr P0          = 0x80;
sfr SP          = 0x81;
sfr DPL         = 0x82;
sfr DPH         = 0x83;
sfr RCTRIM0     = 0x84;
sfr RCTRIM1     = 0x85;  
sfr RWK         = 0x86;
sfr PCON        = 0x87;

sfr TCON        = 0x88;
sfr TMOD        = 0x89;
sfr TL0         = 0x8A;
sfr TL1         = 0x8B;
sfr TH0         = 0x8C;
sfr TH1         = 0x8D;
sfr CKCON       = 0x8E;
sfr WKCON       = 0x8F;

sfr P1          = 0x90;
sfr SFRS        = 0x91; //TA Protection
sfr CAPCON0     = 0x92;
sfr CAPCON1     = 0x93;
sfr CAPCON2     = 0x94;
sfr CKDIV       = 0x95;
sfr CKSWT       = 0x96; //TA Protection
sfr CKEN        = 0x97; //TA Protection

sfr SCON        = 0x98;
sfr SBUF        = 0x99;
sfr SBUF_1      = 0x9A;
sfr EIE         = 0x9B;
sfr EIE1        = 0x9C;
sfr CHPCON      = 0x9F; //TA Protection

sfr P2          = 0xA0;
sfr AUXR1       = 0xA2;
sfr BODCON0     = 0xA3; //TA Protection
sfr IAPTRG      = 0xA4; //TA Protection
sfr IAPUEN      = 0xA5;  //TA Protection
sfr IAPAL       = 0xA6;
sfr IAPAH       = 0xA7;

sfr IE          = 0xA8;
sfr SADDR       = 0xA9;
sfr WDCON       = 0xAA; //TA Protection
sfr BODCON1     = 0xAB; //TA Protection
sfr P3M1        = 0xAC;
sfr P3S         = 0xAC; //Page1
sfr P3M2        = 0xAD;
sfr P3SR        = 0xAD; //Page1
sfr IAPFD       = 0xAE;
sfr IAPCN       = 0xAF;

sfr P3          = 0xB0;
sfr P0M1        = 0xB1;
sfr P0S         = 0xB1; //Page1
sfr P0M2        = 0xB2;
sfr P0SR        = 0xB2; //Page1
sfr P1M1        = 0xB3;
sfr P1S         = 0xB3; //Page1
sfr P1M2        = 0xB4;
sfr P1SR        = 0xB4; //Page1
sfr P2S         = 0xB5; 
sfr IPH         = 0xB7;
sfr PWMINTC     = 0xB7;  //Page1

sfr IP          = 0xB8;
sfr SADEN       = 0xB9;
sfr SADEN_1     = 0xBA;
sfr SADDR_1     = 0xBB;
sfr I2DAT       = 0xBC;
sfr I2STAT      = 0xBD;
sfr I2CLK       = 0xBE;
sfr I2TOC       = 0xBF;

sfr I2CON       = 0xC0;
sfr I2ADDR      = 0xC1;
sfr ADCRL       = 0xC2;
sfr ADCRH       = 0xC3;
sfr T3CON       = 0xC4;
sfr PWM4H       = 0xC4; //Page1
sfr RL3         = 0xC5;
sfr PWM5H       = 0xC5;  //Page1
sfr RH3         = 0xC6;
sfr PIOCON1     = 0xC6; //Page1
sfr TA          = 0xC7;

sfr T2CON       = 0xC8;
sfr T2MOD       = 0xC9;
sfr RCMP2L      = 0xCA;
sfr RCMP2H      = 0xCB;
sfr TL2         = 0xCC; 
sfr PWM4L       = 0xCC; //Page1
sfr TH2         = 0xCD;
sfr PWM5L       = 0xCD; //Page1
sfr ADCMPL      = 0xCE;
sfr ADCMPH      = 0xCF;

sfr PSW         = 0xD0;
sfr PWMPH       = 0xD1;
sfr PWM0H       = 0xD2;
sfr PWM1H       = 0xD3;
sfr PWM2H       = 0xD4;
sfr PWM3H       = 0xD5;
sfr PNP         = 0xD6;
sfr FBD         = 0xD7;

sfr PWMCON0     = 0xD8;
sfr PWMPL       = 0xD9;
sfr PWM0L       = 0xDA;
sfr PWM1L       = 0xDB;
sfr PWM2L       = 0xDC;
sfr PWM3L       = 0xDD;
sfr PIOCON0     = 0xDE;
sfr PWMCON1     = 0xDF;

sfr ACC         = 0xE0;
sfr ADCCON1     = 0xE1;
sfr ADCCON2     = 0xE2;
sfr ADCDLY      = 0xE3;
sfr C0L         = 0xE4;
sfr C0H         = 0xE5;
sfr C1L         = 0xE6;
sfr C1H         = 0xE7;

sfr ADCCON0     = 0xE8;
sfr PICON       = 0xE9;
sfr PINEN       = 0xEA;
sfr PIPEN       = 0xEB;
sfr PIF         = 0xEC;
sfr C2L         = 0xED;
sfr C2H         = 0xEE;
sfr EIP         = 0xEF;

sfr B           = 0xF0;
sfr CAPCON3     = 0xF1;
sfr CAPCON4     = 0xF2;
sfr SPCR        = 0xF3;
sfr SPCR2       = 0xF3; //Page1
sfr SPSR        = 0xF4;
sfr SPDR        = 0xF5;
sfr AINDIDS     = 0xF6;
sfr EIPH        = 0xF7;

sfr SCON_1      = 0xF8;
sfr PDTEN       = 0xF9; //TA Protection
sfr PDTCNT      = 0xFA; //TA Protection
sfr PMEN        = 0xFB;
sfr PMD         = 0xFC;
sfr EIP1        = 0xFE;
sfr EIPH1       = 0xFF;


/**************define for bit**************/
/*P0*/
sbit P00        = P0^0;
sbit P01        = P0^1;
sbit P02        = P0^2;
sbit P03        = P0^3;
sbit P04        = P0^4;
sbit P05        = P0^5;
sbit P06        = P0^6;
sbit P07        = P0^7;

/*P1*/
sbit P10        = P1^0;
sbit P11        = P1^1;
sbit P12        = P1^2;
sbit P13        = P1^3;
sbit P14        = P1^4;
sbit P15        = P1^5;
sbit P16        = P1^6;
sbit P17        = P1^7;

/*P2*/
sbit P20        = P2^0;
sbit P21        = P2^1;
sbit P22        = P2^2;
sbit P23        = P2^3;
sbit P24        = P2^4;
sbit P25        = P2^5;
sbit P26        = P2^6;
sbit P27        = P2^7;

/*P3*/
sbit P30        = P3^0;

/*  IE  */
sbit EA         = IE^7;
sbit EADC       = IE^6;
sbit EBOD       = IE^5;
sbit ES         = IE^4;
sbit ET1        = IE^3;
sbit EX1        = IE^2;
sbit ET0        = IE^1;
sbit EX0        = IE^0;

/*TMOD*/

#define GATE1		7
#define CT1   	6
#define M11			5
#define M01  	  4
#define GATE0		3
#define CT0  	  2
#define M10			1
#define M00   	0

/*TCON*/
#define TF1			7
#define TR1 		6
#define TF0			5
#define TR0 		4
#define IE1			3
#define CT0 		2
#define M10			1
#define M00 		0

/*CKCON*/
#define T1OE		5
#define T1M			4
#define T0M			3
#define T0OE		2

/*T2CON*/
#define TF2			7
#define TR2			2
#define RL2			0

/*T2MOD*/
#define LDEN		7
#define T2DIV2	6
#define T2DIV1	5
#define T2DIV0	4
#define CAPCR	  3
#define CMPCR		2
#define LDTS1		1
#define LDTS0		0


/*T3CON*/
#define TF3 		4
#define TR3 		3
#define T3PS2		2
#define T3PS1		1
#define T3PS0		0


/*ADCCON0*/
#define ADCF		7
#define ADCS		6
#define ETGSEL1	5
#define ETGSEL0	4
#define ADCHS3  3
#define ADCHS2	2
#define ADCHS1	1
#define ADCHS0	0


/*ADCCON1*/

#define STADCPX	6
#define ADCDIV1	5
#define ADCDIV0	4
#define ETGTYP1 3
#define ETGTYP0	2
#define ADCEX		1
#define ADCEN		0

/*ADCCON2*/

#define ADFBEN	7
#define ADCMPOP	6
#define ADCMPEN	5
#define ADCMPO	4
#define ADCAQT2 3
#define ADCAQT1	2
#define ADCAQT0	1
#define ADCDLY8	0

typedef unsigned char uint8_t;
typedef unsigned char u8;

typedef unsigned int uint16_t;
typedef unsigned int u16;

typedef unsigned long uint32_t;
typedef unsigned long u32;


/****SFRS    91H ****/
//bit BIT_TMP;

#define set_SFRS_SFRPAGE         BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;SFRS=1;EA=BIT_TMP
#define clr_SFRS_SFRPAGE         BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;SFRS=0;EA=BIT_TMP
#define clr_T3CON_TF3            T3CON&=0xEF
#define clr_ADCCON0_ADCF         ADCCON0 &= ~(1 << ADCF)

/****************MACRO FOR GPIO*****************/

//------------------- Define Port as Push Pull mode -------------------
#define    P00_PUSHPULL_MODE         P0M1&=0xFE;P0M2|=0x01
#define    P01_PUSHPULL_MODE         P0M1&=0xFD;P0M2|=0x02
#define    P02_PUSHPULL_MODE         P0M1&=0xFB;P0M2|=0x04
#define    P03_PUSHPULL_MODE         P0M1&=0xF7;P0M2|=0x08
#define    P04_PUSHPULL_MODE         P0M1&=0xEF;P0M2|=0x10
#define    P05_PUSHPULL_MODE         P0M1&=0xDF;P0M2|=0x20
#define    P06_PUSHPULL_MODE         P0M1&=0xBF;P0M2|=0x40
#define    P07_PUSHPULL_MODE         P0M1&=0x7F;P0M2|=0x80
#define    P10_PUSHPULL_MODE         P1M1&=0xFE;P1M2|=0x01
#define    P11_PUSHPULL_MODE         P1M1&=0xFD;P1M2|=0x02
#define    P12_PUSHPULL_MODE         P1M1&=0xFB;P1M2|=0x04
#define    P13_PUSHPULL_MODE         P1M1&=0xF7;P1M2|=0x08
#define    P14_PUSHPULL_MODE         P1M1&=0xEF;P1M2|=0x10
#define    P15_PUSHPULL_MODE         P1M1&=0xDF;P1M2|=0x20
#define    P16_PUSHPULL_MODE         P1M1&=0xBF;P1M2|=0x40
#define    P17_PUSHPULL_MODE         P1M1&=0x7F;P1M2|=0x80
#define    P30_PUSHPULL_MODE         P3M1&=0xFE;P3M2|=0x01
#define    ALL_GPIO_PUSHPULL_MODE    P0M1=0;P0M2=0xFF;P1M1=0;P1M2=0xFF;P3M1=0;P3M2=0xFF
//------------------- Define Port as Input Only mode -------------------
#define    P00_INPUT_MODE            P0M1|=0x01;P0M2&=0xFE
#define    P01_INPUT_MODE            P0M1|=0x02;P0M2&=0xFD
#define    P02_INPUT_MODE            P0M1|=0x04;P0M2&=0xFB
#define    P03_INPUT_MODE            P0M1|=0x08;P0M2&=0xF7
#define    P04_INPUT_MODE            P0M1|=0x10;P0M2&=0xEF
#define    P05_INPUT_MODE            P0M1|=0x20;P0M2&=0xDF
#define    P06_INPUT_MODE            P0M1|=0x40;P0M2&=0xBF
#define    P07_INPUT_MODE            P0M1|=0x80;P0M2&=0x7F
#define    P10_INPUT_MODE            P1M1|=0x01;P1M2&=0xFE
#define    P11_INPUT_MODE            P1M1|=0x02;P1M2&=0xFD
#define    P12_INPUT_MODE            P1M1|=0x04;P1M2&=0xFB
#define    P13_INPUT_MODE            P1M1|=0x08;P1M2&=0xF7
#define    P14_INPUT_MODE            P1M1|=0x10;P1M2&=0xEF
#define    P15_INPUT_MODE            P1M1|=0x20;P1M2&=0xDF
#define    P16_INPUT_MODE            P1M1|=0x40;P1M2&=0xBF
#define    P17_INPUT_MODE            P1M1|=0x80;P1M2&=0x7F
#define    P30_INPUT_MODE            P3M1|=0x01;P3M2&=0xFE
#define    ALL_GPIO_INPUT_MODE       P0M1=0xFF;P0M2=0;P1M1=0xFF;P1M2=0;P3M1=0xFF;P3M2=0

//------------------------------- for I2C ---------------------------//

#define I2C_SET_START()				I2CON |= (1 << 5)
#define I2C_CLEAR_START()			I2CON &= ~(1 << 5)
#define I2C_SET_STOP()				I2CON |= (1 << 4)
#define I2C_CLEAR_STOP()			I2CON &= ~(1 << 4)
#define I2C_SET_SI()					I2CON |= (1 << 3)
#define I2C_CLEAR_SI()				I2CON &= ~(1 << 3)
#define I2C_SET_AA()					I2CON |= (1 << 2)
#define I2C_CLEAR_AA()				I2CON &= ~(1 << 2)


