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

#ifndef __PMIC_MIC23156_CHG_VQ1V1_H__
#define __PMIC_MIC23156_CHG_VQ1V1_H__

//GPD5_2 SDA
//GPD5_3 SCL

#define GPD5CON		*(volatile unsigned int *)(0x13470120)
#define GPD5DAT		*(volatile unsigned int *)(0x13470124)
#define GPD5PUD		*(volatile unsigned int *)(0x13470128)

#define IIC_DISP_ESCL_Hi		GPD5DAT |= (0x1<<3)
#define IIC_DISP_ESCL_Lo	GPD5DAT &= ~(0x1<<3)
#define IIC_DISP_ESDA_Hi	GPD5DAT |= (0x1<<2)
#define IIC_DISP_ESDA_Lo	GPD5DAT &= ~(0x1<<2)

#define IIC_DISP_ESCL_INP	GPD5CON &= ~(0xf<<12)
#define IIC_DISP_ESCL_OUTP	GPD5CON = (GPD5CON & ~(0xf<<12))|(0x1<<12)
#define IIC_DISP_ESDA_INP	GPD5CON &= ~(0xf<<8)
#define IIC_DISP_ESDA_OUTP	GPD5CON = (GPD5CON & ~(0xf<<8))|(0x1<<8)

#define GPIO_DAT		GPD5DAT
#define GPIO_DAT_SHIFT	(2)
#define DIS_GPIO_PUD	GPD5PUD &= ~(0xf<<4)

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

extern void pmic_mic23156_chg_vq1v1_init(unsigned int val);


#endif /*__PMIC_DISPLAY_H__*/

