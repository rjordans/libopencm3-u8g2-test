
# U8G2 graphics library, grab the U8X8 interfaces
U8G2_SRCS = $(wildcard ../u8g2/csrc/u8x8*.c)

# Add to object files
OBJS += $(U8G2_SRCS:%.c=%.o)

OPENCM3_DIR := ../libopencm3
BINARY = main
LDSCRIPT = stm32f1.ld

CSTD = -std=c99
DEFS += -I../u8g2/csrc

LIBNAME		= opencm3_stm32f1
DEFS		+= -DSTM32F1

FP_FLAGS	?= -msoft-float
ARCH_FLAGS	= -mthumb -mcpu=cortex-m3 $(FP_FLAGS) -mfix-cortex-m3-ldrd

################################################################################
# OpenOCD specific variables

OOCD		?= openocd
OOCD_INTERFACE	?= stlink-v2
OOCD_TARGET	?= stm32f1x

################################################################################
# Black Magic Probe specific variables
# Set the BMP_PORT to a serial port and then BMP is used for flashing
BMP_PORT	?=

################################################################################
# texane/stlink specific variables
STLINK_PORT	?= :4242

include rules.mk
