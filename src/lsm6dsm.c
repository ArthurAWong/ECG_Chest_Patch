#include <string.h>
#include <stdio.h>
#include <zephyr/zephyr.h>

#include "utils.h"
#include "spi_config.h"
#include "lsm6dsm.h"


#define WRITE_BIT_SET(reg) (reg & ~(1 << 7))
#define READ_BIT_SET(reg) (reg | (1 << 7))


uint8_t buffer_tx[BUF_SIZE] = {0x00, 0x00};
uint8_t buffer_rx[BUF_SIZE] = {0x00, 0x00};

uint8_t buffer_print_tx[BUF_SIZE * 5 + 1];
uint8_t buffer_print_rx[BUF_SIZE * 5 + 1];

struct spi_buf tx_bufs[1] = {
	{
		.buf = buffer_tx,
		.len = BUF_SIZE
	}
};
struct spi_buf rx_bufs[1] = 
{
	{
		.buf = buffer_rx,
		.len = BUF_SIZE
	}
};
const struct spi_buf_set tx = {
	.buffers = tx_bufs,
	.count = ARRAY_SIZE(tx_bufs)
};
const struct spi_buf_set rx = {
	.buffers = rx_bufs,
	.count = ARRAY_SIZE(rx_bufs)
};

void lsm6dsm_test()
{
    to_display_format(buffer_tx, BUF_SIZE, buffer_print_tx);
	printk("Sending:  %s\n", buffer_print_tx);
	int ret = spi_transceive(spi_device, &spi_cfg, &tx, &rx);
	
	if (ret) {
		printk("Code %d\n", ret);
		printk("SPI transceive failed\n");
		return;
	}

	to_display_format(buffer_rx, BUF_SIZE, buffer_print_rx);
	printk("Recieved: %s\n",buffer_print_rx);
}

int lsm6dsm_write_buffer(uint8_t reg, uint8_t data)
{
    uint8_t reg_write = WRITE_BIT_SET(reg);
    buffer_tx[0] = reg_write;
    buffer_tx[1] = data;
    return spi_transceive(spi_device, &spi_cfg, &tx, NULL);
}

int lsm6dsm_read_buffer(uint8_t reg, uint8_t *out_data)
{
    buffer_tx[0] = READ_BIT_SET(reg);
    int ret = spi_transceive(spi_device, &spi_cfg, &tx, &rx);
    out_data[0] = buffer_rx[1];

    return ret;
}