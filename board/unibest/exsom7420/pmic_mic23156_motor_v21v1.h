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

#ifndef __PMIC_MIC23156_MOTOR_V21V1_H__
#define __PMIC_MIC23156_MOTOR_V21V1_H__

//GPG3_2 SDA
//GPG3_3 SCL

#define GPG3CON		*(volatile unsigned int *)(0x134701C0)
#define GPG3DAT		*(volatile unsigned int *)(0x134701C4)
#define GPG3PUD		*(volatile unsigned int *)(0x134701C8)

#define IIC_DISP_ESCL_Hi		GPG3DAT |= (0x1<<3)
#define IIC_DISP_ESCL_Lo	GPG3DAT &= ~(0x1<<3)
#define IIC_DISP_ESDA_Hi	GPG3DAT |= (0x1<<2)
#define IIC_DISP_ESDA_Lo	GPG3DAT &= ~(0x1<<2)

#define IIC_DISP_ESCL_INP	GPG3CON &= ~(0xf<<12)
#define IIC_DISP_ESCL_OUTP	GPG3CON = (GPG3CON & ~(0xf<<12))|(0x1<<12)
#define IIC_DISP_ESDA_INP	GPG3CON &= ~(0xf<<8)
#define IIC_DISP_ESDA_OUTP	GPG3CON = (GPG3CON & ~(0xf<<8))|(0x1<<8)

#define GPIO_DAT			GPG3DAT
#define GPIO_DAT_SHIFT		(2)
#define DIS_GPIO_PUD		GPG3PUD &= ~(0xf<<4)

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

extern void pmic_mic23156_motor_v21v1_init(unsigned int val);


#endif /*__PMIC_DISPLAY_H__*/

