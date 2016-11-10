/*
 * Copyright (C) 2009 Samsung Electronics
 * Minkyu Kang <mk7.kang@samsung.com>
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
#include <common.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <asm/arch/clk.h>

/* Each chip has own unique ID */
unsigned int s5p_chip_id[2] = {0x0, 0x0};

/* Default is s5pc100 */
unsigned int s5p_cpu_id = 0xC100;
/* Default is EVT1 */
unsigned int s5p_cpu_rev = 1;

#ifdef CONFIG_ARCH_CPU_INIT
int arch_cpu_init(void)
{
	s5p_set_cpu_id();

	return 0;
}
#endif

void arch_preboot_os(void)
{
#ifdef CONFIG_OF_LIBFDT
	prepare_dtb();
#endif
}

u32 get_device_type(void)
{
	return s5p_cpu_id;
}

#ifdef CONFIG_DISPLAY_CPUINFO
int print_cpuinfo(void)
{
#if defined(CONFIG_EXYNOS5) || defined(CONFIG_ARCH_EXYNOS5)
	unsigned int cpuid;
	unsigned int subrev;

	__asm__ __volatile__("mrs %0, midr_el1":"=r"(cpuid));
	subrev = (readl(EXYNOS5_PRO_ID) & 0x0000000F);

	printf("CPU: %s%x Rev%x.%x [Samsung SOC on SMP Platform Base on ARM CortexA5%d]\n"	\
		, s5p_get_cpu_name(), s5p_cpu_id, s5p_cpu_rev, subrev, ((cpuid >> 4) & 0xf));

#if defined(CONFIG_CPU_EXYNOS5430)
#ifdef CONFIG_CPU_EXYNOS5433
	return 0;
#endif
	if (s5p_cpu_rev != 0)
		return 0;
	unsigned int egl_pll = get_pll_clk(EGL_PLL);
	unsigned int kfc_pll = get_pll_clk(KFC_PLL);
	unsigned int mem_pll = get_pll_clk(MEM_PLL);
	unsigned int bus_pll = get_pll_clk(BUS_PLL);

	printf("EGL_PLL = %ldMHz, KFC_PLL = %ldMHz\n",
					egl_pll/1000000, kfc_pll/1000000);
	printf("MEM_PLL = %ldMHz, BUS_PLL = %ldMHz\n",
					mem_pll/1000000, bus_pll/1000000);

#endif
#elif defined(CONFIG_ARCH_EXYNOS7)
	unsigned int cpuid;
	unsigned int subrev;

	__asm__ __volatile__("mrs %0, midr_el1":"=r"(cpuid));
	subrev = (readl(EXYNOS7_PRO_ID) & 0x0000000F);

	printf("CPU: %s%x Rev%x.%x [Samsung SOC on SMP Platform Base on ARM CortexA5%d]\n"	\
		, s5p_get_cpu_name(), s5p_cpu_id, s5p_cpu_rev, subrev, ((cpuid >> 4) & 0xf));

#ifdef CONFIG_CPU_EXYNOS7420
	unsigned long atlas_pll = get_pll_clk(ATLAS_PLL);
	unsigned long apollo_pll = get_pll_clk(APOLLO_PLL);
	unsigned long mif_pll = get_pll_clk(MIF_PLL);
	unsigned long mfc_pll = get_pll_clk(MFC_PLL);
	unsigned long cci_pll = get_pll_clk(CCI_PLL);
	unsigned long bus0_pll = get_pll_clk(BUS0_PLL);
	unsigned long bus1_pll = get_pll_clk(BUS1_PLL);

	printf("ATLAS_PLL = %ldMHz\tAPOLLO_PLL = %ldMHz\tMIF_PLL = %ldMHz\n",
		atlas_pll/1000000, apollo_pll/1000000, mif_pll/1000000);
	printf("MFC_PLL = %ldMHz\tCCI_PLL = %ldMHz\n",
		mfc_pll/1000000, cci_pll/1000000);
	printf("BUS0_PLL = %ldMHz\tBUS1_PLL = %ldMHz\n",
		bus0_pll/1000000, bus1_pll/1000000);
#endif

#ifdef CONFIG_CPU_EXYNOS7580
       unsigned long cpu_pll = get_pll_clk(PLL_CPU);
       unsigned long apl_pll = get_pll_clk(PLL_APL);
       unsigned long mem0_pll = get_pll_clk(PLL_MEM0);
       unsigned long media_pll = get_pll_clk(PLL_MEDIA);
       unsigned long bus_pll = get_pll_clk(PLL_BUS);
       unsigned long usb_pll = get_pll_clk(PLL_USB);

       printf("PLL_CPU = %ldMHz\tPLL_APL = %ldMHz\n",
               cpu_pll/1000000, apl_pll/1000000);
       printf("PLL_MEM0 = %ldMHz\tPLL_MEDIA = %ldMHz\n",
               mem0_pll/1000000, media_pll/1000000);
       printf("PLL_BUS = %ldMHz\tPLL_USB = %ldMHz\n",
               bus_pll/1000000, usb_pll/1000000);
#endif

#endif

	return 0;
}
#endif
