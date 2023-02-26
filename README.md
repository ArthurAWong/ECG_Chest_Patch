# ECG_ChestPatch

## Wiring up the Accelerometer

Assuming that you are using the nRF52DK device tree, SPI1, and the chip select pin is P0.28, wire the accelerometer development board as follows

```
BMD350             LSM6DSM
VSHLD <----------> VDD
VSHLD <----------> VDDIO
GND <------------> GND
P0.31 (SCL) <----> SCL
P0.30 (MOSI) <---> SDA
P0.29 (MISO) <---> SDO
P0.28 (CS) <-----> CS
```
