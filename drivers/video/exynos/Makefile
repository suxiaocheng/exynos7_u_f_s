include $(TOPDIR)/config.mk

LIB := $(obj)libdecon.o

#
#	CONFIG_EXYNOS_DECON (exynos7xxx)
#

#	Driver file configuration
COBJS-y = decon_core.o dsim_drv.o
COBJS-$(CONFIG_CPU_EXYNOS7420) += decon_reg_7420.o dsim_reg_7420.o
COBJS-$(CONFIG_CPU_EXYNOS7580) += decon_reg_7580.o dsim_reg_7580.o


#	LCD file configuration
COBJS-$(CONFIG_DECON_LCD_S6E3HA0) += ./panels/s6e3ha0_mipi_lcd.o ./panels/s6e3ha0_lcd_ctrl.o
COBJS-$(CONFIG_DECON_LCD_S6E3HF2) += ./panels/s6e3hf2_mipi_lcd.o ./panels/s6e3hf2_lcd_ctrl.o
COBJS-$(CONFIG_DECON_LCD_S6E3FA2) += ./panels/s6e3fa2_mipi_lcd.o ./panels/s6e3fa2_lcd_ctrl.o

COBJS   := $(COBJS-y)
SRCS    := $(COBJS:.o=.c)
OBJS    := $(addprefix $(obj),$(COBJS))

all:    $(LIB)

$(LIB): $(obj).depend $(OBJS)
	  $(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
