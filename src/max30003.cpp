
#include <zephyr/drivers/spi.h>
#include "max30003_zephyr.h"

//****************************************************************************
uint32_t max30003_readRegister(MAX30003_t *dev, const Registers_e reg)
{
    uint32_t data = 0;

    // pull CS low

    // m_cs = 0;
    // m_spiBus.write((reg << 1) | 1);
    // data |= (m_spiBus.write(0xFF) << 16);
    // data |= (m_spiBus.write(0xFF) << 8);
    // data |= m_spiBus.write(0xFF);
    // pull CS high
    // m_cs = 1;

    return data;
}

//****************************************************************************
void max30003_writeRegister(MAX30003_t *dev, const Registers_e reg, const uint32_t data)
{
    // pull CS low
    // m_cs = 0;
    // m_spiBus.write(reg << 1);
    // m_spiBus.write((0x00FF0000 & data) >> 16);
    // m_spiBus.write((0x0000FF00 & data) >> 8);
    // m_spiBus.write(0x000000FF & data);
    // pull CS High
    // m_cs = 1;
}