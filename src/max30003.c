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

/**
* @brief Reading a register of the MAX
* @param reg The register to be read
* @param data Array containing the information of the register
* @param size The size of the input data
*
* @retval 0 if successful
* @retval 1 if failed
*/
int max30003_read(uint8_t reg, void *data, size_t size)
{
	reg = READ_REG(reg);
	uint8_t dummy;
	tx_bufs[0].buf = &reg;
	tx_bufs[0].len = 1;
	tx_bufs[1].buf = NULL;
	tx_bufs[1].len = 0;
	rx_bufs[0].buf = &dummy;
	rx_bufs[0].len = 1;
	rx_bufs[1].buf = &data;
	rx_bufs[1].len = size;

	int ret = spi_transceive(spi_device, &spi_cfg, &tx, &rx);
	return ret;
}

/**
* @brief Writting a register of the MAX
* @param reg The register to be written
* @param Array containing the information to be written
* @param size The size of the input data
*
* @retval 0 if successful
* @retval 1 if failed
*/
int max30003_write(uint8_t reg, void *data, size_t size)
{   
	reg = WRITE_REG(reg);
	tx_bufs[0].buf = &reg;
	tx_bufs[0].len = 1;
	tx_bufs[1].buf = &data;
	tx_bufs[1].len = size;
	int ret = spi_transceive(spi_device, &spi_cfg, &tx, NULL);
	return ret;
}

/**
* @brief Software reset, equivalent to power-cycling the device
* 
* @retval Returns 0 after operation is done
*/
bool max_reset_sw()
{
	int ret;
	ret = max30003_write(SW_RST, 0x000000, 24);
	return 0;
}

/**
* @brief Reads the INFO (0x0F) register
*
* @retval Returns true if the device is read
* @retval Returns false if the device is not read
*/
bool max_readinfo()
{
	uint8_t data[3];

	int ret = max30003_read(INFO, data, sizeof data);
	// to_display_format(data, 3, buffer_print_rx);
	// printk("Receiving: %s\n", buffer_print_rx);
	// printk("spi_transcieve returns %d \n", ret);

	if ((data[0] & 0xF0) == 0x50)
	{
		printk("MAX is ready!\n");
		printk("Dev ID: ");
		printk("%x \n", data[0] & 0xFF);
		return true;
	}
	else
	{
		printk("MAX read info error! Check wiring!\n");
		return false;
	}
}

/**
 * @brief Reads STATUS (0x01) 
 * 
 * @retval 3 Byte Array of the contents of the STATUS register
 */
uint8_t max_readstatus()
{
	uint8_t data[3];
	int ret = max30003_read(STATUS, data, sizeof data);
	to_display_format(data, 3, buffer_print_rx);
	printk("Status Register: %s\n", buffer_print_rx);

	if(ret != 0)
	{
		printk("Read status failed!\n");
		return -1;
	}

	return data;
}

// int max_enable_ecg()
// {
// 	int ret;
// 	uint8_t data[3];
// 	uint8_t out_var[3];

// 	ret = max30003_read(CNFG_GEN, data, sizeof(data));
// 	to_display_format(data, 3, buffer_print_rx);
// 	printk("Initial Config Reg: %s\n", buffer_print_rx);
	
// 	max_readstatus();
// 	ret = max30003_write(MNGR_INT, 0x000000, 24);

// 	ret = max30003_write(CNFG_GEN, 0x000007, 24);

// 	ret = max30003_write(CNFG_EMUX, 0x000000, 24);
// 	ret = max30003_write(CNFG_ECG, 0x000000, 24);

// 	ret = max30003_write(SYNCH, 0x000000, 24);
// 	ret = max30003_write(FIFO_RST, 0x000000, 24);
// 	ret = max30003_read(ECG_FIFO, &out_var, sizeof(out_var));
// 	to_display_format(out_var, 3, buffer_print_rx);
// 	printk("FIFO Register after reset: %s\n", buffer_print_rx);

// 	ret = max30003_write(CNFG_GEN, 0x080007, 24);

// 	for(int i = 0; i < 32; i++)
// 	{
// 		ret = max30003_read(ECG_FIFO, &out_var, sizeof(out_var));
// 		to_display_format(out_var, 3, buffer_print_rx);
// 		printk("FIFO Register: %s\n", buffer_print_rx);
// 		ret = max30003_write(CNFG_GEN, 0x000007, 24);
// 		ret = max30003_write(SYNCH, 0x000000, 24);
// 		ret = max30003_write(FIFO_RST, 0x000000, 24);
// 		ret = max30003_write(CNFG_GEN, 0x080007, 24);
// 	}
	
// 	return ret;
// }