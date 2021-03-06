CONFIG_CMD_FAT=y
CONFIG_BOOTM_NETBSD=y
CONFIG_BOARD_EARLY_INIT_F=y
CONFIG_PWM=y
CONFIG_CMD_ITEST=y
CONFIG_UFS_ENV_OFFSET="(UFS_ENV_POS * UFS_BLKSIZE)"
CONFIG_CMD_EDITENV=y
CONFIG_FACTORY_RESET_MODE=0xf
CONFIG_RAMDUMP_GPR_POWER_STAT="(CONFIG_RAMDUMP_BASE + 0x480)"
CONFIG_EMMC_UFS_POWER_ON=y
CONFIG_CPU_EXYNOS7420_EVT1=y
CONFIG_CMD_CRC32=y
CONFIG_SYS_LONGHELP=y
CONFIG_SYS_LOAD_ADDR="CONFIG_SYS_TEXT_BASE"
CONFIG_RECOVERYCOMMAND="emmc partition 0 10 0;mmc erase user 0 1072 1;movi r f 1 40000000;emmc open 0;movi w z f 0 40000000;emmc close 0;movi r b 1 40000000;emmc open 0;movi w z b 0 40000000;emmc close 0;movi r u 1 40000000;emmc open 0;movi w z u 0 40000000;emmc close 0;movi r e 1 40000000;emmc open 0;movi w z e 0 40000000;emmc close 0;movi r t 1 40000000;emmc open 0;movi w z t 0 40000000;emmc close 0;reset"
CONFIG_SPL_19KB=y
CONFIG_SCSI_UFS=y
CONFIG_SYS_SCSI_MAX_SCSI_ID=y
CONFIG_RAMDUMP_COREREG="(CONFIG_RAMDUMP_BASE + 0x2000)"
CONFIG_CMD_XIMG=y
CONFIG_STACKSIZE="(256 << 10)"
CONFIG_BOOTDELAY=3
CONFIG_SYS_HELP_CMD_WIDTH=8
CONFIG_NR_DRAM_BANKS=12
CONFIG_BOOTM_RTEMS=y
CONFIG_SYS_CBSIZE=256
CONFIG_RAMDUMP_SCRATCH="(CONFIG_RAMDUMP_BASE + 0x100)"
CONFIG_SDHCI=y
CONFIG_BOOTM_LINUX=y
CONFIG_CMD_UFS=y
CONFIG_BOARD_LATE_INIT=y
CONFIG_CMD_CONSOLE=y
CONFIG_SMC_CMD=y
CONFIG_SYS_CPU="armv8"
CONFIG_FDT_LOAD_COMMAND_EMMC="movi r d 0 48000000;fdt addr 48000000;"
CONFIG_RAMDUMP_DUMP_GPR_WAIT="(CONFIG_RAMDUMP_BASE + 0x380)"
CONFIG_MMC=y
CONFIG_MMC_SMU_INIT=y
CONFIG_REVISION_TAG=y
CONFIG_PHY_BL_SYS_INFO_DRAM_SIZE="(CONFIG_PHY_BL_SYS_INFO + 0x48)"
CONFIG_CMD_MISC=y
CONFIG_PM_VDD_INT=1000
CONFIG_SECOND_BOOT_INFORM_SIZE="(0x4)"
CONFIG_ZERO_BOOTDELAY_CHECK=y
CONFIG_DEFAULT_CONSOLE="console=ttySAC1,115200n8\0"
CONFIG_ENV_OFFSET="(MOVI_ENV_POS * MOVI_BLKSIZE)"
CONFIG_ENV_OVERWRITE=y
CONFIG_CMD_NET=y
CONFIG_ROOTFS_ATAGS=y
CONFIG_CMD_NFS=y
CONFIG_ENV_SIZE="(16 << 10)"
CONFIG_CMD_PING=y
CONFIG_RAMDUMP_GPR=y
CONFIG_MACH_ESPRESSO7420=y
CONFIG_SYS_MALLOC_LEN="(CONFIG_ENV_SIZE + (1 << 20))"
CONFIG_INITRD_TAG=y
CONFIG_SYS_MMC_ENV_DEV=0
CONFIG_PM_VDD_MIF=900
CONFIG_DISPLAY_CHIPID=y
CONFIG_SYS_TEXT_BASE=0x8F000000
CONFIG_PHY_ALIVE_IRAM_BASE="(CONFIG_PHY_IRAM_BASE + 0x5D000)"
CONFIG_CMD_FLASH=y
CONFIG_CMD_SAVEENV=y
CONFIG_IRAM_STACK="(CONFIG_PHY_IRAM_NS_BASE + 0x1000)"
CONFIG_SYS_BARGSIZE="CONFIG_SYS_CBSIZE"
CONFIG_SPL_TEXT_BASE=0x02134000
CONFIG_RAMDUMP_LOGSZ=0x200000
CONFIG_FDT_SAVE_COMMAND_SDMMC="movi w d 0 48000000;"
CONFIG_FDT_SAVE_COMMAND_EMMC="movi w d 0 48000000;"
CONFIG_RAMDUMP_LASTBUF="(CONFIG_RAMDUMP_BASE + 0x200)"
CONFIG_S5P_MSHC=y
CONFIG_ARCH_EXYNOS=y
CONFIG_CMD_MEMORY=y
CONFIG_SYS_MAXARGS=16
CONFIG_BOOTCOMMAND_UFS="movi read scsi kernel %u 40080000;movi read scsi rootfs %u 42000000 A00000;movi r s d 0 48000000;bootm 40080000 42000000 48000000"
CONFIG_CMD_RUN=y
CONFIG_SYS_PBSIZE=384
CONFIG_FDT_LOAD_COMMAND_UFS="movi r s d 0 48000000;fdt addr 48000000;"
CONFIG_BOARDDIR="board/unibest/exsom7420"
CONFIG_FDT_LOAD_COMMAND_SDMMC="movi r d 0 48000000;fdt addr 48000000;"
CONFIG_CMD_IORW=y
CONFIG_PM_VDD_ATLAS=982
CONFIG_OF_LIBFDT=y
CONFIG_RAMDUMP_CORE_PANIC_STAT="(CONFIG_RAMDUMP_BASE + 0x500)"
CONFIG_CMDLINE_EDITING=y
CONFIG_LINUX_KERNEL_FASTBOOT=y
CONFIG_PHYS_64BIT=y
CONFIG_TRUSTZONE_RESERVED_DRAM=0x1800000
CONFIG_EXYNOS_USBD3=y
CONFIG_CMD_EXT2=y
CONFIG_BOOTCOMMAND="movi read kernel 0 40080000;movi read rootfs 0 42000000 A00000;movi r d 0 48000000;bootm 40080000 42000000 48000000"
CONFIG_BL1_VERSION_INFORM="(CONFIG_PHY_IRAM_NS_BASE + 0x810)"
CONFIG_CMD_SETGETDCR=y
CONFIG_PM_VDD_DISP=1000
CONFIG_BL_MONITOR=y
CONFIG_SYS_CLK_FREQ=24000000
CONFIG_SYS_BOOT_RAMDISK_HIGH=y
CONFIG_NVDATA_PARTITION=y
CONFIG_ARCH_EXYNOS7=y
CONFIG_ZLIB=y
CONFIG_RECOVERYCOMMAND_USB=y
CONFIG_CMD_GO=y
CONFIG_CMD_BOOTD=y
CONFIG_CMD_BOOTM=y
CONFIG_CMD_BOOTZ=y
CONFIG_MMC_64BIT_BUS=y
CONFIG_RECOVERY_MODE=y
CONFIG_PMIC_ACT8892_OLED_FHD=y
CONFIG_S5P_SDHCI=y
CONFIG_CPU_EXYNOS7420=y
CONFIG_MACH_EXSOM7420SB=y
CONFIG_SYS_SOC="exynos"
CONFIG_PM_VDD_G3D=900
CONFIG_SYS_SCSI_MAX_LUN=8
CONFIG_ENV_IS_IN_MMC=y
CONFIG_SYS_HZ=1000
CONFIG_PHY_BL_SYS_INFO="(CONFIG_PHY_IRAM_NS_BASE + 0x800)"
CONFIG_PM=y
CONFIG_DOS_PARTITION=y
CONFIG_GZIP=y
CONFIG_SYS_VENDOR="unibest"
CONFIG_BOOTSTAGE_USER_COUNT=20
CONFIG_CMD_FPGA=y
CONFIG_IDENT_STRING="" for " CONFIG_BOARD_NAME"
CONFIG_SYS_BAUDRATE_TABLE="{ 9600, 19200, 38400, 57600, 115200 }"
CONFIG_SYS_DCACHE_OFF=y
CONFIG_SYS_HUSH_PARSER=y
CONFIG_SYS_SDRAM_BASE=0x40000000
CONFIG_PHY_IRAM_NS_BASE="(CONFIG_PHY_IRAM_BASE + 0x57000)"
CONFIG_CMD_ECHO=y
CONFIG_GENERIC_MMC=y
CONFIG_SYS_SCSI_MAX_DEVICE="(CONFIG_SYS_SCSI_MAX_SCSI_ID * CONFIG_SYS_SCSI_MAX_LUN)"
CONFIG_DISPLAY_BOARDINFO=y
CONFIG_FAT_WRITE=y
CONFIG_FUSING_COMMAND_EMMC="emmc open 0;movi w z f 0 40000000;movi w z b 0 40008000;movi w z u 0 42000000;movi w z e 0 42100000;movi w z t 0 42200000;movi w l 0   42300000;emmc close 0;reset"
CONFIG_FUSING_COMMAND_UFS="movi w s f 1 40000000;movi w s b 1 40008000;movi w s u 1 42000000;movi w s e 1 42100000;movi w s t 1 42200000;movi w s l 0 42300000;reset"
CONFIG_SYS_MONITOR_BASE=0x00000000
CONFIG_EXTRA_ENV_SETTINGS="rootfslen= 0xA00000"
CONFIG_CMD_SCSI=y
CONFIG_TRUSTZONE_ENABLE=y
CONFIG_PMIC_ACT8892_TEST=y
CONFIG_RAMDUMP_CORE_POWER_STAT="(CONFIG_RAMDUMP_BASE + 0x400)"
CONFIG_SYS_INIT_SP_ADDR="(CONFIG_SYS_LOAD_ADDR - 0x100000)"
CONFIG_SYS_ARCH="arm"
CONFIG_S5P=y
CONFIG_BAUDRATE=115200
CONFIG_SYS_BOARD="exsom7420"
CONFIG_RAMDUMP_REASON="(CONFIG_RAMDUMP_BASE + 0x300)"
CONFIG_FASTBOOT=y
CONFIG_CMDLINE_TAG=y
CONFIG_SYS_ROOTFS_BASE=0x42000000
CONFIG_FDT_SAVE_COMMAND_UFS="movi w s d 0 48000000;"
CONFIG_FUSING_COMMAND_SDMMC="movi w f 0 40000000;movi w b 0 40008000;movi w u 0 42000000;movi w e 0 42100000;movi w t 0 42200000;movi w l 0 42300000;reset"
CONFIG_CMD_IMPORTENV=y
CONFIG_CMD_EXPORTENV=y
CONFIG_PARTITIONS=y
CONFIG_ARM64=y
CONFIG_SERIAL_MULTI=y
CONFIG_ARCH_CPU_INIT=y
CONFIG_CMD_ELF=y
CONFIG_PM_VDD_APOLLO=1107
CONFIG_RAMDUMP_BASE="(CONFIG_SYS_SDRAM_BASE + 0x6000000)"
CONFIG_SYS_NO_FLASH=y
CONFIG_PHY_IRAM_BASE="(0x02100000)"
CONFIG_SYS_DEF_EEPROM_ADDR=0
CONFIG_MACH_TYPE="MACH_TYPE_EXSOM7420"
CONFIG_SPL=y
CONFIG_SYS_MEMTEST_END="(CONFIG_SYS_SDRAM_BASE + 0x5E00000)"
CONFIG_CMD_SOURCE=y
CONFIG_SYS_PROMPT="CONFIG_BOARD_NAME " # ""
CONFIG_DISPLAY_CPUINFO=y
CONFIG_FACTORY_RESET_BOOTCOMMAND="ext3format mmc 0:3;ext3format mmc 0:4;movi read kernel 0 40080000;movi read rootfs 0 42000000 A00000;movi r d 0 48000000;bootm 40080000 42000000 48000000"
CONFIG_SMC_IMAGE_INFORM_BASE="(CONFIG_PHY_IRAM_NS_BASE + nscode_end - nscode_base + CONFIG_SECOND_BOOT_INFORM_SIZE)"
CONFIG_SETUP_MEMORY_TAGS=y
CONFIG_SERIAL2=y
CONFIG_CMD_RPMB=y
CONFIG_SYS_MEMTEST_START="CONFIG_SYS_SDRAM_BASE"
CONFIG_CMD_LOADB=y
CONFIG_CMD_LOADS=y
CONFIG_CMD_IMI=y
CONFIG_UFS_ADD_WAITING_DELAY=y
CONFIG_LMB=y
CONFIG_SECOND_BOOT_INFORM_BASE="(CONFIG_PHY_IRAM_NS_BASE + nscode_end - nscode_base)"
CONFIG_ARM=y
CONFIG_SAMSUNG=y
CONFIG_BOARD_NAME="EXSOM7420_AARCH64"
CONFIG_RAMDUMP_LOGBUF="(CONFIG_RAMDUMP_BASE + 0x800000)"
CONFIG_CMD_BDI=y
CONFIG_STATIC_RELA=y
CONFIG_CMD_MMC=y
CONFIG_MMC_EARLY_INIT=y
CONFIG_RAMDUMP_DUMP_GPR_DEBUG="(CONFIG_RAMDUMP_BASE + 0x320)"
