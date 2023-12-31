#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <string.h>
#include <stdio.h>
#include <zephyr/zephyr.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>

#include "spi_config.h"


// Setting up which pins is responsible for MOSI, MISO, CLK, and SCK is done in the device tree or overlay file (nrf52dk_nrf52832.overlay).

struct spi_cs_control max_spi_cs =
{
	.gpio = 
	{
		.port = DEVICE_DT_GET(DT_NODELABEL(gpio0)),
		.pin = 11,
		.dt_flags = GPIO_ACTIVE_LOW
	},
	.delay = 0
};

struct spi_cs_control accel_spi_cs =
{
	.gpio = 
	{
		.port = DEVICE_DT_GET(DT_NODELABEL(gpio0)),
		.pin = 12,
		.dt_flags = GPIO_ACTIVE_LOW
	},
	.delay = 0
};

struct spi_config max_spi_cfg = 
{
	.frequency = 125000,
	.operation = SPI_OP_MODE_MASTER | SPI_MODE_CPOL | SPI_CONFIG_ORDER_MsbFirst |
                SPI_MODE_CPHA | SPI_WORD_SET(8) | SPI_LINES_SINGLE,
	.slave = 0,
	.cs = (&max_spi_cs)
};


struct spi_config accel_spi_cfg = 
{
	.frequency = 125000,
	.operation = SPI_OP_MODE_MASTER | SPI_MODE_CPOL | SPI_CONFIG_ORDER_MsbFirst |
                SPI_MODE_CPHA | SPI_WORD_SET(8) | SPI_LINES_SINGLE,
	.slave = 0,
	.cs = (&accel_spi_cs)
};

const struct device *spi_device = DEVICE_DT_GET(DT_NODELABEL(spi1));