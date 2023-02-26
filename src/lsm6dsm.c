#include <string.h>
#include <stdio.h>
#include <zephyr/zephyr.h>

#include "utils.h"
#include "spi_config.h"
#include "lsm6dsm.h"


#define WRITE_BIT_SET(reg) (reg & ~(1 << 7))
#define READ_BIT_SET(reg) (reg | (1 << 7))


uint8_t buffer_tx[BUF_SIZE] = {0x00, 0x00};
uint8_t buffer_rx[BUF_SIZE] = {0x00, 0x00};

uint8_t buffer_print_tx[BUF_SIZE * 5 + 1];
uint8_t buffer_print_rx[BUF_SIZE * 5 + 1];

struct spi_buf tx_bufs[1] = {
	{
		.buf = buffer_tx,
		.len = BUF_SIZE
	}
};
struct spi_buf rx_bufs[1] = 
{
	{
		.buf = buffer_rx,
		.len = BUF_SIZE
	}
};
const struct spi_buf_set tx = {
	.buffers = tx_bufs,
	.count = ARRAY_SIZE(tx_bufs)
};
const struct spi_buf_set rx = {
	.buffers = rx_bufs,
	.count = ARRAY_SIZE(rx_bufs)
};

void lsm6dsm_test()
{
    to_display_format(buffer_tx, BUF_SIZE, buffer_print_tx);
	printk("Sending:  %s\n", buffer_print_tx);
	int ret = spi_transceive(spi_device, &accel_spi_cfg, &tx, &rx);
	
	if (ret) {
		printk("Code %d\n", ret);
		printk("SPI transceive failed\n");
		return;
	}

	to_display_format(buffer_rx, BUF_SIZE, buffer_print_rx);
	printk("Recieved: %s\n",buffer_print_rx);
}

int lsm6dsm_write_buffer(uint8_t reg, uint8_t data)
{
    uint8_t reg_write = WRITE_BIT_SET(reg);
    buffer_tx[0] = reg_write;
    buffer_tx[1] = data;
    return spi_transceive(spi_device, &accel_spi_cfg, &tx, NULL);
}

int lsm6dsm_read_buffer(uint8_t reg, uint8_t *out_data)
{
    buffer_tx[0] = READ_BIT_SET(reg);
    int ret = spi_transceive(spi_device, &accel_spi_cfg, &tx, &rx);
    *out_data = buffer_rx[1];

    return ret;
}

int lsm6dsm_read_accel(float *out_xyz)
{
	uint8_t out_var = 0;
	uint8_t outx_xl[2] = {}; // element 0 is LSB, element 1 is MSB
	uint8_t outy_xl[2] = {};
	uint8_t outz_xl[2] = {};
	lsm6dsm_read_buffer(LSM6DSM_STATUS_REG, &out_var);
	if (out_var & (1<<0))
	{
		lsm6dsm_read_buffer(LSM6DSM_OUTX_L_XL, &outx_xl[0]);
		lsm6dsm_read_buffer(LSM6DSM_OUTX_H_XL, &outx_xl[1]);
		lsm6dsm_read_buffer(LSM6DSM_OUTY_L_XL, &outy_xl[0]);
		lsm6dsm_read_buffer(LSM6DSM_OUTY_H_XL, &outy_xl[1]);
		lsm6dsm_read_buffer(LSM6DSM_OUTZ_L_XL, &outz_xl[0]);
		lsm6dsm_read_buffer(LSM6DSM_OUTZ_H_XL, &outz_xl[1]);
		int16_t raw_outx_xl = outx_xl[0] | (outx_xl[1] << 8);
		if (!(raw_outx_xl < 32768))
		{
			raw_outx_xl -= 65536;
		}
		int16_t raw_outy_xl = outy_xl[0] | (outy_xl[1] << 8);
		if (!(raw_outy_xl < 32768))
		{
			raw_outy_xl -= 65536;
		}
		int16_t raw_outz_xl = outz_xl[0] | (outz_xl[1] << 8);
		if (!(raw_outz_xl < 32768))
		{
			raw_outz_xl -= 65536;
		}
		/*
		0.061 is the conversion factor between signed bits and mili-gravity. It is defined in pg 23 of the  datasheet
		1/1000 is the converstion from mili to base
		9.81 is the converstion from gravity to m/s^2
		*/
		out_xyz[0] = ((float)raw_outx_xl)*0.061/1000*9.81;
		out_xyz[1] = ((float)raw_outy_xl)*0.061/1000*9.81;
		out_xyz[2] = ((float)raw_outz_xl)*0.061/1000*9.81;
	}
	else
	{
		return 1;
	};
	return 0;
}