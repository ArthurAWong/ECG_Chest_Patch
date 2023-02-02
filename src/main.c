
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
#include "max30003_zephyr.h"

#define BUF_SIZE 4

const struct device *spi_slow;
// uint8_t op_byte = (RTOR << 4) | 1;
uint8_t buffer_tx[BUF_SIZE] = {0x1f, 0x00, 0x00, 0x00};
uint8_t buffer_rx[BUF_SIZE] = {};

uint8_t buffer_print_tx[BUF_SIZE * 5 + 1];
uint8_t buffer_print_rx[BUF_SIZE * 5 + 1];

struct spi_cs_control spi_cs = {
	.gpio = {
		.port = DEVICE_DT_GET(DT_NODELABEL(gpio0)),
		.pin = 28,
		.dt_flags = GPIO_ACTIVE_LOW}, // So this active low is just how the GPIO is configured at the beginning and when it's not being explicitly told what to do
	.delay = 0,
};
// TODO change .cs to use overlay
struct spi_config spi_cfg_slow = {
	.frequency = 8000000,
	.operation = SPI_OP_MODE_MASTER | SPI_MODE_CPOL | SPI_CONFIG_ORDER_MsbFirst |
				 SPI_MODE_CPHA | SPI_WORD_SET(8) | SPI_LINES_SINGLE,
	.slave = 0,
	.cs = (&spi_cs),
};

/*Number inside the [] Tells us how many buffers*/
/*.buf is the type of buffer*/
/*.len is the length of the buffer*/
struct spi_buf tx_bufs[1] =
	{
		{.buf = buffer_tx,
		 .len = BUF_SIZE},
};

/*Number inside the [] Tells us how many buffers*/
/*.buf is the type of buffer*/
/*.len is the length of the buffer*/
struct spi_buf rx_bufs[1] =
	{
		{.buf = buffer_rx,
		 .len = BUF_SIZE}};

// Buffer definitions
const struct spi_buf_set tx = {
	.buffers = tx_bufs,
	.count = ARRAY_SIZE(tx_bufs)};
const struct spi_buf_set rx = {
	.buffers = rx_bufs,
	.count = ARRAY_SIZE(rx_bufs)};

static void to_display_format(const uint8_t *src, size_t size, char *dst)
{
	size_t i;

	for (i = 0; i < size; i++)
	{
		sprintf(dst + 5 * i, "0x%02x,", src[i]);
	}
}

bool max_readinfo()
{
	printk("Shifting Registers!\n");
	// *buffer_tx = {0x1f, 0x00, 0x00, 0x00}; // Shift and make the last bit 1 to indicate read mode in SPI

	int ret = spi_transceive(spi_slow, &spi_cfg_slow, &tx, &rx);

	to_display_format(buffer_tx, BUF_SIZE, buffer_print_tx);
	printk("Sending:   %s\n", buffer_print_tx);

	to_display_format(buffer_rx, BUF_SIZE, buffer_print_rx);
	printk("Recieving: %s\n", buffer_print_rx);
	printk("spi_transcieve returns %d \n", ret);

	// First check if the transieve was successful
	if (ret != false)
	{
		printk("Device Failed to transceive\n");
		return false;
	}

	if ((buffer_rx[1] & 0xF0) == 0x50)
	{
		printk("MAX is ready!\n");
		printk("Dev ID: ");
		printk("%x", buffer_rx[1] & 0xFF);
		return true;
	}
	else
	{
		printk("MAX read info error! Check wiring!\n");
		return false;
	}
}

void main(void)
{
	int ret;

	spi_slow = DEVICE_DT_GET(DT_NODELABEL(spi1));

	if (!spi_slow)
	{
		printk("Cannot find SPI_1\n");
		printk("Invalid SPI device");
		return;
	}
	// printk("SPI test slow config\n");
	// ret = spi_transceive(spi_slow, &spi_cfg_slow, &tx, &rx);
	// if (ret)
	// {
	// 	printk("Code %d\n", ret);
	// 	printk("SPI transceive failed\n");
	// 	return;
	// }

	printk("Getting device ID...\n");
	// gpio_pin_set(spi_cs.gpio.port, spi_cs.gpio.pin, GPIO_ACTIVE_HIGH);

	max_readinfo();

	// gpio_pin_set("GPIO0", 28, GPIO_ACTIVE_HIGH);
	// k_sleep(K_MSEC(5000));

	// if (memcmp(buffer_tx, buffer_rx, BUF_SIZE))
	// {
	// 	to_display_format(buffer_tx, BUF_SIZE, buffer_print_tx);
	// 	to_display_format(buffer_rx, BUF_SIZE, buffer_print_rx);
	// 	printk("Buffer contents are different: %s\n", buffer_print_tx);
	// 	printk("                           vs: %s\n", buffer_print_rx);
	// 	printk("Buffer contents are different\n");
	// 	goto end;
	// }

	// printk("spi_complete_loop Passed\n");
	// end:;
}
