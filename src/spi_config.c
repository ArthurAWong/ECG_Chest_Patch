#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <string.h>
#include <stdio.h>
#include <zephyr/zephyr.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>

#include "spi_config.h"

struct spi_cs_control spi_cs =
{
	.gpio = 
	{
		.port = DEVICE_DT_GET(DT_NODELABEL(gpio0)),
		.pin = 28,
		.dt_flags = GPIO_ACTIVE_LOW
	},
	.delay = 0
};

struct spi_config spi_cfg = 
{
	.frequency = 800000,
	.operation = SPI_OP_MODE_MASTER | SPI_MODE_CPOL | SPI_CONFIG_ORDER_MsbFirst |
                SPI_MODE_CPHA | SPI_WORD_SET(8) | SPI_LINES_SINGLE,
	.slave = 0,
	.cs = (&spi_cs)
};

const struct device *spi_device = DEVICE_DT_GET(DT_NODELABEL(spi1));