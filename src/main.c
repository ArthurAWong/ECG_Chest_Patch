
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
	// max_readinfo();
	// max_readstatus();

	int ret;
	uint8_t lsm6dsm_status;
	float accel_xyz[3] = {};
	float gyro_xyz[3] = {};
	uint8_t ecg_num;

	lsm6dsm_enable_accel();
	lsm6dsm_enable_gyro();
	max_enable_ecg();
	
	while(1)
	{
		ret = lsm6dsm_read_status(&lsm6dsm_status);
		if (lsm6dsm_status & (1<<0))
		{
			ret = lsm6dsm_read_accel(accel_xyz);
			if (!ret)
			{
				printf("accel_x: % 3.3f, accel_y: % 3.3f, accel_z: % 3.3f\n", accel_xyz[0], accel_xyz[1], accel_xyz[2]);
			}
		}
		if (lsm6dsm_status & (1<<1))
		{
			ret = lsm6dsm_read_accel(gyro_xyz);
			if (!ret)
			{
				printf("gyroX: % 3.3f, gyroY: % 3.3f, gyroZ: % 3.3f\n", gyro_xyz[0], gyro_xyz[1], gyro_xyz[2]);
			}
		}
		ret = max_read_ecg(&ecg_num);
		if (!ret)
			printk("%d\n", ecg_num);
	}
}
