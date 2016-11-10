/*
 * Copyright@ Samsung Electronics Co. LTD
 *
 * This software is proprietary of Samsung Electronics.
 * No part of this software, either material or conceptual may be copied or distributed, transmitted,
 * transcribed, stored in a retrieval system or translated into any human or computer language in any form by any means,
 * electronic, mechanical, manual or otherwise, or disclosed
 * to third parties without the express written permission of Samsung Electronics.


 * Alternatively, this program is free software in case of open source projec;
 * you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.

 */

#include <asm/arch/cpu.h>

#ifndef __ASM_ARM_ARCH_TMU_H_
#define __ASM_ARM_ARCH_TMU_H_

#define EXYNOS5430_TMU_TRIMINFO_0_REG		(EXYNOS5430_TMU0_BASE + 0x0000)
#define EXYNOS5430_TMU_TRIMINFO_1_REG		(EXYNOS5430_TMU0_BASE + 0x8000)
#define EXYNOS5430_TMU_TRIMINFO_2_REG		(EXYNOS5430_TMU1_BASE + 0x0000)
#define EXYNOS5430_TMU_TRIMINFO_3_REG		(EXYNOS5430_TMU1_BASE + 0x8000)
#define EXYNOS5430_TMU_TRIMINFO_4_REG		(EXYNOS5430_TMU1_BASE + 0xC000)

#define EXYNOS5430_TMU_CTRL_0_REG		(EXYNOS5430_TMU0_BASE + 0x0020)
#define EXYNOS5430_TMU_CTRL_1_REG		(EXYNOS5430_TMU0_BASE + 0x8020)
#define EXYNOS5430_TMU_CTRL_2_REG		(EXYNOS5430_TMU1_BASE + 0x0020)
#define EXYNOS5430_TMU_CTRL_3_REG		(EXYNOS5430_TMU1_BASE + 0x8020)
#define EXYNOS5430_TMU_CTRL_4_REG		(EXYNOS5430_TMU1_BASE + 0xC020)

#define EXYNOS5430_TMU_STATUS_0_REG		(EXYNOS5430_TMU0_BASE + 0x0028)
#define EXYNOS5430_TMU_STATUS_1_REG		(EXYNOS5430_TMU0_BASE + 0x8028)
#define EXYNOS5430_TMU_STATUS_2_REG		(EXYNOS5430_TMU1_BASE + 0x0028)
#define EXYNOS5430_TMU_STATUS_3_REG		(EXYNOS5430_TMU1_BASE + 0x8028)
#define EXYNOS5430_TMU_STATUS_4_REG		(EXYNOS5430_TMU1_BASE + 0xC028)

#define EXYNOS5430_TMU_CURTEMP_0_REG		(EXYNOS5430_TMU0_BASE + 0x0040)
#define EXYNOS5430_TMU_CURTEMP_1_REG		(EXYNOS5430_TMU0_BASE + 0x8040)
#define EXYNOS5430_TMU_CURTEMP_2_REG		(EXYNOS5430_TMU1_BASE + 0x0040)
#define EXYNOS5430_TMU_CURTEMP_3_REG		(EXYNOS5430_TMU1_BASE + 0x8040)
#define EXYNOS5430_TMU_CURTEMP_4_REG		(EXYNOS5430_TMU1_BASE + 0xC040)

#define EXYNOS5433_OTP_TEMP25_ATLAS0	7
#define EXYNOS5433_OTP_TEMP85_ATLAS0	8
#define EXYNOS5433_OTP_VPTAT_ATLAS0		10
#define EXYNOS5433_OTP_CALTYPE_ATLAS0	11
#define EXYNOS5433_OTP_TEMP25_ATLAS1	12
#define EXYNOS5433_OTP_TEMP85_ATLAS1	13
#define EXYNOS5433_OTP_VPTAT_ATLAS1		15
#define EXYNOS5433_OTP_CALTYPE_ATLAS1	16
#define EXYNOS5433_OTP_TEMP25_APOLO		17
#define EXYNOS5433_OTP_TEMP85_APOLO		18
#define EXYNOS5433_OTP_VPTAT_APOLO		20
#define EXYNOS5433_OTP_CALTYPE_APOLO	21
#define EXYNOS5433_OTP_TEMP25_GPU		22
#define EXYNOS5433_OTP_TEMP85_GPU		23
#define EXYNOS5433_OTP_VPTAT_GPU		25
#define EXYNOS5433_OTP_CALTYPE_GPU		26
#define EXYNOS5433_OTP_TEMP25_ISP		27
#define EXYNOS5433_OTP_TEMP85_ISP		28
#define EXYNOS5433_OTP_VPTAT_ISP		30
#define EXYNOS5433_OTP_CALTYPE_ISP		31

/* EXYNOS7420 TMU REGISTER */
#define EXYNOS7420_TMU_TRIMINFO_0_REG		(EXYNOS7420_TMU_BASE + 0x0000)
#define EXYNOS7420_TMU_TRIMINFO_1_REG		(EXYNOS7420_TMU_BASE + 0x4000)
#define EXYNOS7420_TMU_TRIMINFO_2_REG		(EXYNOS7420_TMU_BASE + 0x8000)
#define EXYNOS7420_TMU_TRIMINFO_3_REG		(EXYNOS7420_TMU_BASE + 0xc000)

#define EXYNOS7420_TMU_CTRL_0_REG		(EXYNOS7420_TMU_BASE + 0x0020)
#define EXYNOS7420_TMU_CTRL_1_REG		(EXYNOS7420_TMU_BASE + 0x4020)
#define EXYNOS7420_TMU_CTRL_2_REG		(EXYNOS7420_TMU_BASE + 0x8020)
#define EXYNOS7420_TMU_CTRL_3_REG		(EXYNOS7420_TMU_BASE + 0xC020)

#define EXYNOS7420_TMU_STATUS_0_REG		(EXYNOS7420_TMU_BASE + 0x0028)
#define EXYNOS7420_TMU_STATUS_1_REG		(EXYNOS7420_TMU_BASE + 0x4028)
#define EXYNOS7420_TMU_STATUS_2_REG		(EXYNOS7420_TMU_BASE + 0x8028)
#define EXYNOS7420_TMU_STATUS_3_REG		(EXYNOS7420_TMU_BASE + 0xC028)

#define EXYNOS7420_TMU_CURTEMP_0_REG		(EXYNOS7420_TMU_BASE + 0x0040)
#define EXYNOS7420_TMU_CURTEMP_1_REG		(EXYNOS7420_TMU_BASE + 0x4040)
#define EXYNOS7420_TMU_CURTEMP_2_REG		(EXYNOS7420_TMU_BASE + 0x8040)
#define EXYNOS7420_TMU_CURTEMP_3_REG		(EXYNOS7420_TMU_BASE + 0xC040)

#endif
