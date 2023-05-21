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

// Verifies successful wiring/SPI configuration.
bool lsm6dsm_read_who_am_i()
{
    uint8_t who_am_i = 0;
	lsm6dsm_read_buffer(LSM6DSM_WHO_AM_I, &who_am_i);
	if (who_am_i == 0x6a)
	{
		printk("Accelerometer WHO_AM_I successful!\n");
		return true;
	} else 
	{
		printk("Accelerometer WHO_AM_I failed!\n");
		return false;
	}

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

int lsm6dsm_enable_accel()
{
	int ret;
	ret = lsm6dsm_write_buffer(LSM6DSM_CTRL1_XL, 0x10); // Enable accelerometer to produce an output data rate of 12.5 Hz
	if (!ret)
	{
		return -1;
	}
	return 0;
}

int lsm6dsm_enable_gyro()
{
	int ret;
	ret = lsm6dsm_write_buffer(LSM6DSM_CTRL2_G, 0x10); // Enable gyrometer to produce an output data rate of 12.5 Hz
	if (!ret)
	{
		return -1;
	}
	return 0;
}

int lsm6dsm_read_status(uint8_t *status_data)
{
	int ret;
	ret = lsm6dsm_read_buffer(LSM6DSM_STATUS_REG, status_data);
	return ret;
}

int lsm6dsm_reset_sw(void)
{
	int ret = lsm6dsm_write_buffer(LSM6DSM_CTRL3_C, 0x01);
	return ret;
}

int lsm6dsm_read_accel(int16_t *out_xyz)
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
		out_xyz[0] = (raw_outx_xl); //*0.061/1000*9.81
		out_xyz[1] = (raw_outy_xl);
		out_xyz[2] = (raw_outz_xl);
	}
	else
	{
		return 1;
	};
	return 0;
}

int lsm6dsm_read_gyro(int16_t *out_xyz)
{
	int ret;
	uint8_t out_var = 0;
	uint8_t outx_g[2] = {}; // element 0 is LSB, element 1 is MSB
	uint8_t outy_g[2] = {};
	uint8_t outz_g[2] = {};
	{
		ret = lsm6dsm_read_buffer(LSM6DSM_OUTX_L_G, &outx_g[0]);
		ret = lsm6dsm_read_buffer(LSM6DSM_OUTX_H_G, &outx_g[1]);
		ret = lsm6dsm_read_buffer(LSM6DSM_OUTY_L_G, &outy_g[0]);
		ret = lsm6dsm_read_buffer(LSM6DSM_OUTY_H_G, &outy_g[1]);
		ret = lsm6dsm_read_buffer(LSM6DSM_OUTZ_L_G, &outz_g[0]);
		ret = lsm6dsm_read_buffer(LSM6DSM_OUTZ_H_G, &outz_g[1]);
		int16_t raw_outx_g = outx_g[0] | (outx_g[1] << 8);
		if (!(raw_outx_g < 32768))
		{
			raw_outx_g -= 65536;
		}
		int16_t raw_outy_g = outy_g[0] | (outy_g[1] << 8);
		if (!(raw_outy_g < 32768))
		{
			raw_outy_g -= 65536;
		}
		int16_t raw_outz_g = outz_g[0] | (outz_g[1] << 8);
		if (!(raw_outz_g < 32768))
		{
			raw_outz_g -= 65536;
		}
		/*
		8.75 is the conversion factor between signed bits and degrees-per-second. It is defined in 
		pg 23 of the datasheet. 1/1000 is the converstion from mili to base.
		*/
		out_xyz[0] = (raw_outx_g); //*8.75/1000
		out_xyz[1] = (raw_outy_g);
		out_xyz[2] = (raw_outz_g);
	}
	return ret;
}