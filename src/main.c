/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/sys/printk.h>
#include <string.h>
#include <stdio.h>
#include <zephyr/zephyr.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>

struct spi_cs_control spi_cs = {
	.gpio = {
		.port = NULL,
		.pin = 28,
		.dt_flags = GPIO_ACTIVE_LOW
	},
	.delay = 0,
};

#define STACK_SIZE 512
#define BUF_SIZE 17
#define BUF2_SIZE 36

uint8_t buffer_tx[] = "0123456789abcdef\0";
uint8_t buffer_rx[BUF_SIZE] = {};

uint8_t buffer2_tx[] = "Thequickbrownfoxjumpsoverthelazydog\0";
uint8_t buffer2_rx[BUF2_SIZE] = {};

uint8_t buffer_print_tx[BUF_SIZE * 5 + 1];
uint8_t buffer_print_rx[BUF_SIZE * 5 + 1];

uint8_t buffer_print_tx2[BUF2_SIZE * 5 + 1];
uint8_t buffer_print_rx2[BUF2_SIZE * 5 + 1];

static void to_display_format(const uint8_t *src, size_t size, char *dst)
{
	size_t i;

	for (i = 0; i < size; i++) {
		sprintf(dst + 5 * i, "0x%02x,", src[i]);
	}
}

struct spi_config spi_cfg_slow = {
	.frequency = 500000,
	.operation = SPI_OP_MODE_MASTER | SPI_MODE_CPOL |
	SPI_MODE_CPHA | SPI_WORD_SET(8) | SPI_LINES_SINGLE,
	.slave = 0,
	.cs = (&spi_cs),
};

struct spi_config spi_cfg_fast = {
	.frequency = 16000000,
	.operation = SPI_OP_MODE_MASTER | SPI_MODE_CPOL |
	SPI_MODE_CPHA | SPI_WORD_SET(8) | SPI_LINES_SINGLE,
	.slave = 0,
	.cs = (&spi_cs),
};

static int cs_ctrl_gpio_config(void)
{
	spi_cs.gpio.port = DEVICE_DT_GET(DT_NODELABEL(gpio0));
	if (!spi_cs.gpio.port) {
		printk("Invalid gpio device");
		return -1;
	}

	return 0;
}

static int spi_complete_multiple(const struct device *dev,
				 struct spi_config *spi_conf)
{
	struct spi_buf tx_bufs[2];
	const struct spi_buf_set tx = {
		.buffers = tx_bufs,
		.count = ARRAY_SIZE(tx_bufs)
	};

	tx_bufs[0].buf = buffer_tx;
	tx_bufs[0].len = BUF_SIZE;

	tx_bufs[1].buf = buffer2_tx;
	tx_bufs[1].len = BUF2_SIZE;


	struct spi_buf rx_bufs[2];
	const struct spi_buf_set rx = {
		.buffers = rx_bufs,
		.count = ARRAY_SIZE(rx_bufs)
	};

	rx_bufs[0].buf = buffer_rx;
	rx_bufs[0].len = BUF_SIZE;

	rx_bufs[1].buf = buffer2_rx;
	rx_bufs[1].len = BUF2_SIZE;

	int ret;

	printk("Start complete multiple\n");

	printk("Before:\n");
	printk("tx.bufs[0]: %s\n", (char*) tx_bufs[0].buf);
	printk("tx.bufs[1]: %s\n", (char*) tx_bufs[1].buf);
	printk("rx.bufs[0]: %s\n", (char*) rx_bufs[0].buf);
	printk("rx.bufs[1]: %s\n", (char*) rx_bufs[1].buf);

	ret = spi_transceive(dev, spi_conf, &tx, &rx);

	printk("Before:\n");
	printk("tx.bufs[0]: %s\n", (char*) tx_bufs[0].buf);
	printk("tx.bufs[1]: %s\n", (char*) tx_bufs[1].buf);
	printk("rx.bufs[0]: %s\n", (char*) rx_bufs[0].buf);
	printk("rx.bufs[1]: %s\n", (char*) rx_bufs[1].buf);

	if (ret) {
		printk("Code %d\n", ret);
		printk("SPI transceive failed\n");
		return ret;
	}

	if (memcmp(buffer_tx, buffer_rx, BUF_SIZE)) {
		to_display_format(buffer_tx, BUF_SIZE, buffer_print_tx);
		to_display_format(buffer_rx, BUF_SIZE, buffer_print_rx);
		printk("Buffer contents are different: %s\n", buffer_print_tx);
		printk("                           vs: %s\n", buffer_print_rx);
		printk("Buffer contents are different\n");
		return -1;
	}

	if (memcmp(buffer2_tx, buffer2_rx, BUF2_SIZE)) {
		to_display_format(buffer2_tx, BUF2_SIZE, buffer_print_tx2);
		to_display_format(buffer2_rx, BUF2_SIZE, buffer_print_rx2);
		printk("Buffer 2 contents are different: %s\n", buffer_print_tx2);
		printk("                             vs: %s\n", buffer_print_rx2);
		printk("Buffer 2 contents are different\n");
		return -1;
	}

	printk("spi_complete_multiple Passed\n");

	return 0;
}

static int spi_complete_loop(const struct device *dev,
			     struct spi_config *spi_conf)
{
	const struct spi_buf tx_bufs[] = {
		{
			.buf = buffer_tx,
			.len = BUF_SIZE,
		},
	};
	const struct spi_buf rx_bufs[] = {
		{
			.buf = buffer_rx,
			.len = BUF_SIZE,
		},
	};
	const struct spi_buf_set tx = {
		.buffers = tx_bufs,
		.count = ARRAY_SIZE(tx_bufs)
	};
	const struct spi_buf_set rx = {
		.buffers = rx_bufs,
		.count = ARRAY_SIZE(rx_bufs)
	};

	int ret;

	printk("Start complete loop\n");

	ret = spi_transceive(dev, spi_conf, &tx, &rx);
	if (ret) {
		printk("Code %d\n", ret);
		printk("SPI transceive failed\n");
		return ret;
	}

	if (memcmp(buffer_tx, buffer_rx, BUF_SIZE)) {
		to_display_format(buffer_tx, BUF_SIZE, buffer_print_tx);
		to_display_format(buffer_rx, BUF_SIZE, buffer_print_rx);
		printk("Buffer contents are different: %s\n", buffer_print_tx);
		printk("                           vs: %s\n", buffer_print_rx);
		printk("Buffer contents are different\n");
		return -1;
	}

	printk("spi_complete_loop Passed\n");

	return 0;
}

static int spi_null_tx_buf(const struct device *dev,
			   struct spi_config *spi_conf)
{
	static const uint8_t EXPECTED_NOP_RETURN_BUF[BUF_SIZE] = { 0 };

	(void)memset(buffer_rx, 0x77, BUF_SIZE);

	const struct spi_buf tx_bufs[] = {
		/* According to documentation, when sending NULL tx buf -
		 *  NOP frames should be sent on MOSI line
		 */
		{
			.buf = NULL,
			.len = BUF_SIZE,
		},
	};
	const struct spi_buf rx_bufs[] = {
		{
			.buf = buffer_rx,
			.len = BUF_SIZE,
		},
	};
	const struct spi_buf_set tx = {
		.buffers = tx_bufs,
		.count = ARRAY_SIZE(tx_bufs)
	};
	const struct spi_buf_set rx = {
		.buffers = rx_bufs,
		.count = ARRAY_SIZE(rx_bufs)
	};

	int ret;

	printk("Start null tx\n");

	ret = spi_transceive(dev, spi_conf, &tx, &rx);
	if (ret) {
		printk("Code %d\n", ret);
		printk("SPI transceive failed\n");
		return ret;
	}


	if (memcmp(buffer_rx, EXPECTED_NOP_RETURN_BUF, BUF_SIZE)) {
		to_display_format(buffer_rx, BUF_SIZE, buffer_print_rx);
		printk("Rx Buffer should contain NOP frames but got: %s\n",
			buffer_print_rx);
		printk("Buffer not as expected\n");
		return -1;
	}

	printk("spi_null_tx_buf Passed\n");

	return 0;
}

static int spi_rx_half_start(const struct device *dev,
			     struct spi_config *spi_conf)
{
	const struct spi_buf tx_bufs[] = {
		{
			.buf = buffer_tx,
			.len = BUF_SIZE,
		},
	};
	const struct spi_buf rx_bufs[] = {
		{
			.buf = buffer_rx,
			.len = 8,
		},
	};
	const struct spi_buf_set tx = {
		.buffers = tx_bufs,
		.count = ARRAY_SIZE(tx_bufs)
	};
	const struct spi_buf_set rx = {
		.buffers = rx_bufs,
		.count = ARRAY_SIZE(rx_bufs)
	};
	int ret;

	printk("Start half start\n");

	(void)memset(buffer_rx, 0, BUF_SIZE);

	ret = spi_transceive(dev, spi_conf, &tx, &rx);
	if (ret) {
		printk("Code %d\n", ret);
		printk("SPI transceive failed\n");
		return -1;
	}

	if (memcmp(buffer_tx, buffer_rx, 8)) {
		to_display_format(buffer_tx, 8, buffer_print_tx);
		to_display_format(buffer_rx, 8, buffer_print_rx);
		printk("Buffer contents are different: %s\n", buffer_print_tx);
		printk("                           vs: %s\n", buffer_print_rx);
		printk("Buffer contents are different\n");
		return -1;
	}

	printk("spi_rx_half_start Passed\n");

	return 0;
}

static int spi_rx_half_end(const struct device *dev,
			   struct spi_config *spi_conf)
{
	const struct spi_buf tx_bufs[] = {
		{
			.buf = buffer_tx,
			.len = BUF_SIZE,
		},
	};
	const struct spi_buf rx_bufs[] = {
		{
			.buf = NULL,
			.len = 8,
		},
		{
			.buf = buffer_rx,
			.len = 8,
		},
	};
	const struct spi_buf_set tx = {
		.buffers = tx_bufs,
		.count = ARRAY_SIZE(tx_bufs)
	};
	const struct spi_buf_set rx = {
		.buffers = rx_bufs,
		.count = ARRAY_SIZE(rx_bufs)
	};
	int ret;

	if (IS_ENABLED(CONFIG_SPI_STM32_DMA)) {
		printk("Skip half end\n");
		return 0;
	}

	printk("Start half end\n");

	(void)memset(buffer_rx, 0, BUF_SIZE);

	ret = spi_transceive(dev, spi_conf, &tx, &rx);
	if (ret) {
		printk("Code %d\n", ret);
		printk("SPI transceive failed\n");
		return -1;
	}

	if (memcmp(buffer_tx + 8, buffer_rx, 8)) {
		to_display_format(buffer_tx + 8, 8, buffer_print_tx);
		to_display_format(buffer_rx, 8, buffer_print_rx);
		printk("Buffer contents are different: %s\n", buffer_print_tx);
		printk("                           vs: %s\n", buffer_print_rx);
		printk("Buffer contents are different\n");
		return -1;
	}

	printk("spi_rx_half_end Passed\n");

	return 0;
}

static int spi_rx_every_4(const struct device *dev,
			  struct spi_config *spi_conf)
{
	const struct spi_buf tx_bufs[] = {
		{
			.buf = buffer_tx,
			.len = BUF_SIZE,
		},
	};
	const struct spi_buf rx_bufs[] = {
		{
			.buf = NULL,
			.len = 4,
		},
		{
			.buf = buffer_rx,
			.len = 4,
		},
		{
			.buf = NULL,
			.len = 4,
		},
		{
			.buf = buffer_rx + 4,
			.len = 4,
		},
	};
	const struct spi_buf_set tx = {
		.buffers = tx_bufs,
		.count = ARRAY_SIZE(tx_bufs)
	};
	const struct spi_buf_set rx = {
		.buffers = rx_bufs,
		.count = ARRAY_SIZE(rx_bufs)
	};
	int ret;

	if (IS_ENABLED(CONFIG_SPI_STM32_DMA)) {
		printk("Skip every 4\n");
		return 0;
	}

	if (IS_ENABLED(CONFIG_DSPI_MCUX_EDMA)) {
		printk("Skip every 4\n");
		return 0;
	}

	printk("Start every 4\n");

	(void)memset(buffer_rx, 0, BUF_SIZE);

	ret = spi_transceive(dev, spi_conf, &tx, &rx);
	if (ret) {
		printk("Code %d\n", ret);
		printk("SPI transceive failed\n");
		return -1;
	}

	if (memcmp(buffer_tx + 4, buffer_rx, 4)) {
		to_display_format(buffer_tx + 4, 4, buffer_print_tx);
		to_display_format(buffer_rx, 4, buffer_print_rx);
		printk("Buffer contents are different: %s\n", buffer_print_tx);
		printk("                           vs: %s\n", buffer_print_rx);
		printk("Buffer contents are different\n");
		return -1;
	} else if (memcmp(buffer_tx + 12, buffer_rx + 4, 4)) {
		to_display_format(buffer_tx + 12, 4, buffer_print_tx);
		to_display_format(buffer_rx + 4, 4, buffer_print_rx);
		printk("Buffer contents are different: %s\n", buffer_print_tx);
		printk("                           vs: %s\n", buffer_print_rx);
		printk("Buffer contents are different\n");
		return -1;
	}

	printk("spi_rx_every_4 Passed\n");

	return 0;
}

static int spi_resource_lock_test(const struct device *lock_dev,
				  struct spi_config *spi_conf_lock,
				  const struct device *try_dev,
				  struct spi_config *spi_conf_try)
{
	spi_conf_lock->operation |= SPI_LOCK_ON;

	if (spi_complete_loop(lock_dev, spi_conf_lock)) {
		return -1;
	}

	if (spi_release(lock_dev, spi_conf_lock)) {
		printk("Deadlock now?\n");
		printk("SPI release failed\n");
		return -1;
	}

	if (spi_complete_loop(try_dev, spi_conf_try)) {
		return -1;
	}

	return 0;
}

void main(void)
{
	const struct device *spi_slow;
	const struct device *spi_fast;

	printk("SPI test on buffers TX/RX %p/%p\n", buffer_tx, buffer_rx);
	if (cs_ctrl_gpio_config()) {
		return;
	}

	spi_slow = DEVICE_DT_GET(DT_NODELABEL(spi1));
	if (!spi_slow) {
		printk("Cannot find SPI_1\n");
		printk("Invalid SPI device");
		return;
	}

	spi_fast = spi_slow;

	printk("SPI test slow config\n");
	if (spi_complete_multiple(spi_slow, &spi_cfg_slow))
	    //spi_complete_loop(spi_slow, &spi_cfg_slow)
	    // spi_null_tx_buf(spi_slow, &spi_cfg_slow) ||
	    // spi_rx_half_start(spi_slow, &spi_cfg_slow) ||
	    // spi_rx_half_end(spi_slow, &spi_cfg_slow) ||
	    // spi_rx_every_4(spi_slow, &spi_cfg_slow)) 
	{
		goto end;
	}

	if (spi_complete_multiple(spi_fast, &spi_cfg_fast))
	    // spi_complete_loop(spi_fast, &spi_cfg_fast)
	    // spi_null_tx_buf(spi_fast, &spi_cfg_fast) ||
	    // spi_rx_half_start(spi_fast, &spi_cfg_fast) ||
	    // spi_rx_half_end(spi_fast, &spi_cfg_fast) ||
	    // spi_rx_every_4(spi_fast, &spi_cfg_fast))
	{
		goto end;
	}

	if (spi_resource_lock_test(spi_slow, &spi_cfg_slow,
				   spi_fast, &spi_cfg_fast)) {
		goto end;
	}

	printk("All tx/rx passed");

	end:;
}
