/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "lsm6dsm.h"

void main(void)
{
	uint8_t out_var = 0;
	int ret;
	lsm6dsm_write_buffer(LSM6DSM_CTRL1_XL, 0x10); // Enable accelerometer to produce an output data rate of 12.5 Hz

	printk("%x\n", out_var);
	ret = lsm6dsm_read_buffer(LSM6DSM_CTRL1_XL, &out_var);
	printk("%x\n", out_var);
	ret = lsm6dsm_read_buffer(LSM6DSM_WHO_AM_I, &out_var);
	printk("%x\n", out_var);
	while(1)
	{
		float xyz[3] = {0};
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
			xyz[0] = ((float)raw_outx_xl)*0.061/1000*9.81;
			xyz[1] = ((float)raw_outy_xl)*0.061/1000*9.81;
			xyz[2] = ((float)raw_outz_xl)*0.061/1000*9.81;
			printf("x: % 3.3f, y: % 3.3f, z: % 3.3f\n", xyz[0], xyz[1], xyz[2]);
		}
	}
}
