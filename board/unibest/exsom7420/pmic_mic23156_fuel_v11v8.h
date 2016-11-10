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

#ifndef __PMIC_MIC23156_FUEL_VQ1V1_H__
#define __PMIC_MIC23156_FUEL_VQ1V1_H__

//GPD2_0 SDA
//GPD2_1 SCL

#define GPD2CON		*(volatile unsigned int *)(0x134700E0)
#define GPD2DAT		*(volatile unsigned int *)(0x134700E4)
#define GPD2PUD		*(volatile unsigned int *)(0x134700E8)

#define IIC_DISP_ESCL_Hi		GPD2DAT |= (0x1<<1)
#define IIC_DISP_ESCL_Lo	GPD2DAT &= ~(0x1<<1)
#define IIC_DISP_ESDA_Hi	GPD2DAT |= (0x1<<0)
#define IIC_DISP_ESDA_Lo	GPD2DAT &= ~(0x1<<0)

#define IIC_DISP_ESCL_INP	GPD2CON &= ~(0xf<<4)
#define IIC_DISP_ESCL_OUTP	GPD2CON = (GPD2CON & ~(0xf<<4))|(0x1<<4)
#define IIC_DISP_ESDA_INP	GPD2CON &= ~(0xf<<0)
#define IIC_DISP_ESDA_OUTP	GPD2CON = (GPD2CON & ~(0xf<<0))|(0x1<<0)

#define GPIO_DAT			GPD2DAT
#define GPIO_DAT_SHIFT		(0)
#define DIS_GPIO_PUD		GPD2PUD &= ~(0xf<<0)

#define DELAY		100

/* MIC23156 slave address */
#define MIC23156_W_ADDR	0xB6
#define MIC23156_R_ADDR	0xB7

/* MIC23156 Register Address */
#define MIC23156_CTRL			0x01
#define MIC23156_BUCK1			0x02

#define PGOOD	(1 << 4)
#define SSL		(1 << 1)
#define BUCK_EN	(1 << 0)

extern void pmic_mic23156_fuel_v11v8_init(unsigned int val);

#endif /*__PMIC_DISPLAY_H__*/

