/*
 * Samsung SoC MIPI LCD CONTROL functions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/
#include <asm/arch/mipi_dsim.h>

#include "s6e3ha0_gamma.h"
#include "s6e3ha0_param.h"
#include "lcd_ctrl.h"
#include "../dsim.h"

#define GAMMA_PARAM_SIZE 26

/* Porch values. It depends on command or video mode */
#define S6E3HA0_CMD_VBP		20
#define S6E3HA0_CMD_VFP		1
#define S6E3HA0_CMD_VSA		1
#define S6E3HA0_CMD_HBP		1
#define S6E3HA0_CMD_HFP		1
#define S6E3HA0_CMD_HSA		1

/* These need to define */
#define S6E3HA0_VIDEO_VBP	15
#define S6E3HA0_VIDEO_VFP	1
#define S6E3HA0_VIDEO_VSA	1
#define S6E3HA0_VIDEO_HBP	20
#define S6E3HA0_VIDEO_HFP	20
#define S6E3HA0_VIDEO_HSA	20

#define S6E3HA0_HORIZONTAL	1440
#define S6E3HA0_VERTICAL	2560

#define msleep(a) udelay(a * 1000)

struct decon_lcd s6e3ha0_lcd_info = {
#ifdef CONFIG_FB_VIDEO_MODE
	.mode = DECON_MIPI_VIDEO_MODE,
	.vfp = S6E3HA0_VIDEO_VFP,
	.vbp = S6E3HA0_VIDEO_VBP,
	.hfp = S6E3HA0_VIDEO_HFP,
	.hbp = S6E3HA0_VIDEO_HBP,
	.vsa = S6E3HA0_VIDEO_VSA,
	.hsa = S6E3HA0_VIDEO_HSA,
#else
	.mode = DECON_MIPI_COMMAND_MODE,
	.vfp = S6E3HA0_CMD_VFP,
	.vbp = S6E3HA0_CMD_VBP,
	.hfp = S6E3HA0_CMD_HFP,
	.hbp = S6E3HA0_CMD_HBP,
	.vsa = S6E3HA0_CMD_VSA,
	.hsa = S6E3HA0_CMD_HSA,
#endif
	.xres = S6E3HA0_HORIZONTAL,
	.yres = S6E3HA0_VERTICAL,

	/* Maybe, width and height will be removed */
	.width = 70,
	.height = 121,

	/* Mhz */
	.hs_clk = 1100,
	.esc_clk = 20,

	.fps = 60,
	.mic_enabled = 1,
};

struct decon_lcd *decon_get_lcd_info(void)
{
	return &s6e3ha0_lcd_info;
}

/*
 * Read ID for LCD panel
*/
static int lcd_read_id(u32 id)
{
	int i, ret;
	u8 buf[LDI_ID_LEN + 1] = {0,};
	unsigned int lcd_type;

	ret = dsim_rd_data(id, MIPI_DSI_DCS_READ, LDI_ID_REG, LDI_ID_LEN, buf);

	if (!ret) {
		dsim_err("%s: failed\n", __func__);
		return -EBUSY;
	}

	lcd_type = buf[0] << 16 | buf[1] << 8 | buf[2];

	dsim_dbg("%s is successfully , data = 0x%08x\n", __func__, lcd_type);

	return ret;
}

/*
 * 3FAH0 lcd init sequence
 *
 * Parameters
 *	- mic_enabled : if mic is enabled, MIC_ENABLE command must be sent
 *	- mode : LCD init sequence depends on command or video mode
 */
void lcd_init(u32 id, struct decon_lcd *lcd)
{
	lcd_read_id(id);

	if (dsim_wr_data(id, MIPI_DSI_DCS_LONG_WRITE,
				(unsigned long)SEQ_TEST_KEY_ON_F0,
				ARRAY_SIZE(SEQ_TEST_KEY_ON_F0)) == -1)
		dsim_err("fail to write F0 init command.\n");

	if (dsim_wr_data(id, MIPI_DSI_DCS_SHORT_WRITE, SEQ_SLEEP_OUT[0], 0) < 0)
		dsim_err("fail to send SEQ_SLEEP_OUT command.\n");

	msleep(120);

	if (dsim_wr_data(id, MIPI_DSI_DCS_LONG_WRITE,
				(unsigned long)SEQ_TEST_KEY_ON_F1,
				ARRAY_SIZE(SEQ_TEST_KEY_ON_F1)) == -1)
		dsim_err("fail to write F1 init command.\n");

	if (dsim_wr_data(id, MIPI_DSI_DCS_LONG_WRITE,
				(unsigned long)SEQ_TEST_KEY_ON_FC,
				ARRAY_SIZE(SEQ_TEST_KEY_ON_FC)) == -1)
		dsim_err("fail to write FC init command.\n");

	if (dsim_wr_data(id, MIPI_DSI_DCS_LONG_WRITE,
				(unsigned long)SEQ_TEST_KEY_ON_F9,
				ARRAY_SIZE(SEQ_TEST_KEY_ON_F9)) == -1)
		dsim_err("fail to write F9 init command.\n");

	if (dsim_wr_data(id, MIPI_DSI_DCS_LONG_WRITE,
				(unsigned long)SEQ_TEST_KEY_ON_F2,
				ARRAY_SIZE(SEQ_TEST_KEY_ON_F2)) == -1)
		dsim_err("fail to write F9 init command.\n");

	if (dsim_wr_data(id, MIPI_DSI_DCS_SHORT_WRITE, SEQ_TE_ON[0], 0) < 0)
		dsim_err("fail to send SEQ_TE_ON command.\n");

	if (dsim_wr_data(id, MIPI_DSI_DCS_SHORT_WRITE_PARAM,
				(unsigned long)SEQ_TEST_KEY_ON_D0[0],
				(unsigned int)SEQ_TEST_KEY_ON_D0[1]) == -1)
		dsim_err("fail to write D0 init command.\n");

	dsim_dbg("%s: is successfully\n", __func__);
}

void lcd_enable_exynos(u32 id)
{
	if (dsim_wr_data(id, MIPI_DSI_DCS_SHORT_WRITE, SEQ_DISPLAY_ON[0], 0) < 0)
		dsim_err("fail to send SEQ_DISPLAY_ON command.\n");
}

void lcd_disable_exynos(u32 id)
{
	return;
}

/*
 * Set gamma values
 *
 * Parameter
 *	- backlightlevel : It is from 0 to 26.
 */
int lcd_gamma_ctrl(u32 backlightlevel)
{
	return 0;
}

int lcd_gamma_update(void)
{
	return 0;
}
