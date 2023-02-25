/*******************************************************************************
 * Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */

#ifndef _MAX30003_H_
#define _MAX30003_H_

// #include "mbed.h"
#include <string.h>
#include <stdio.h>
#include <zephyr/zephyr.h>

#include "spi_config.h"

#define MAX_BUF_SIZE 4

/**
 * @brief Library for MAX30003\n
 * The MAX30003 is a complete, biopotential, analog frontend solution for
 * wearable applications. It offers high performance for clinical and fitness
 * applications, with ultra-low power for long battery life. The MAX30003 is a
 * single biopotential channel providing ECG waveforms and heart rate detection.
 *
 * @code
 * #include "mbed.h"
 * #include "max32630fthr.h"
 * #include "MAX30003.h"
 *
 * MAX32630FTHR pegasus(MAX32630FTHR::VIO_3V3);
 *
 * int main(void)
 * {
 *     DigitalOut rLed(LED1, 1);
 *     DigitalOut gLed(LED2, 1);
 *
 *     Serial pc(USBTX, USBRX);
 *     pc.baud(115200);
 *
 *     SPI spiBus(SPI2_MOSI, SPI2_MISO, SPI2_SCK);
 *     MAX30003 ecgAfe(spiBus, P5_3);
 *
 *     while(1)
 *     {
 *         gLed = !gLed;
 *         wait(0.1);
 *     }
 * }
 * @endcode
 */

enum Registers_e
{
    NO_OP = 0x00,
    STATUS = 0x01,
    EN_INT = 0x02,
    EN_INT2 = 0x03,
    MNGR_INT = 0x04,
    MNGR_DYN = 0x05,
    SW_RST = 0x08,
    SYNCH = 0x09,
    FIFO_RST = 0x0A,
    INFO = 0x0F,
    CNFG_GEN = 0x10,
    CNFG_ALL = 0x12,
    CNFG_EMUX = 0x14,
    CNFG_ECG = 0x15,
    CNFG_RTOR1 = 0x1D,
    CNFG_RTOR2 = 0x1E,
    ECG_FIFO_BURST = 0x20,
    ECG_FIFO = 0x21,
    RTOR = 0x25,
    NO_OP2 = 0x7F
};

#define STATUS_LOFF_NL 0
#define STATUS_LOFF_NH 1
#define STATUS_LOFF_PL 2
#define STATUS_LOFF_PH 3
#define STATUS_PLLINT 8
#define STATUS_SAMP 9
#define STATUS_RRINT 10
#define STATUS_LONINT 11
#define STATUS_DCLOFFINT 20
#define STATUS_FSTINT 21
#define STATUS_EOVF 22
#define STATUS_EINT 23

#define EN_INT_INTB_TYPE 0
#define EN_INT_EN_PLLINT 8
#define EN_INT_EN_SAMP 9
#define EN_INT_EN_RRINT 10
#define EN_INT_EN_LONINT 11
#define EN_INT_EN_DCL_OFFINT 20
#define EN_INT_EN_FSTINT 21
#define EN_INT_EN_EOVF 22
#define EN_INT_EN_EINT 23

#define MNGR_INT_SAMP_IT 0
#define MNGR_INT_CLR_SAMP 2
#define MNGR_INT_CLR_RRINT 4
#define MNGR_INT_CLR_FAST 6
#define MNGR_INT_EFIT 19

#define MNGR_DYN_FAST_TH 16
#define MNGR_DYN_FAST 22

#define CNFG_GEN_RBIASN 0
#define CNFG_GEN_RBIASP 1
#define CNFG_GEN_RBIASV 2
#define CNFG_GEN_EN_RBIAS 4
#define CNFG_GEN_VTH 6
#define CNFG_GEN_IMAG 8
#define CNFG_GEN_IPOL 3
#define CNFG_GEN_EN_DCLOFF 12
#define CNFG_GEN_EN_ECG 19
#define CNFG_GEN_FMSTR 20
#define CNFG_GEN_EN_ULP_LON 22

#define CNFG_CAL_THIGH 0
#define CNFG_CAL_FIFTY 11
#define CNFG_CAL_FCAL 12
#define CNFG_CAL_VMAG 20
#define CNFG_CAL_VMODE 21
#define CNFG_CAL_EN_VCAL 22

#define CNFG_EMUX_CALN_SEL 16
#define CNFG_EMUX_CALP_SEL 18
#define CNFG_EMUX_OPENN 20
#define CNFG_EMUX_OPENP 21
#define CNFG_EMUX_POL 23

#define CNFG_ECG_DLPF 12
#define CNFG_ECG_DHPF 14
#define CNFG_ECG_GAIN 16
#define CNFG_ECG_RATE 22

#define CNFG_RTOR1_PTSF 8
#define CNFG_RTOR1_PAVG 12
#define CNFG_RTOR1_EN_RTOR 15
#define CNFG_RTOR1_GAIN 16
#define CNFG_RTOR1_WNDW 20

#define CNFG_RTOR2_RHSF 8
#define CNFG_RTOR2_RAVG 12
#define CNFG_RTOR2_HOFF 16

bool max_readinfo(void);

bool max_reset_sw(void);

int max_enable_ecg(void);

int max_readstatus(void);

int setup_interrupt(void);

int max30003_write(uint8_t reg, void *data, size_t size);

int max30003_write_uint32(uint8_t reg, uint32_t data);

int max30003_read(uint8_t reg, void *data, size_t size);

#endif /* _MAX30003_H_ */