/*
 * (C) Copyright 2014 Samsung Electronics Co. Ltd
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <common.h>
#include <asm/arch/cpu.h>
#include <asm/arch/smc.h>
#include "pmic.h"
#include "asv-exynos_cal.h"

#include "pmic_mic23156_chg_vq1v1.h"
#include "pmic_mic23156_fuel_v11v8.h"
#include "pmic_mic23156_motor_v21v1.h"

#define ASV_TBL_ADDR_BASE	(0x101E0160)

#ifndef CONFIG_SPL_BUILD
u32 uBigAsvGrpVolt;
u32 uLitAsvGrpVolt;
u32 uG3dAsvGrpVolt;
u32 uMifAsvGrpVolt;
u32 uIntAsvGrpVolt;
u32 uDisAsvGrpVolt;
#endif
u32 int_m_vthr = 0;
u32 int_m_delta = 0;

static unsigned char pmic_id;
u32 asv_efuse = 0;


void Delay(void)
{
	unsigned long i;
	for (i = 0; i < DELAY; i++)
		;
}

void IIC0_SCLH_SDAH(void)
{
	IIC0_ESCL_Hi;
	IIC0_ESDA_Hi;
	Delay();
}

void IIC0_SCLH_SDAL(void)
{
	IIC0_ESCL_Hi;
	IIC0_ESDA_Lo;
	Delay();
}

void IIC0_SCLL_SDAH(void)
{
	IIC0_ESCL_Lo;
	IIC0_ESDA_Hi;
	Delay();
}

void IIC0_SCLL_SDAL(void)
{
	IIC0_ESCL_Lo;
	IIC0_ESDA_Lo;
	Delay();
}

void IIC0_ELow(void)
{
	IIC0_SCLL_SDAL();
	IIC0_SCLH_SDAL();
	IIC0_SCLH_SDAL();
	IIC0_SCLL_SDAL();
}

void IIC0_EHigh(void)
{
	IIC0_SCLL_SDAH();
	IIC0_SCLH_SDAH();
	IIC0_SCLH_SDAH();
	IIC0_SCLL_SDAH();
}

void IIC0_EStart(void)
{
	IIC0_SCLH_SDAH();
	IIC0_SCLH_SDAL();
	Delay();
	IIC0_SCLL_SDAL();
}

void IIC0_EEnd(void)
{
	IIC0_SCLL_SDAL();
	IIC0_SCLH_SDAL();
	Delay();
	IIC0_SCLH_SDAH();
}

void IIC0_EAck_write(void)
{
	unsigned long ack;

	/* Function <- Input */
	IIC0_ESDA_INP;

	IIC0_ESCL_Lo;
	Delay();
	IIC0_ESCL_Hi;
	Delay();
	ack = GPIO_DAT;
	IIC0_ESCL_Hi;
	Delay();
	IIC0_ESCL_Hi;
	Delay();

	/* Function <- Output (SDA) */
	IIC0_ESDA_OUTP;

	ack = (ack >> GPIO_DAT_SHIFT) & 0x1;

	IIC0_SCLL_SDAL();
}

void IIC0_EAck_read(void)
{
	/* Function <- Output */
	IIC0_ESDA_OUTP;

	IIC0_ESCL_Lo;
	IIC0_ESCL_Lo;
	IIC0_ESDA_Hi;
	IIC0_ESCL_Hi;
	IIC0_ESCL_Hi;
	/* Function <- Input (SDA) */
	IIC0_ESDA_INP;

	IIC0_SCLL_SDAL();
}

void IIC0_ESetport(void)
{
	/* Pull Up/Down Disable	SCL, SDA */
	DIS_GPIO_PUD;

	IIC0_ESCL_Hi;
	IIC0_ESDA_Hi;

	/* Function <- Output (SCL) */
	IIC0_ESCL_OUTP;
	/* Function <- Output (SDA) */
	IIC0_ESDA_OUTP;

	Delay();
}

void IIC0_EWrite(unsigned char ChipId,
		unsigned char IicAddr, unsigned char IicData)
{
	unsigned long i;

	IIC0_EStart();

	/* write chip id */
	for (i = 7; i > 0; i--) {
		if ((ChipId >> i) & 0x0001)
			IIC0_EHigh();
		else
			IIC0_ELow();
	}

	/* write */
	IIC0_ELow();

	/* ACK */
	IIC0_EAck_write();

	/* write reg. addr. */
	for (i = 8; i > 0; i--) {
		if ((IicAddr >> (i-1)) & 0x0001)
			IIC0_EHigh();
		else
			IIC0_ELow();
	}

	/* ACK */
	IIC0_EAck_write();

	/* write reg. data. */
	for (i = 8; i > 0; i--) {
		if ((IicData >> (i-1)) & 0x0001)
			IIC0_EHigh();
		else
			IIC0_ELow();
	}

	/* ACK */
	IIC0_EAck_write();

	IIC0_EEnd();
}

void IIC0_ERead(unsigned char ChipId,
		unsigned char IicAddr, unsigned char *IicData)
{
	unsigned long i, reg;
	unsigned char data = 0;

	IIC0_EStart();

	/* write chip id */
	for (i = 7; i > 0; i--) {
		if ((ChipId >> i) & 0x0001)
			IIC0_EHigh();
		else
			IIC0_ELow();
	}

	/* write */
	IIC0_ELow();

	/* ACK */
	IIC0_EAck_write();

	/* write reg. addr. */
	for (i = 8; i > 0; i--) {
		if ((IicAddr >> (i-1)) & 0x0001)
			IIC0_EHigh();
		else
			IIC0_ELow();
	}

	/* ACK */
	IIC0_EAck_write();

	IIC0_EStart();

	/* write chip id */
	for (i = 7; i > 0; i--) {
		if ((ChipId >> i) & 0x0001)
			IIC0_EHigh();
		else
			IIC0_ELow();
	}

	/* read */
	IIC0_EHigh();
	/* ACK */
	IIC0_EAck_write();

	/* read reg. data. */
	IIC0_ESDA_INP;

	IIC0_ESCL_Lo;
	IIC0_ESCL_Lo;
	Delay();

	for (i = 8; i > 0; i--) {
		IIC0_ESCL_Lo;
		IIC0_ESCL_Lo;
		Delay();
		IIC0_ESCL_Hi;
		IIC0_ESCL_Hi;
		Delay();
		reg = GPIO_DAT;
		IIC0_ESCL_Hi;
		IIC0_ESCL_Hi;
		Delay();
		IIC0_ESCL_Lo;
		IIC0_ESCL_Lo;
		Delay();

		reg = (reg >> GPIO_DAT_SHIFT) & 0x1;

		data |= reg << (i-1);
	}

	/* ACK */
	IIC0_EAck_read();
	IIC0_ESDA_OUTP;

	IIC0_EEnd();

	*IicData = data;
}

void get_asv_efuse(void)
{
	u64 tmp = 0;

	tmp = exynos_smc_read(SMC_CMD_REG, SMC_REG_ID_SFR_R((unsigned long) ASV_TBL_ADDR_BASE + 0xC));
	asv_efuse = (u32)tmp;
}

static u32 get_ema_efuse(unsigned int offset)
{
	u64 tmp = 0;

	tmp = exynos_smc_read(SMC_CMD_REG, SMC_REG_ID_SFR_R((unsigned long) ASV_TBL_ADDR_BASE + offset));
	return (u32)tmp;
}

static void set_ema_atlas(u32 ema_con, u32 ema_reg, u32 offset_l1, u32 offset_l2, u32 default_l1, u32 default_l2)
{
	u32 fuse = 0;
	u32 mask = 0xF;

	fuse = get_ema_efuse(ASV3_2_ATLAS_L1);
	fuse = (fuse & (mask << offset_l1)) >> offset_l1;
	if (fuse == 0)
		ema_con = (ema_con & ~(mask << 4)) | (default_l1 << 4);
	else
		ema_con = (ema_con & ~(mask << 4)) | (fuse << 4);

	fuse = get_ema_efuse(ASV3_3_ATLAS_L2_INT_CAM);
	fuse = (fuse & (mask << offset_l2)) >> offset_l2;
	if (fuse == 0)
		ema_reg = (ema_con & ~(mask << 8)) | (default_l2 << 8);
	else
		ema_reg = (ema_con & ~(mask << 8)) | (fuse << 8);

	writel(ema_con, EXYNOS7420_SYSREG_BASE + ATLAS_EMA_CON);
	writel(ema_reg, EXYNOS7420_SYSREG_BASE + ATLAS_EMA_REG1);
}

static void set_ema_apollo(u32 ema_con, u32 offset, u32 value)
{
	u32 fuse = 0;
	u32 mask = 0x7;

	fuse = get_ema_efuse(ASV1_1_APOLLO_G3D);
	fuse = (fuse & (mask << offset)) >> offset;
	if (fuse == 0)
		ema_con = (ema_con & ~(mask << 20) & ~(mask << 8)) | (value << 20) | (value << 8);
	else
		ema_con = (ema_con & ~(mask << 20) & ~(mask << 8)) | (fuse << 20) | (fuse << 8);

	writel(ema_con, EXYNOS7420_SYSREG_BASE + APOLLO_EMA_CON);
}

static void set_ema_g3d(u32 ema_con, u32 offset, u32 value)
{
	u32 fuse = 0;
	u32 mask = 0x3;

	fuse = get_ema_efuse(ASV1_1_APOLLO_G3D);
	fuse = (fuse & (mask << offset)) >> offset;
	if (fuse == 0)
		ema_con = (ema_con & ~(mask << 4)) | (value << 4);
	else
		ema_con = (ema_con & ~(mask << 4)) | (fuse << 4);

	writel(ema_con, EXYNOS7420_SYSREG_BASE + G3D_EMA_RF2_UHD_CON);
}

static void set_ema_int_cam_disp(u32 ema_con, u32 offset, u32 value)
{
	u32 fuse = 0;
	u32 mask = 0x7;

	fuse = get_ema_efuse(ASV3_3_ATLAS_L2_INT_CAM);
	fuse = (fuse & (mask << offset)) >> offset;
	if (fuse == 0)
		ema_con = (ema_con & ~(mask << 0)) | (value << 0);
	else
		ema_con = (ema_con & ~(mask << 0)) | (fuse << 0);
	if (offset == 16) /* INT */
		writel(ema_con, EXYNOS7420_SYSREG_BASE + EMA_RF2_HS_CON_INT);
	else /* CAM_DISP */
		writel(ema_con, EXYNOS7420_SYSREG_BASE + EMA_RF2_HS_CON);
}

/*#define EMA_DEBUG*/
static void set_ema(u8 buck, u16 vdd)
{
	switch(buck) {
	case BUCK2_OUT:
		/* Atlas */
#if !defined(CONFIG_SPL_BUILD) && defined(EMA_DEBUG)
		printf("[EMA] set ATLAS\n");
#endif
		if (vdd >= 1000) {
			set_ema_atlas(0x62, 0x530, 16, 0, 0x6, 0x5);
		} else if ((vdd >= 900) && (vdd < 1000)) {
			set_ema_atlas(0x62, 0x530, 20, 4, 0x6, 0x5);
		} else if ((vdd >= 800) && (vdd < 900)) {
			set_ema_atlas(0x82, 0xb30, 24, 8, 0x8, 0xB);
		} else {
			set_ema_atlas(0x72, 0x930, 24, 12, 0x7, 0x9);
		}
		break;
	case BUCK3_OUT:
		/* Apollo */
#if !defined(CONFIG_SPL_BUILD) && defined(EMA_DEBUG)
		printf("[EMA] set APOLLO\n");
#endif
		if (vdd >= 1000) {
			set_ema_apollo(0x210211, 9, 0x2);
		} else if ((vdd >= 900) && (vdd < 1000)) {
			set_ema_apollo(0x210211, 12, 0x2);
		} else if ((vdd >= 800) && (vdd < 900)) {
			set_ema_apollo(0x210211, 15, 0x2);
		} else {
			set_ema_apollo(0x210211, 18, 0x2);
		}
		break;
	case BUCK6_OUT:
		/* G3D */
#if !defined(CONFIG_SPL_BUILD) && defined(EMA_DEBUG)
		printf("[EMA] set G3D\n");
#endif
		if (vdd >= 900) {
			set_ema_g3d(0x11, 21, 0x1);
		} else if ((vdd >= 800) && (vdd < 900)) {
			set_ema_g3d(0x11, 23, 0x1);
		} else if ((vdd >= 700) && (vdd < 800)) {
			set_ema_g3d(0x11, 25, 0x1);
		} else {
			set_ema_g3d(0x31, 27, 0x3);
		}
		break;
	case BUCK4_OUT:
		/* INT */
#if !defined(CONFIG_SPL_BUILD) && defined(EMA_DEBUG)
		printf("[EMA] set INT\n");
#endif
		set_ema_int_cam_disp(0x12, 16, 0x2);
		break;
	case BUCK5_OUT:
		/* CAM_DISP */
#if !defined(CONFIG_SPL_BUILD) && defined(EMA_DEBUG)
		printf("[EMA] set CAM_DISP\n");
#endif
		set_ema_int_cam_disp(0x12, 19, 0x2);
		break;
	default:
		while(1);
	}
}

#ifdef CONFIG_SPL_BUILD
static void set_vdd_with_ema(u8 buck, float vdd)
{
	/* make high vdd only */
	u32 max_val = 0;
	u32 int_val = 0;
	u32 cam_disp_val = 0;

	switch(buck) {
	case BUCK4_OUT:
		/* INT */
		int_val = (u32)vdd;
		IIC0_ERead(S2MPS15_ADDR, BUCK5_OUT, &cam_disp_val);
		cam_disp_val = RD_BUCK_VOLT_EVT1((unsigned int)cam_disp_val);
		max_val = MAX(int_val, cam_disp_val);
		max_val = MAX(max_val, int_m_vthr) + int_m_delta;
		IIC0_EWrite(S2MPS15_ADDR, LDO8_CTRL,
			(WR_LDO_VOLT(max_val) | (0x3 << 6)));
		break;
	case BUCK5_OUT:
		/* DISP_CAM */
		IIC0_ERead(S2MPS15_ADDR, BUCK4_OUT, &int_val);
		int_val = RD_BUCK_VOLT_EVT1((unsigned int)int_val);
		cam_disp_val = (u32)vdd;
		max_val = MAX(int_val, cam_disp_val);
		max_val = MAX(max_val, int_m_vthr) + int_m_delta;
		IIC0_EWrite(S2MPS15_ADDR, LDO8_CTRL,
			(WR_LDO_VOLT(max_val) | (0x3 << 6)));
		break;
	}

	IIC0_EWrite(S2MPS15_ADDR, buck, WR_BUCK_VOLT_EVT1(vdd));
	set_ema(buck, (u16)vdd);
}
#else
static void set_vdd_with_ema(u8 buck, float vdd)
{
	u16 old_vdd = 0;
	u32 max_val = 0;
	u32 int_val = 0;
	u32 cam_disp_val = 0;

	switch(buck) {
	case BUCK4_OUT:
		/* INT */
		int_val = (u32)vdd;
		IIC0_ERead(S2MPS15_ADDR, BUCK5_OUT, &cam_disp_val);
		cam_disp_val = RD_BUCK_VOLT_EVT1((unsigned int)cam_disp_val);
		max_val = MAX(int_val, cam_disp_val);
		max_val = MAX(max_val, int_m_vthr) + int_m_delta;
#if defined(EMA_DEBUG)
	printf("\n");
	printf("[EMA] int_val : %d, cam_disp_val : %d\n", int_val, cam_disp_val);
	printf("[EMA] max_val : %d, int_m_vthr : %d\n", max_val, int_m_vthr);
	printf("\n");
#endif
		break;
	case BUCK5_OUT:
		/* DISP_CAM */
		IIC0_ERead(S2MPS15_ADDR, BUCK4_OUT, &int_val);
		int_val = RD_BUCK_VOLT_EVT1((unsigned int)int_val);
		cam_disp_val = (u32)vdd;
		max_val = MAX(int_val, cam_disp_val);
		max_val = MAX(max_val, int_m_vthr) + int_m_delta;
#if defined(EMA_DEBUG)
	printf("\n");
	printf("[EMA] int_val : %d, cam_disp_val : %d\n", int_val, cam_disp_val);
	printf("[EMA] max_val : %d, int_m_vthr : %d\n", max_val, int_m_vthr);
	printf("\n");
#endif
		break;
	}

	IIC0_ERead(S2MPS15_ADDR, buck, &old_vdd);
#if defined(EMA_DEBUG)
	printf("[EMA] old : %d, new : %d\n", RD_BUCK_VOLT_EVT1(old_vdd), (u16)vdd);
#endif
	if (RD_BUCK_VOLT_EVT1(old_vdd) == (u16)vdd) {
		return ;
	} else if (RD_BUCK_VOLT_EVT1(old_vdd) < (u16)vdd) {
		/* make high vdd */
#if defined(EMA_DEBUG)
		printf("[EMA] set high\n");
#endif
		if ((buck == BUCK4_OUT) || (buck == BUCK5_OUT))
			IIC0_EWrite(S2MPS15_ADDR, LDO8_CTRL,
				(WR_LDO_VOLT(max_val) | (0x3 << 6)));
		IIC0_EWrite(S2MPS15_ADDR, buck, WR_BUCK_VOLT_EVT1(vdd));
		set_ema(buck, (u16)vdd);
	} else {
		/* make low vdd */
#if defined(EMA_DEBUG)
		printf("[EMA] set low\n");
#endif
		set_ema(buck, (u16)vdd);
		IIC0_EWrite(S2MPS15_ADDR, buck, WR_BUCK_VOLT_EVT1(vdd));
		if ((buck == BUCK4_OUT) || (buck == BUCK5_OUT)) {
			IIC0_EWrite(S2MPS15_ADDR, LDO8_CTRL,
				(WR_LDO_VOLT(max_val) | (0x3 << 6)));
		}
	}
}
#endif

static void set_ldo_dvs1(void)
{
	u32 fuse = 0;
	u32 mask = 0x3;
	u32 value = 0;

	/* vthr */
	fuse = get_ema_efuse(ASV3_2_ATLAS_L1);
	fuse = (fuse & (mask << 0)) >> 0;
	switch(fuse) {
	case 0b00:
		/* 800mV */
		value = (value & ~(mask << 0)) | (0b100 << 5);
		break;
	case 0b01:
		/* 750mV */
		value = (value & ~(mask << 0)) | (0b011 << 5);
		break;
	case 0b10:
		/* 700mV */
		value = (value & ~(mask << 0)) | (0b010 << 5);
		break;
	case 0b11:
		/* 900mV */
		value = (value & ~(mask << 0)) | (0b110 << 5);
		break;
	default:
		while(1);
	}

	/* delta */
	fuse = get_ema_efuse(ASV3_2_ATLAS_L1);
	fuse = (fuse & (mask << 2)) >> 2;
	value = (value & ~(mask << 3)) | (fuse << 3);

	/* sync_en */
	value = value | (0x1 << 2);

	IIC0_EWrite(S2MPS15_ADDR, LDO_DVS1, value);
}

static void set_ldo_dvs2(void)
{
	u32 fuse = 0;
	u32 mask = 0x3;
	u32 value = 0;

	/* vthr */
	fuse = get_ema_efuse(ASV3_2_ATLAS_L1);
	fuse = (fuse & (mask << 12)) >> 12;
	switch(fuse) {
	case 0b00:
		/* 750mV */
		value = (value & ~(mask << 0)) | (0b011 << 5);
		int_m_vthr = 750;
		break;
	case 0b01:
		/* 800mV */
		value = (value & ~(mask << 0)) | (0b100 << 5);
		int_m_vthr = 800;
		break;
	case 0b10:
		/* 700mV */
		value = (value & ~(mask << 0)) | (0b010 << 5);
		int_m_vthr = 700;
		break;
	case 0b11:
		/* 850mV */
		value = (value & ~(mask << 0)) | (0b101 << 5);
		int_m_vthr = 850;
		break;
	default:
		while(1);
	}

	/* delta */
	fuse = get_ema_efuse(ASV3_2_ATLAS_L1);
	fuse = (fuse & (mask << 14)) >> 14;
	int_m_delta = 25 + 25 * fuse;
	value = (value & ~(mask << 3)) | (fuse << 3);

	/* sync_en : disable */
	value = value | (0x0 << 2);

	IIC0_EWrite(S2MPS15_ADDR, LDO_DVS2, value);
}

static void set_ldo_dvs3(void)
{
	u32 fuse = 0;
	u32 mask = 0x3;
	u32 value = 0;

	/* vthr */
	fuse = get_ema_efuse(ASV3_2_ATLAS_L1);
	fuse = (fuse & (mask << 4)) >> 4;
	switch(fuse) {
	case 0b00:
		/* 800mV */
		value = (value & ~(mask << 0)) | (0b100 << 5);
		break;
	case 0b01:
		/* 750mV */
		value = (value & ~(mask << 0)) | (0b011 << 5);
		break;
	case 0b10:
		/* 700mV */
		value = (value & ~(mask << 0)) | (0b010 << 5);
		break;
	case 0b11:
		/* 900mV */
		value = (value & ~(mask << 0)) | (0b110 << 5);
		break;
	default:
		while(1);
	}

	/* delta */
	fuse = get_ema_efuse(ASV3_2_ATLAS_L1);
	fuse = (fuse & (mask << 6)) >> 6;
	value = (value & ~(mask << 3)) | (fuse << 3);

	/* sync_en */
	value = value | (0x1 << 2);

	IIC0_EWrite(S2MPS15_ADDR, LDO_DVS3, value);
}

static void set_ldo_dvs4(void)
{
	u32 fuse = 0;
	u32 mask = 0x3;
	u32 value = 0;

	/* vthr */
	fuse = get_ema_efuse(ASV3_2_ATLAS_L1);
	fuse = (fuse & (mask << 8)) >> 8;
	switch(fuse) {
	case 0b00:
		/* 750mV */
		value = (value & ~(mask << 0)) | (0b011 << 5);
		break;
	case 0b01:
		/* 800mV */
		value = (value & ~(mask << 0)) | (0b100 << 5);
		break;
	case 0b10:
		/* 700mV */
		value = (value & ~(mask << 0)) | (0b010 << 5);
		break;
	case 0b11:
		/* 850mV */
		value = (value & ~(mask << 0)) | (0b101 << 5);
		break;
	default:
		while(1);
	}

	/* delta */
	fuse = get_ema_efuse(ASV3_2_ATLAS_L1);
	fuse = (fuse & (mask << 10)) >> 10;
	value = (value & ~(mask << 3)) | (fuse << 3);

	/* sync_en */
	value = value | (0x1 << 2);

	IIC0_EWrite(S2MPS15_ADDR, LDO_DVS4, value);
}

void pmic_init(void)
{
	IIC0_ESetport();

	set_ldo_dvs1();
	set_ldo_dvs2();
	set_ldo_dvs3();
	set_ldo_dvs4();

	if (pmic_id >= 0x11) {
		/* BUCK Control (EVT1) */
		IIC0_EWrite(S2MPS15_ADDR, BUCK1_OUT,
				WR_BUCK_VOLT_EVT1(CONFIG_PM_VDD_MIF));
		set_vdd_with_ema(BUCK2_OUT, (float)CONFIG_PM_VDD_ATLAS);
		set_vdd_with_ema(BUCK3_OUT, (float)CONFIG_PM_VDD_APOLLO);
		set_vdd_with_ema(BUCK4_OUT, (float)CONFIG_PM_VDD_INT);
		set_vdd_with_ema(BUCK5_OUT, (float)CONFIG_PM_VDD_DISP);
		set_vdd_with_ema(BUCK6_OUT, (float)CONFIG_PM_VDD_G3D);
		writel(0x7, EXYNOS7420_SYSREG_BASE + IMEM_EMA_VROMP_HD_CON);
	} else {
#ifndef CONFIG_MACH_BTYPE7420_UPOP
		/* BUCK Control (EVT0) */
		IIC0_EWrite(S2MPS15_ADDR, BUCK1_OUT,
				WR_BUCK_VOLT(CONFIG_PM_VDD_MIF));
		IIC0_EWrite(S2MPS15_ADDR, BUCK2_OUT,
				WR_BUCK_VOLT(CONFIG_PM_VDD_ATLAS));
		IIC0_EWrite(S2MPS15_ADDR, BUCK3_OUT,
				WR_BUCK_VOLT(CONFIG_PM_VDD_APOLLO));
		IIC0_EWrite(S2MPS15_ADDR, BUCK4_OUT,
				WR_BUCK_VOLT(CONFIG_PM_VDD_INT));
		IIC0_EWrite(S2MPS15_ADDR, BUCK5_OUT,
				WR_BUCK_VOLT(CONFIG_PM_VDD_DISP));
		IIC0_EWrite(S2MPS15_ADDR, BUCK6_OUT,
				WR_BUCK_VOLT(CONFIG_PM_VDD_G3D));
#endif /* CONFIG_MACH_BTYPE7420_UPOP */
	}

#if 0
	pmic_mic23156_chg_vq1v1_init(40);
	pmic_mic23156_motor_v21v1_init(40);
	pmic_mic23156_fuel_v11v8_init(110);
#endif
}

#ifndef CONFIG_SPL_BUILD
void pmic_asv_init(void)
{
	unsigned char read_vol_int = 0;

	set_ldo_dvs1();
	set_ldo_dvs2();
	set_ldo_dvs3();
	set_ldo_dvs4();

	/* MIF */
	IIC0_EWrite(S2MPS15_ADDR, BUCK1_OUT,
		WR_BUCK_VOLT_EVT1((float)uMifAsvGrpVolt/1000));
	/* EGL */
	set_vdd_with_ema(BUCK2_OUT, ((float)uBigAsvGrpVolt/1000));
	/* KFC */
	set_vdd_with_ema(BUCK3_OUT, ((float)uLitAsvGrpVolt/1000));
	/* INT */
	set_vdd_with_ema(BUCK4_OUT, ((float)uIntAsvGrpVolt/1000));
	/* CAM0_DISP */
	set_vdd_with_ema(BUCK5_OUT, (float)uDisAsvGrpVolt/1000);
	/* G3D */
	set_vdd_with_ema(BUCK6_OUT, ((float)uG3dAsvGrpVolt/1000));
}

void Get_ASV_Group(void)
{
	cal_init();
	get_asv_efuse();

	/* get voltage from table */
	uBigAsvGrpVolt = cal_get_volt(SYSC_DVFS_BIG, 13);
	uLitAsvGrpVolt = cal_get_volt(SYSC_DVFS_LIT, 8);
	uG3dAsvGrpVolt = cal_get_volt(SYSC_DVFS_G3D, 7);
	asv_efuse = (asv_efuse & 0xf);
	if ((asv_efuse <= 1) || (asv_efuse == 4) || (asv_efuse >= 9)) {
		/* 1264 MHz */
		uMifAsvGrpVolt = cal_get_volt(SYSC_DVFS_MIF, 2);
	} else if ((asv_efuse == 2) || (asv_efuse == 3) || (asv_efuse == 6)
		|| (asv_efuse == 7) || (asv_efuse == 8)) {
		/* 1552MHz */
		uMifAsvGrpVolt = cal_get_volt(SYSC_DVFS_MIF, 0);
	}
	uIntAsvGrpVolt = cal_get_volt(SYSC_DVFS_INT, 0);
	uDisAsvGrpVolt = cal_get_volt(SYSC_DVFS_CAM, 0);
}
#endif

void pmic_enable_peric_dev(void)
{
	unsigned char ldo_ctrl;
	unsigned char rtc_ctrl;
	unsigned char wrstbi_ctrl;
	unsigned char wrstbi_addr = 0;
	unsigned char wrstbi_en = 0;

	IIC0_ESetport();

	/* read pmic revision number */
	IIC0_ERead(S2MPS15_ADDR, S2MPS15_PMIC_ID, &pmic_id);		
	
	if (pmic_id >= 0x11) {
		/* BUCK Control (EVT1) */
		wrstbi_addr = WRSTBI_CTRL_EVT1;
		wrstbi_en = WRSTBI_EN_EVT1;
	} else {
		/* BUCK Control (EVT0) */
		wrstbi_addr = WRSTBI_CTRL_EVT0;
		wrstbi_en = WRSTBI_EN_EVT0;
	}

	/* enable WRSTBI detection */
	IIC0_ERead(S2MPS15_ADDR, wrstbi_addr, &wrstbi_ctrl);
	wrstbi_ctrl |= wrstbi_en;
	IIC0_EWrite(S2MPS15_ADDR, wrstbi_addr, wrstbi_ctrl);

	/* BT 32KHz On, CP 32KHz On, AP 32KHz On */
	IIC0_ERead(S2MPS15_ADDR, RTC_BUF, &rtc_ctrl);
	rtc_ctrl |= (BT32KHZ_EN | CP32KHZ_EN | AP32KHZ_EN);
	IIC0_EWrite(S2MPS15_ADDR, RTC_BUF, rtc_ctrl);

	/* power control for SERIAL */
	/* enable LDO14 : VCC_2.8V_PERI */
	IIC0_ERead(S2MPS15_ADDR, LDO14_CTRL, &ldo_ctrl);
	ldo_ctrl |= OUTPUT_PWREN_ON;
	IIC0_EWrite(S2MPS15_ADDR, LDO14_CTRL, ldo_ctrl);

	/* enable LDO15 : VCC_1.8V_PERI */
	IIC0_ERead(S2MPS15_ADDR, LDO15_CTRL, &ldo_ctrl);
	ldo_ctrl |= OUTPUT_PWREN_ON;
	IIC0_EWrite(S2MPS15_ADDR, LDO15_CTRL, ldo_ctrl);

	/* enable LDO17 : VCC_2.8V_PERI */
	IIC0_ERead(S2MPS15_ADDR, LDO17_CTRL, &ldo_ctrl);
	ldo_ctrl |= OUTPUT_PWREN_ON;
	IIC0_EWrite(S2MPS15_ADDR, LDO17_CTRL, ldo_ctrl);

#if defined(CONFIG_M400)
	IIC0_ERead(S2MPS15_ADDR, LDO27_CTRL, &ldo_ctrl);
	ldo_ctrl |= OUTPUT_PWREN_ON;
	IIC0_EWrite(S2MPS15_ADDR, LDO27_CTRL, ldo_ctrl);
#endif
}

unsigned char get_pmic_id(void)
{
	unsigned pmic_id0;
	IIC0_ESetport();

	IIC0_ERead(S2MPS15_ADDR, S2MPS15_PMIC_ID, &pmic_id0);

	return pmic_id0;
}

#if 0
void pmic_s2mps15_buck7(unsigned int val)
{
	IIC0_EWrite(S2MPS15_ADDR, BUCK7_OUT, val);
}

void pmic_s2mps15_ldo16(unsigned int val)
{
	unsigned char ldo_ctrl;
	
	/* enable LDO15 : VCC_1.8V_PERI */
	//IIC0_ERead(S2MPS15_ADDR, LDO16_CTRL, &ldo_ctrl);
	ldo_ctrl = val | (1<<6);
	IIC0_EWrite(S2MPS15_ADDR, LDO16_CTRL, ldo_ctrl);
}
#endif
