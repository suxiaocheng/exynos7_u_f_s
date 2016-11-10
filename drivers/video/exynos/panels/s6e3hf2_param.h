/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __S6E3HF2_PARAM_H__
#define __S6E3HF2_PARAM_H__

struct lcd_seq_info {
	unsigned char	*cmd;
	unsigned int	len;
	unsigned int	sleep;
};

static const unsigned char SEQ_TEST_KEY_ON_F0[] = {
	0xF0,
	0x5A, 0x5A,
};

static const unsigned char SEQ_TEST_KEY_OFF_F0[] = {
	0xF0,
	0xA5, 0xA5,
};

static const unsigned char SEQ_TEST_KEY_ON_F1[] = {
	0xF1,
	0x5A, 0x5A,
};

static const unsigned char SEQ_TEST_KEY_ON_F2[] = {
	0xF2,
	0x60,
};

static const unsigned char SEQ_TEST_KEY_OFF_F1[] = {
	0xF1,
	0xA5, 0xA5,
};

static const unsigned char SEQ_TEST_KEY_ON_F9[] = {
	0xF9,
	0x29,
};

static const unsigned char SEQ_TEST_KEY_ON_FC[] = {
	0xFC,
	0x5A, 0x5A,
};

static const unsigned char SEQ_TEST_KEY_OFF_FC[] = {
	0xFC,
	0xA5, 0xA5,
};

static const unsigned char SEQ_TEST_KEY_CA[] = {
	0xCA,
	0x01, 0x00, 0x01, 0x00, 0x01, 0x00,
};

static const unsigned char SEQ_GAMMA_CONTROL_SET[] = {
	0xCA,
	0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x80, 0x00, 0x00
};

static const unsigned char SEQ_AOR_CONTROL[] = {
	0xB2,
	0x03, 0x10
};

static const unsigned char SEQ_ELVSS_SET[] = {
	0xB6,
	0x9C,	/* MPS_CON: ACL OFF */
	0x0A	/* ELVSS: MAX */
};

static const unsigned char SEQ_GAMMA_UPDATE[] = {
	0xF7,
	0x03,
};

static const unsigned char SEQ_GAMMA_UPDATE_L[] = {
	0xF7,
	0x00,
};

static const unsigned char SEQ_TSET[] = {
	0xB8,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19	/* Global para(8th) + 25 degrees  : 0x19 */
};

static const unsigned char SEQ_SLEEP_IN[] = {
	0x10,
};

static const unsigned char SEQ_SLEEP_OUT[] = {
	0x11,
};

static const unsigned char SEQ_TE_ON[] = {
	0x35,
	0x00
};

static const unsigned char SEQ_TE_OFF[] = {
	0x34,
};

static const unsigned char SEQ_DISPLAY_ON[] = {
	0x29,
};

static const unsigned char SEQ_DISPLAY_OFF[] = {
	0x28,
};

static const unsigned char SEQ_HBM_OFF[] = {
	0x53,
	0x00
};

static const unsigned char SEQ_HBM_ON[] = {
	0x53,
	0xC0
};

#ifdef CONFIG_DECON_MIC
static const unsigned char SEQ_SINGLE_DSI_SET1[] = {
	0xF2,
	0x67
};

static const unsigned char SEQ_SINGLE_DSI_SET2[] = {
	0xF9,
	0x09,
};
#else
static const unsigned char SEQ_SINGLE_DSI_SET1[] = {
	0xF2,
	0x63,
};

static const unsigned char SEQ_SINGLE_DSI_SET2[] = {
	0xF9,
	0x0A,
};
#endif

static const unsigned char SEQ_ACL_OFF[] = {
	0x55,
	0x00
};

static const unsigned char SEQ_ACL_15[] = {
	0x55,
	0x02,
};

static const unsigned char SEQ_ACL_OFF_OPR_AVR[] = {
	0xB5,
	0x40
};

static const unsigned char SEQ_ACL_ON_OPR_AVR[] = {
	0xB5,
	0x50
};

static const unsigned char SEQ_TOUCH_HSYNC_ON[] = {
	0xBD,
	0x11, 0x11, 0x02, 0x16, 0x02, 0x16
};

static const unsigned char SEQ_PENTILE_CONTROL[] = {
	0xC0,
	0x00, 0x00, 0xD8, 0xD8
};

static const unsigned char SEQ_POC_SETTING[] = {
	0xC3,
	0xC0, 0x00, 0x33
};

static const unsigned char SEQ_PCD_SET_OFF[] = {
	0xCC,
	0x40, 0x51
};

static const unsigned char SEQ_ERR_FG_SETTING[] = {
	0xED,
	0x44
};
#if 0
static unsigned char SEQ_PARTIAL_AREA[] = {
	0x30,
	0x00, 0x00, 0x00, 0x00
};
#endif
static const unsigned char SEQ_PARTIAL_MODE_ON[] = {
	0x12,
};

static const unsigned char SEQ_TE_START_SETTING[] = {
	0xB9,
	0x10, 0x09, 0xFF, 0x00, 0x09
};

static const unsigned char SEQ_FREQ_CALIBRATION_SETTING1[] = {
	0xB0,
	0x02
};

static const unsigned char SEQ_FREQ_CALIBRATION_SETTING2[] = {
	0xF2,
	0xC5
};

static const unsigned char SEQ_FREQ_CALIBRATION_SETTING3[] = {
	0xFD,
	0x1C
};

static const unsigned char SEQ_FREQ_CALIBRATION_SETTING4[] = {
	0xB0,
	0x01
};

static const unsigned char SEQ_FREQ_CALIBRATION_SETTING5[] = {
	0xFE,
	0x39
};

static const unsigned char SEQ_FREQ_CALIBRATION_SETTING6[] = {
	0xFE,
	0xA0
};

static const unsigned char SEQ_FREQ_CALIBRATION_SETTING7[] = {
	0xFE,
	0x20
};

static const unsigned char SEQ_FREQ_CALIBRATION_SETTING8[] = {
	0xCE,
	0x03, 0x3B, 0x14, 0x6D
};

static const unsigned char SEQ_FREQ_CALIBRATION_SETTING9[] = {
	0xB0,
	0x0C
};

static const unsigned char SEQ_FREQ_CALIBRATION_SETTING10[] = {
	0xCE,
	0xC5
};

#endif /* __S6E3HF2_PARAM_H__ */
