/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "lsm6dsm.h"

void main(void)
{
	lsm6dsm_write_buffer(0x04, 0x05);
	uint8_t out_var = 0;

	printk("%d\n", out_var);
	int ret = lsm6dsm_read_buffer(0x04, &out_var);
	printk("%d\n", out_var);
}
