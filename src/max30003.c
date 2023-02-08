#include <zephyr/drivers/spi.h>
#include "utils.h"
#include "spi_config.h"

#include "max30003.h"


#define WRITE_REG(reg) ((reg << 1) & ~(1 << 0))
#define READ_REG(reg) ((reg << 1) | (1 << 0))

uint8_t buffer_tx[MAX_BUF_SIZE] = {};
uint8_t buffer_rx[MAX_BUF_SIZE] = {};

uint8_t buffer_print_tx[MAX_BUF_SIZE * 5 + 1];
uint8_t buffer_print_rx[MAX_BUF_SIZE * 5 + 1];

/*Number inside the [] Tells us how many buffers*/
/*.buf is the type of buffer*/
/*.len is the length of the buffer*/
struct spi_buf tx_bufs[2] = {};

struct spi_buf rx_bufs[2] = {};

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

int max30003_read(uint8_t reg, void *data, size_t size)
{
	uint8_t dummy;
	tx_bufs[0].buf = &reg;
	tx_bufs[0].len = 1;
	tx_bufs[1].buf = NULL;
	tx_bufs[1].len = 0;
	rx_bufs[0].buf = &dummy;
	rx_bufs[0].len = 1;
	rx_bufs[1].buf = data;
	rx_bufs[1].len = size;

	int ret = spi_transceive(spi_device, &spi_cfg, &tx, &rx);
	return ret;
}

int max3003_write(uint8_t reg, void *data, size_t size)
{   
	tx_bufs[0].buf = &reg;
	tx_bufs[0].len = 1;
	tx_bufs[1].buf = data;
	tx_bufs[1].len = size;
	int ret = spi_transceive(spi_device, &spi_cfg, &tx, NULL);
	return ret;
}

bool max_readinfo()
{
	// printk("Shifting Registers!\n");
	const uint8_t reg = 0x1f;
	uint8_t data[3];
	//memset(data, 0, sizeof data);

	//int ret = max30003_read(reg, data, sizeof data);
	int ret = max30003_read(0x1f, data, sizeof data);
	to_display_format(data, 3, buffer_print_rx);
	printk("Receiving: %s\n", buffer_print_rx);
	printk("spi_transcieve returns %d \n", ret);

	// First check if the transieve was successful
	if (ret != false)
	{
		printk("Device Failed to transceive\n");
		return false;
	}

	if ((data[0] & 0xF0) == 0x50)
	{
		printk("MAX is ready!\n");
		printk("Dev ID: ");
		printk("%x", data[0] & 0xFF);
		return true;
	}
	else
	{
		printk("MAX read info error! Check wiring!\n");
		return false;
	}

}