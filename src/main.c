
/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "max30003.h"
#include "lsm6dsm.h"

void main() 
{
	max_reset_sw();
	max_readinfo();
	max_readstatus();

	int ret;
	float xyz[3] = {};
	uint8_t ecg_num;
	lsm6dsm_write_buffer(LSM6DSM_CTRL1_XL, 0x10); // Enable accelerometer to produce an output data rate of 12.5 Hz
	max_enable_ecg();
	while(1)
	{
		ret = lsm6dsm_read_accel(xyz);
		if (!ret)
			//printf("x: % 3.3f, y: % 3.3f, z: % 3.3f\n", xyz[0], xyz[1], xyz[2]);
		ret = max_read_ecg(&ecg_num);
		if (!ret)
			printk("%d\n", ecg_num);
	}
}
