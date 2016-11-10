/*
 * Clock setup for SMDK5412 board based on EXYNOS5
 *
 * Copyright (C) 2013 Samsung Electronics
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <config.h>
#include <version.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <asm/arch/power.h>
#include <asm/arch/cpu.h>
#include <asm/arch/gpio.h>
#include "setup.h"

void system_clock_init()
{
	struct exynos5412_clock *clk =
		(struct exynos5412_clock *)EXYNOS5_CLOCK_BASE;

	/* Set CMU_CPU, MUX & DIV */
	writel(0x00000000, &clk->src_cpu);
	writel(0x00000000, &clk->src_top7);
	writel(0x00000000, &clk->src_cdrex);
	writel(0x00000000, &clk->src_cci);
	writel(0x00000000, &clk->src_kfc);
	writel(0x00000077, &clk->div_cpu1);
	writel(0x03550020, &clk->div_cpu0);

	/* Set APLL */
	writel(0x00000258, &clk->apll_lock);
	writel(0x0020F300, &clk->apll_con1);
	writel(0x80C80301, &clk->apll_con0);
	while(!(readl(&clk->apll_con0) & (0x1 << 29)));

	/* Set KPLL */
	writel(0x00000190, &clk->kpll_lock);
	writel(0x00008000, &clk->src_kfc);
	writel(0x03400720, &clk->div_kfc0);
	writel(0x00200000, &clk->kpll_con1);
	writel(0x80C80202, &clk->kpll_con0);
	while(!(readl(&clk->kpll_con0) & (0x1 << 29)));

	/* Set MPLL */
	writel(0x00000258, &clk->mpll_lock);
	writel(0x0020F300, &clk->mpll_con1);
	writel(0x810A0302, &clk->mpll_con0);
	while(!(readl(&clk->mpll_con0) & (0x1 << 29)));

	/* Set DPLL */
	writel(0x00000190, &clk->dpll_lock);
	writel(0x0020F300, &clk->dpll_con1);
	writel(0x80640201, &clk->dpll_con0);
	while(!(readl(&clk->dpll_con0) & (0x1 << 29)));

	/* Set EPLL */
	writel(0x00002328, &clk->epll_lock);
	writel(0x00000080, &clk->epll_con2);
	writel(0x00000000, &clk->epll_con1);
	writel(0x80C80304, &clk->epll_con0);
	while(!(readl(&clk->epll_con0) & (0x1 << 29)));

	/* Set RPLL */
	writel(0x00002328, &clk->rpll_lock);
	writel(0x00000080, &clk->rpll_con2);
	writel(0x00000000, &clk->rpll_con1);
	writel(0x810A0303, &clk->rpll_con0);
	while(!(readl(&clk->rpll_con0) & (0x1 << 29)));

	/* Set CPLL */
	writel(0x00000190, &clk->cpll_lock);
	writel(0x0020F300, &clk->cpll_con1);
	writel(0x806f0201, &clk->cpll_con0);
	while(!(readl(&clk->cpll_con0) & (0x1 << 29)));

	/* Set IPLL */
	writel(0x00000190, &clk->ipll_lock);
	writel(0x0020F300, &clk->ipll_con1);
	writel(0x80900202, &clk->ipll_con0);
	while(!(readl(&clk->ipll_con0) & (0x1 << 29)));

	/* Set VPLL */
	writel(0x00000258, &clk->vpll_lock);
	writel(0x0020F300, &clk->vpll_con1);
	writel(0x80C80302, &clk->vpll_con0);
	while(!(readl(&clk->vpll_con0) & (0x1 << 29)));

	/* Setting SPLL */
	writel(0x00000258, &clk->spll_lock);
	writel(0x0020F300, &clk->spll_con1);
	writel(0x80C80302, &clk->spll_con0);
	while(!(readl(&clk->spll_con0) & (0x1 << 29)));

	/* Setting BPLL(466MHz) */
	writel(0x00000258, &clk->bpll_lock);
	writel(0x0020f300, &clk->bpll_con1);
	writel(0x80E90302, &clk->bpll_con0);
	while(!(readl(&clk->bpll_con0) & (0x1 << 29)));

	/* Setting XPLL(825MHz) */
	writel(0x00000320, &clk->xpll_lock);
	writel(0x0020f300, &clk->xpll_con1);
	writel(0x81130401, &clk->xpll_con0);
	while(!(readl(&clk->xpll_con0) & (0x1 << 29)));

	/* Setting TPLL(532MHz) */
	writel(0x00000258, &clk->tpll_lock);
	writel(0x0020f300, &clk->tpll_con1);
	writel(0x810a0302, &clk->tpll_con0);
	while(!(readl(&clk->tpll_con0) & (0x1 << 29)));

	/* Set CMU_CDREX, MUX & DIV */
	writel(0x30010100, &clk->div_cdrex0);
	writel(0x00000300, &clk->div_cdrex1);

	/* Set CMU_TOP, MUX & DIV */
	writel(0x11101102, &clk->src_top0);
	writel(0x00200000, &clk->src_top1);
	writel(0x1510C030, &clk->src_top2);
	writel(0x03033300, &clk->src_top9);

	writel(0x22502200, &clk->div_top0);
	writel(0x13100900, &clk->div_top1);
	writel(0x12101713, &clk->div_top2);
	writel(0x00901000, &clk->div_top3);

	writel(0x00010011, &clk->src_top10);
	writel(0x00000000, &clk->src_top11);
	writel(0x44040000, &clk->src_top12);

	writel(0x11111111, &clk->src_top3);
	writel(0x11111111, &clk->src_top4);
	writel(0x15111011, &clk->src_top5);
	writel(0x00111111, &clk->src_top6);
	writel(0x00000001, &clk->src_top8);
	writel(0x10000000, &clk->src_disp10);

	writel(0x00044400, &clk->src_fsys);
	writel(0x00000000, &clk->div_fsys0);
	writel(0x00902809, &clk->div_fsys1);

	writel(0x03033330, &clk->src_peric0);
	writel(0x90999900, &clk->div_peric0);
	writel(0x00000030, &clk->div_cci0);

	/* Turn On PLL Mout */
	writel(0x00000001, &clk->src_cpu);
	writel(0x00000001, &clk->src_kfc);
	writel(0x11111111, &clk->src_top7);
	writel(0x00000001, &clk->src_cci);
	writel(0x00000001, &clk->src_cdrex);
}
