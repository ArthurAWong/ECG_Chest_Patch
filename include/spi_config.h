#ifndef SPI_CONFIG
#define SPI_CONFIG

#include <zephyr/drivers/spi.h>

// CONFIGURATION STRUCTURES
extern struct spi_cs_control max_spi_cs;
extern struct spi_cs_control accel_spi_cs;

extern struct spi_config max_spi_cfg;
extern struct spi_config accel_spi_cfg;

// DEVICE POINTER
extern const struct device *spi_device;

#endif //SPI_CONFIG