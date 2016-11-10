/*
 * (C) Copyright 2011 Samsung Electronics Co. Ltd
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
#include <asm/arch/movi_partition.h>
#include <asm/arch/cpu.h>

//#define DEBUG_MOVI_PARTITION

#ifndef CONFIG_SPL_BUILD
#ifdef DEBUG_MOVI_PARTITION
#define dbg(x...)       printf(x)
#else
#define dbg(x...)       do { } while (0)
#endif

raw_area_t raw_area_control;

int init_raw_area_table(int location)
{
	int i;
	member_t *image;

	/* init raw_area will be 16MB */
	raw_area_control.start_blk = 16*1024*1024/MOVI_BLKSIZE;
	raw_area_control.next_raw_area = 0;
	strcpy(raw_area_control.description, "initial raw table");

	image = raw_area_control.image;

	/* For eMMC partition BLOCK Change*/

	/* image 0 should be bl1 */
	image[0].start_blk = (location == 1) ? 1 : 0;
	image[0].used_blk = MOVI_BL1_BLKCNT;
	image[0].size = PART_SIZE_BL1;
	image[0].attribute = 0x0;
	strcpy(image[0].description, "fwbl1");
	dbg("fwbl1: %d\n", image[0].start_blk);	

	/* image 1 should be secure monitor */
	image[1].start_blk = image[0].start_blk + MOVI_BL1_BLKCNT;
	image[1].used_blk = MOVI_EL3_MON_BLKCNT;
	image[1].size = PART_SIZE_EL3_MON;
	image[1].attribute = 0x1;
	strcpy(image[1].description, "el3_mon");
	dbg("el3_mon: %d\n", image[1].start_blk);

	/* image 2 should be bl2 */
	image[2].start_blk = image[1].start_blk + MOVI_EL3_MON_BLKCNT;
	image[2].used_blk = MOVI_BL2_BLKCNT;
	image[2].size = PART_SIZE_BL2;
	image[2].attribute = 0x2;
	strcpy(image[2].description, "bl2");
	dbg("bl2: %d\n", image[2].start_blk);

	/* image 3 should be u-boot */
	image[3].start_blk = image[2].start_blk + MOVI_BL2_BLKCNT;
	image[3].used_blk = MOVI_UBOOT_BLKCNT;
	image[3].size = PART_SIZE_UBOOT;
	image[3].attribute = 0x3;
	strcpy(image[3].description, "u-boot");
	dbg("u-boot: %d\n", image[3].start_blk);

	/* image 4 should be TrustZone S/W */
	image[4].start_blk = image[3].start_blk + MOVI_UBOOT_BLKCNT;
	image[4].used_blk = MOVI_MOBICORE_BLKCNT;
	image[4].size = PART_SIZE_MOBICORE;
	image[4].attribute = 0x4;
	strcpy(image[4].description, "TrustZone S/W");
	dbg("TrustZone S/W: %d\n", image[4].start_blk);

	/* image 5 should be environment */
	image[5].start_blk = image[4].start_blk + MOVI_MOBICORE_BLKCNT;
	image[5].used_blk = MOVI_ENV_BLKCNT;
	image[5].size = CONFIG_ENV_SIZE;
	image[5].attribute = 0x5;
	strcpy(image[5].description, "environment");
	dbg("env: %d\n", image[5].start_blk);


	/* For eMMC partition BLOCK Change*/
	if (location == 0)
		image[5].start_blk = image[5].start_blk + 1;

	/* image 6 should be kernel */
	image[6].start_blk = image[5].start_blk + MOVI_ENV_BLKCNT;
	image[6].used_blk = MOVI_KERNEL_BLKCNT;
	image[6].size = PART_SIZE_KERNEL;
	image[6].attribute = 0x6;
	strcpy(image[6].description, "kernel");
	dbg("knl: %d\n", image[6].start_blk);

	/* image 7 should be RFS */
	image[7].start_blk = image[6].start_blk + MOVI_KERNEL_BLKCNT;
	image[7].used_blk = MOVI_ROOTFS_BLKCNT;
	image[7].size = PART_SIZE_ROOTFS;
	image[7].attribute = 0x7;
	strcpy(image[7].description, "rfs");
	dbg("rfs: %d\n", image[7].start_blk);

	/* image 8 should be DTB */
	image[8].start_blk = image[7].start_blk + MOVI_ROOTFS_BLKCNT;
	image[8].used_blk = MOVI_DTB_BLKCNT;
	image[8].size = PART_SIZE_DTB;
	image[8].attribute = 0x8;
	strcpy(image[8].description, "dtb");
	dbg("dtb: %d\n", image[8].start_blk);

	/* image 9 should be Loadable F/W */
	image[9].start_blk = image[8].start_blk + MOVI_DTB_BLKCNT;
	image[9].used_blk = MOVI_LOADABLE_FW_BLKCNT;
	image[9].size = PART_SIZE_LOADABLE_FW;
	image[9].attribute = 0xa;
	strcpy(image[9].description, "ldfw");
	dbg("Loadable F/W: %d\n", image[9].start_blk);
	dbg("Loadable F/W end: %d\n", image[9].start_blk + MOVI_LOADABLE_FW_BLKCNT - 1);

	for (i=10; i<15; i++) {
		raw_area_control.image[i].start_blk = 0;
		raw_area_control.image[i].used_blk = 0;
	}

	return 0;
}
#endif /* CONFIG_SPL_BUILD */