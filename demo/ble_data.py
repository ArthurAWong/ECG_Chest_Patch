import asyncio
from bleak import *
import queue
address = "F2:5C:F3:93:F0:8E"
MODEL_NBR_UUID = "3b8cdf21-24a2-468c-9236-20f50b770bf5"

q = queue.Queue()
accel_data_uuid = ("3b8cdf21-24a2-468c-9236-20f50b770bf5","1094b4ef-76bd-4341-aa96-83f99198db63","f5f4e4fe-1ab0-4aea-9334-b19fc27d6e7c")
gyro_data_uuid = ("ff3675ff-5479-4e1c-9d87-a0ac6e2dadbf","c040eb24-7174-4f86-a547-659b538d9c31","4f30dd9e-9c7c-48d5-b071-d1924af65ac6")
ecg_data_uuid = "60a19d0d-7e6c-4d6a-ac49-9b3c29a0ca94"
flag = False

x_xl_val, y_xl_val, z_xl_val, x_gyro_val, y_gyro_val, z_gyro_val, ecg_val = 0,0,0,0,0,0,0
async def callback(sender: BleakGATTCharacteristic, data: bytearray):
    global x_xl_val, y_xl_val, z_xl_val, x_gyro_val, y_gyro_val, z_gyro_val, ecg_val
    
    if sender.uuid == accel_data_uuid[0]:
        x_xl_val = data
        # print("X set")
    elif sender.uuid == accel_data_uuid[1]:
        y_xl_val = data
        # print("Y set")
    elif sender.uuid == accel_data_uuid[2]:
        z_xl_val = data
        # print("Z set")
    elif sender.uuid == gyro_data_uuid[0]:
        x_gyro_val = data
        # print("x_gyro_val set")
    elif sender.uuid == gyro_data_uuid[1]:
        y_gyro_val = data
        # print("y_gyro_val set")
    elif sender.uuid == gyro_data_uuid[2]:
        z_gyro_val = data
        # print("z_gyro_val set")
    elif sender.uuid == ecg_data_uuid:
        ecg_val = data
        # print("ecg_val set")
    
async def get_notifications(address):
    async with BleakClient(address) as client:
        global flag
        
        # await client.start_notify(MODEL_NBR_UUID, callback)
        # for service in client.services:
        #     for chrc in service.characteristics:
        #         if chrc.uuid == MODEL_NBR_UUID:
        #             client.start_notify(chrc, callback)
        flag = True
        print(f"flag status is {flag}")
        # characteristic = await client.get_characteristic(MODEL_NBR_UUID)
        for i in range(len(accel_data_uuid)):
            await client.start_notify(accel_data_uuid[i],callback)
        for i in range(len(gyro_data_uuid)):
            await client.start_notify(gyro_data_uuid[i],callback)
        await client.start_notify(ecg_data_uuid,callback)
        await asyncio.sleep(5)

        returnvals()

        for i in range(len(accel_data_uuid)):
            await client.stop_notify(accel_data_uuid[i])
            await client.stop_notify(gyro_data_uuid[i])
        await client.stop_notify(ecg_data_uuid)

        
        
        print("finished")

        # print("Model Number: {0}".format("".join(map(chr, characteristic))))

def returnvals():
    return x_xl_val, y_xl_val, z_xl_val, x_gyro_val, y_gyro_val, z_gyro_val, ecg_val

async def main():
    await get_notifications(address)

if __name__ == "__main__":
    loop = asyncio.get_event_loop()
    loop.run_until_complete(asyncio.ensure_future(main()))
    loop.close()






