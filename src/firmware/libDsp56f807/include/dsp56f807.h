/*
 * dsp56f807 include file.
 *
 */

#ifndef __dsp56f807h__
#define __dsp56f807h__

#define  FALSE  0
#define  TRUE   1

/* beware of true/false definitions */
#ifndef __ONLY_DEF
#define false 0
#define true 1
#endif

#ifndef NULL
  #define  NULL   0
#endif

/* 
 * Motorola types.
 */
typedef unsigned char bool;
typedef unsigned char byte;
typedef unsigned int word;
typedef unsigned long dword;
typedef unsigned long dlong[2];

typedef char           Word8;
typedef unsigned char  UWord8;
typedef short          Word16;
typedef unsigned short UWord16;
typedef long           Word32;
typedef unsigned long  UWord32;
typedef char           Int8;
typedef unsigned char  UInt8;
typedef int            Int16;
typedef unsigned int   UInt16;
typedef long           Int32;
typedef unsigned long  UInt32;
typedef __typeof__(sizeof(0)) ssize_t;

extern volatile word __dummy;

/* set or get a register */ 
#define setReg(addr, value) 		(((*(word *)addr)) = (word)value)
#define touchReg(addr) 				(__dummy = *(char *)addr)
#define getReg(addr) 				(__dummy = *(char *)addr)

/* set/clr a bit by name */
#define setRegBit(addr, mask)		((*(char *)addr) |= addr##_##mask##_MASK)
#define clrRegBit(addr, mask)		((*(char *)addr) &= ~addr##_##mask##_MASK)
#define getRegBit(addr, mask)		(((__dummy = *(char *)addr) & addr##_##mask##_MASK)?1:0)

/* set/clr bits by mask */
#define setRegBits(addr, mask)		((*(char *)addr) |= (mask))
#define clrRegBits(addr, mask)		((*(char *)addr) &= ~(mask))
#define getRegBits(addr, mask)		(((__dummy = *(char *)addr) & (mask))?1:0)

#define MAX_32 (Word32)0x7fffffffL
#define MIN_32 (Word32)0x80000000L

#define MAX_16 (Word16)0x7fff
#define MIN_16 (Word16)0x8000

/******************************************************* 

   Predefined basic intrinsics. 

   Builtin support for these functions will be implemented 
   in the CodeWarrior C compiler code generator in Release 3.0.

   The intrinsic functions are defined in the compiler
   defined functions name space. They are redefined here
   according to the ETSI naming convention.

 ******************************************************/

/************************************/
/* Fractional arithmetic primitives */
/************************************/

#define add             __add
#define sub             __sub
#define abs_s           __abs
#define mult            __mult
#define mult_r          __mult_r
#define negate          __negate
#define extract_h       __extract_h
#define round           __round
#define extract_l       __extract_l
#define shl             __shl
#define shr             __shr
#define div_s           __div

/*****************************************/
/* Long Fractional arithmetic primitives */
/*****************************************/

#define L_add           _L_add
#define L_sub           _L_sub
#define L_negate        _L_negate
#define L_abs           __labs
#define mac_r           __mac_r
#define msu_r           __msu_r
#define L_mult          _L_mult
#define L_mac           _L_mac
#define L_msu           _L_msu
#define L_shl           _L_shl
#define L_shr           _L_shr
#define shr_r           __shr_r
#define L_deposit_l     _L_deposit_l
#define L_deposit_h     _L_deposit_h
#define norm_s          __norm_s
#define norm_l          __norm_l

/* defined by MetroWerks but not by ITU */
#define div_ls          __div_ls
#define L_mult_ls       _L_mult_ls

/* To resolve bug in MetroWerks CW 3.5 */
Word32 L_shr_r(Word32 L_var1, Word16 var2);
//Word32 L_deposit_h(Word16 var1);

/* disable/enable global interrupts */
extern volatile word SR_reg;
extern volatile word SR_lock;

#define SaveStatusReg()     { asm(move SR,SR_reg); asm(bfset  #0x0300,SR); asm(nop); asm(nop); } /* Save status register and disable interrupts */
#define RestoreStatusReg()  { asm(move SR_reg,SR); asm(nop); asm(nop); } /* Restore status register  */
#define EnterCritical()     { if (++SR_lock == 1) SaveStatusReg(); } /* Save status register and disable interrupts */
#define ExitCritical()      { if (--SR_lock == 0) RestoreStatusReg(); } /* Restore status register  */

/* enable/disable global interrupts */
#define __EI0() { asm(bfset  #0x0100,SR); asm(bfclr  #0x0200,SR); } /* Enable interrupts of level 0,1 */
#define __EI(level)  __EI0()           /* Enable interrupts of level 0,1 */
#define __DI() { asm(bfset  #0x0300,SR); } /* Disable interrupts, only level 1 is allowed */

/**
 * enables a specific interrupt.
 * @param x is the interrupt number (0 to 63).
 * @param level is the interrupt level (0: disabled, 1 to 7 level).
 */
#define __ENIGROUP(x, level) \
{ \
	int mask; \
	int in = (x & 0x0003) << 2; \
	int grp = x >> 2; \
	(*(char *)(GPR0+grp)) &= ~(0x0f << in); \
	(*(char *)(GPR0+grp)) |= (level << in); \
}

//	if (level > 0) \
//	{ mask = (0x01) << (15 - level - 1); setRegBits (IPR, mask); } \


/*
 * register names
 *
 */

#define IPR    					0xFFFB  		// Interrupt priority register
#define IPR_CH0_MASK			0x8000
#define IPR_CH1_MASK			0x4000
#define IPR_CH2_MASK			0x2000
#define IPR_CH3_MASK			0x1000
#define IPR_CH4_MASK			0x0800
#define IPR_CH5_MASK			0x0400
#define IPR_CH6_MASK			0x0200
#define IPR_IBL1_MASK			0x0020
#define IPR_IBL0_MASK			0x0010
#define IPR_IBINV_MASK			0x0008
#define IPR_IAL1_MASK			0x0004
#define IPR_IAL0_MASK			0x0002
#define IPR_IAINV_MASK			0x0001

#define BCR						0xFFF9			// Bus control register
 
 
#define SYS_BASE				0x1000			/* the base of the periph. regs */
#define SYS_CNTL				SYS_BASE		
#define SYS_STS					(SYS_BASE+1)
#define MSH_ID					(SYS_BASE+6)
#define LSH_ID					(SYS_BASE+7)
#define TST_REG0				(SYS_BASE+0x18)
#define TST_REG1				(SYS_BASE+0x19)
#define TST_REG2				(SYS_BASE+0x1A)
#define TST_REG3				(SYS_BASE+0x1B)
#define TST_REG4				(SYS_BASE+0x1C)

#define TMRA_BASE				0x1100
#define TMRA0_CMP1				(TMRA_BASE)
#define TMRA0_CMP2				(TMRA_BASE+0x01)
#define TMRA0_CAP				(TMRA_BASE+0x02)
#define TMRA0_LOAD				(TMRA_BASE+0x03)
#define TMRA0_HOLD				(TMRA_BASE+0x04)
#define TMRA0_CNTR				(TMRA_BASE+0x05)

#define TMRA0_CTRL				(TMRA_BASE+0x06)
#define TMRA0_CTRL_CO_INIT_MASK 0x0008
#define TMRA0_CTRL_DIR_MASK		0x0010
#define TMRA0_CTRL_LENGTH_MASK	0x0020
#define TMRA0_CTRL_ONCE_MASK	0x0040

#define TMRA0_SCR				(TMRA_BASE+0x07)
#define TMRA0_SCR_OEN_MASK		0x0001
#define TMRA0_SCR_OPS_MASK		0x0002
#define TMRA0_SCR_FORCE_MASK	0x0004
#define TMRA0_SCR_VAL_MASK		0x0008
#define TMRA0_SCR_EEOF_MASK		0x0010
#define TMRA0_SCR_MSTR_MASK		0x0020
#define TMRA0_SCR_INPUT_MASK	0x0100
#define TMRA0_SCR_IPS_MASK		0x0200
#define TMRA0_SCR_IEFIE_MASK	0x0400
#define TMRA0_SCR_IEF_MASK		0x0800
#define TMRA0_SCR_TOFIE_MASK	0x1000
#define TMRA0_SCR_TOF_MASK		0x2000
#define TMRA0_SCR_TCFIE_MASK	0x4000
#define TMRA0_SCR_TCF_MASK		0x8000

#define TMRA1_CMP1				(TMRA_BASE+0x08)
#define TMRA1_CMP2				(TMRA_BASE+0x09)
#define TMRA1_CAP				(TMRA_BASE+0x0A)
#define TMRA1_LOAD				(TMRA_BASE+0x0B)
#define TMRA1_HOLD				(TMRA_BASE+0x0C)
#define TMRA1_CNTR				(TMRA_BASE+0x0D)

#define TMRA1_CTRL				(TMRA_BASE+0x0E)
#define TMRA1_CTRL_CO_INIT_MASK 0x0008
#define TMRA1_CTRL_DIR_MASK		0x0010
#define TMRA1_CTRL_LENGTH_MASK	0x0020
#define TMRA1_CTRL_ONCE_MASK	0x0040

#define TMRA1_SCR				(TMRA_BASE+0x0F)
#define TMRA1_SCR_OEN_MASK		0x0001
#define TMRA1_SCR_OPS_MASK		0x0002
#define TMRA1_SCR_FORCE_MASK	0x0004
#define TMRA1_SCR_VAL_MASK		0x0008
#define TMRA1_SCR_EEOF_MASK		0x0010
#define TMRA1_SCR_MSTR_MASK		0x0020
#define TMRA1_SCR_INPUT_MASK	0x0100
#define TMRA1_SCR_IPS_MASK		0x0200
#define TMRA1_SCR_IEFIE_MASK	0x0400
#define TMRA1_SCR_IEF_MASK		0x0800
#define TMRA1_SCR_TOFIE_MASK	0x1000
#define TMRA1_SCR_TOF_MASK		0x2000
#define TMRA1_SCR_TCFIE_MASK	0x4000
#define TMRA1_SCR_TCF_MASK		0x8000

#define TMRA2_CMP1				(TMRA_BASE+0x10)
#define TMRA2_CMP2				(TMRA_BASE+0x11)
#define TMRA2_CAP				(TMRA_BASE+0x12)
#define TMRA2_LOAD				(TMRA_BASE+0x13)
#define TMRA2_HOLD				(TMRA_BASE+0x14)
#define TMRA2_CNTR				(TMRA_BASE+0x15)

#define TMRA2_CTRL				(TMRA_BASE+0x16)
#define TMRA2_CTRL_CO_INIT_MASK 0x0008
#define TMRA2_CTRL_DIR_MASK		0x0010
#define TMRA2_CTRL_LENGTH_MASK	0x0020
#define TMRA2_CTRL_ONCE_MASK	0x0040

#define TMRA2_SCR				(TMRA_BASE+0x17)
#define TMRA2_SCR_OEN_MASK		0x0001
#define TMRA2_SCR_OPS_MASK		0x0002
#define TMRA2_SCR_FORCE_MASK	0x0004
#define TMRA2_SCR_VAL_MASK		0x0008
#define TMRA2_SCR_EEOF_MASK		0x0010
#define TMRA2_SCR_MSTR_MASK		0x0020
#define TMRA2_SCR_INPUT_MASK	0x0100
#define TMRA2_SCR_IPS_MASK		0x0200
#define TMRA2_SCR_IEFIE_MASK	0x0400
#define TMRA2_SCR_IEF_MASK		0x0800
#define TMRA2_SCR_TOFIE_MASK	0x1000
#define TMRA2_SCR_TOF_MASK		0x2000
#define TMRA2_SCR_TCFIE_MASK	0x4000
#define TMRA2_SCR_TCF_MASK		0x8000

#define TMRA3_CMP1				(TMRA_BASE+0x18)
#define TMRA3_CMP2				(TMRA_BASE+0x19)
#define TMRA3_CAP				(TMRA_BASE+0x1A)
#define TMRA3_LOAD				(TMRA_BASE+0x1B)
#define TMRA3_HOLD				(TMRA_BASE+0x1C)
#define TMRA3_CNTR				(TMRA_BASE+0x1D)

#define TMRA3_CTRL				(TMRA_BASE+0x1E)
#define TMRA3_CTRL_CO_INIT_MASK 0x0008
#define TMRA3_CTRL_DIR_MASK		0x0010
#define TMRA3_CTRL_LENGTH_MASK	0x0020
#define TMRA3_CTRL_ONCE_MASK	0x0040

#define TMRA3_SCR				(TMRA_BASE+0x1F)
#define TMRA3_SCR_OEN_MASK		0x0001
#define TMRA3_SCR_OPS_MASK		0x0002
#define TMRA3_SCR_FORCE_MASK	0x0004
#define TMRA3_SCR_VAL_MASK		0x0008
#define TMRA3_SCR_EEOF_MASK		0x0010
#define TMRA3_SCR_MSTR_MASK		0x0020
#define TMRA3_SCR_INPUT_MASK	0x0100
#define TMRA3_SCR_IPS_MASK		0x0200
#define TMRA3_SCR_IEFIE_MASK	0x0400
#define TMRA3_SCR_IEF_MASK		0x0800
#define TMRA3_SCR_TOFIE_MASK	0x1000
#define TMRA3_SCR_TOF_MASK		0x2000
#define TMRA3_SCR_TCFIE_MASK	0x4000
#define TMRA3_SCR_TCF_MASK		0x8000

#define TMRB_BASE				0x1120
#define TMRB0_CMP1				(TMRB_BASE)
#define TMRB0_CMP2				(TMRB_BASE+0x01)
#define TMRB0_CAP				(TMRB_BASE+0x02)
#define TMRB0_LOAD				(TMRB_BASE+0x03)
#define TMRB0_HOLD				(TMRB_BASE+0x04)
#define TMRB0_CNTR				(TMRB_BASE+0x05)

#define TMRB0_CTRL				(TMRB_BASE+0x06)
#define TMRB0_CTRL_CO_INIT_MASK 0x0008
#define TMRB0_CTRL_DIR_MASK		0x0010
#define TMRB0_CTRL_LENGTH_MASK	0x0020
#define TMRB0_CTRL_ONCE_MASK	0x0040

#define TMRB0_SCR				(TMRB_BASE+0x07)
#define TMRB0_SCR_OEN_MASK		0x0001
#define TMRB0_SCR_OPS_MASK		0x0002
#define TMRB0_SCR_FORCE_MASK	0x0004
#define TMRB0_SCR_VAL_MASK		0x0008
#define TMRB0_SCR_EEOF_MASK		0x0010
#define TMRB0_SCR_MSTR_MASK		0x0020
#define TMRB0_SCR_INPUT_MASK	0x0100
#define TMRB0_SCR_IPS_MASK		0x0200
#define TMRB0_SCR_IEFIE_MASK	0x0400
#define TMRB0_SCR_IEF_MASK		0x0800
#define TMRB0_SCR_TOFIE_MASK	0x1000
#define TMRB0_SCR_TOF_MASK		0x2000
#define TMRB0_SCR_TCFIE_MASK	0x4000
#define TMRB0_SCR_TCF_MASK		0x8000

#define TMRB1_CMP1				(TMRB_BASE+0x08)
#define TMRB1_CMP2				(TMRB_BASE+0x09)
#define TMRB1_CAP				(TMRB_BASE+0x0A)
#define TMRB1_LOAD				(TMRB_BASE+0x0B)
#define TMRB1_HOLD				(TMRB_BASE+0x0C)
#define TMRB1_CNTR				(TMRB_BASE+0x0D)

#define TMRB1_CTRL				(TMRB_BASE+0x0E)
#define TMRB1_CTRL_CO_INIT_MASK 0x0008
#define TMRB1_CTRL_DIR_MASK		0x0010
#define TMRB1_CTRL_LENGTH_MASK	0x0020
#define TMRB1_CTRL_ONCE_MASK	0x0040

#define TMRB1_SCR				(TMRB_BASE+0x0F)
#define TMRB1_SCR_OEN_MASK		0x0001
#define TMRB1_SCR_OPS_MASK		0x0002
#define TMRB1_SCR_FORCE_MASK	0x0004
#define TMRB1_SCR_VAL_MASK		0x0008
#define TMRB1_SCR_EEOF_MASK		0x0010
#define TMRB1_SCR_MSTR_MASK		0x0020
#define TMRB1_SCR_INPUT_MASK	0x0100
#define TMRB1_SCR_IPS_MASK		0x0200
#define TMRB1_SCR_IEFIE_MASK	0x0400
#define TMRB1_SCR_IEF_MASK		0x0800
#define TMRB1_SCR_TOFIE_MASK	0x1000
#define TMRB1_SCR_TOF_MASK		0x2000
#define TMRB1_SCR_TCFIE_MASK	0x4000
#define TMRB1_SCR_TCF_MASK		0x8000

#define TMRB2_CMP1				(TMRB_BASE+0x10)
#define TMRB2_CMP2				(TMRB_BASE+0x11)
#define TMRB2_CAP				(TMRB_BASE+0x12)
#define TMRB2_LOAD				(TMRB_BASE+0x13)
#define TMRB2_HOLD				(TMRB_BASE+0x14)
#define TMRB2_CNTR				(TMRB_BASE+0x15)

#define TMRB2_CTRL				(TMRB_BASE+0x16)
#define TMRB2_CTRL_CO_INIT_MASK 0x0008
#define TMRB2_CTRL_DIR_MASK		0x0010
#define TMRB2_CTRL_LENGTH_MASK	0x0020
#define TMRB2_CTRL_ONCE_MASK	0x0040

#define TMRB2_SCR				(TMRB_BASE+0x17)
#define TMRB2_SCR_OEN_MASK		0x0001
#define TMRB2_SCR_OPS_MASK		0x0002
#define TMRB2_SCR_FORCE_MASK	0x0004
#define TMRB2_SCR_VAL_MASK		0x0008
#define TMRB2_SCR_EEOF_MASK		0x0010
#define TMRB2_SCR_MSTR_MASK		0x0020
#define TMRB2_SCR_INPUT_MASK	0x0100
#define TMRB2_SCR_IPS_MASK		0x0200
#define TMRB2_SCR_IEFIE_MASK	0x0400
#define TMRB2_SCR_IEF_MASK		0x0800
#define TMRB2_SCR_TOFIE_MASK	0x1000
#define TMRB2_SCR_TOF_MASK		0x2000
#define TMRB2_SCR_TCFIE_MASK	0x4000
#define TMRB2_SCR_TCF_MASK		0x8000

#define TMRB3_CMP1				(TMRB_BASE+0x18)
#define TMRB3_CMP2				(TMRB_BASE+0x19)
#define TMRB3_CAP				(TMRB_BASE+0x1A)
#define TMRB3_LOAD				(TMRB_BASE+0x1B)
#define TMRB3_HOLD				(TMRB_BASE+0x1C)
#define TMRB3_CNTR				(TMRB_BASE+0x1D)

#define TMRB3_CTRL				(TMRB_BASE+0x1E)
#define TMRB3_CTRL_CO_INIT_MASK 0x0008
#define TMRB3_CTRL_DIR_MASK		0x0010
#define TMRB3_CTRL_LENGTH_MASK	0x0020
#define TMRB3_CTRL_ONCE_MASK	0x0040

#define TMRB3_SCR				(TMRB_BASE+0x1F)
#define TMRB3_SCR_OEN_MASK		0x0001
#define TMRB3_SCR_OPS_MASK		0x0002
#define TMRB3_SCR_FORCE_MASK	0x0004
#define TMRB3_SCR_VAL_MASK		0x0008
#define TMRB3_SCR_EEOF_MASK		0x0010
#define TMRB3_SCR_MSTR_MASK		0x0020
#define TMRB3_SCR_INPUT_MASK	0x0100
#define TMRB3_SCR_IPS_MASK		0x0200
#define TMRB3_SCR_IEFIE_MASK	0x0400
#define TMRB3_SCR_IEF_MASK		0x0800
#define TMRB3_SCR_TOFIE_MASK	0x1000
#define TMRB3_SCR_TOF_MASK		0x2000
#define TMRB3_SCR_TCFIE_MASK	0x4000
#define TMRB3_SCR_TCF_MASK		0x8000

#define TMRC_BASE				0x1140
#define TMRC0_CMP1				(TMRC_BASE)
#define TMRC0_CMP2				(TMRC_BASE+0x01)
#define TMRC0_CAP				(TMRC_BASE+0x02)
#define TMRC0_LOAD				(TMRC_BASE+0x03)
#define TMRC0_HOLD				(TMRC_BASE+0x04)
#define TMRC0_CNTR				(TMRC_BASE+0x05)

#define TMRC0_CTRL				(TMRC_BASE+0x06)
#define TMRC0_CTRL_CO_INIT_MASK 0x0008
#define TMRC0_CTRL_DIR_MASK		0x0010
#define TMRC0_CTRL_LENGTH_MASK	0x0020
#define TMRC0_CTRL_ONCE_MASK	0x0040

#define TMRC0_SCR				(TMRC_BASE+0x07)
#define TMRC0_SCR_OEN_MASK		0x0001
#define TMRC0_SCR_OPS_MASK		0x0002
#define TMRC0_SCR_FORCE_MASK	0x0004
#define TMRC0_SCR_VAL_MASK		0x0008
#define TMRC0_SCR_EEOF_MASK		0x0010
#define TMRC0_SCR_MSTR_MASK		0x0020
#define TMRC0_SCR_INPUT_MASK	0x0100
#define TMRC0_SCR_IPS_MASK		0x0200
#define TMRC0_SCR_IEFIE_MASK	0x0400
#define TMRC0_SCR_IEF_MASK		0x0800
#define TMRC0_SCR_TOFIE_MASK	0x1000
#define TMRC0_SCR_TOF_MASK		0x2000
#define TMRC0_SCR_TCFIE_MASK	0x4000
#define TMRC0_SCR_TCF_MASK		0x8000

#define TMRC1_CMP1				(TMRC_BASE+0x08)
#define TMRC1_CMP2				(TMRC_BASE+0x09)
#define TMRC1_CAP				(TMRC_BASE+0x0A)
#define TMRC1_LOAD				(TMRC_BASE+0x0B)
#define TMRC1_HOLD				(TMRC_BASE+0x0C)
#define TMRC1_CNTR				(TMRC_BASE+0x0D)

#define TMRC1_CTRL				(TMRC_BASE+0x0E)
#define TMRC1_CTRL_CO_INIT_MASK 0x0008
#define TMRC1_CTRL_DIR_MASK		0x0010
#define TMRC1_CTRL_LENGTH_MASK	0x0020
#define TMRC1_CTRL_ONCE_MASK	0x0040

#define TMRC1_SCR				(TMRC_BASE+0x0F)
#define TMRC1_SCR_OEN_MASK		0x0001
#define TMRC1_SCR_OPS_MASK		0x0002
#define TMRC1_SCR_FORCE_MASK	0x0004
#define TMRC1_SCR_VAL_MASK		0x0008
#define TMRC1_SCR_EEOF_MASK		0x0010
#define TMRC1_SCR_MSTR_MASK		0x0020
#define TMRC1_SCR_INPUT_MASK	0x0100
#define TMRC1_SCR_IPS_MASK		0x0200
#define TMRC1_SCR_IEFIE_MASK	0x0400
#define TMRC1_SCR_IEF_MASK		0x0800
#define TMRC1_SCR_TOFIE_MASK	0x1000
#define TMRC1_SCR_TOF_MASK		0x2000
#define TMRC1_SCR_TCFIE_MASK	0x4000
#define TMRC1_SCR_TCF_MASK		0x8000

#define TMRC2_CMP1				(TMRC_BASE+0x10)
#define TMRC2_CMP2				(TMRC_BASE+0x11)
#define TMRC2_CAP				(TMRC_BASE+0x12)
#define TMRC2_LOAD				(TMRC_BASE+0x13)
#define TMRC2_HOLD				(TMRC_BASE+0x14)
#define TMRC2_CNTR				(TMRC_BASE+0x15)

#define TMRC2_CTRL				(TMRC_BASE+0x16)
#define TMRC2_CTRL_CO_INIT_MASK 0x0008
#define TMRC2_CTRL_DIR_MASK		0x0010
#define TMRC2_CTRL_LENGTH_MASK	0x0020
#define TMRC2_CTRL_ONCE_MASK	0x0040

#define TMRC2_SCR				(TMRC_BASE+0x17)
#define TMRC2_SCR_OEN_MASK		0x0001
#define TMRC2_SCR_OPS_MASK		0x0002
#define TMRC2_SCR_FORCE_MASK	0x0004
#define TMRC2_SCR_VAL_MASK		0x0008
#define TMRC2_SCR_EEOF_MASK		0x0010
#define TMRC2_SCR_MSTR_MASK		0x0020
#define TMRC2_SCR_INPUT_MASK	0x0100
#define TMRC2_SCR_IPS_MASK		0x0200
#define TMRC2_SCR_IEFIE_MASK	0x0400
#define TMRC2_SCR_IEF_MASK		0x0800
#define TMRC2_SCR_TOFIE_MASK	0x1000
#define TMRC2_SCR_TOF_MASK		0x2000
#define TMRC2_SCR_TCFIE_MASK	0x4000
#define TMRC2_SCR_TCF_MASK		0x8000

#define TMRC3_CMP1				(TMRC_BASE+0x18)
#define TMRC3_CMP2				(TMRC_BASE+0x19)
#define TMRC3_CAP				(TMRC_BASE+0x1A)
#define TMRC3_LOAD				(TMRC_BASE+0x1B)
#define TMRC3_HOLD				(TMRC_BASE+0x1C)
#define TMRC3_CNTR				(TMRC_BASE+0x1D)

#define TMRC3_CTRL				(TMRC_BASE+0x1E)
#define TMRC3_CTRL_CO_INIT_MASK 0x0008
#define TMRC3_CTRL_DIR_MASK		0x0010
#define TMRC3_CTRL_LENGTH_MASK	0x0020
#define TMRC3_CTRL_ONCE_MASK	0x0040

#define TMRC3_SCR				(TMRC_BASE+0x1F)
#define TMRC3_SCR_OEN_MASK		0x0001
#define TMRC3_SCR_OPS_MASK		0x0002
#define TMRC3_SCR_FORCE_MASK	0x0004
#define TMRC3_SCR_VAL_MASK		0x0008
#define TMRC3_SCR_EEOF_MASK		0x0010
#define TMRC3_SCR_MSTR_MASK		0x0020
#define TMRC3_SCR_INPUT_MASK	0x0100
#define TMRC3_SCR_IPS_MASK		0x0200
#define TMRC3_SCR_IEFIE_MASK	0x0400
#define TMRC3_SCR_IEF_MASK		0x0800
#define TMRC3_SCR_TOFIE_MASK	0x1000
#define TMRC3_SCR_TOF_MASK		0x2000
#define TMRC3_SCR_TCFIE_MASK	0x4000
#define TMRC3_SCR_TCF_MASK		0x8000

#define TMRD_BASE				0x1160
#define TMRD0_CMP1				(TMRD_BASE)
#define TMRD0_CMP2				(TMRD_BASE+0x01)
#define TMRD0_CAP				(TMRD_BASE+0x02)
#define TMRD0_LOAD				(TMRD_BASE+0x03)
#define TMRD0_HOLD				(TMRD_BASE+0x04)
#define TMRD0_CNTR				(TMRD_BASE+0x05)

#define TMRD0_CTRL				(TMRD_BASE+0x06)
#define TMRD0_CTRL_CO_INIT_MASK 0x0008
#define TMRD0_CTRL_DIR_MASK		0x0010
#define TMRD0_CTRL_LENGTH_MASK	0x0020
#define TMRD0_CTRL_ONCE_MASK	0x0040

#define TMRD0_SCR				(TMRD_BASE+0x07)
#define TMRD0_SCR_OEN_MASK		0x0001
#define TMRD0_SCR_OPS_MASK		0x0002
#define TMRD0_SCR_FORCE_MASK	0x0004
#define TMRD0_SCR_VAL_MASK		0x0008
#define TMRD0_SCR_EEOF_MASK		0x0010
#define TMRD0_SCR_MSTR_MASK		0x0020
#define TMRD0_SCR_INPUT_MASK	0x0100
#define TMRD0_SCR_IPS_MASK		0x0200
#define TMRD0_SCR_IEFIE_MASK	0x0400
#define TMRD0_SCR_IEF_MASK		0x0800
#define TMRD0_SCR_TOFIE_MASK	0x1000
#define TMRD0_SCR_TOF_MASK		0x2000
#define TMRD0_SCR_TCFIE_MASK	0x4000
#define TMRD0_SCR_TCF_MASK		0x8000

#define TMRD1_CMP1				(TMRD_BASE+0x08)
#define TMRD1_CMP2				(TMRD_BASE+0x09)
#define TMRD1_CAP				(TMRD_BASE+0x0A)
#define TMRD1_LOAD				(TMRD_BASE+0x0B)
#define TMRD1_HOLD				(TMRD_BASE+0x0C)
#define TMRD1_CNTR				(TMRD_BASE+0x0D)

#define TMRD1_CTRL				(TMRD_BASE+0x0E)
#define TMRD1_CTRL_CO_INIT_MASK 0x0008
#define TMRD1_CTRL_DIR_MASK		0x0010
#define TMRD1_CTRL_LENGTH_MASK	0x0020
#define TMRD1_CTRL_ONCE_MASK	0x0040

#define TMRD1_SCR				(TMRD_BASE+0x0F)
#define TMRD1_SCR_OEN_MASK		0x0001
#define TMRD1_SCR_OPS_MASK		0x0002
#define TMRD1_SCR_FORCE_MASK	0x0004
#define TMRD1_SCR_VAL_MASK		0x0008
#define TMRD1_SCR_EEOF_MASK		0x0010
#define TMRD1_SCR_MSTR_MASK		0x0020
#define TMRD1_SCR_INPUT_MASK	0x0100
#define TMRD1_SCR_IPS_MASK		0x0200
#define TMRD1_SCR_IEFIE_MASK	0x0400
#define TMRD1_SCR_IEF_MASK		0x0800
#define TMRD1_SCR_TOFIE_MASK	0x1000
#define TMRD1_SCR_TOF_MASK		0x2000
#define TMRD1_SCR_TCFIE_MASK	0x4000
#define TMRD1_SCR_TCF_MASK		0x8000

#define TMRD2_CMP1				(TMRD_BASE+0x10)
#define TMRD2_CMP2				(TMRD_BASE+0x11)
#define TMRD2_CAP				(TMRD_BASE+0x12)
#define TMRD2_LOAD				(TMRD_BASE+0x13)
#define TMRD2_HOLD				(TMRD_BASE+0x14)
#define TMRD2_CNTR				(TMRD_BASE+0x15)

#define TMRD2_CTRL				(TMRD_BASE+0x16)
#define TMRD2_CTRL_CO_INIT_MASK 0x0008
#define TMRD2_CTRL_DIR_MASK		0x0010
#define TMRD2_CTRL_LENGTH_MASK	0x0020
#define TMRD2_CTRL_ONCE_MASK	0x0040

#define TMRD2_SCR				(TMRD_BASE+0x17)
#define TMRD2_SCR_OEN_MASK		0x0001
#define TMRD2_SCR_OPS_MASK		0x0002
#define TMRD2_SCR_FORCE_MASK	0x0004
#define TMRD2_SCR_VAL_MASK		0x0008
#define TMRD2_SCR_EEOF_MASK		0x0010
#define TMRD2_SCR_MSTR_MASK		0x0020
#define TMRD2_SCR_INPUT_MASK	0x0100
#define TMRD2_SCR_IPS_MASK		0x0200
#define TMRD2_SCR_IEFIE_MASK	0x0400
#define TMRD2_SCR_IEF_MASK		0x0800
#define TMRD2_SCR_TOFIE_MASK	0x1000
#define TMRD2_SCR_TOF_MASK		0x2000
#define TMRD2_SCR_TCFIE_MASK	0x4000
#define TMRD2_SCR_TCF_MASK		0x8000

#define TMRD3_CMP1				(TMRD_BASE+0x18)
#define TMRD3_CMP2				(TMRD_BASE+0x19)
#define TMRD3_CAP				(TMRD_BASE+0x1A)
#define TMRD3_LOAD				(TMRD_BASE+0x1B)
#define TMRD3_HOLD				(TMRD_BASE+0x1C)
#define TMRD3_CNTR				(TMRD_BASE+0x1D)

#define TMRD3_CTRL				(TMRD_BASE+0x1E)
#define TMRD3_CTRL_CO_INIT_MASK 0x0008
#define TMRD3_CTRL_DIR_MASK		0x0010
#define TMRD3_CTRL_LENGTH_MASK	0x0020
#define TMRD3_CTRL_ONCE_MASK	0x0040

#define TMRD3_SCR				(TMRD_BASE+0x1F)
#define TMRD3_SCR_OEN_MASK		0x0001
#define TMRD3_SCR_OPS_MASK		0x0002
#define TMRD3_SCR_FORCE_MASK	0x0004
#define TMRD3_SCR_VAL_MASK		0x0008
#define TMRD3_SCR_EEOF_MASK		0x0010
#define TMRD3_SCR_MSTR_MASK		0x0020
#define TMRD3_SCR_INPUT_MASK	0x0100
#define TMRD3_SCR_IPS_MASK		0x0200
#define TMRD3_SCR_IEFIE_MASK	0x0400
#define TMRD3_SCR_IEF_MASK		0x0800
#define TMRD3_SCR_TOFIE_MASK	0x1000
#define TMRD3_SCR_TOF_MASK		0x2000
#define TMRD3_SCR_TCFIE_MASK	0x4000
#define TMRD3_SCR_TCF_MASK		0x8000

#define CAN_BASE				0x1180
#define CAN_CTL0				(CAN_BASE)
#define CAN_CTL0_SFTRES_MASK	0x0001
#define CAN_CTL0_SLPRQ_MASK		0x0002
#define CAN_CTL0_SLPAK_MASK		0x0004
#define CAN_CTL0_SYNCH_MASK		0x0010
#define CAN_CTL0_CSWAI_MASK		0x0020
#define CAN_CTL0_RXACT_MASK		0x0040
#define CAN_CTL0_RXFRM_MASK		0x0080

#define CAN_CTL1				(CAN_BASE+1)
#define CAN_CTL1_CLKSRC_MASK	0x0001
#define CAN_CTL1_WUPM_MASK		0x0002
#define CAN_CTL1_LOOPB_MASK		0x0004
#define CAN_CTL1_CANE_MASK		0x0080

#define CAN_BTR0				(CAN_BASE+2)
#define CAN_BTR0_BRP0_MASK		0x0001
#define CAN_BTR0_BRP1_MASK		0x0002
#define CAN_BTR0_BRP2_MASK		0x0004
#define CAN_BTR0_BRP3_MASK		0x0008
#define CAN_BTR0_BRP4_MASK		0x0010
#define CAN_BTR0_BRP5_MASK		0x0020
#define CAN_BTR0_SJW0_MASK		0x0040
#define CAN_BTR0_SJW1_MASK		0x0080

#define CAN_BTR1				(CAN_BASE+3)
#define CAN_BTR1_TSEG10_MASK	0x0001
#define CAN_BTR1_TSEG11_MASK	0x0002
#define CAN_BTR1_TSEG12_MASK	0x0004
#define CAN_BTR1_TSEG13_MASK	0x0008
#define CAN_BTR1_TSEG20_MASK	0x0010
#define CAN_BTR1_TSEG21_MASK	0x0020
#define CAN_BTR1_TSEG22_MASK	0x0040
#define CAN_BTR1_SAMP_MASK		0x0080

#define CAN_RFLG				(CAN_BASE+4)
#define CAN_RFLG_RXF_MASK		0x0001
#define CAN_RFLG_OVRIF_MASK		0x0002
#define CAN_RFLG_BOFFIF_MASK	0x0004
#define CAN_RFLG_TERRIF_MASK	0x0008
#define CAN_RFLG_RERRIF_MASK	0x0010
#define CAN_RFLG_TWRNIF_MASK	0x0020
#define CAN_RFLG_RWRNIF_MASK	0x0040
#define CAN_RFLG_WUPIF_MASK		0x0080

#define CAN_RIER				(CAN_BASE+5)
#define CAN_RIER_RXFIE_MASK		0x0001
#define CAN_RIER_OVRIE_MASK		0x0002
#define CAN_RIER_BOFFIE_MASK	0x0004
#define CAN_RIER_TERRIE_MASK	0x0008
#define CAN_RIER_RERRIE_MASK	0x0010
#define CAN_RIER_TWRNIE_MASK	0x0020
#define CAN_RIER_RWRNIE_MASK	0x0040
#define CAN_RIER_WUPIE_MASK		0x0080

#define CAN_TFLG				(CAN_BASE+6)
#define CAN_TFLG_TXE0_MASK		0x0001
#define CAN_TFLG_TXE1_MASK		0x0002
#define CAN_TFLG_TXE2_MASK		0x0004
#define CAN_TFLG_ABTAK0_MASK	0x0010
#define CAN_TFLG_ABTAK1_MASK	0x0020
#define CAN_TFLG_ABTAK2_MASK	0x0040

#define CAN_TCR					(CAN_BASE+7)
#define CAN_TCR_TXEIE0_MASK		0x0001
#define CAN_TCR_TXEIE1_MASK		0x0002
#define CAN_TCR_TXEIE2_MASK		0x0004
#define CAN_TCR_ABTRQ0_MASK		0x0010
#define CAN_TCR_ABTRQ1_MASK		0x0020
#define CAN_TCR_ABTRQ2_MASK		0x0040

#define CAN_IDAC				(CAN_BASE+8)
#define CAN_IDAC_IDHIT0			0x0001
#define CAN_IDAC_IDHIT1			0x0002
#define CAN_IDAC_IDHIT2			0x0004
#define CAN_IDAC_IDAM0			0x0010
#define CAN_IDAC_IDAM1			0x0020

#define CAN_RXERR				(CAN_BASE+0xE)
#define CAN_RXERR_RXERR0_MASK	0x0001
#define CAN_RXERR_RXERR1_MASK	0x0002
#define CAN_RXERR_RXERR2_MASK	0x0004
#define CAN_RXERR_RXERR3_MASK	0x0008
#define CAN_RXERR_RXERR4_MASK	0x0010
#define CAN_RXERR_RXERR5_MASK	0x0020
#define CAN_RXERR_RXERR6_MASK	0x0040
#define CAN_RXERR_RXERR7_MASK	0x0080

#define CAN_TXERR				(CAN_BASE+0xF)
#define CAN_TXERR_TXERR0_MASK	0x0001
#define CAN_TXERR_TXERR1_MASK	0x0002
#define CAN_TXERR_TXERR2_MASK	0x0004
#define CAN_TXERR_TXERR3_MASK	0x0008
#define CAN_TXERR_TXERR4_MASK	0x0010
#define CAN_TXERR_TXERR5_MASK	0x0020
#define CAN_TXERR_TXERR6_MASK	0x0040
#define CAN_TXERR_TXERR7_MASK	0x0080

#define CAN_IDAR0				(CAN_BASE+0x10)
#define CAN_IDAR1				(CAN_BASE+0x11)
#define CAN_IDAR2				(CAN_BASE+0x12)
#define CAN_IDAR3				(CAN_BASE+0x13)
#define CAN_IDAR4				(CAN_BASE+0x18)
#define CAN_IDAR5				(CAN_BASE+0x19)
#define CAN_IDAR6				(CAN_BASE+0x1A)
#define CAN_IDAR7				(CAN_BASE+0x1B)
#define CAN_IDMR0				(CAN_BASE+0x14)
#define CAN_IDMR1				(CAN_BASE+0x15)
#define CAN_IDMR2				(CAN_BASE+0x16)
#define CAN_IDMR3				(CAN_BASE+0x17)
#define CAN_IDMR4				(CAN_BASE+0x1C)
#define CAN_IDMR5				(CAN_BASE+0x1D)
#define CAN_IDMR6				(CAN_BASE+0x1E)
#define CAN_IDMR7				(CAN_BASE+0x1F)
#define CAN_RB_IDR0				(CAN_BASE+0x40)
#define CAN_RB_IDR1				(CAN_BASE+0x41)
#define CAN_RB_IDR2				(CAN_BASE+0x42)
#define CAN_RB_IDR3				(CAN_BASE+0x43)
#define CAN_RB_DSR0				(CAN_BASE+0x44)
#define CAN_RB_DSR1				(CAN_BASE+0x45)
#define CAN_RB_DSR2				(CAN_BASE+0x46)
#define CAN_RB_DSR3				(CAN_BASE+0x47)
#define CAN_RB_DSR4				(CAN_BASE+0x48)
#define CAN_RB_DSR5				(CAN_BASE+0x49)
#define CAN_RB_DSR6				(CAN_BASE+0x4A)
#define CAN_RB_DSR7				(CAN_BASE+0x4B)
#define CAN_RB_DLR				(CAN_BASE+0x4C)
#define CAN_RB_TBPR				(CAN_BASE+0x4D)

#define CAN_TB0_IDR0			(CAN_BASE+0x50)
#define CAN_TB0_IDR1			(CAN_BASE+0x51)
#define CAN_TB0_IDR2			(CAN_BASE+0x52)
#define CAN_TB0_IDR3			(CAN_BASE+0x53)
#define CAN_TB0_DSR0			(CAN_BASE+0x54)
#define CAN_TB0_DSR1			(CAN_BASE+0x55)
#define CAN_TB0_DSR2			(CAN_BASE+0x56)
#define CAN_TB0_DSR3			(CAN_BASE+0x57)
#define CAN_TB0_DSR4			(CAN_BASE+0x58)
#define CAN_TB0_DSR5			(CAN_BASE+0x59)
#define CAN_TB0_DSR6			(CAN_BASE+0x5A)
#define CAN_TB0_DSR7			(CAN_BASE+0x5B)
#define CAN_TB0_DLR				(CAN_BASE+0x5C)
#define CAN_TB0_TBPR			(CAN_BASE+0x5D)

#define CAN_TB1_IDR0			(CAN_BASE+0x60)
#define CAN_TB1_IDR1			(CAN_BASE+0x61)
#define CAN_TB1_IDR2			(CAN_BASE+0x62)
#define CAN_TB1_IDR3			(CAN_BASE+0x63)
#define CAN_TB1_DSR0			(CAN_BASE+0x64)
#define CAN_TB1_DSR1			(CAN_BASE+0x65)
#define CAN_TB1_DSR2			(CAN_BASE+0x66)
#define CAN_TB1_DSR3			(CAN_BASE+0x67)
#define CAN_TB1_DSR4			(CAN_BASE+0x68)
#define CAN_TB1_DSR5			(CAN_BASE+0x69)
#define CAN_TB1_DSR6			(CAN_BASE+0x6A)
#define CAN_TB1_DSR7			(CAN_BASE+0x6B)
#define CAN_TB1_DLR				(CAN_BASE+0x6C)
#define CAN_TB1_TBPR			(CAN_BASE+0x6D)

#define CAN_TB2_IDR0			(CAN_BASE+0x70)
#define CAN_TB2_IDR1			(CAN_BASE+0x71)
#define CAN_TB2_IDR2			(CAN_BASE+0x72)
#define CAN_TB2_IDR3			(CAN_BASE+0x73)
#define CAN_TB2_DSR0			(CAN_BASE+0x74)
#define CAN_TB2_DSR1			(CAN_BASE+0x75)
#define CAN_TB2_DSR2			(CAN_BASE+0x76)
#define CAN_TB2_DSR3			(CAN_BASE+0x77)
#define CAN_TB2_DSR4			(CAN_BASE+0x78)
#define CAN_TB2_DSR5			(CAN_BASE+0x79)
#define CAN_TB2_DSR6			(CAN_BASE+0x7A)
#define CAN_TB2_DSR7			(CAN_BASE+0x7B)
#define CAN_TB2_DLR				(CAN_BASE+0x7C)
#define CAN_TB2_TBPR			(CAN_BASE+0x7D)

#define PWMA_BASE				0x1200
#define PWMA_PMCTL				(PWMA_BASE)
#define PWMA_PMCTL_PWMEN_MASK	0x0001
#define PWMA_PMCTL_LDOK_MASK	0x0002
#define PWMA_PMCTL_ISENS0_MASK	0x0004
#define PWMA_PMCTL_ISENS1_MASK	0x0008
#define PWMA_PMCTL_PWMF_MASK	0x0010
#define PWMA_PMCTL_PWMRIE_MASK	0x0020
#define PWMA_PMCTL_PRSC0_MASK	0x0040
#define PWMA_PMCTL_PRSC1_MASK	0x0080
#define PWMA_PMCTL_IPOL2_MASK	0x0100
#define PWMA_PMCTL_IPOL1_MASK	0x0200
#define PWMA_PMCTL_IPOL0_MASK	0x0400
#define PWMA_PMCTL_HALF_MASK	0x0800
#define PWMA_PMCTL_LDFQ0_MASK	0x1000
#define PWMA_PMCTL_LDFQ1_MASK	0x2000
#define PWMA_PMCTL_LDFQ2_MASK	0x4000
#define PWMA_PMCTL_LDFQ3_MASK	0x8000

#define PWMA_PMFCTL				(PWMA_BASE+0x01)
#define PWMA_PMFCTL_FMODE0_MASK	0x0001
#define PWMA_PMFCTL_FIE0_MASK	0x0002
#define PWMA_PMFCTL_FMODE1_MASK 0x0004
#define PWMA_PMFCTL_FIE1_MASK	0x0008
#define PWMA_PMFCTL_FMODE2_MASK	0x0010
#define PWMA_PMFCTL_FIE2_MASK	0x0020
#define PWMA_PMFCTL_FMODE3_MASK	0x0040
#define PWMA_PMFCTL_FIE3_MASK	0x0080

#define PWMA_PMFSA				(PWMA_BASE+0x02)
#define PWMA_PMFSA_DT0_MASK		0x0001
#define PWMA_PMFSA_FTACK0_MASK	0x0001
#define PWMA_PMFSA_DT1_MASK		0x0002
#define PWMA_PMFSA_DT2_MASK		0x0004
#define PWMA_PMFSA_FTACK1_MASK	0x0004
#define PWMA_PMFSA_DT3_MASK		0x0008
#define PWMA_PMFSA_DT4_MASK		0x0010
#define PWMA_PMFSA_FTACK2_MASK	0x0010
#define PWMA_PMFSA_DT5_MASK		0x0020
#define PWMA_PMFSA_FTACK3_MASK	0x0040
#define PWMA_PMFSA_FFLAG0_MASK	0x0100
#define PWMA_PMFSA_FPIN0_MASK	0x0200
#define PWMA_PMFSA_FFLAG1_MASK	0x0400
#define PWMA_PMFSA_FPIN1_MASK	0x0800
#define PWMA_PMFSA_FFLAG2_MASK	0x1000
#define PWMA_PMFSA_FPIN2_MASK	0x2000
#define PWMA_PMFSA_FFLAG3_MASK	0x4000
#define PWMA_PMFSA_FPIN3_MASK	0x8000

#define PWMA_PMOUT				(PWMA_BASE+0x03)
#define PWMA_PMOUT_OUT0_MASK	0x0001
#define PWMA_PMOUT_OUT1_MASK	0x0002
#define PWMA_PMOUT_OUT2_MASK	0x0004
#define PWMA_PMOUT_OUT3_MASK	0x0008
#define PWMA_PMOUT_OUT4_MASK	0x0010
#define PWMA_PMOUT_OUT5_MASK	0x0020
#define PWMA_PMOUT_OUTCTL0_MASK	0x0100
#define PWMA_PMOUT_OUTCTL1_MASK	0x0200
#define PWMA_PMOUT_OUTCTL2_MASK	0x0400
#define PWMA_PMOUT_OUTCTL3_MASK	0x0800
#define PWMA_PMOUT_OUTCTL4_MASK	0x1000
#define PWMA_PMOUT_OUTCTL5_MASK	0x2000
#define PWMA_PMOUT_PAD_EN_MASK	0x8000

#define PWMA_PMCNT				(PWMA_BASE+0x04)
#define PWMA_PWMCM				(PWMA_BASE+0x05)
#define PWMA_PWMVAL0			(PWMA_BASE+0x06)
#define PWMA_PWMVAL1			(PWMA_BASE+0x07)
#define PWMA_PWMVAL2			(PWMA_BASE+0x08)
#define PWMA_PWMVAL3			(PWMA_BASE+0x09)
#define PWMA_PWMVAL4			(PWMA_BASE+0x0A)
#define PWMA_PWMVAL5			(PWMA_BASE+0x0B)
#define PWMA_PMDEADTM			(PWMA_BASE+0x0C)
#define PWMA_PMDISMAP1			(PWMA_BASE+0x0D)
#define PWMA_PMDISMAP2			(PWMA_BASE+0x0E)

#define PWMA_PMCFG				(PWMA_BASE+0x0F)
#define PWMA_PMCFG_WP_MASK		0x0001
#define PWMA_PMCFG_INDEP01_MASK	0x0002
#define PWMA_PMCFG_INDEP23_MASK	0x0004
#define PWMA_PMCFG_INDEP45_MASK	0x0008
#define PWMA_PMCFG_BOTNEG01_MASK 0x0010
#define PWMA_PMCFG_BOTNEG23_MASK 0x0020
#define PWMA_PMCFG_BOTNEG45_MASK 0x0040
#define PWMA_PMCFG_TOPNEG01_MASK 0x0100
#define PWMA_PMCFG_TOPNEG23_MASK 0x0200
#define PWMA_PMCFG_TOPNEG45_MASK 0x0400
#define PWMA_PMCFG_EDG_MASK		0x1000

#define PWMA_PMCCR				(PWMA_BASE+0x10)
#define PWMA_PMCCR_SWP0_MASK	0x0001
#define PWMA_PMCCR_SWP1_MASK	0x0002
#define PWMA_PMCCR_SWP2_MASK	0x0004
#define PWMA_PMCCR_VLMODE0_MASK 0x0010
#define PWMA_PMCCR_VLMODE1_MASK 0x0020
#define PWMA_PMCCR_MSK0_MASK	0x0100
#define PWMA_PMCCR_MSK1_MASK	0x0200
#define PWMA_PMCCR_MSK2_MASK	0x0400
#define PWMA_PMCCR_MSK3_MASK	0x0800
#define PWMA_PMCCR_MSK4_MASK	0x1000
#define PWMA_PMCCR_MSK5_MASK	0x2000
#define PWMA_PMCCR_ENHA_MASK	0x8000

#define PWMA_PMPORT				(PWMA_BASE+0x11)

#define PWMB_BASE				0x1220
#define PWMB_PMCTL				(PWMB_BASE)
#define PWMB_PMCTL_PWMEN_MASK	0x0001
#define PWMB_PMCTL_LDOK_MASK	0x0002
#define PWMB_PMCTL_ISENS0_MASK	0x0004
#define PWMB_PMCTL_ISENS1_MASK	0x0008
#define PWMB_PMCTL_PWMF_MASK	0x0010
#define PWMB_PMCTL_PWMRIE_MASK	0x0020
#define PWMB_PMCTL_PRSC0_MASK	0x0040
#define PWMB_PMCTL_PRSC1_MASK	0x0080
#define PWMB_PMCTL_IPOL2_MASK	0x0100
#define PWMB_PMCTL_IPOL1_MASK	0x0200
#define PWMB_PMCTL_IPOL0_MASK	0x0400
#define PWMB_PMCTL_HALF_MASK	0x0800
#define PWMB_PMCTL_LDFQ0_MASK	0x1000
#define PWMB_PMCTL_LDFQ1_MASK	0x2000
#define PWMB_PMCTL_LDFQ2_MASK	0x4000
#define PWMB_PMCTL_LDFQ3_MASK	0x8000

#define PWMB_PMFCTL				(PWMB_BASE+0x01)
#define PWMB_PMFCTL_FMODE0_MASK	0x0001
#define PWMB_PMFCTL_FIE0_MASK	0x0002
#define PWMB_PMFCTL_FMODE1_MASK 0x0004
#define PWMB_PMFCTL_FIE1_MASK	0x0008
#define PWMB_PMFCTL_FMODE2_MASK	0x0010
#define PWMB_PMFCTL_FIE2_MASK	0x0020
#define PWMB_PMFCTL_FMODE3_MASK	0x0040
#define PWMB_PMFCTL_FIE3_MASK	0x0080

#define PWMB_PMFSA				(PWMB_BASE+0x02)
#define PWMB_PMFSA_DT0_MASK		0x0001
#define PWMB_PMFSA_FTACK0_MASK	0x0001
#define PWMB_PMFSA_DT1_MASK		0x0002
#define PWMB_PMFSA_DT2_MASK		0x0004
#define PWMB_PMFSA_FTACK1_MASK	0x0004
#define PWMB_PMFSA_DT3_MASK		0x0008
#define PWMB_PMFSA_DT4_MASK		0x0010
#define PWMB_PMFSA_FTACK2_MASK	0x0010
#define PWMB_PMFSA_DT5_MASK		0x0020
#define PWMB_PMFSA_FTACK3_MASK	0x0040
#define PWMB_PMFSA_FFLAG0_MASK	0x0100
#define PWMB_PMFSA_FPIN0_MASK	0x0200
#define PWMB_PMFSA_FFLAG1_MASK	0x0400
#define PWMB_PMFSA_FPIN1_MASK	0x0800
#define PWMB_PMFSA_FFLAG2_MASK	0x1000
#define PWMB_PMFSA_FPIN2_MASK	0x2000
#define PWMB_PMFSA_FFLAG3_MASK	0x4000
#define PWMB_PMFSA_FPIN3_MASK	0x8000

#define PWMB_PMOUT				(PWMB_BASE+0x03)
#define PWMB_PMOUT_OUT0_MASK	0x0001
#define PWMB_PMOUT_OUT1_MASK	0x0002
#define PWMB_PMOUT_OUT2_MASK	0x0004
#define PWMB_PMOUT_OUT3_MASK	0x0008
#define PWMB_PMOUT_OUT4_MASK	0x0010
#define PWMB_PMOUT_OUT5_MASK	0x0020
#define PWMB_PMOUT_OUTCTL0_MASK	0x0100
#define PWMB_PMOUT_OUTCTL1_MASK	0x0200
#define PWMB_PMOUT_OUTCTL2_MASK	0x0400
#define PWMB_PMOUT_OUTCTL3_MASK	0x0800
#define PWMB_PMOUT_OUTCTL4_MASK	0x1000
#define PWMB_PMOUT_OUTCTL5_MASK	0x2000
#define PWMB_PMOUT_PAD_EN_MASK	0x8000

#define PWMB_PMCNT				(PWMB_BASE+0x04)
#define PWMB_PWMCM				(PWMB_BASE+0x05)
#define PWMB_PWMVAL0			(PWMB_BASE+0x06)
#define PWMB_PWMVAL1			(PWMB_BASE+0x07)
#define PWMB_PWMVAL2			(PWMB_BASE+0x08)
#define PWMB_PWMVAL3			(PWMB_BASE+0x09)
#define PWMB_PWMVAL4			(PWMB_BASE+0x0A)
#define PWMB_PWMVAL5			(PWMB_BASE+0x0B)
#define PWMB_PMDEADTM			(PWMB_BASE+0x0C)
#define PWMB_PMDISMAP1			(PWMB_BASE+0x0D)
#define PWMB_PMDISMAP2			(PWMB_BASE+0x0E)

#define PWMB_PMCFG				(PWMB_BASE+0x0F)
#define PWMB_PMCFG_WP_MASK		0x0001
#define PWMB_PMCFG_INDEP01_MASK	0x0002
#define PWMB_PMCFG_INDEP23_MASK	0x0004
#define PWMB_PMCFG_INDEP45_MASK	0x0008
#define PWMB_PMCFG_BOTNEG01_MASK 0x0010
#define PWMB_PMCFG_BOTNEG23_MASK 0x0020
#define PWMB_PMCFG_BOTNEG45_MASK 0x0040
#define PWMB_PMCFG_TOPNEG01_MASK 0x0100
#define PWMB_PMCFG_TOPNEG23_MASK 0x0200
#define PWMB_PMCFG_TOPNEG45_MASK 0x0400
#define PWMB_PMCFG_EDG_MASK		0x1000

#define PWMB_PMCCR				(PWMB_BASE+0x10)
#define PWMB_PMCCR_SWP0_MASK	0x0001
#define PWMB_PMCCR_SWP1_MASK	0x0002
#define PWMB_PMCCR_SWP2_MASK	0x0004
#define PWMB_PMCCR_VLMODE0_MASK 0x0010
#define PWMB_PMCCR_VLMODE1_MASK 0x0020
#define PWMB_PMCCR_MSK0_MASK	0x0100
#define PWMB_PMCCR_MSK1_MASK	0x0200
#define PWMB_PMCCR_MSK2_MASK	0x0400
#define PWMB_PMCCR_MSK3_MASK	0x0800
#define PWMB_PMCCR_MSK4_MASK	0x1000
#define PWMB_PMCCR_MSK5_MASK	0x2000
#define PWMB_PMCCR_ENHA_MASK	0x8000

#define PWMB_PMPORT				(PWMB_BASE+0x11)



#define DEC0_BASE				0x1240
#define QD0_DECCR				(DEC0_BASE)
#define QD0_DECCR_MODE0_MASK	0x0001
#define QD0_DECCR_MODE1_MASK	0x0002
#define QD0_DECCR_WDE_MASK		0x0004
#define QD0_DECCR_DIE_MASK		0x0008
#define QD0_DECCR_DIRQ_MASK		0x0010
#define QD0_DECCR_XNE_MASK		0x0020
#define QD0_DECCR_XIP_MASK		0x0040
#define QD0_DECCR_XIE_MASK		0x0080
#define QD0_DECCR_XIRQ_MASK		0x0100
#define QD0_DECCR_PH1_MASK		0x0200
#define QD0_DECCR_REV_MASK		0x0400
#define QD0_DECCR_SWIP_MASK		0x0800
#define QD0_DECCR_HNE_MASK		0x1000
#define QD0_DECCR_HIP_MASK		0x2000
#define QD0_DECCR_HIE_MASK		0x4000
#define QD0_DECCR_HIRQ_MASK		0x8000

#define QD0_FIR					(DEC0_BASE+0x01)
#define QD0_WTR					(DEC0_BASE+0x02)
#define QD0_POSD				(DEC0_BASE+0x03)
#define QD0_POSDH				(DEC0_BASE+0x04)
#define QD0_REV					(DEC0_BASE+0x05)
#define QD0_REVH				(DEC0_BASE+0x06)
#define QD0_UPOS				(DEC0_BASE+0x07)
#define QD0_LPOS				(DEC0_BASE+0x08)
#define QD0_UPOSH				(DEC0_BASE+0x09)
#define QD0_LPOSH				(DEC0_BASE+0x0A)
#define QD0_UIR					(DEC0_BASE+0x0B)
#define QD0_LIR					(DEC0_BASE+0x0C)

#define QD0_IMR					(DEC0_BASE+0x0D)
#define QD0_IMR_HOME_MASK		0x0001
#define QD0_IMR_INDEX_MASK		0x0002
#define QD0_IMR_PHB_MASK		0x0004
#define QD0_IMR_PHA_MASK		0x0008
#define QD0_IMR_FHOM_MASK		0x0010
#define QD0_IMR_FIND_MASK		0x0020
#define QD0_IMR_FPHB_MASK		0x0040
#define QD0_IMR_FPHA_MASK		0x0080

#define QD0_TSTREG				(DEC0_BASE+0x0E)
#define QD0_TSTREG_QDN_MASK		0x2000
#define QD0_TSTREG_TCE_MASK		0x4000
#define QD0_TSTREG_TEN_MASK		0x8000

#define DEC1_BASE				0x1250
#define QD1_DECCR				(DEC1_BASE)
#define QD1_DECCR_MODE0_MASK	0x0001
#define QD1_DECCR_MODE1_MASK	0x0002
#define QD1_DECCR_WDE_MASK		0x0004
#define QD1_DECCR_DIE_MASK		0x0008
#define QD1_DECCR_DIRQ_MASK		0x0010
#define QD1_DECCR_XNE_MASK		0x0020
#define QD1_DECCR_XIP_MASK		0x0040
#define QD1_DECCR_XIE_MASK		0x0080
#define QD1_DECCR_XIRQ_MASK		0x0100
#define QD1_DECCR_PH1_MASK		0x0200
#define QD1_DECCR_REV_MASK		0x0400
#define QD1_DECCR_SWIP_MASK		0x0800
#define QD1_DECCR_HNE_MASK		0x1000
#define QD1_DECCR_HIP_MASK		0x2000
#define QD1_DECCR_HIE_MASK		0x4000
#define QD1_DECCR_HIRQ_MASK		0x8000

#define QD1_DECCR				(DEC1_BASE)
#define QD1_FIR					(DEC1_BASE+0x01)
#define QD1_WTR					(DEC1_BASE+0x02)
#define QD1_POSD				(DEC1_BASE+0x03)
#define QD1_POSDH				(DEC1_BASE+0x04)
#define QD1_REV					(DEC1_BASE+0x05)
#define QD1_REVH				(DEC1_BASE+0x06)
#define QD1_UPOS				(DEC1_BASE+0x07)
#define QD1_LPOS				(DEC1_BASE+0x08)
#define QD1_UPOSH				(DEC1_BASE+0x09)
#define QD1_LPOSH				(DEC1_BASE+0x0A)
#define QD1_UIR					(DEC1_BASE+0x0B)
#define QD1_LIR					(DEC1_BASE+0x0C)

#define QD1_IMR					(DEC1_BASE+0x0D)
#define QD0_IMR_HOME_MASK		0x0001
#define QD1_IMR_INDEX_MASK		0x0002
#define QD1_IMR_PHB_MASK		0x0004
#define QD1_IMR_PHA_MASK		0x0008
#define QD1_IMR_FHOM_MASK		0x0010
#define QD1_IMR_FIND_MASK		0x0020
#define QD1_IMR_FPHB_MASK		0x0040
#define QD1_IMR_FPHA_MASK		0x0080

#define QD1_TSTREG				(DEC1_BASE+0x0E)
#define QD1_TSTREG_QDN_MASK		0x2000
#define QD1_TSTREG_TCE_MASK		0x4000
#define QD1_TSTREG_TEN_MASK		0x8000

#define ITCN_BASE				0x1260
#define GPR0					(ITCN_BASE)
#define GPR1					(ITCN_BASE+1)
#define GPR2					(ITCN_BASE+2)
#define GPR3					(ITCN_BASE+3)
#define GPR4					(ITCN_BASE+4)
#define GPR5					(ITCN_BASE+5)
#define GPR6					(ITCN_BASE+6)
#define GPR7					(ITCN_BASE+7)
#define GPR8					(ITCN_BASE+8)
#define GPR9					(ITCN_BASE+9)
#define GPR10					(ITCN_BASE+0xA)
#define GPR11					(ITCN_BASE+0xB)
#define GPR12					(ITCN_BASE+0xC)
#define GPR13					(ITCN_BASE+0xD)
#define GPR14					(ITCN_BASE+0xE)
#define GPR15					(ITCN_BASE+0xF)
#define TIRQ0					(ITCN_BASE+0x10)
#define TIRQ1					(ITCN_BASE+0x11)
#define TIRQ2					(ITCN_BASE+0x12)
#define TIRQ3					(ITCN_BASE+0x13)
#define TISR0					(ITCN_BASE+0x18)
#define TISR1					(ITCN_BASE+0x19)
#define TISR2					(ITCN_BASE+0x1A)
#define TISR3					(ITCN_BASE+0x1B)
#define TCSR					(ITCN_BASE+0x1C)

#define ADCA_BASE					0x1280
#define ADCA_ADCR1					(ADCA_BASE)
#define ADCA_ADCR1_SMODE0_MASK		0x01
#define ADCA_ADCR1_SMODE1_MASK		0x02
#define ADCA_ADCR1_SMODE2_MASK		0x04
#define ADCA_ADCR1_CHNCFG0_MASK		0x10
#define ADCA_ADCR1_CHNCFG1_MASK		0x20
#define ADCA_ADCR1_CHNCFG2_MASK		0x40
#define ADCA_ADCR1_CHNCFG3_MASK		0x80
#define ADCA_ADCR1_HLMTIE_MASK		0x100		
#define ADCA_ADCR1_LLMTIE_MASK		0x200		
#define ADCA_ADCR1_ZCIE_MASK		0x400		
#define ADCA_ADCR1_EOSIE_MASK		0x800		
#define ADCA_ADCR1_SYNC_MASK		0x1000		
#define ADCA_ADCR1_START_MASK		0x2000		
#define ADCA_ADCR1_STOP_MASK		0x4000		

#define ADCA_ADCR2					(ADCA_BASE+1)
#define ADCA_ADCR2_DIV0_MASK		0x01
#define ADCA_ADCR2_DIV1_MASK		0x02
#define ADCA_ADCR2_DIV2_MASK		0x04

#define ADCA_ADZCC				(ADCA_BASE+2)
#define ADCA_ADLST1				(ADCA_BASE+3)
#define ADCA_ADLST2				(ADCA_BASE+4)
#define ADCA_ADSDIS				(ADCA_BASE+5)
#define ADCA_ADSTAT				(ADCA_BASE+6)
#define ADCA_ADLSTAT			(ADCA_BASE+7)
#define ADCA_ADZCSTAT			(ADCA_BASE+8)

#define ADCA_ADRSLT0			(ADCA_BASE+9)
#define ADCA_ADRSLT1			(ADCA_BASE+0xA)
#define ADCA_ADRSLT2			(ADCA_BASE+0xB)
#define ADCA_ADRSLT3			(ADCA_BASE+0xC)
#define ADCA_ADRSLT4			(ADCA_BASE+0xD)
#define ADCA_ADRSLT5			(ADCA_BASE+0xE)
#define ADCA_ADRSLT6			(ADCA_BASE+0xF)
#define ADCA_ADRSLT7			(ADCA_BASE+0x10)

#define ADCA_ADLLMT0			(ADCA_BASE+0x11)
#define ADCA_ADLLMT1			(ADCA_BASE+0x12)
#define ADCA_ADLLMT2			(ADCA_BASE+0x13)
#define ADCA_ADLLMT3			(ADCA_BASE+0x14)
#define ADCA_ADLLMT4			(ADCA_BASE+0x15)
#define ADCA_ADLLMT5			(ADCA_BASE+0x16)
#define ADCA_ADLLMT6			(ADCA_BASE+0x17)
#define ADCA_ADLLMT7			(ADCA_BASE+0x18)

#define ADCA_ADHLMT0			(ADCA_BASE+0x19)
#define ADCA_ADHLMT1			(ADCA_BASE+0x1A)
#define ADCA_ADHLMT2			(ADCA_BASE+0x1B)
#define ADCA_ADHLMT3			(ADCA_BASE+0x1C)
#define ADCA_ADHLMT4			(ADCA_BASE+0x1D)
#define ADCA_ADHLMT5			(ADCA_BASE+0x1E)
#define ADCA_ADHLMT6			(ADCA_BASE+0x1F)
#define ADCA_ADHLMT7			(ADCA_BASE+0x20)

#define ADCA_ADOFS0				(ADCA_BASE+0x21)
#define ADCA_ADOFS1				(ADCA_BASE+0x22)
#define ADCA_ADOFS2				(ADCA_BASE+0x23)
#define ADCA_ADOFS3				(ADCA_BASE+0x24)
#define ADCA_ADOFS4				(ADCA_BASE+0x25)
#define ADCA_ADOFS5				(ADCA_BASE+0x26)
#define ADCA_ADOFS6				(ADCA_BASE+0x27)
#define ADCA_ADOFS7				(ADCA_BASE+0x28)

#define ADCB_BASE					0x12C0
#define ADCB_ADCR1					(ADCB_BASE)
#define ADCB_ADCR1_SMODE0_MASK		0x01
#define ADCB_ADCR1_SMODE1_MASK		0x02
#define ADCB_ADCR1_SMODE2_MASK		0x04
#define ADCB_ADCR1_CHNCFG0_MASK		0x10
#define ADCB_ADCR1_CHNCFG1_MASK		0x20
#define ADCB_ADCR1_CHNCFG2_MASK		0x40
#define ADCB_ADCR1_CHNCFG3_MASK		0x80
#define ADCB_ADCR1_HLMTIE_MASK		0x100		
#define ADCB_ADCR1_LLMTIE_MASK		0x200		
#define ADCB_ADCR1_ZCIE_MASK		0x400		
#define ADCB_ADCR1_EOSIE_MASK		0x800		
#define ADCB_ADCR1_SYNC_MASK		0x1000		
#define ADCB_ADCR1_START_MASK		0x2000		
#define ADCB_ADCR1_STOP_MASK		0x4000		

#define ADCB_ADCR2					(ADCB_BASE+1)
#define ADCB_ADCR2_DIV0_MASK		0x01
#define ADCB_ADCR2_DIV1_MASK		0x02
#define ADCB_ADCR2_DIV2_MASK		0x04

#define ADCB_ADZCC				(ADCB_BASE+2)
#define ADCB_ADLST1				(ADCB_BASE+3)
#define ADCB_ADLST2				(ADCB_BASE+4)
#define ADCB_ADSDIS				(ADCB_BASE+5)
#define ADCB_ADSTAT				(ADCB_BASE+6)
#define ADCB_ADLSTAT			(ADCB_BASE+7)
#define ADCB_ADZCSTAT			(ADCB_BASE+8)

#define ADCB_ADRSLT0			(ADCB_BASE+9)
#define ADCB_ADRSLT1			(ADCB_BASE+0xA)
#define ADCB_ADRSLT2			(ADCB_BASE+0xB)
#define ADCB_ADRSLT3			(ADCB_BASE+0xC)
#define ADCB_ADRSLT4			(ADCB_BASE+0xD)
#define ADCB_ADRSLT5			(ADCB_BASE+0xE)
#define ADCB_ADRSLT6			(ADCB_BASE+0xF)
#define ADCB_ADRSLT7			(ADCB_BASE+0x10)

#define ADCB_ADLLMT0			(ADCB_BASE+0x11)
#define ADCB_ADLLMT1			(ADCB_BASE+0x12)
#define ADCB_ADLLMT2			(ADCB_BASE+0x13)
#define ADCB_ADLLMT3			(ADCB_BASE+0x14)
#define ADCB_ADLLMT4			(ADCB_BASE+0x15)
#define ADCB_ADLLMT5			(ADCB_BASE+0x16)
#define ADCB_ADLLMT6			(ADCB_BASE+0x17)
#define ADCB_ADLLMT7			(ADCB_BASE+0x18)

#define ADCB_ADHLMT0			(ADCB_BASE+0x19)
#define ADCB_ADHLMT1			(ADCB_BASE+0x1A)
#define ADCB_ADHLMT2			(ADCB_BASE+0x1B)
#define ADCB_ADHLMT3			(ADCB_BASE+0x1C)
#define ADCB_ADHLMT4			(ADCB_BASE+0x1D)
#define ADCB_ADHLMT5			(ADCB_BASE+0x1E)
#define ADCB_ADHLMT6			(ADCB_BASE+0x1F)
#define ADCB_ADHLMT7			(ADCB_BASE+0x20)

#define ADCB_ADOFS0				(ADCB_BASE+0x21)
#define ADCB_ADOFS1				(ADCB_BASE+0x22)
#define ADCB_ADOFS2				(ADCB_BASE+0x23)
#define ADCB_ADOFS3				(ADCB_BASE+0x24)
#define ADCB_ADOFS4				(ADCB_BASE+0x25)
#define ADCB_ADOFS5				(ADCB_BASE+0x26)
#define ADCB_ADOFS6				(ADCB_BASE+0x27)
#define ADCB_ADOFS7				(ADCB_BASE+0x28)


#define SCI0_BASE				0x1300			/* serial 0 base addr. */

#define SCI0_SCIBR				(SCI0_BASE+0)	/* baud rate reg */
#define SCI0_SCIBR_SBR_MASK		0x1fff			/* 12 bits clock divisor */

#define SCI0_SCICR				(SCI0_BASE+1)	/* control reg */
#define SCI0_SCICR_SBK_MASK		0x0001
#define SCI0_SCICR_RWU_MASK		0x0002
#define SCI0_SCICR_RE_MASK		0x0004
#define SCI0_SCICR_TE_MASK		0x0008
#define SCI0_SCICR_REIE_MASK	0x0010
#define SCI0_SCICR_RIE_MASK		0x0020
#define SCI0_SCICR_TIIE_MASK	0x0040
#define SCI0_SCICR_TEIE_MASK	0x0080
#define SCI0_SCICR_PT_MASK		0x0100
#define SCI0_SCICR_PE_MASK		0x0200
#define SCI0_SCICR_POL_MASK		0x0400
#define SCI0_SCICR_WAKE_MASK	0x0800
#define SCI0_SCICR_M_MASK		0x1000
#define SCI0_SCICR_RSRC_MASK	0x2000
#define SCI0_SCICR_SWAI_MASK	0x4000
#define SCI0_SCICR_LOOP_MASK	0x8000

#define SCI0_SCISR				(SCI0_BASE+2)	/* status reg */
#define SCI0_SCISR_RAF_MASK		0x0001
#define SCI0_SCISR_PF_MASK		0x0100
#define SCI0_SCISR_FE_MASK		0x0200
#define SCI0_SCISR_NF_MASK		0x0400
#define SCI0_SCISR_OR_MASK		0x0800
#define SCI0_SCISR_RIDLE_MASK	0x1000
#define SCI0_SCISR_RDRF_MASK	0x2000
#define SCI0_SCISR_TIDLE_MASK	0x4000
#define SCI0_SCISR_TDRE_MASK	0x8000

#define SCI0_SCIDR				(SCI0_BASE+3)	/* data reg */
#define SCI0_SCIDR_DATA			0x01ff

#define DFIU_BASE				0x1360

#define DFIU_CNTL				(DFIU_BASE)
#define DFIU_CNTL_BUSY_MASK		0x8000
#define DFIU_CNTL_IFREN_MASK	0x0040
#define DFIU_CNTL_XE_MASK		0x0020
#define DFIU_CNTL_YE_MASK		0x0010
#define DFIU_CNTL_PROG_MASK		0x0008
#define DFIU_CNTL_ERASE_MASK	0x0004
#define DFIU_CNTL_MAS1_MASK		0x0002
#define DFIU_CNTL_NVSTR_MASK	0x0001

#define DFIU_PE					(DFIU_BASE+0x1)
#define DFIU_PE_DPE_MASK		0x8000
#define DFIU_PE_IPE_MASK		0x4000

#define DFIU_EE					(DFIU_BASE+0x2)
#define DFIU_EE_DEE_MASK		0x8000
#define DFIU_EE_IEE_MASK		0x4000

#define DFIU_ADDR				(DFIU_BASE+0x3)
#define DFIU_DATA				(DFIU_BASE+0x4)
#define DFIU_IE					(DFIU_BASE+0x5)
#define DFIU_IS					(DFIU_BASE+0x6)
#define DFIU_IP					(DFIU_BASE+0x7)
#define DFIU_CKDIVISOR			(DFIU_BASE+0x8)
#define DFIU_TERASEL			(DFIU_BASE+0x9)
#define DFIU_TMEL				(DFIU_BASE+0xA)
#define DFIU_TNVSL				(DFIU_BASE+0xB)
#define DFIU_TPGSL				(DFIU_BASE+0xC)
#define DFIU_TPROGL				(DFIU_BASE+0xD)
#define DFIU_TNVHL				(DFIU_BASE+0xE)
#define DFIU_TNVHL1				(DFIU_BASE+0xF)
#define DFIU_TRCVL				(DFIU_BASE+0x10)

#define SPI_BASE                0x1320
#define SPSCR                   (SPI_BASE+0x0)
#define SPDSR                   (SPI_BASE+0x1)
#define SPDRR                   (SPI_BASE+0x2)
#define SPDTR                   (SPI_BASE+0x3)

#define GPIOA_BASE              0x13B0
#define GPIOB_BASE              0x13C0
#define GPIOD_BASE              0x13E0
#define GPIOE_BASE              0x13F0

#define GPIO_A_PUR              (GPIOA_BASE+0x00)
#define GPIO_A_DR               (GPIOA_BASE+0x01)
#define GPIO_A_DDR              (GPIOA_BASE+0x02)
#define GPIO_A_PER              (GPIOA_BASE+0x03)
#define GPIO_A_IAR              (GPIOA_BASE+0x04)
#define GPIO_A_IENR             (GPIOA_BASE+0x05)
#define GPIO_A_IPOLR            (GPIOA_BASE+0x06)
#define GPIO_A_IPR              (GPIOA_BASE+0x07)
#define GPIO_A_IESR             (GPIOA_BASE+0x08)
#define GPIO_A0                 0x01
#define GPIO_A1                 0x02
#define GPIO_A2                 0x04
#define GPIO_A3                 0x08
#define GPIO_A4                 0x10
#define GPIO_A5                 0x20
#define GPIO_A6                 0x40
#define GPIO_A7                 0x80

#define GPIO_B_PUR              (GPIOB_BASE+0x00)
#define GPIO_B_DR               (GPIOB_BASE+0x01)
#define GPIO_B_DDR              (GPIOB_BASE+0x02)
#define GPIO_B_PER              (GPIOB_BASE+0x03)
#define GPIO_B_IAR              (GPIOB_BASE+0x04)
#define GPIO_B_IENR             (GPIOB_BASE+0x05)
#define GPIO_B_IPOLR            (GPIOB_BASE+0x06)
#define GPIO_B_IPR              (GPIOB_BASE+0x07)
#define GPIO_B_IESR             (GPIOB_BASE+0x08)
#define GPIO_B0                 0x01
#define GPIO_B1                 0x02
#define GPIO_B2                 0x04
#define GPIO_B3                 0x08
#define GPIO_B4                 0x10
#define GPIO_B5                 0x20
#define GPIO_B6                 0x40
#define GPIO_B7                 0x80

#define GPIO_D_PUR              (GPIOD_BASE+0x00)
#define GPIO_D_DR               (GPIOD_BASE+0x01)
#define GPIO_D_DDR              (GPIOD_BASE+0x02)
#define GPIO_D_PER              (GPIOD_BASE+0x03)
#define GPIO_D_IAR              (GPIOD_BASE+0x04)
#define GPIO_D_IENR             (GPIOD_BASE+0x05)
#define GPIO_D_IPOLR            (GPIOD_BASE+0x06)
#define GPIO_D_IPR              (GPIOD_BASE+0x07)
#define GPIO_D_IESR             (GPIOD_BASE+0x08)
#define GPIO_D0                 0x01
#define GPIO_D1                 0x02
#define GPIO_D2                 0x04
#define GPIO_D3                 0x08
#define GPIO_D4                 0x10
#define GPIO_D5                 0x20
#define GPIO_D6                 0x40
#define GPIO_D7                 0x80

#define GPIO_E_PUR              (GPIOE_BASE+0x00)
#define GPIO_E_DR               (GPIOE_BASE+0x01)
#define GPIO_E_DDR              (GPIOE_BASE+0x02)
#define GPIO_E_PER              (GPIOE_BASE+0x03)
#define GPIO_E_IAR              (GPIOE_BASE+0x04)
#define GPIO_E_IENR             (GPIOE_BASE+0x05)
#define GPIO_E_IPOLR            (GPIOE_BASE+0x06)
#define GPIO_E_IPR              (GPIOE_BASE+0x07)
#define GPIO_E_IESR             (GPIOE_BASE+0x08)
#define GPIO_E0                 0x01
#define GPIO_E1                 0x02
#define GPIO_E2                 0x04
#define GPIO_E3                 0x08
#define GPIO_E4                 0x10
#define GPIO_E5                 0x20
#define GPIO_E6                 0x40
#define GPIO_E7                 0x80

/*
 * error types.
 *
 */
#define ERR_OK					 0
#define ERR_TXFULL 				-1
#define ERR_RXEMPTY				-2
#define ERR_COMMON				-3
#define ERR_VALUE				-4
#define ERR_RANGE				-5
#define ERR_SPEED				-6
#define ERR_BUSY				-7
#define ERR_NOTAVAIL			-8
#define ERR_ABS_SSI             -9

#endif
