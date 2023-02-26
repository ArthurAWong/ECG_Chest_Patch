#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>
#include "utils.h"
#include "spi_config.h"

#include "max30003.h"

#define WRITE_REG(reg) ((reg << 1) & ~(1 << 0))
#define READ_REG(reg) ((reg << 1) | (1 << 0))

uint8_t max_buffer_tx[MAX_BUF_SIZE] = {};
uint8_t max_buffer_rx[MAX_BUF_SIZE] = {};

uint8_t max_buffer_print_tx[MAX_BUF_SIZE * 5 + 1];
uint8_t max_buffer_print_rx[MAX_BUF_SIZE * 5 + 1];

/*Number inside the [] Tells us how many buffers*/
/*.buf is the type of buffer*/
/*.len is the length of the buffer*/
struct spi_buf max_tx_bufs[2] = {};

struct spi_buf max_rx_bufs[2] = {};

const struct spi_buf_set max_tx = 
{
	.buffers = max_tx_bufs,
	.count = ARRAY_SIZE(max_tx_bufs)
};

const struct spi_buf_set max_rx = 
{
	.buffers = max_rx_bufs,
	.count = ARRAY_SIZE(max_rx_bufs)
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
	max_tx_bufs[0].buf = &reg;
	max_tx_bufs[0].len = 1;
	max_tx_bufs[1].buf = NULL;
	max_tx_bufs[1].len = 0;
	max_rx_bufs[0].buf = &dummy;
	max_rx_bufs[0].len = 1;
	max_rx_bufs[1].buf = data;
	max_rx_bufs[1].len = size;

	int ret = spi_transceive(spi_device, &max_spi_cfg, &max_tx, &max_rx);
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
	max_tx_bufs[0].buf = &reg;
	max_tx_bufs[0].len = 1;
	max_tx_bufs[1].buf = data;
	max_tx_bufs[1].len = size;
	int ret = spi_transceive(spi_device, &max_spi_cfg, &max_tx, NULL);
	return ret;
}

int max30003_write_uint32(uint8_t reg, uint32_t data)
{   
	reg = WRITE_REG(reg);

	uint8_t write_out[3] = {(data >> 16) &(0xff), (data >> 8) &(0xff), (data >> 0) &(0xff)};
	//uint8_t write_out[3] = {(data >> 0) &(0xff), (data >> 8) &(0xff), (data >> 16) &(0xff)};
	max_tx_bufs[0].buf = &reg;
	max_tx_bufs[0].len = 1;
	max_tx_bufs[1].buf = write_out;
	max_tx_bufs[1].len = 3;
	int ret = spi_transceive(spi_device, &max_spi_cfg, &max_tx, NULL);
	if (ret != 0)
	{
		printk("Error spi writing!\n");
	}
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
	ret = max30003_write_uint32(SW_RST, 0);
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
	// to_display_format(data, 3, max_buffer_print_rx);
	// printk("Receiving: %s\n", max_buffer_print_rx);
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
int max_readstatus()
{
	uint8_t data[3];
	int ret = max30003_read(STATUS, data, sizeof data);
	to_display_format(data, 3, max_buffer_print_rx);
	printk("Status Register: %s\n", max_buffer_print_rx);

	if(ret != 0)
	{
		printk("Read status failed!\n");
		return -1;
	}

	return ret;
}

int max_enable_ecg()
{
	int ret;

	uint32_t cnfg_gen_reg = (1 << CNFG_GEN_EN_ECG) | (1 << CNFG_GEN_RBIASN) | (1 << CNFG_GEN_RBIASP) | (1 << CNFG_GEN_EN_RBIAS)
								| (2 << CNFG_GEN_IMAG) | (1 << CNFG_GEN_EN_DCLOFF);
	ret = max30003_write_uint32(CNFG_GEN, cnfg_gen_reg);

    uint32_t cnfg_ecg_reg = (0b01 << CNFG_ECG_DLPF) | (0 << CNFG_ECG_DHPF) | (0b11 << CNFG_ECG_GAIN) | (0b10 << CNFG_ECG_RATE);
	ret = max30003_write_uint32(CNFG_ECG, cnfg_ecg_reg);

    uint32_t cnfg_rtor_reg = (0b0011 << CNFG_RTOR1_WNDW) | (0b1111 << CNFG_RTOR1_GAIN) | (0b11 << CNFG_RTOR1_PAVG) 
								| (0b0011 << CNFG_RTOR1_PTSF) | (1 << CNFG_RTOR1_EN_RTOR);
    ret = max30003_write_uint32(CNFG_RTOR1, cnfg_rtor_reg);

    // uint32_t mngr_int_reg = (0b00011 << MNGR_INT_EFIT) | (0b01 << MNGR_INT_CLR_RRINT);
	// ret = max30003_write_uint32(MNGR_INT, mngr_int_reg);

	// uint32_t en_int_reg = (1 << EN_INT_EN_EINT) | (1 << EN_INT_EN_RRINT) | (0b11 << EN_INT_INTB_TYPE);
	// ret = max30003_write_uint32(EN_INT, en_int_reg);

	uint32_t mngr_dyn_reg = 0x00000000 & ~(1 << MNGR_DYN_FAST);
	ret = max30003_write_uint32(MNGR_DYN, mngr_dyn_reg);

	uint32_t cnfg_emux_reg = 0x00000000 & ~(1 << CNFG_EMUX_OPENN) & ~(1 << CNFG_EMUX_OPENP);
	ret = max30003_write_uint32(CNFG_EMUX, cnfg_emux_reg);

	// ret = max30003_read(CNFG_GEN, data, sizeof(data));
	// to_display_format(data, 3, max_buffer_print_rx);
	// printk("%s\n", max_buffer_print_rx);
	// ret = max30003_read(CNFG_ECG, data, sizeof(data));
	// to_display_format(data, 3, max_buffer_print_rx);
	// printk("%s\n", max_buffer_print_rx);
	// ret = max30003_read(CNFG_RTOR1, data, sizeof(data));
	// to_display_format(data, 3, max_buffer_print_rx);
	// printk("%s\n", max_buffer_print_rx);
	// ret = max30003_read(MNGR_INT, data, sizeof(data));
	// to_display_format(data, 3, max_buffer_print_rx);
	// printk("%s\n", max_buffer_print_rx);
	// ret = max30003_read(EN_INT, data, sizeof(data));
	// to_display_format(data, 3, max_buffer_print_rx);
	// printk("%s\n", max_buffer_print_rx);
	// ret = max30003_read(MNGR_DYN, data, sizeof(data));
	// to_display_format(data, 3, max_buffer_print_rx);
	// printk("%s\n", max_buffer_print_rx);
	// ret = max30003_read(CNFG_EMUX, data, sizeof(data));
	// to_display_format(data, 3, max_buffer_print_rx);
	// printk("%s\n", max_buffer_print_rx);

	ret = max30003_write_uint32(SYNCH, 0x00000000);
	ret = max30003_write_uint32(FIFO_RST, 0x00000000);

	return ret;
}

int max_read_ecg(int32_t *out_val)
{
	int ret;
	uint8_t data[3] = {0x00, 0x00, 0x00};
	ret = max30003_read(STATUS, data, sizeof(data));
	if (data[0] & (1 << 6))
	{
		printk("OVERFLOW!\n");
		ret = max30003_write_uint32(FIFO_RST, 0x00000000);
		return 1;
	}

	if (data[0] & (1 << 7))
	{
		ret = max30003_read(ECG_FIFO, data, sizeof(data));
		//to_display_format(data, 3, max_buffer_print_rx);
		//printk("%s\n", max_buffer_print_rx);
		uint32_t ecg_data = ((data[0] << 16) | (data[1] << 8) | (data[2] << 0)) >> 6;
		uint8_t data_tags = ((data[2] & (0x3F)) >> 3);
		if (data_tags != 0)
		{
			return 1;
		}
		if (ecg_data > 131072)
		{
			*out_val = ecg_data - 262144;
		} else
		{
			*out_val = ecg_data;
		}
		return 0;
	}
	else
	{
		return 1;
	}
}