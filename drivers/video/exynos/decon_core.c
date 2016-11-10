/*
 * SAMSUNG SoC DECON driver
 *
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * Sewoon Park <seuni.park@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <malloc.h>
#include "decon.h"
#include "dsim.h"
#include "decon_config.h"
#include "./panels/lcd_ctrl.h"

struct decon_device *decon_int_drvdata;
EXPORT_SYMBOL(decon_int_drvdata);
struct decon_device *decon_ext_drvdata;
EXPORT_SYMBOL(decon_ext_drvdata);

extern struct dsim_device *dsim0_for_decon;
extern struct dsim_device *dsim1_for_decon;

/* ---------- CHECK FUNCTIONS ----------- */
static void decon_to_psr_info(struct decon_device *decon,
				struct decon_psr_info *psr)
{
	psr->psr_mode = decon->pdata->psr_mode;
	psr->trig_mode = decon->pdata->trig_mode;
}

static void decon_to_init_param(struct decon_device *decon, struct decon_init_param *p)
{
	struct decon_lcd *lcd_info = decon_get_lcd_info();
	int ret = 0;

	if (decon->id) {
		/* should be control a DECON_EXT */
	} else {
		p->lcd_info = lcd_info;
	}
	p->psr.psr_mode = decon->pdata->psr_mode;
	p->psr.trig_mode = decon->pdata->trig_mode;
	p->nr_windows = decon->pdata->max_win;
}

static u32 vidosd_a(int x, int y)
{
	return VIDOSD_A_TOPLEFT_X(x) |
			VIDOSD_A_TOPLEFT_Y(y);
}

static u32 vidosd_b(int x, int y, u32 xres, u32 yres)
{
	return VIDOSD_B_BOTRIGHT_X(x + xres - 1) |
		VIDOSD_B_BOTRIGHT_Y(y + yres - 1);
}

static u32 vidosd_c(u8 r0, u8 g0, u8 b0)
{
	return VIDOSD_C_ALPHA0_R_F(r0) |
		VIDOSD_C_ALPHA0_G_F(g0) |
		VIDOSD_C_ALPHA0_B_F(b0);
}

static u32 vidosd_d(u8 r1, u8 g1, u8 b1)
{
	return VIDOSD_D_ALPHA1_R_F(r1) |
		VIDOSD_D_ALPHA1_G_F(g1) |
		VIDOSD_D_ALPHA1_B_F(b1);
}

/* ---------- FB_IOCTL INTERFACE ----------- */
static int decon_wait_for_vsync(struct decon_device *decon, u32 timeout)
{
	return decon_reg_wait_for_update_timeout(decon->id, timeout * 1000);
}

static void decon_parse_pdata(struct decon_device *decon, u32 dev_id)
{
	struct exynos_decon_platdata *pdata = get_decon_pdata();

	decon->id = dev_id;
	decon->pdata->ip_ver = pdata->ip_ver;
	decon->pdata->psr_mode = pdata->psr_mode;
	decon->pdata->trig_mode = pdata->trig_mode;
	decon->pdata->dsi_mode = pdata->dsi_mode;
	decon->pdata->max_win = pdata->max_win;
	decon->pdata->default_win = pdata->default_win;

	decon_info("decon-%s: ver%d, max win%d, %s mode, %s trigger\n",
			decon->id ? "ext" : "int", decon->pdata->ip_ver,
			decon->pdata->max_win,
			decon->pdata->psr_mode ? "command" : "video",
			decon->pdata->trig_mode ? "sw" : "hw");
	decon_info("%s dsi mode\n", decon->pdata->dsi_mode ? "dual" : "single");
}

/* --------- DRIVER INITIALIZATION ---------- */
static int decon_probe(u32 dev_id)
{
	struct decon_device *decon;
	int ret = 0;
	struct decon_psr_info psr;
	struct decon_init_param p;
	struct decon_regs_data win_regs;
	struct dsim_device *dsim;

	if (dev_id > MAX_DECON_ID) {
		decon_err("does not support (%d) decon device\n", dev_id);
		return -ERANGE;
	}

	decon = kzalloc(sizeof(struct decon_device), GFP_KERNEL);
	if (!decon) {
		decon_err("no memory for decon device\n");
		return -ENOMEM;
	}

	/* setup pointer to master device */
	decon->pdata = kzalloc(sizeof(struct exynos_decon_platdata),
						GFP_KERNEL);
	if (!decon->pdata) {
		decon_err("no memory for DECON platdata\n");
		return -ENOMEM;
	}

	/* store platform data ptr to decon_tv context */
	decon_parse_pdata(decon, dev_id);

	/* init clock setting for decon */
	if (!decon->id) {
		decon_int_drvdata = decon;
		dsim = dsim0_for_decon;
		decon->regs = DECON_INT_BASE_ADDR;
	} else {
		decon_ext_drvdata = decon;
		dsim = dsim1_for_decon;
		decon->regs = DECON_EXT_BASE_ADDR;
	}

	if (!decon->id) {
		decon_reg_shadow_protect_win(decon->id, 0, 1);

		decon_to_init_param(decon, &p);
		decon_reg_init(decon->id, decon->pdata->dsi_mode, &p);

		win_regs.wincon = WINCON_BPPMODE_ARGB8888;
		win_regs.winmap = 0x0;
		win_regs.vidosd_a = vidosd_a(0, 0);
		win_regs.vidosd_b = vidosd_b(0, 0,
					p.lcd_info->xres, p.lcd_info->yres);
		win_regs.vidosd_c = vidosd_c(0x0, 0x0, 0x0);
		win_regs.vidosd_d = vidosd_d(0xff, 0xff, 0xff);
		win_regs.vidw_buf_start = UBOOT_ON_LCD_START_ADDR;
		win_regs.vidw_whole_w = p.lcd_info->xres;
		win_regs.vidw_whole_h = p.lcd_info->yres;
		win_regs.vidw_offset_x = 0;
		win_regs.vidw_offset_y = 0;
		win_regs.type = IDMA_G0;

		decon_reg_set_regs_data(decon->id, 0, &win_regs);

		decon_reg_shadow_protect_win(decon->id, 0, 0);

		decon_to_psr_info(decon, &psr);
		decon_reg_start(decon->id, decon->pdata->dsi_mode, &psr);

		decon_reg_activate_window(decon->id, 0);

		decon_reg_set_winmap(decon->id, 0, 0x000033 /* black */, 1);

		decon_reg_set_trigger(decon->id, decon->pdata->dsi_mode,
				decon->pdata->trig_mode, DECON_TRIG_ENABLE);

		call_panel_ops(dsim, displayon, dsim);

		/* Sent first frame to LCD */
		decon_wait_for_vsync(decon, VSYNC_TIMEOUT_MSEC);

		if (decon->pdata->trig_mode == DECON_HW_TRIG)
			decon_reg_set_trigger(decon->id, decon->pdata->dsi_mode,
					decon->pdata->trig_mode, DECON_TRIG_DISABLE);
	}

	decon_info("decon%d registered successfully\n", decon->id);

	return 0;
}

/* --------- DRIVER DISPLAY START ---------- */
int drv_video_init(void)
{
	struct exynos_decon_platdata *pdata = get_decon_pdata();
	int ret = 0;
	int i;

	for (i = 0; i <= pdata->dsi_mode; i++) {
		ret = dsim_probe(i);
		if (ret < 0) {
			decon_err("dsim%d probe was failed\n", i);
			return ret;
		}
		ret = decon_probe(i);
		if (ret < 0) {
			decon_err("decon%d probe was failed\n", i);
			return ret;
		}
	}

	return ret;
}
