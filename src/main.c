/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <string.h>
#include <stdio.h>
#include <zephyr/zephyr.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>

#define STACK_SIZE 512
#define BUF_SIZE 17

uint8_t buffer_tx[] = "0123456789abcdef\0";
uint8_t buffer_rx[BUF_SIZE] = {};

uint8_t buffer_print_tx[BUF_SIZE * 5 + 1];
uint8_t buffer_print_rx[BUF_SIZE * 5 + 1];

struct spi_cs_control spi_cs = {
	.gpio = {
		.port = DEVICE_DT_GET(DT_NODELABEL(gpio0)),
		.pin = 28,
		.dt_flags = GPIO_ACTIVE_LOW
	},
	.delay = 0,
};

struct spi_config spi_cfg_slow = {
	.frequency = 500000,
	.operation = SPI_OP_MODE_MASTER | SPI_MODE_CPOL |
	SPI_MODE_CPHA | SPI_WORD_SET(8) | SPI_LINES_SINGLE,
	.slave = 0,
	.cs = (&spi_cs),
};

struct spi_buf tx_bufs[1];
struct spi_buf rx_bufs[1];

const struct spi_buf_set tx = {
	.buffers = tx_bufs,
	.count = ARRAY_SIZE(tx_bufs)
};
const struct spi_buf_set rx = {
	.buffers = rx_bufs,
	.count = ARRAY_SIZE(rx_bufs)
};

static void to_display_format(const uint8_t *src, size_t size, char *dst)
{
	size_t i;

	for (i = 0; i < size; i++) {
		sprintf(dst + 5 * i, "0x%02x,", src[i]);
	}
}

void main(void)
{
	int ret;
	const struct device *spi_slow;
	spi_slow = DEVICE_DT_GET(DT_NODELABEL(spi1));
	if (!spi_slow) {
		printk("Cannot find SPI_1\n");
		printk("Invalid SPI device");
		return;
	}
	printk("SPI test slow config\n");
	ret = spi_transceive(spi_slow, &spi_cfg_slow, &tx, &rx);
	if (ret) {
		printk("Code %d\n", ret);
		printk("SPI transceive failed\n");
		return;
	}

	if (memcmp(buffer_tx, buffer_rx, BUF_SIZE)) {
		to_display_format(buffer_tx, BUF_SIZE, buffer_print_tx);
		to_display_format(buffer_rx, BUF_SIZE, buffer_print_rx);
		printk("Buffer contents are different: %s\n", buffer_print_tx);
		printk("                           vs: %s\n", buffer_print_rx);
		printk("Buffer contents are different\n");
	}

	printk("spi_complete_loop Passed\n");

	printk("Hello World! %s\n", CONFIG_BOARD);
}
