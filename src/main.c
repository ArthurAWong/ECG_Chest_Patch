
/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "max30003.h"

void main(void)
{	
	//setup_interrupt();
	max_reset_sw();
	max_readinfo();
	max_readstatus();
	max_enable_ecg();
}
