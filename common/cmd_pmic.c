#include <common.h>
#include <command.h>

extern void pmic_mic23156_chg_vq1v1_init(unsigned int val);
extern void pmic_mic23156_motor_v21v1_init(unsigned int val);
extern void pmic_mic23156_fuel_v11v8_init(unsigned int val);

#if 0
void pmic_s2mps15_buck6(unsigned int val)
{
	IIC0_EWrite(S2MPS15_ADDR, BUCK6_OUT, val);
}

void pmic_s2mps15_ldo16(unsigned int val)
{
	unsigned char ldo_ctrl;
	
	/* enable LDO15 : VCC_1.8V_PERI */
	//IIC0_ERead(S2MPS15_ADDR, LDO16_CTRL, &ldo_ctrl);
	ldo_ctrl = val | (1<<6);
	IIC0_EWrite(S2MPS15_ADDR, LDO16_CTRL, ldo_ctrl);
}
#else

extern void pmic_s2mps15_buck7(unsigned int val);
extern void pmic_s2mps15_ldo16(unsigned int val);
	
#endif
int do_pmic(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned int len = simple_strtoul(argv[2], NULL, 10);
	unsigned int repeat = simple_strtoul(argv[3], NULL, 10);
	unsigned char addr, data;
	unsigned int vol1, vol2, vol3;
	unsigned int buck_val1, buck_val2, buck_val3;
	unsigned int out_vol1, out_vol2, out_vol3;
	
	if ((argc > 5) || (argc < 4))  {
		 printf("help:\n%s\n", cmdtp->help);
		 return -1;
	}

	addr = simple_strtoul(argv[2], NULL, 16);
	if (strcmp(argv[1], "s") == 0) 
	{
		if (argc != 4) {
			 printf("help:\n%s\n", cmdtp->help);
			 return -1;
		}
		
		vol1 = simple_strtoul(argv[2], NULL, 10);
		vol2 = simple_strtoul(argv[3], NULL, 10);

		if ((vol1 < 1000) || (vol2 < 1000) || 
		     (vol1 > 1800) || (vol2 > 1300))
		{
			printf("help:\n%s\n", cmdtp->help);
			return -1;
		}
		
		buck_val1 = (unsigned int)((vol1 - 1000)/25) + 0x0C;
		buck_val2 = (unsigned int)((vol2 - 500)/6.25) + 0x20;

//		printf("buck1 vol = %d\n", buck_val1);
//		printf("buck2 vol = %d\n", buck_val2);
		
		out_vol1 = (double)(1000 + 25 * (double)(buck_val1 - 0x0C));
		out_vol2 = (double)(500 + 6.25 * (double)(buck_val2 - 0x20));
		
		pmic_s2mps15_ldo16(buck_val1);		
		pmic_s2mps15_buck7(buck_val2);
			
		printf("vdd1 vol = %dmV\n", out_vol1);
		printf("vdd2 vol = %dmV\n", out_vol2);
		
	}
	else if (strcmp(argv[1], "m") == 0) 
	{
		if (argc != 5) {
			 printf("help:\n%s\n", cmdtp->help);
			 return -1;
		}
		
		vol1 = simple_strtoul(argv[2], NULL, 10);
		vol2 = simple_strtoul(argv[3], NULL, 10);
		vol3 = simple_strtoul(argv[4], NULL, 10);
		
		if ((vol1 < 1000) || (vol2 < 1000) || (vol3 < 1000) ||
		     (vol1 > 1800) || (vol2 > 1300) || (vol3 > 1300))
		{
			printf("help:\n%s\n", cmdtp->help);
			return -1;
		}
		
		buck_val1 = (unsigned int)((vol1 - 700)/10);
		buck_val2 = (unsigned int)((vol2 - 700)/10);
		buck_val3 = (unsigned int)((vol3 - 700)/10);

		out_vol1 = (700 + 10 * buck_val1);
		out_vol2 = (700 + 10 * buck_val2);
		out_vol3 = (700 + 10 * buck_val3);

		pmic_mic23156_chg_vq1v1_init(buck_val1);
		pmic_mic23156_motor_v21v1_init(buck_val2);
		pmic_mic23156_fuel_v11v8_init(buck_val3);
		
		printf("vdd1 vol = %dmV\n", out_vol1);
		printf("vdd2 vol = %dmV\n", out_vol2);
		printf("vddq vol = %dmV\n", out_vol3);
	}

	return 0;
}

#if 1
U_BOOT_CMD(
	pmic,	5,	0,	do_pmic,
	"pmic control for dram. vdd2 or vddq range 1000mV ~ 1300mV, vdd1 range 1000mV ~ 1800mV",
	"pmic s vdd1 vdd2\n"
	"pmic m vdd1 vdd2 vddq\n"
	"ex) pmic m 1000 1200 1300\n"
);
#endif
