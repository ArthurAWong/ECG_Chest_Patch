#include <zephyr/drivers/spi.h>
#include "utils.h"
#include "spi_config.h"

#include "max30003.h"

uint8_t buffer_tx[BUF_SIZE] = {0x1f, 0x00, 0x00, 0x00};
uint8_t buffer_rx[BUF_SIZE] = {};

uint8_t buffer_print_tx[BUF_SIZE * 5 + 1];
uint8_t buffer_print_rx[BUF_SIZE * 5 + 1];

/*Number inside the [] Tells us how many buffers*/
/*.buf is the type of buffer*/
/*.len is the length of the buffer*/
struct spi_buf tx_bufs[1] =
{
    {
        .buf = buffer_tx,
        .len = BUF_SIZE
    },
};

struct spi_buf rx_bufs[1] =
{
    {
        .buf = buffer_rx,
        .len = BUF_SIZE
    }
};

const struct spi_buf_set tx = 
{
	.buffers = tx_bufs,
	.count = ARRAY_SIZE(tx_bufs)
};

const struct spi_buf_set rx = 
{
	.buffers = rx_bufs,
	.count = ARRAY_SIZE(rx_bufs)
};

bool max_readinfo()
{
	// printk("Shifting Registers!\n");
	// *buffer_tx = {0x1f, 0x00, 0x00, 0x00}; // Shift and make the last bit 1 to indicate read mode in SPI

	to_display_format(buffer_tx, BUF_SIZE, buffer_print_tx);
	printk("Sending:   %s\n", buffer_print_tx);

	int ret = spi_transceive(spi_device, &spi_cfg, &tx, &rx);

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