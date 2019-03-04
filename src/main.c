#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <u8x8.h>

/* Wait a bit - the lazy version */
static void delay(int n) {
	for(int i = 0; i < n; i++)
		__asm__("nop");
}

/* Initialize I2C1 interface */
static void i2c_setup(void) {
	/* Enable GPIOB and I2C1 clocks */
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_AFIO);
	rcc_periph_clock_enable(RCC_I2C1);

	/* Set alternate functions for SCL and SDA pins of I2C1 */
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
				  GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
				  GPIO_I2C1_SCL | GPIO_I2C1_SDA);

	/* Disable the I2C peripheral before configuration */
	i2c_peripheral_disable(I2C1);

	/* APB1 running at 36MHz */
	i2c_set_clock_frequency(I2C1, I2C_CR2_FREQ_36MHZ);

	/* 400kHz - I2C fast mode */
	i2c_set_fast_mode(I2C2);
	i2c_set_ccr(I2C1, 0x1e);
	i2c_set_trise(I2C1, 0x0b);

	/* And go */
	i2c_peripheral_enable(I2C1);
}

static uint8_t u8x8_gpio_and_delay_cm3(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
	switch(msg) {
	case U8X8_MSG_GPIO_AND_DELAY_INIT:
		i2c_setup();  /* Init I2C communication */
		break;

	default:
		u8x8_SetGPIOResult(u8x8, 1);
		break;
	}

	return 1;
}

/* I2C hardware transfer based on u8x8_byte.c implementation */
static uint8_t u8x8_byte_hw_i2c_cm3(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
	static uint8_t buffer[32];   /* u8g2/u8x8 will never send more than 32 bytes */
	static uint8_t buf_idx;
	uint8_t *data;

	switch(msg) {
	case U8X8_MSG_BYTE_SEND:
		data = (uint8_t *)arg_ptr;
		while(arg_int > 0) {
			buffer[buf_idx++] = *data;
			data++;
			arg_int--;
		}
		break;
	case U8X8_MSG_BYTE_INIT:
		break;
	case U8X8_MSG_BYTE_SET_DC:
		break;
	case U8X8_MSG_BYTE_START_TRANSFER:
		buf_idx = 0;
		break;
	case U8X8_MSG_BYTE_END_TRANSFER:
		i2c_transfer7(I2C1, 0x3C, buffer, buf_idx, NULL, 0);
		break;
	default:
		return 0;
	}
	return 1;
}

int main(void) {
	u8x8_t u8x8_i, *u8x8 = &u8x8_i;

	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_clock_setup_in_hse_8mhz_out_72mhz();

	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

	u8x8_Setup(u8x8, u8x8_d_ssd1306_128x64_noname, u8x8_cad_ssd13xx_fast_i2c, u8x8_byte_hw_i2c_cm3, u8x8_gpio_and_delay_cm3);

	u8x8_InitDisplay(u8x8);
	u8x8_SetPowerSave(u8x8,0);
	u8x8_SetFont(u8x8, u8x8_font_7x14B_1x2_f);

	u8x8_ClearDisplay(u8x8);
	u8x8_DrawString(u8x8, 1,1, "Hello!!!");
	u8x8_Draw2x2Glyph(u8x8, 0,0, 'H');
	u8x8_SetInverseFont(u8x8, 1);
	u8x8_DrawString(u8x8, 0,5, "Roel says hi!  ");
	u8x8_SetInverseFont(u8x8, 0);

	u8x8_SetFont(u8x8, u8x8_font_open_iconic_embedded_2x2);
	u8x8_DrawGlyph(u8x8, 11,1, 65); /* Bell */

	while(true) {
		gpio_toggle(GPIOC, GPIO13);

		delay(8000000);
	}
	return 0;
}

