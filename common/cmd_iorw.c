/*
 * (C) Copyright 2001
 * Denis Peter, MPL AG Switzerland
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
 *
 *
 *
 */

#include <common.h>
#include <command.h>
#include <compiler.h>
#include <linux/compat.h>
#include <malloc.h>
#include <asm/dma-mapping.h>
#include <scsi.h>
#include <ufs.h>

/*********************************************************************************
 * iorw command intepreter
 */
int do_iorw(cmd_tbl_t * cmdtp, int flag, int argc, char *const argv[])
{
	ulong addr;
	ulong range;
	ulong dispcount;
	ulong start_addr;
	ulong max_addr;
	u32 val;
	ulong i;

	switch(argc)
	{
	case 4:
	case 5:	
		if (strcmp(argv[1], "r") == 0) {
			addr = simple_strtoul(argv[2], NULL, 16);
			range = simple_strtoul(argv[3], NULL, 16);
			if (argc == 4)
				dispcount = 8;
			else
				dispcount = simple_strtoul(argv[4], NULL, 16);

			printf("start address : 0x%08X range = 0x%X\n", addr, range);

			start_addr = addr;
			max_addr = addr + range;
			i = 0;
			printf("0x%08X: ", addr);
			while(start_addr < max_addr)
			{
				val = readl(start_addr);
				i++;
				printf("0x%08X ", val);
				if (i == dispcount)
				{
					if (start_addr+4 < max_addr)
						printf("\n0x%08X: ", start_addr+4);
					i=0;
				}
				start_addr+=4;
			}

			printf("\n");
			return 0;
		}
		break;
	default:
		printf("command ignored !\n");
		break;
	}
	
	return CMD_RET_USAGE;
}

U_BOOT_CMD(
		  iorw, 5, 1, do_iorw,
		  "iorw command",
		  "iorw r address range dispcount - show io address range\n"
);
