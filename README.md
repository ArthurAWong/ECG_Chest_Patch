# ECG_ChestPatch

The development of the ECG Chest Patch was done using nRF Connect SDK 2.1.2, which includes Zephyr RTOS. Details about the code can be seen inside the source files.

## Wiring up the LSM6DSM Development Board

Assuming that you are using the nRF52DK device tree, wire the LSM6DSM development board as follows

```
BMD350               LSM6DSM
VSHLD <------------> VDD
VSHLD <------------> VDDIO
GND <--------------> GND
P0.13 (SCL) <------> SCL
P0.18 (MOSI) <-----> SDA
P0.26 (MISO) <-----> SDO
P0.12 (ACCEL CS) <-> CS
```

## Wiring up the MAX30003 Development Board

Wiring the MAX30003WING (MAX30003 development kit) to the BMD350 requires use of the MAX30003WING datasheet. Assuming that you are using the nRF52DK device tree, the MAX30003WING is wired as follows

```
BMD350               MAX30003
VSHLD <------------> 3V3
GND <--------------> GND (Only one GND pin needs to be connected)
P0.13 (SCL) <------> SCK
P0.18 (MOSI) <-----> MOSI
P0.26 (MISO) <-----> MISO
P0.11 (ECG CS) <---> SSEL
```

## Wiring up the Accelerometer-ECG PCB

The Accelerometer-ECG PCB should have wires jetting out of it with labels attached. The wiring of the PCB is as follows

```
BMD350               PCB
VSHLD <------------> RED (WIRE COLOR)
GND <--------------> BLACK (WIRE COLOR)
P0.13 (SCL) <------> CLK (LABEL)
P0.18 (MOSI) <-----> MOSI (LABEL)
P0.26 (MISO) <-----> MISO (LABEL)
P0.11 (ECG CS) <---> ECG (LABEL)
P0.12 (ACCEL CS) <-> ACCEL (LABEL)
```

## Interpreting BLE Data

There are currently 3 characteristics being advertised, one for ECG, one for the accelerometer, and one for the gyroscope. Each transfer produces 30 data points for the ECG, 15 data points for the accelerometer, and 15 data points for the gyroscope. The data points for the accelerometer and gyroscope alternate between X, Y, and Z (XYZXYZXYZ...)

## Peculiarties and Defects

### JLink Programming Wire

The given JLink programming wire has some difficulties in programming the microprocessor-containing board. This problem has been partially fixed by wiggling the USB-C side of the wire. This means that there might be a weak connection on the USB-C side.

### Programming the Microprocessor-containing Board

If you have a debug light that is always on it may fail to turn on after programming the microprocessor-containing board. It was revealed that the microprocessor was halted after programming. This can be fixed by connecting the board to the BMD350 via JLink, opening up J-Link Commander on your computer, connecting to the board with the command `connect`, specifying the target interface as SWD, `S`, and keeping the default speed (press enter), then typing in the commands

```
Reset
Go
```

You will have to do this every time you program the board. You might (we forgot to record what happens when the board is power cycled) have to do this every time you repower the board.

### Combining the Accelerometer-ECG Board with the Microprocessor-containing Board

Combining the accelerometer-ECG board with the microprocessor-containing board via the soldered wires results in an incredibly weak Bluetooth RSSI. This issue could be addressed by integrating the ECG and accelerometer on the same board as the microprocessor. Combining the accelerometer-ECG board with the BMD350 is okay and produces no deficiencies in the Bluetooth connection.

### Makerdiary nRF52840 MDK USB Dongle

This dongle is unneccesairly difficult to work with. It requires the use of third party tools to get it working. We have not gotten it working and instead opted to get an nRF52840 USB dongle straight from Nordic Semiconductor using our own funds. 

### Leftover Accelerometer-ECG PCBs

These are essentially e-waste. It was revealed there were some errors during the design of our PCB.

    1. Y1 (oscillator) did not have its power pin correctly labeled, leaving it unpowered.
    2. The OVDD pin of U2 (MAX30003) was not connected to 3.3V, leaving the MAX30003 unpowered.
    3. The buffer amplifiers (U4 and U5) were revealed to be unessential, wasting space on the board.

The latest PCB schematics have addressed these issues.

### Anomalous ECG Readings

During the last few days of the capstone the ECG readings became noisy and had random spikes baked into the data. We temporairly fixed the problem without knowing how and has reverted back to being noisy. We believe that the problem may be hardware based due to the fixes we had to implement on our PCB.