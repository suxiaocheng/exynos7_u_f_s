/*
 * Universal Flash Storage Host controller driver
 *
 * This code is based on drivers/scsi/ufs/ufs.h
 * Copyright (C) 2011-2012 Samsung India Software Operations
 *
 * Santosh Yaraganavi <santosh.sy@samsung.com>
 * Vinayak Holikatti <h.vinayak@samsung.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * NO WARRANTY
 * THE PROGRAM IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED INCLUDING, WITHOUT
 * LIMITATION, ANY WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT,
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. Each Recipient is
 * solely responsible for determining the appropriateness of using and
 * distributing the Program and assumes all risks associated with its
 * exercise of rights under this Agreement, including but not limited to
 * the risks and costs of program errors, damage to or loss of data,
 * programs or equipment, and unavailability or interruption of operations.

 * DISCLAIMER OF LIABILITY
 * NEITHER RECIPIENT NOR ANY CONTRIBUTORS SHALL HAVE ANY LIABILITY FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING WITHOUT LIMITATION LOST PROFITS), HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OR DISTRIBUTION OF THE PROGRAM OR THE EXERCISE OF ANY RIGHTS GRANTED
 * HEREUNDER, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#if defined(CONFIG_CMD_SCSI) && defined(CONFIG_SCSI_UFS)

#define UFS_QUIRK_HELSINKI_EVT0	(1<<0)
#define UFS_QUIRK_USE_1LANE		(1<<1)
#define UFS_QUIRK_BROKEN_HCE	(1<<2)

#undef	SCSI_UFS_DEBUG
//#define       SCSI_UFS_DEBUG

#define UFS_FIRMWARE_UPDATE

#ifdef SCSI_UFS_DEBUG
#define debug printf
#endif

#define MAX_CDB_SIZE		16
#define ALIGNED_UPIU_SIZE	1024
#define SCSI_MAX_SG_SEGMENTS	(128)

#define UFS_ACCESS_ONLY_4K	TRUE

#define UFS_LOGICAL_BLOCK_SIZE	512
#define UFS_SG_BLOCK_SIZE_BIT	12
#define UFS_SG_BLOCK_SIZE	(1<<UFS_SG_BLOCK_SIZE_BIT)

#define UFS_NUTRS		2
#define UFS_NUTMRS		2

#define UFS_DEBUG_UPIU		0x00323110
#define UFS_DEBUG_UPIU_ALL	0x00743112
#define UFS_DEBUG_UPIU_ALL2	0x00743110
#define UFS_DEBUG_UPIU_MIN	0x00111110

#define UPIU_HEADER_DWORD(byte3, byte2, byte1, byte0)\
			((byte3 << 24) | (byte2 << 16) |\
			 (byte1 << 8) | (byte0))

#define UFS_GET_SFR(addr, mask, shift)	((readl(addr)>>(shift))&(mask))
#define UFS_SET_SFR(addr, value, mask, shift)	writel((readl(addr)&(~((mask)<<(shift))))|((value)<<(shift)), (addr))
#define UFS_WAIT_CLK(addr, value, mask, shift)	while( ((readl(addr)>>(shift))&(mask))!=(value) )

/* UFSHCI Registers */
enum {
	REG_CONTROLLER_CAPABILITIES = 0x00,
	REG_UFS_VERSION = 0x08,
	REG_CONTROLLER_PID = 0x10,
	REG_CONTROLLER_MID = 0x14,
	REG_INTERRUPT_STATUS = 0x20,
	REG_INTERRUPT_ENABLE = 0x24,
	REG_CONTROLLER_STATUS = 0x30,
	REG_CONTROLLER_ENABLE = 0x34,
	REG_UIC_ERROR_CODE_PHY_ADAPTER_LAYER = 0x38,
	REG_UIC_ERROR_CODE_DATA_LINK_LAYER = 0x3C,
	REG_UIC_ERROR_CODE_NETWORK_LAYER = 0x40,
	REG_UIC_ERROR_CODE_TRANSPORT_LAYER = 0x44,
	REG_UIC_ERROR_CODE_DME = 0x48,
	REG_UTP_TRANSFER_REQ_INT_AGG_CONTROL = 0x4C,
	REG_UTP_TRANSFER_REQ_LIST_BASE_L = 0x50,
	REG_UTP_TRANSFER_REQ_LIST_BASE_H = 0x54,
	REG_UTP_TRANSFER_REQ_DOOR_BELL = 0x58,
	REG_UTP_TRANSFER_REQ_LIST_CLEAR = 0x5C,
	REG_UTP_TRANSFER_REQ_LIST_RUN_STOP = 0x60,
	REG_UTP_TASK_REQ_LIST_BASE_L = 0x70,
	REG_UTP_TASK_REQ_LIST_BASE_H = 0x74,
	REG_UTP_TASK_REQ_DOOR_BELL = 0x78,
	REG_UTP_TASK_REQ_LIST_CLEAR = 0x7C,
	REG_UTP_TASK_REQ_LIST_RUN_STOP = 0x80,
	REG_UIC_COMMAND = 0x90,
	REG_UIC_COMMAND_ARG_1 = 0x94,
	REG_UIC_COMMAND_ARG_2 = 0x98,
	REG_UIC_COMMAND_ARG_3 = 0x9C,

	VS_TXPRDT_ENTRY_SIZE = 0x0100,
	VS_RXPRDT_ENTRY_SIZE = 0x0104,
	VS_TO_CNT_DIV_VAL = 0x0108,
	VS_40US_TO_CNT_VAL = 0x010C,
	VS_INVALID_UPIU_CTRL = 0x0110,
	VS_INVALID_UPIU_BADDR = 0x0114,
	VS_INVALID_UPIU_UBADDR = 0x0118,
	VS_INVALID_UTMR_OFFSET_ADDR = 0x011C,
	VS_INVALID_UTR_OFFSET_ADDR = 0x0120,
	VS_INVALID_DIN_OFFSET_ADDR = 0x0124,
	VS_DOORBELL_TIMER_CONFIG = 0x0128,
	VS_DOORBELL_TIMER_STATUS = 0x012C,
	VS_CRC_STATUS = 0x0130,
	VS_IS = 0x0138,
	VS_IE = 0x013C,
	VS_UTRL_NEXUS_TYPE = 0x0140,
	VS_UMTRL_NEXUS_TYPE = 0x0144,
	VS_E2EFC_CTRL = 0x0148,
	VS_SW_RST = 0x0150,
	VS_LINK_VERSION = 0x0154,
	VS_IDLE_TIMER_CONFIG = 0x0158,
	VS_RX_UPIU_MATCH_ERROR_CODE = 0x015C,
	VS_DATA_REORDER = 0x0160,
	VS_UNIPRO_APB_CLK_CTRL = 0x0168,
	VS_AXIDMA_RWDATA_BURST_LEN = 0x016C,
	VS_GPIO_OUT = 0x0170,
	VS_FORCE_HCS = 0x01B4,

	UNIPRO_COMP_RESET = 0x1010,
	UNIPRO_DME_PACP_CNFBIT = 0x10c8,
	UNIPRO_DME_INTR_ENABLE = 0x10e4,

	UFSP_UPRSECURITY = 0x2010,
	UFSP_UPWSECURITY = 0x2110,
	UFSP_UPSBEGIN0 = 0x2200,
	UFSP_UPSEND0 = 0x2204,
	UFSP_UPLUN0 = 0x2208,
	UFSP_UPSCTRL0 = 0x220c,
};

/* UIC Commands */
enum {
	UIC_CMD_DME_GET = 0x01,
	UIC_CMD_DME_SET = 0x02,
	UIC_CMD_DME_PEER_GET = 0x03,
	UIC_CMD_DME_PEER_SET = 0x04,
	UIC_CMD_DME_POWERON = 0x10,
	UIC_CMD_DME_POWEROFF = 0x11,
	UIC_CMD_DME_ENABLE = 0x12,
	UIC_CMD_DME_RESET = 0x14,
	UIC_CMD_DME_END_PT_RST = 0x15,
	UIC_CMD_DME_LINK_STARTUP = 0x16,
	UIC_CMD_DME_HIBER_ENTER = 0x17,
	UIC_CMD_DME_HIBER_EXIT = 0x18,
	UIC_CMD_DME_TEST_MODE = 0x1A,
	UIC_CMD_WAIT = 0x80,
	UIC_CMD_WAIT_ISR = 0x90,
	PHY_PMA_COMN_SET = 0xf0,
	PHY_PMA_TRSV_SET = 0xf1,
	PHY_PMA_COMN_WAIT = 0xf2,
	PHY_PMA_TRSV_WAIT = 0xf3,
	UIC_CMD_REGISTER_SET = 0xff,
};

#define UIC_ARG_MIB_SEL(attr, sel)	((((attr) & 0xFFFF) << 16) |\
					 ((sel) & 0xFFFF))
#define UIC_ARG_MIB(attr)		UIC_ARG_MIB_SEL(attr, 0)

#define MPHY_PMA_COMN_ADDR(reg)		((reg) << 2)
#define MPHY_PMA_TRSV_ADDR(reg, lane)	(((reg) + (0x30 * (lane))) << 2)

/* UIC Config result code / Generic error code */
enum {
	UIC_CMD_RESULT_SUCCESS = 0x00,
	UIC_CMD_RESULT_INVALID_ATTR = 0x01,
	UIC_CMD_RESULT_FAILURE = 0x01,
	UIC_CMD_RESULT_INVALID_ATTR_VALUE = 0x02,
	UIC_CMD_RESULT_READ_ONLY_ATTR = 0x03,
	UIC_CMD_RESULT_WRITE_ONLY_ATTR = 0x04,
	UIC_CMD_RESULT_BAD_INDEX = 0x05,
	UIC_CMD_RESULT_LOCKED_ATTR = 0x06,
	UIC_CMD_RESULT_BAD_TEST_FEATURE_INDEX = 0x07,
	UIC_CMD_RESULT_PEER_COMM_FAILURE = 0x08,
	UIC_CMD_RESULT_BUSY = 0x09,
	UIC_CMD_RESULT_DME_FAILURE = 0x0A,
};

enum {
	UTP_SCSI_COMMAND = 0x00000000,
	UTP_NATIVE_UFS_COMMAND = 0x10000000,
	UTP_DEVICE_MANAGEMENT_FUNCTION = 0x20000000,
	UTP_REQ_DESC_INT_CMD = 0x01000000,
};

/* UTP Transfer Request Data Direction (DD) */
enum {
	UTP_NO_DATA_TRANSFER = 0x00000000,
	UTP_HOST_TO_DEVICE = 0x02000000,
	UTP_DEVICE_TO_HOST = 0x04000000,
};

/* Overall command status values */
enum {
	OCS_SUCCESS = 0x0,
	OCS_INVALID_CMD_TABLE_ATTR = 0x1,
	OCS_INVALID_PRDT_ATTR = 0x2,
	OCS_MISMATCH_DATA_BUF_SIZE = 0x3,
	OCS_MISMATCH_RESP_UPIU_SIZE = 0x4,
	OCS_PEER_COMM_FAILURE = 0x5,
	OCS_ABORTED = 0x6,
	OCS_FATAL_ERROR = 0x7,
	OCS_INVALID_COMMAND_STATUS = 0x0F,
	MASK_OCS = 0x0F,
};

/* Task management functions */
enum {
	UFS_ABORT_TASK = 0x01,
	UFS_ABORT_TASK_SET = 0x02,
	UFS_CLEAR_TASK_SET = 0x04,
	UFS_LOGICAL_RESET = 0x08,
	UFS_QUERY_TASK = 0x80,
	UFS_QUERY_TASK_SET = 0x81,
};

/* Query request functions */
enum {
	UFS_STD_READ_REQ = 0x01,
	UFS_STD_WRITE_REQ = 0x81,
	UFS_VENDOR_READ_REQ = 0x40,
	UFS_VENDOR_WRITE_REQ = 0xc0,
	UFS_VENDOR_FORMAT_REQ = 0xc2,
};

#define UFS_QUERY_FUNCTION_MASK	0xc0

/* UTP UPIU Transaction Codes Initiator to Target */
enum {
	UPIU_TRANSACTION_NOP_OUT = 0x00,
	UPIU_TRANSACTION_COMMAND = 0x01,
	UPIU_TRANSACTION_DATA_OUT = 0x02,
	UPIU_TRANSACTION_TASK_REQ = 0x04,
	UPIU_TRANSACTION_QUERY_REQ = 0x16,
};

/* UTP UPIU Transaction Codes Target to Initiator */
enum {
	UPIU_TRANSACTION_NOP_IN = 0x20,
	UPIU_TRANSACTION_RESPONSE = 0x21,
	UPIU_TRANSACTION_DATA_IN = 0x22,
	UPIU_TRANSACTION_TASK_RSP = 0x24,
	UPIU_TRANSACTION_READY_XFER = 0x31,
	UPIU_TRANSACTION_QUERY_RSP = 0x36,
	UPIU_TRANSACTION_REJECT_UPIU = 0x3f,
};

/* UPIU Read/Write flags */
enum {
	UPIU_CMD_FLAGS_NONE = 0x00,
	UPIU_CMD_FLAGS_WRITE = 0x20,
	UPIU_CMD_FLAGS_READ = 0x40,
};

/* UPIU Task Attributes */
enum {
	UPIU_TASK_ATTR_SIMPLE = 0x00,
	UPIU_TASK_ATTR_ORDERED = 0x01,
	UPIU_TASK_ATTR_HEADQ = 0x02,
	UPIU_TASK_ATTR_ACA = 0x03,
};

/* Descriptor ID */
enum {
	UPIU_DESC_ID_DEVICE = 0x0,
	UPIU_DESC_ID_CONFIGURATION = 0x1,
	UPIU_DESC_ID_UNIT = 0x2,
	UPIU_DESC_ID_INTERCONNECT = 0x4,
	UPIU_DESC_ID_STRING = 0x5,
	UPIU_DESC_ID_GEOMETRY = 0x7,
	UPIU_DESC_ID_POWER = 0x8,
};

/* Flag ID */
enum {
	UPIU_FLAG_ID_DEVICEINIT = 0x1,
	UPIU_FLAG_ID_PER_WRITEPROTECT = 0x2,
	UPIU_FLAG_ID_POW_WRITEPROTECT = 0x3,
	UPIU_FLAG_ID_BG_OPERATRION = 0x4,
	UPIU_FLAG_ID_PURGE_ENABLE = 0x6,
	UPIU_FLAG_ID_ALL = 0xff,
};

/* Attribute ID */
enum {
	UPIU_ATTR_ID_BOOTLUNEN = 0x0,
	UPIU_ATTR_ID_POWERMODE = 0x2,
	UPIU_ATTR_ID_ACTIVECCLEVEL = 0x3,
	UPIU_ATTR_ID_OOODATAEN = 0x4,
	UPIU_ATTR_ID_BACKGROUNDOP = 0x5,
	UPIU_ATTR_ID_PURGESTATUS = 0x6,
	UPIU_ATTR_ID_MAXDATAIN = 0x7,
	UPIU_ATTR_ID_MAXDATAOUT = 0x8,
	UPIU_ATTR_ID_DYNCAPNEEDED = 0x9,
	UPIU_ATTR_ID_REFCLKFREQ = 0xa,
	UPIU_ATTR_ID_CONFIGDESCLOCK = 0xb,
	UPIU_ATTR_ID_MAXNUMRTT = 0xc,
	UPIU_ATTR_ID_EXCEPTIONEVENTCTL = 0xd,
	UPIU_ATTR_ID_EXCEPTIONEVENTSTATUS = 0xe,
	UPIU_ATTR_ID_SECONDPASSED = 0xf,
	UPIU_ATTR_ID_CONTEXTCONF = 0x10,
	UPIU_ATTR_ID_CORRPRGBLKNUM = 0x11,
	UPIU_ATTR_ID_ALL = 0xff,
};

/* UTP QUERY Transaction Specific Fields OpCode */
enum {
	UPIU_QUERY_OPCODE_NOP = 0x0,
	UPIU_QUERY_OPCODE_READ_DESC = 0x1,
	UPIU_QUERY_OPCODE_WRITE_DESC = 0x2,
	UPIU_QUERY_OPCODE_READ_ATTR = 0x3,
	UPIU_QUERY_OPCODE_WRITE_ATTR = 0x4,
	UPIU_QUERY_OPCODE_READ_FLAG = 0x5,
	UPIU_QUERY_OPCODE_SET_FLAG = 0x6,
	UPIU_QUERY_OPCODE_CLEAR_FLAG = 0x7,
	UPIU_QUERY_OPCODE_TOGGLE_FLAG = 0x8,
};

enum {
	MASK_SCSI_STATUS = 0xFF,
	MASK_TASK_RESPONSE = 0xFF00,
	MASK_RSP_UPIU_RESULT = 0xFFFF,
};

/* Task management service response */
enum {
	UPIU_TASK_MANAGEMENT_FUNC_COMPL = 0x00,
	UPIU_TASK_MANAGEMENT_FUNC_NOT_SUPPORTED = 0x04,
	UPIU_TASK_MANAGEMENT_FUNC_SUCCEEDED = 0x08,
	UPIU_TASK_MANAGEMENT_FUNC_FAILED = 0x05,
	UPIU_INCORRECT_LOGICAL_UNIT_NO = 0x09,
};

enum {
	UFS_TRANSFER_COMMAND = (1 << 0),
	UFS_TRANSFER_UIC = (1 << 1),
	UFS_TRANSFER_TASK = (1 << 2),
	UFS_TRANSFER_COMM_DONE = (1 << 3),
	UFS_TRANSFER_TASK_DONE = (1 << 4),
	UFS_TRANSFER_ERROR = (1 << 16),
	UFS_TRANSFER_FATAL_ERROR = (1 << 20),
	UFS_TRANSFER_ERROR_RETRY = (1 << 21),
	UFS_TRANSFER_TIMEOUT = (1 << 24),
	UFS_TRANSFER_COMPLETE = (1 << 31),
};

/*
	command_type :
	| descriptor ID | opcode | function | UPIU type |

	UPIU type[7:0] : NOP_OUT, COMMAND, DATA_OUT, TASK_REQ, QUERY_REQ
	function[15:8] : Used by QUERY_REQ / TASK_REQ
	opcode[23:16] : Opcode in query / input parameter1 (TASK_REQ)
	descriptor ID[31:24] : descriptor ID in query / input parameter2 (TASK_REQ)

*/
typedef enum {
	UFS_SEND_CMD = UPIU_HEADER_DWORD(0,
					 0,
					 0,
					 UPIU_TRANSACTION_COMMAND),
	UFS_SEND_REQUEST_SENSE_CMD = UPIU_HEADER_DWORD(0,
						       SCSI_REQ_SENSE,
						       0,
						       UPIU_TRANSACTION_COMMAND),
	UFS_SEND_INQUIRY_CMD = UPIU_HEADER_DWORD(0,
						 SCSI_INQUIRY,
						 0,
						 UPIU_TRANSACTION_COMMAND),
	UFS_SEND_NOP = UPIU_HEADER_DWORD(0,
					 0,
					 0,
					 UPIU_TRANSACTION_NOP_OUT),

	/* For set fDeviceInit */
	UFS_SEND_SET_DEVICEINIT_FLAG = UPIU_HEADER_DWORD(UPIU_FLAG_ID_DEVICEINIT,
							 UPIU_QUERY_OPCODE_SET_FLAG,
							 UFS_STD_WRITE_REQ,
							 UPIU_TRANSACTION_QUERY_REQ),
	UFS_SEND_READ_DEVICEINIT_FLAG = UPIU_HEADER_DWORD(UPIU_FLAG_ID_DEVICEINIT,
							  UPIU_QUERY_OPCODE_READ_FLAG,
							  UFS_STD_READ_REQ,
							  UPIU_TRANSACTION_QUERY_REQ),

	/* For set fPermanentWPEn */
	UFS_SEND_SET_PERM_WP_FLAG = UPIU_HEADER_DWORD(UPIU_FLAG_ID_PER_WRITEPROTECT,
						      UPIU_QUERY_OPCODE_SET_FLAG,
						      UFS_STD_WRITE_REQ,
						      UPIU_TRANSACTION_QUERY_REQ),
	UFS_SEND_GET_PERM_WP_FLAG = UPIU_HEADER_DWORD(UPIU_FLAG_ID_POW_WRITEPROTECT,
						      UPIU_QUERY_OPCODE_READ_FLAG,
						      UFS_STD_READ_REQ,
						      UPIU_TRANSACTION_QUERY_REQ),

	/* For set fPowerOnWPEn */
	UFS_SEND_SET_POW_WP_FLAG = UPIU_HEADER_DWORD(UPIU_FLAG_ID_POW_WRITEPROTECT,
						     UPIU_QUERY_OPCODE_SET_FLAG,
						     UFS_STD_WRITE_REQ,
						     UPIU_TRANSACTION_QUERY_REQ),
	UFS_SEND_GET_POW_WP_FLAG = UPIU_HEADER_DWORD(UPIU_FLAG_ID_POW_WRITEPROTECT,
						     UPIU_QUERY_OPCODE_READ_FLAG,
						     UFS_STD_READ_REQ,
						     UPIU_TRANSACTION_QUERY_REQ),

	/* For set fBackgroundOpsEn */
	UFS_SEND_SET_BGO_EN_FLAG = UPIU_HEADER_DWORD(UPIU_FLAG_ID_BG_OPERATRION,
						     UPIU_QUERY_OPCODE_SET_FLAG,
						     UFS_STD_WRITE_REQ,
						     UPIU_TRANSACTION_QUERY_REQ),
	UFS_SEND_GET_BGO_EN_FLAG = UPIU_HEADER_DWORD(UPIU_FLAG_ID_BG_OPERATRION,
						     UPIU_QUERY_OPCODE_READ_FLAG,
						     UFS_STD_READ_REQ,
						     UPIU_TRANSACTION_QUERY_REQ),
	UFS_SEND_CLN_BGO_EN_FLAG = UPIU_HEADER_DWORD(UPIU_FLAG_ID_BG_OPERATRION,
						     UPIU_QUERY_OPCODE_CLEAR_FLAG,
						     UFS_STD_WRITE_REQ,
						     UPIU_TRANSACTION_QUERY_REQ),

	/* For set fPurgeEnable Write Only */
	UFS_SEND_SET_PERGE_EN_FLAG = UPIU_HEADER_DWORD(UPIU_FLAG_ID_PURGE_ENABLE,
						       UPIU_QUERY_OPCODE_SET_FLAG,
						       UFS_STD_WRITE_REQ,
						       UPIU_TRANSACTION_QUERY_REQ),

	UFS_SEND_READ_DEVICE_DESC = UPIU_HEADER_DWORD(UPIU_DESC_ID_DEVICE,
						      UPIU_QUERY_OPCODE_READ_DESC,
						      UFS_STD_READ_REQ,
						      UPIU_TRANSACTION_QUERY_REQ),
	UFS_SEND_READ_CONFIG_DESC = UPIU_HEADER_DWORD(UPIU_DESC_ID_CONFIGURATION,
						      UPIU_QUERY_OPCODE_READ_DESC,
						      UFS_STD_READ_REQ,
						      UPIU_TRANSACTION_QUERY_REQ),
	UFS_SEND_WRITE_CONFIG_DESC = UPIU_HEADER_DWORD(UPIU_DESC_ID_CONFIGURATION,
						       UPIU_QUERY_OPCODE_WRITE_DESC,
						       UFS_STD_WRITE_REQ,
						       UPIU_TRANSACTION_QUERY_REQ),
	UFS_SEND_READ_UNIT_DESC = UPIU_HEADER_DWORD(UPIU_DESC_ID_UNIT,
						    UPIU_QUERY_OPCODE_READ_DESC,
						    UFS_STD_READ_REQ,
						    UPIU_TRANSACTION_QUERY_REQ),
	UFS_SEND_READ_GEOMETRY_DESC = UPIU_HEADER_DWORD(UPIU_DESC_ID_GEOMETRY,
							UPIU_QUERY_OPCODE_READ_DESC,
							UFS_STD_READ_REQ,
							UPIU_TRANSACTION_QUERY_REQ),
	UFS_SEND_READ_BOOTLUNEN_ATTR = UPIU_HEADER_DWORD(UPIU_ATTR_ID_BOOTLUNEN,
							 UPIU_QUERY_OPCODE_READ_ATTR,
							 UFS_STD_READ_REQ,
							 UPIU_TRANSACTION_QUERY_REQ),
	UFS_SEND_WRITE_BOOTLUNEN_ATTR = UPIU_HEADER_DWORD(UPIU_ATTR_ID_BOOTLUNEN,
							  UPIU_QUERY_OPCODE_WRITE_ATTR,
							  UFS_STD_WRITE_REQ,
							  UPIU_TRANSACTION_QUERY_REQ),
	UFS_SEND_READ_POWERMODE_ATTR = UPIU_HEADER_DWORD(UPIU_ATTR_ID_POWERMODE,
							 UPIU_QUERY_OPCODE_READ_ATTR,
							 UFS_STD_READ_REQ,
							 UPIU_TRANSACTION_QUERY_REQ),
	UFS_SEND_REQUEST_FORMAT_QUERY_REQ = UPIU_HEADER_DWORD(0,
							      0,
							      0xc2,
							      UPIU_TRANSACTION_QUERY_REQ),
	UFS_SEND_TASKMGNT_QUERY = UPIU_HEADER_DWORD(0,
						    0,
						    UFS_QUERY_TASK,
						    UPIU_TRANSACTION_TASK_REQ),
	UFS_SEND_WRITE_LOCK_ATTR = UPIU_HEADER_DWORD(UPIU_ATTR_ID_CONFIGDESCLOCK,
						     UPIU_QUERY_OPCODE_WRITE_ATTR,
						     UFS_STD_WRITE_REQ,
						     UPIU_TRANSACTION_QUERY_REQ),
} ufs_upiu_cmd;

enum {
	UFS_NO_ERROR = 0,
	UFS_ERROR_TIMEOUT = (1 << 0),
};

struct ufs_upiu_header {
	/* DW0 */
	u8 type;		/* [7]HD, [6]DD, [5:0]Transaction Type : UFS1.1 HD/DD should '0' */
	u8 flags;		/* Task Attribute : simple / ordered / head of queue */
	u8 lun;
	u8 tag;

	/* DW1 */
	u8 cmdtype;		/* Command Set Type */
	u8 function;		/* Query Function / Task Manag. Function */
	u8 response;
	u8 status;

	/* DW2 */
	u8 ehslength;		/* Total EHS length */
	u8 deviceinfo;		/* Device Information */
	u16 datalength;		/* Data Seqment Length (MSB|LSB) */

	/* DW3 ~ DW7 */
	u32 tsf[5];		/* Transaction Specific Fields */
} __attribute__ ((__packed__));

#define UPIU_DATA_SIZE		((ALIGNED_UPIU_SIZE-sizeof(struct ufs_upiu_header))/4)

struct ufs_upiu {
	struct ufs_upiu_header header;
	u32 data[UPIU_DATA_SIZE];
} __attribute__ ((__packed__));

/*	Physical Region Descripton Table	*/
struct ufs_prdt {
	u32 base_addr;
	u32 upper_addr;
	u32 reserved;
	u32 size;		/* MSB(reserved) : LSB(data byte count) */
} __attribute__ ((__packed__));

/*	UTP Command Descriptor	*/
struct ufs_cmd_desc {
	struct ufs_upiu command_upiu;
	struct ufs_upiu response_upiu;
	struct ufs_prdt prd_table[SCSI_MAX_SG_SEGMENTS];
} __attribute__ ((__packed__));

/*	UTP Transfer Request Descriptor	*/
struct ufs_utrd {
	/* DW 0-3 */
	u32 dw[4];

	/* DW 4-5 */
	u32 cmd_desc_addr_l;
	u32 cmd_desc_addr_h;

	/* DW 6 */
	u16 rsp_upiu_len;
	u16 rsp_upiu_off;

	/* DW 7 */
	u16 prdt_len;
	u16 prdt_off;
} __attribute__ ((__packed__));

/*	UTP Task Management Request Descriptor	*/
struct ufs_utmrd {
	/* DW 0-3 */
	u32 dw[4];

	/* DW 4-11 */
	struct ufs_upiu_header task_req_upiu;

	/* DW 12-19 */
	struct ufs_upiu_header task_rsp_upiu;
} __attribute__ ((__packed__));

/*	UIC Command	*/
struct ufs_uic_cmd {
	u32 uiccmdr;		/* cmd_type */
	u32 uiccmdarg1;		/* attr_id / reg_addr */
	u32 uiccmdarg2;		/* attr_set_type/ reg_val */
	u32 uiccmdarg3;		/* attr_val / reg_none */
} __attribute__ ((__packed__));

/*	Configuration Descriptor Header	*/
struct ufs_config_desc_header {
	u8 bLength;
	u8 bDescriptorType;
	u8 bNumberLU;
	u8 bBootEnable;
	u8 bDescrAccessEn;
	u8 bInitPowerMode;
	u8 bHighPriorityLUN;
	u8 bSecureRemovalType;
	u8 bInitActiveICCLevel;
	u16 wPeriodicRTCUpdate;
	u8 reserved[5];
} __attribute__ ((__packed__));

/*	Unit Descriptor Configurable Parameters	*/
struct ufs_unit_desc_param {
	u8 bLUEnable;
	u8 bBootLunID;
	u8 bLUWriteProtect;
	u8 bMemoryType;
	u32 dNumAllocUnits;
	u8 bDataReliability;
	u8 bLogicalBlockSize;
	u8 bProvisioningType;
	u16 wContextCapabilities;
	u8 reserved[3];
} __attribute__ ((__packed__));

/*	Configuration Descriptor	*/
struct ufs_config_desc {
	struct ufs_config_desc_header header;
	struct ufs_unit_desc_param unit[8];
} __attribute__ ((__packed__));

/*	Device Descriptor	*/
struct ufs_device_desc {
	u8 bLength;		/* offset : 0x00 */
	u8 bDescriptorType;
	u8 bDevice;
	u8 bDeviceClass;
	u8 bDeviceSubClass;
	u8 bProtocol;
	u8 bNumberLU;
	u8 iNumberWLU;
	u8 bBootEnable;		/* offset : 0x08 */
	u8 bDescrAccessEn;
	u8 bInitPowerMode;
	u8 bHighPriorityLUN;
	u8 bSecureRemovalType;
	u8 bSecurityLU;
	u8 reserved;
	u8 bInitActiveICCLevel;
	u16 wSpecVersion;	/* offset : 0x10 */
	u16 wManufactureData;
	u8 iManufacturerName;
	u8 iProductName;
	u8 iSerialNumber;
	u8 iOemID;
	u16 wManufacturerID;	/* offset : 0x18 */
	u8 bUD0BaseOffset;
	u8 bUDConfigPlength;
	u8 bDeviceRTTCap;
	u16 wPeriodicRTCUpdate;
} __attribute__ ((__packed__));

/*	Unit Descriptor	*/
struct ufs_unit_desc {
	u8 bLength;		/* offset : 0x00 */
	u8 bDescriptorType;
	u8 bUnitIndex;
	u8 bLUEnable;
	u8 bBootLunID;

	u8 bLUWriteProtect;
	u8 bLUQueueDepth;
	u8 Reserved;
	u8 bMemoryType;		/* offset : 0x08 */
	u8 bDataReliability;

	u8 bLogicalBlockSize;
	u32 qLogicalBlockCount_h;
	u32 qLogicalBlockCount_l;
	u32 dEraseBlockSize;	/* offset : 0x13 */
	u8 bProvisioningType;
	u32 qPhyMemResourceCount_h;	/* offset : 0x18 */
	u32 qPhyMemResourceCount_l;
	u16 wContextCapabilities;	/* offset : 0x20 */
	u8 bLargeUnitSize_M1;
} __attribute__ ((__packed__));

/*	Geometry Descriptor	*/
struct ufs_geometry_desc {
	u8 bLength;		/* offset : 0x00 */
	u8 bDescriptorType;
	u8 bMediaTechnology;
	u8 Reserved_03;
	u32 qTotalRawDeviceCapacity_h;
	u32 qTotalRawDeviceCapacity_l;
	u8 Reserved_0c;
	u32 dSegmentSize;	/* offset : 0x0D */
	u8 bAllocationUnitSize;
	u8 bMinAddrBlockSize;
	u8 bOptimalReadBlockSize;
	u8 bOptimalWriteBlockSize;
	u8 bMaxInBufferSize;
	u8 bMaxOutBufferSize;
	u8 bRPMB_ReadWriteSize;
	u8 Reserved_18;		/* offset : 0x18 */
	u8 bDataOrdering;
	u8 bMaxContexIDNumber;
	u8 bSysDataTagUnitSize;
	u8 bSysDataTagResSize;
	u8 bSupportedSecRTypes;
	u16 wSupportedMemoryTypes;
	u32 dSystemCodeMaxNAllocU;	/* offset : 0x20 */
	u16 wSystemCodeCapAdjFac;
	u32 dNonPersistMaxNAllocU;	/* offset : 0x26 */
	u16 wNonPersistCapAdjFac;
	u32 dEnhanced1MaxNAllocU;
	u16 wEnhanced1CapAdjFac;	/* offset : 0x30 */
	u32 dEnhanced2MaxNAllocU;
	u16 wEnhanced2CapAdjFac;
	u32 dEnhanced3MaxNAllocU;	/* offset : 0x38 */
	u16 wEnhanced3CapAdjFac;
	u32 dEnhanced4MaxNAllocU;
	u16 wEnhanced4CapAdjFac;	/* offset : 0x42 */
} __attribute__ ((__packed__));

struct ufs_flag_bit {
	u8 reserved_0;		// bit[0] : Reserved
	u8 fDeviceInit;		// bit[1] : Device Initialization
	u8 fPermanentWPEn;	// bit[2] : Permanent Write Protect Enable
	u8 fPowerOnWPEn;	// bit[3] : Power On Write Protection Enable
	u8 fBackgroundOpsEn;	// bit[4] : Background Operation Enable
	u8 reserved_5;		// bit[5] : Reserved
	u8 fPurgeEnable;	// bit[6] : Purge Enable
	u8 reserved_7;		// bit[7] : Reserved
	u8 fPhyResourceRemoval;	// bit[8] : Physical Resource Removal
	u8 fBusyRTC;		// bit[9] : Busy Real Time Clock
	u8 reserved_10_31[22];	// bit[31:10] : Reserved
} __attribute__ ((__packed__));

/*	Flags	*/
union ufs_flags {
	u8 arry[32];
	struct ufs_flag_bit flag;
};

struct __ufs_attributes {
	u32 bBootLunEn;		// id : 0
	u32 reserved;		// id : 1
	u32 bCurrentPowerMode;	// id : 2
	u32 bActiveICCLevel;	// id : 3
	u32 bOutOfOrderDataEn;	// id : 4
	u32 bBackgroundOpStatus;	// id : 5
	u32 bPurgeStatus;	// id : 6
	u32 bMaxDataInSize;	// id : 7
	u32 bMaxDataOutSize;	// id : 8
	u32 dDynCapNeeded;	// id : 9
	u32 bRefClkFreq;	// id : 10
	u32 bConfigDescrLock;	// id : 11
	u32 bMaxNumOfRTT;	// id : 12
	u32 wExceptionEventControl;	// id : 13
	u32 wExceptionEventStatus;	// id : 14
	u32 dSecondsPassed;	// id : 15
	u32 wContextConf;	// id : 16
	u32 dCorrPrgBlkNum;	// id : 17
};

/*	Attributes		*/
union ufs_attributes {
	u32 arry[18];
	struct __ufs_attributes attr;
};

#ifndef __iomem
#define __iomem
#endif

struct ufs_host {
	char host_name[16];
	unsigned int irq;
	void __iomem *ioaddr;
	void __iomem *phy_pma;
	int host_index;

	ccb *scsi_cmd;
	u8 *ufs_descriptor;
	u8 *arglist;
	ufs_upiu_cmd command_type;
	u32 lun;
	int scsi_status;
	u8 *sense_buffer;
	u32 sense_buflen;

	struct ufs_cmd_desc *cmd_desc_addr;
	struct ufs_utrd *utrd_addr;
	struct ufs_utmrd *utmrd_addr;
	struct ufs_uic_cmd *uic_cmd;

	u32 capabilities;
	int nutrs;
	int nutmrs;
	u32 ufs_version;

	u32 ufs_state;
	u32 int_enable_mask;

	u32 quirks;		/* Quirk flags */

	int (*ufs_board_pre_setup) (struct ufs_host * hba);
	int (*ufs_board_post_setup) (struct ufs_host * hba);
	int (*ufs_hw_reset) (struct ufs_host * hba, int onoff);
	int (*ufs_device_power) (struct ufs_host * hba, int onoff);
	struct ufs_uic_cmd *pre_uic_cmd;	/* should be NULL terminated */
	struct ufs_uic_cmd *post_uic_cmd;	/* should be NULL terminated */
	struct ufs_uic_cmd *pwr_mode_change;	/* should be NULL terminated */

	/* HBA Errors */
	u32 errors;

	/* timeout */
	u32 ufs_cmd_timeout;
	u32 uic_cmd_timeout;
	u32 timeout;

	union ufs_flags flags;
	union ufs_attributes attributes;

	struct ufs_config_desc config_desc;
	struct ufs_device_desc device_desc;
	struct ufs_geometry_desc geometry_desc;
	struct ufs_unit_desc unit_desc[8];
	u16 data_seg_len;
	u8 upiu_data[UPIU_DATA_SIZE];
};

int ufs_init(int mode);
int ufs_board_init(int host_index, struct ufs_host *ufs);

#endif				/* End of Header */
