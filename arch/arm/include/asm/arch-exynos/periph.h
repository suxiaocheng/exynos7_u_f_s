/*
 * Copyright (C) 2012 Samsung Electronics
 * Rajeshwari Shinde <rajeshwari.s@samsung.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __ASM_ARM_ARCH_PERIPH_H
#define __ASM_ARM_ARCH_PERIPH_H

/*
 * Peripherals requiring clock/pinmux configuration. List will
 * grow with support for more devices getting added.
 *
 */
enum periph_id {
	PERIPH_ID_SDMMC0,
	PERIPH_ID_SDMMC1,
	PERIPH_ID_SDMMC2,
	PERIPH_ID_SDMMC3,
	PERIPH_ID_SROMC,
	PERIPH_ID_UART0,
	PERIPH_ID_UART1,
	PERIPH_ID_UART2,
	PERIPH_ID_UART3,
        PERIPH_ID_INPUT_A0_3,
        PERIPH_ID_INPUT_A2_0,
        PERIPH_ID_INPUT_A2_1,
        PERIPH_ID_INPUT_X0_0,
        PERIPH_ID_INPUT_X2_0,
	PERIPH_ID_UFS0,

	PERIPH_ID_COUNT,
	PERIPH_ID_NONE = -1,
};

#endif /* __ASM_ARM_ARCH_PERIPH_H */
