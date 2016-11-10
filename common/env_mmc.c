/*
 * (C) Copyright 2008-2011 Freescale Semiconductor, Inc.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/* #define DEBUG */

#include <common.h>

#include <command.h>
#include <environment.h>
#include <linux/stddef.h>
#include <malloc.h>
#include <mmc.h>
#include <search.h>
#include <errno.h>

char *env_name_spec;

#ifdef ENV_IS_EMBEDDED
env_t *env_ptr = &environment;
#else /* ! ENV_IS_EMBEDDED */
env_t *env_ptr;
#endif /* ENV_IS_EMBEDDED */

DECLARE_GLOBAL_DATA_PTR;

#if !defined(CONFIG_ENV_OFFSET)
#define CONFIG_ENV_OFFSET 0
#endif

#if defined(CONFIG_EXYNOS4X12)
#define OmPin	readl(EXYNOS4_POWER_BASE + INFORM3_OFFSET)
#elif defined(CONFIG_CPU_EXYNOS4415) || defined(CONFIG_CPU_EXYNOS3250) || defined(CONFIG_CPU_EXYNOS3472)
#define OmPin	readl(EXYNOS4_POWER_BASE + INFORM3_OFFSET)
#elif defined(CONFIG_CPU_EXYNOS5260)
#define OmPin	readl(EXYNOS5260_POWER_BASE + INFORM3_OFFSET)
#elif defined(CONFIG_CPU_EXYNOS5430)
#define OmPin	readl(EXYNOS5430_POWER_BASE + INFORM3_OFFSET)
#elif defined(CONFIG_CPU_EXYNOS7420)
#define OmPin	readl(EXYNOS7420_POWER_BASE + INFORM3_OFFSET)
#elif defined(CONFIG_CPU_EXYNOS7580)
#define OmPin	readl(EXYNOS7580_POWER_BASE + INFORM3_OFFSET)
#else
#define OmPin	readl(EXYNOS5_POWER_BASE + INFORM3_OFFSET)
#endif

int exynos_boot_mode(void)
{
	return OmPin;
}

static int __mmc_get_env_addr(u32 *env_addr)
{
#if defined(CONFIG_CMD_SCSI)
	if (exynos_boot_mode()==BOOT_UFS) {
		*env_addr = CONFIG_UFS_ENV_OFFSET;
	} else {
		*env_addr = CONFIG_ENV_OFFSET;
	}
#else
	*env_addr = CONFIG_ENV_OFFSET;
#endif
	return 0;
}
int mmc_get_env_addr(u32 *env_addr)
	__attribute__((weak, alias("__mmc_get_env_addr")));

int env_init(void)
{
	/* use default */
	gd->env_addr	= (ulong)&default_environment[0];
	gd->env_valid	= 1;
	env_name_spec = get_boot_dev_name(1);

	return 0;
}

static int init_mmc_for_env(void)
{
	struct mmc *mmc;
	env_name_spec = get_boot_dev_name(1);
#if defined(CONFIG_CMD_SCSI)
	if (exynos_boot_mode()==BOOT_UFS) {
		strncpy(env_name_spec,"UFS", 8);
		return 0;
	}
#endif
	mmc = find_mmc_device(get_boot_dev());
	if (!mmc) {
		puts("No MMC card found\n");
		return -1;
	}

	if (mmc_init(mmc)) {
		puts("MMC init failed\n");
		return -1;
	}

	return 0;
}

#ifdef CONFIG_CMD_SAVEENV
static inline int write_env(unsigned long size,
			    unsigned long offset, const void *buffer)
{
	uint blk_start, blk_cnt, n;
	block_dev_desc_t *dev_desc;

	dev_desc = get_dev(get_boot_dev_name(0),get_boot_dev());
#if defined(CONFIG_CMD_SCSI)
	blk_start = ALIGN(offset, 512) / 512;
	blk_cnt = ALIGN(size, 512) / 512;
#else
	blk_start = ALIGN(offset, dev_desc->blksz) / dev_desc->blksz;
	blk_cnt = ALIGN(size, dev_desc->blksz) / dev_desc->blksz;
#endif

#if defined(CONFIG_PREVENT_HALTING_UFS_FAIL_AT_ENV_READ_WRITE)
	if (dev_desc->block_write)
	{
#endif
	n = dev_desc->block_write(get_boot_dev(), blk_start,
					blk_cnt, (u_char *)buffer);
#if defined(CONFIG_PREVENT_HALTING_UFS_FAIL_AT_ENV_READ_WRITE)
	}
	else
		n = -1;
#endif

	return (n == blk_cnt) ? 0 : -1;
}

int saveenv(void)
{
	ALLOC_CACHE_ALIGN_BUFFER(env_t, env_new, 1);
	ssize_t	len;
	char	*res;
	u32	offset;

	if (init_mmc_for_env() || mmc_get_env_addr(&offset))
		return 1;

	res = (char *)&env_new->data;
	len = hexport_r(&env_htab, '\0', &res, ENV_SIZE, 0, NULL);
	if (len < 0) {
		error("Cannot export environment: errno = %d\n", errno);
		return 1;
	}

	env_new->crc = crc32(0, &env_new->data[0], ENV_SIZE);
	printf("\nWriting to %s(%d)... ", env_name_spec, get_boot_dev());
	if (write_env(CONFIG_ENV_SIZE, offset, (u_char *)env_new)) {
		puts("failed\n");
		return 1;
	}

	puts("done\n");
	return 0;
}
#endif /* CONFIG_CMD_SAVEENV */

static inline int read_env(unsigned long size,
			   unsigned long offset, const void *buffer)
{
	uint blk_start, blk_cnt, n;
	block_dev_desc_t *dev_desc;

	dev_desc = get_dev(get_boot_dev_name(0),get_boot_dev());
#if defined(CONFIG_CMD_SCSI)
	blk_start = ALIGN(offset, 512) / 512;
	blk_cnt = ALIGN(size, 512) / 512;
#else
	blk_start = ALIGN(offset, dev_desc->blksz) / dev_desc->blksz;
	blk_cnt = ALIGN(size, dev_desc->blksz) / dev_desc->blksz;
#endif
#if defined(CONFIG_PREVENT_HALTING_UFS_FAIL_AT_ENV_READ_WRITE)
	if (dev_desc->block_read)	// by bskim
	{
#endif	
		n = dev_desc->block_read(get_boot_dev(), blk_start,
						blk_cnt, (uchar *)buffer);
#if defined(CONFIG_PREVENT_HALTING_UFS_FAIL_AT_ENV_READ_WRITE)
	}
	else
		n = 0;
#endif
	return (n == blk_cnt) ? 0 : -1;
}

void env_relocate_spec(void)
{
#if !defined(ENV_IS_EMBEDDED)
	ALLOC_CACHE_ALIGN_BUFFER(char, buf, CONFIG_ENV_SIZE);
	u32 offset;

	if (init_mmc_for_env() || mmc_get_env_addr(&offset))
	{
		return set_default_env(NULL);
	}
#if defined(CONFIG_PREVENT_HALTING_UFS_FAIL_AT_ENV_READ_WRITE)
	if (read_env(CONFIG_ENV_SIZE, offset, buf) == 0)
#else
	if (read_env(CONFIG_ENV_SIZE, offset, buf))
#endif	
	{
		return set_default_env(NULL);
	}

	env_import(buf, 1);

#endif
}
