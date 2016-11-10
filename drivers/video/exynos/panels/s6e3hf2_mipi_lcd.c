/*
 * Samsung SoC MIPI LCD driver.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include "../dsim.h"
#include "lcd_ctrl.h"
#include "decon_lcd.h"

static struct dsim_device *dsim_base;

static int s6e3hf2_probe(struct dsim_device *dsim)
{
	return 0;
}

static int s6e3hf2_displayon(struct dsim_device *dsim)
{
	struct decon_lcd *lcd = decon_get_lcd_info();

	lcd_init(dsim->id, lcd);
	lcd_enable_exynos(dsim->id);
	return 1;
}

static int s6e3hf2_suspend(struct dsim_device *dsim)
{
	return 0;
}

static int s6e3hf2_resume(struct dsim_device *dsim)
{
	return 0;
}

struct mipidsim_lcd_driver s6e3hf2_mipi_lcd_driver = {
	.probe		= s6e3hf2_probe,
	.displayon	= s6e3hf2_displayon,
	.suspend	= s6e3hf2_suspend,
	.resume		= s6e3hf2_resume,
};

struct mipidsim_lcd_driver *decon_get_panel_info(void)
{
	return &s6e3hf2_mipi_lcd_driver;
}
