#ifndef SPI_CONFIG
#define SPI_CONFIG

#include <zephyr/drivers/spi.h>

// CONFIGURATION STRUCTURES
extern struct spi_cs_control spi_cs;

extern struct spi_config spi_cfg;

// DEVICE POINTER
extern const struct device *spi_device;

#endif //SPI_CONFIG