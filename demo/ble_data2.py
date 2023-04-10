from bleak import *
import asyncio
import traceback
from contextlib import suppress
from concurrent.futures import ThreadPoolExecutor

BLE_NAME = "Arthur"

accelx_data_uuid = "3b8cdf21-24a2-468c-9236-20f50b770bf5"
accely_data_uuid = "1094b4ef-76bd-4341-aa96-83f99198db63"
accelz_data_uuid = "f5f4e4fe-1ab0-4aea-9334-b19fc27d6e7c"
gyrox_data_uuid = "ff3675ff-5479-4e1c-9d87-a0ac6e2dadbf"
gyroy_data_uuid = "c040eb24-7174-4f86-a547-659b538d9c31"
gyroz_data_uuid = "4f30dd9e-9c7c-48d5-b071-d1924af65ac6"
ecg_data_uuid = "60a19d0d-7e6c-4d6a-ac49-9b3c29a0ca94"

x_xl_queuePut = None
y_xl_queuePut = None
z_xl_queuePut = None
x_gyro_queuePut = None
y_gyro_queuePut = None
z_gyro_queuePut = None
ecg_queuePut = None

async def accelXCallback(sender: BleakGATTCharacteristic, data: bytearray):
    global x_xl_queuePut
    x_xl_queuePut(int.from_bytes(data, "little"))

async def accelYCallback(sender: BleakGATTCharacteristic, data: bytearray):
    global y_xl_queuePut
    y_xl_queuePut(int.from_bytes(data, "little"))

async def accelZCallback(sender: BleakGATTCharacteristic, data: bytearray):
    global z_xl_queuePut
    z_xl_queuePut(int.from_bytes(data, "little"))

async def gyroXCallback(sender: BleakGATTCharacteristic, data: bytearray):
    global x_gyro_queuePut
    x_gyro_queuePut(int.from_bytes(data, "little"))

async def gyroYCallback(sender: BleakGATTCharacteristic, data: bytearray):
    global y_gyro_queuePut
    y_gyro_queuePut(int.from_bytes(data, "little"))

async def gyroZCallback(sender: BleakGATTCharacteristic, data: bytearray):
    global z_gyro_queuePut
    z_gyro_queuePut(int.from_bytes(data, "little"))

async def ecgCallback(sender: BleakGATTCharacteristic, data: bytearray):
    global ecg_queuePut
    ecg_queuePut(int.from_bytes(data, "little"))

async def main():
    BLEDevice = None
    while(BLEDevice == None):
        BLEDevice = await BleakScanner().find_device_by_name(BLE_NAME)
    print(BLEDevice.address)
    try:
        async with BleakClient(BLEDevice) as client:
            print(f"Connected: {client.is_connected}")
            await client.start_notify(accelx_data_uuid, accelXCallback)
            await client.start_notify(accely_data_uuid, accelYCallback)
            await client.start_notify(accelz_data_uuid, accelZCallback)
            await client.start_notify(gyrox_data_uuid, gyroXCallback)
            await client.start_notify(gyroy_data_uuid, gyroYCallback)
            await client.start_notify(gyroz_data_uuid, gyroZCallback)
            await client.start_notify(ecg_data_uuid, ecgCallback)
            executorObj = ThreadPoolExecutor(1)
            input = 0
            while input == 0:
                input = await asyncio.get_running_loop().run_in_executor(executorObj, WaitForInput)
            print("Exiting!")

    except AttributeError:
        print(f"Error in BleakClient! Make sure that there are no devices connected first!\n\n{traceback.format_exc()}")

def WaitForInput():
    return input("Press enter to end session")

def printValueOut(intNum):
    print(f"{intNum}")

def runBleakConnector(queueDict = None):
    global x_xl_queuePut, y_xl_queuePut, z_xl_queuePut, x_gyro_queuePut, y_gyro_queuePut, z_gyro_queuePut, ecg_queuePut
    if type(queueDict) == dict:
        x_xl_queuePut = queueDict["accelXQueue"].put_nowait
        y_xl_queuePut = queueDict["accelYQueue"].put_nowait
        z_xl_queuePut = queueDict["accelZQueue"].put_nowait
        x_gyro_queuePut = queueDict["gyroXQueue"].put_nowait
        y_gyro_queuePut = queueDict["gyroYQueue"].put_nowait
        z_gyro_queuePut = queueDict["gyroZQueue"].put_nowait
        ecg_queuePut = queueDict["ecgQueue"].put_nowait
    else:
        x_xl_queuePut = printValueOut
        y_xl_queuePut = printValueOut
        z_xl_queuePut = printValueOut
        x_gyro_queuePut = printValueOut
        y_gyro_queuePut = printValueOut
        z_gyro_queuePut = printValueOut
        ecg_queuePut = printValueOut
    
    loop = asyncio.new_event_loop()
    asyncio.set_event_loop(loop)

    try:
        loop.run_until_complete(main())
    finally:
        loop.run_until_complete(loop.shutdown_asyncgens())
        loop.close()

if __name__ == "__main__":
    runBleakConnector()