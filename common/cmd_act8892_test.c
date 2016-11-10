#include <common.h>
#include <command.h>

#define GPJ0CON		*(volatile unsigned int *)(0x14CD0000)
#define GPJ0DAT		*(volatile unsigned int *)(0x14CD0004)
#define GPJ0PUD		*(volatile unsigned int *)(0x14CD0008)

#define IIC0_ESCL_Hi		GPJ0DAT |= (0x1<<1)
#define IIC0_ESCL_Lo		GPJ0DAT &= ~(0x1<<1)
#define IIC0_ESDA_Hi		GPJ0DAT |= (0x1<<0)
#define IIC0_ESDA_Lo	GPJ0DAT &= ~(0x1<<0)


#define IIC0_ESCL_INP	GPJ0CON &= ~(0xf<<4)
#define IIC0_ESCL_OUTP	GPJ0CON = (GPJ0CON & ~(0xf<<4))|(0x1<<4)

#define IIC0_ESDA_INP	GPJ0CON &= ~(0xf<<0)
#define IIC0_ESDA_OUTP	GPJ0CON = (GPJ0CON & ~(0xf<<0))|(0x1<<0)

#define DELAY		100

#define GPIO_DAT	GPJ0DAT
#define GPIO_DAT_SHIFT	(0)
#define GPIO_PUD	GPJ0PUD &= ~(0xF<<0)


#define GPD4CON		*(volatile unsigned int *)(0x13470100)
#define GPD4DAT		*(volatile unsigned int *)(0x13470104)
#define GPD4PUD		*(volatile unsigned int *)(0x13470108)

#define OLED_RESET_OUT			GPD4CON = (GPD4CON & ~(0xf<<0))|(0x1<<0)
#define OLED_RESET_Hi			GPD4DAT |= (1 << 0)
#define OLED_RESET_Lo			GPD4DAT &= ~(1 << 0)

#define GPR2CON		*(volatile unsigned int *)(0x15690040)
#define GPR2DAT		*(volatile unsigned int *)(0x15690044)
#define GPR2PUD		*(volatile unsigned int *)(0x15690048)

#define OLED_VDD15_OUT			GPR2CON = (GPR2CON & ~(0xf<<0))|(0x1<<0)
#define OLED_VDD15_Hi			GPR2DAT |= (1 << 0)
#define OLED_VDD15_Lo			GPR2DAT &= ~(1 << 0)


static void Delay(void)
{
	unsigned long i;
	for(i=0;i<DELAY;i++);
}

static void IIC0_SCLH_SDAH(void)
{
	IIC0_ESCL_Hi;
	IIC0_ESDA_Hi;
	Delay();
}

static void IIC0_SCLH_SDAL(void)
{
	IIC0_ESCL_Hi;
	IIC0_ESDA_Lo;
	Delay();
}

static void IIC0_SCLL_SDAH(void)
{
	IIC0_ESCL_Lo;
	IIC0_ESDA_Hi;
	Delay();
}

static void IIC0_SCLL_SDAL(void)
{
	IIC0_ESCL_Lo;
	IIC0_ESDA_Lo;
	Delay();
}

static void IIC0_ELow(void)
{
	IIC0_SCLL_SDAL();
	IIC0_SCLH_SDAL();
	IIC0_SCLH_SDAL();
	IIC0_SCLL_SDAL();
}

static void IIC0_EHigh(void)
{
	IIC0_SCLL_SDAH();
	IIC0_SCLH_SDAH();
	IIC0_SCLH_SDAH();
	IIC0_SCLL_SDAH();
}

static void IIC0_EStart(void)
{
	IIC0_SCLH_SDAH();
	IIC0_SCLH_SDAL();
	Delay();
	IIC0_SCLL_SDAL();
}

static void IIC0_EEnd(void)
{
	IIC0_SCLL_SDAL();
	IIC0_SCLH_SDAL();
	Delay();
	IIC0_SCLH_SDAH();
}

static void IIC0_EAck_write(void)
{
	unsigned long ack;

	IIC0_ESDA_INP;			// Function <- Input

	IIC0_ESCL_Lo;
	Delay();
	IIC0_ESCL_Hi;
	Delay();
	ack = GPIO_DAT;
	IIC0_ESCL_Hi;
	Delay();
	IIC0_ESCL_Hi;
	Delay();

	IIC0_ESDA_OUTP;			// Function <- Output (SDA)

	ack = (ack>>GPIO_DAT_SHIFT)&GPIO_DAT_SHIFT;
//	while(ack!=0);

	IIC0_SCLL_SDAL();
}

static void IIC0_EAck_read(void)
{
	IIC0_ESDA_OUTP;			// Function <- Output

	IIC0_ESCL_Lo;
	IIC0_ESCL_Lo;
	IIC0_ESDA_Hi;
	IIC0_ESCL_Hi;
	IIC0_ESCL_Hi;

	IIC0_ESDA_INP;			// Function <- Input (SDA)

	IIC0_SCLL_SDAL();
}

static void IIC0_ESetport(void)
{
	//printf("[01]\n");
	GPIO_PUD;	// Pull Up/Down Disable	SCL, SDA
	//printf("[02]\n");
	IIC0_ESCL_Hi;
	IIC0_ESDA_Hi;
	//printf("[03]\n");
	IIC0_ESCL_OUTP;		// Function <- Output (SCL)
	IIC0_ESDA_OUTP;		// Function <- Output (SDA)
	//printf("[04]\n");
	Delay();
	//printf("[05]\n");
}

static void IIC_EWrite_GPIO (unsigned char ChipId, unsigned char IicAddr, unsigned char IicData)
{
	unsigned long i;

	IIC0_EStart();

////////////////// write chip id //////////////////
	for(i = 7; i>0; i--)
	{
		if((ChipId >> i) & 0x0001)
			IIC0_EHigh();
		else
			IIC0_ELow();
	}

	IIC0_ELow();	// write

	IIC0_EAck_write();	// ACK

////////////////// write reg. addr. //////////////////
	for(i = 8; i>0; i--)
	{
		if((IicAddr >> (i-1)) & 0x0001)
			IIC0_EHigh();
		else
			IIC0_ELow();
	}

	IIC0_EAck_write();	// ACK

////////////////// write reg. data. //////////////////
	for(i = 8; i>0; i--)
	{
		if((IicData >> (i-1)) & 0x0001)
			IIC0_EHigh();
		else
			IIC0_ELow();
	}

	IIC0_EAck_write();	// ACK

	IIC0_EEnd();
}

static void IIC_ERead_GPIO (unsigned char ChipId, unsigned char IicAddr, unsigned char *IicData)
{
	unsigned long i, reg;
	unsigned char data = 0;

	IIC0_EStart();

////////////////// write chip id //////////////////
	for(i = 7; i>0; i--)
	{
		if((ChipId >> i) & 0x0001)
			IIC0_EHigh();
		else
			IIC0_ELow();
	}

	IIC0_ELow();	// write

	IIC0_EAck_write();	// ACK

////////////////// write reg. addr. //////////////////
	for(i = 8; i>0; i--)
	{
		if((IicAddr >> (i-1)) & 0x0001)
			IIC0_EHigh();
		else
			IIC0_ELow();
	}

	IIC0_EAck_write();	// ACK

	IIC0_EStart();

////////////////// write chip id //////////////////
	for(i = 7; i>0; i--)
	{
		if((ChipId >> i) & 0x0001)
			IIC0_EHigh();
		else
			IIC0_ELow();
	}

	IIC0_EHigh();	// read

	IIC0_EAck_write();	// ACK

////////////////// read reg. data. //////////////////
	IIC0_ESDA_INP;

	IIC0_ESCL_Lo;
	IIC0_ESCL_Lo;
	Delay();

	for(i = 8; i>0; i--)
	{
		IIC0_ESCL_Lo;
		IIC0_ESCL_Lo;
		Delay();
		IIC0_ESCL_Hi;
		IIC0_ESCL_Hi;
		Delay();
		reg = GPIO_DAT;
		IIC0_ESCL_Hi;
		IIC0_ESCL_Hi;
		Delay();
		IIC0_ESCL_Lo;
		IIC0_ESCL_Lo;
		Delay();

		reg = (reg >> GPIO_DAT_SHIFT) & 0x1;

		data |= reg << (i-1);
	}

	IIC0_EAck_read();	// ACK
	IIC0_ESDA_OUTP;

	IIC0_EEnd();

	*IicData = data;
}

static int IIC_Write_With_Confirm(unsigned char addr, unsigned char dat)
{
	unsigned char rd;
	int retry_count = 0;
	do
	{
		IIC_EWrite_GPIO(0xB6, addr, dat);
		IIC_ERead_GPIO(0xB7, addr, &rd);
		retry_count++;
	}while((dat != rd) && (retry_count < 100));

	if (dat != rd)
		return -1;

	return 0;
}

static unsigned char IIC_Read_With_Confirm(unsigned char addr)
{
	unsigned char rd1, rd2;
	int retry_count = 0;
	do
	{		
		IIC_ERead_GPIO (0xB7, addr, &rd1);
		IIC_ERead_GPIO (0xB7, addr, &rd2);
		retry_count++;
	}while((rd1 != rd2) && (retry_count < 100));

	if (rd1 != rd2)
		return 0xFF;

	return rd1;
}

static void lcd_pmic_init(void)
{
	unsigned char rd;	

	OLED_VDD15_OUT;
//	OLED_VDD15_Lo;
	OLED_VDD15_Hi;
	
	OLED_RESET_OUT;
	OLED_RESET_Lo;

	IIC0_ESetport();

	IIC_Write_With_Confirm(0x40, 0x18);		 // out3 1.2V				VDD12_VTCAM

	IIC_Write_With_Confirm(0x20, 0x24);		 // out1 1.8V 				VDVDI  VDD18_LCD
	IIC_Write_With_Confirm(0x30, 0x36);		 // out2 3.0V				VCI	    VDD30_LCD
	IIC_Write_With_Confirm(0x50, 0x24);		 // out4 1.8V				TSP_DVDD_1.8V,
	IIC_Write_With_Confirm(0x54, 0x39);		 // out5 3.3V				TSP_AVDD_3.3V,	
	
	IIC_Write_With_Confirm(0x60, 0x24);		 // out6 1.8V				VDD18_VTCAMIO
	IIC_Write_With_Confirm(0x64, 0x34);		 // out7 2.8V				VDD28_VTCAM

	rd = IIC_Read_With_Confirm(0x51);		// out4
	rd = ((rd &  ~(0x7 << 2)) & ~(1 << 7));
	rd |= (1 << 7);
	IIC_Write_With_Confirm(0x51, rd);	

	rd = IIC_Read_With_Confirm(0x55);		// out5
	rd = ((rd &  ~(0x7 << 2)) & ~(1 << 7));
	rd |= (1 << 7);
	IIC_Write_With_Confirm(0x55, rd);

	rd = IIC_Read_With_Confirm(0x61);		// out6
	rd = ((rd &  ~(0x7 << 2)) & ~(1 << 7));
	rd |= (1 << 7);
	IIC_Write_With_Confirm(0x61, rd);

	rd = IIC_Read_With_Confirm(0x65);		// out7
	rd = ((rd &  ~(0x7 << 2)) & ~(1 << 7));
	rd |= (1 << 7);
	IIC_Write_With_Confirm(0x65, rd);	

#if 0
	rd = IIC_Read_With_Confirm(0x42);		// out3
	rd = ((rd &  ~(0x7 << 2)) | (1 << 7));
	IIC_Write_With_Confirm(0x42, rd);			// the rising time of power voltage is so long
	
	rd = IIC_Read_With_Confirm(0x22);		// out1
	rd = ((rd &  ~(0x7 << 2)) & ~(1 << 7));
	IIC_Write_With_Confirm(0x22, rd);			

	rd = IIC_Read_With_Confirm(0x32);		// out2
	rd = ((rd &  ~(0x7 << 2)) & ~(1 << 7));
	IIC_Write_With_Confirm(0x32, rd);

	rd = IIC_Read_With_Confirm(0x51);		// out4
	rd = ((rd &  ~(0x7 << 2)) & ~(1 << 7));
	IIC_Write_With_Confirm(0x51, rd);	

	rd = IIC_Read_With_Confirm(0x55);		// out5
	rd = ((rd &  ~(0x7 << 2)) & ~(1 << 7));
	IIC_Write_With_Confirm(0x55, rd);

	rd = IIC_Read_With_Confirm(0x61);		// out6
	rd = ((rd &  ~(0x7 << 2)) & ~(1 << 7));
	IIC_Write_With_Confirm(0x61, rd);

	rd = IIC_Read_With_Confirm(0x65);		// out7
	rd = ((rd &  ~(0x7 << 2)) & ~(1 << 7));
	IIC_Write_With_Confirm(0x65, rd);
#endif	
}

int do_i2ct(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{	
	unsigned char addr, data;
	
	if (strcmp(argv[1], "w") == 0) 
	{
		if (argc != 4) {
			 printf("help: argc=%d\n%s\n", argc, cmdtp->help);
			 return -1;
		}
	
		addr = simple_strtoul(argv[2], NULL, 16);	
		data = simple_strtoul(argv[3], NULL, 16);
		IIC0_ESetport();
		IIC_EWrite_GPIO (0xB6, addr, data);
		IIC_EWrite_GPIO (0xB6, addr, data);
		IIC_EWrite_GPIO (0xB6, addr, data);
		IIC_EWrite_GPIO (0xB6, addr, data);

	}
	else if (strcmp(argv[1], "r") == 0) 
	{
		if (argc != 3) {
			 printf("help: argc=%d\n%s\n", argc, cmdtp->help);
			 return -1;
		}

		addr = simple_strtoul(argv[2], NULL, 16);	
		IIC0_ESetport();
		IIC_ERead_GPIO (0xB6, addr, &data);		
		 printf("0x%X = 0x%X\n", addr, data);
	}
	else if (strcmp(argv[1], "g") == 0) 
	{
		if (argc != 2) {
			 printf("help: argc=%d\n%s\n", argc, cmdtp->help);
			 return -1;
		}

		IIC0_ESetport();
		while(1)
		{
			IIC_ERead_GPIO (0xB6, 0, &data);	
			printf("[0x%X = 0x%X]", addr, data);
		}
	}
	else if (strcmp(argv[1], "d") == 0) 
	{
		if (argc != 2) {
			 printf("help: argc=%d\n%s\n", argc, cmdtp->help);
			 return -1;
		}

		lcd_pmic_init();		
	}

	return 0;
}

U_BOOT_CMD(
	i2ct,	4,	0,	do_i2ct,
	"i2c act8892_test with i2c",
	"i2c w a value: write\n"
	"i2c r a: read\n"
	"i2c g : i2c port test\n"
	"i2c d : setting pmic default values\n"
);

