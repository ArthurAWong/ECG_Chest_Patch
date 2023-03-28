import serial.tools.list_ports
import serial
import time
import matplotlib.pyplot as plt
import random
from matplotlib.animation import FuncAnimation
from functools import partial
import re
import warnings
import numpy 
# import panda as pd.
warnings.simplefilter('ignore')
ports = serial.tools.list_ports.comports()

ecg_num = []
timeArr = []
counter = 0

for index,port in enumerate(ports):
    if "JLink" in port.description:
        ser = serial.Serial(port.device, 115200) #115200 for bmd
        ser.port = port.device
        print("Recognized JLink")
        break
    if index == len(ports) - 1:
        print("JLink not found")

def readPortECG(ser):
    data = ser.readline() # Read data from the UART device
    data = data.decode('utf-8')
    ecgData = re.findall(r'^[-]?[0-9]+', data)
    return ecgData

fig, ax = plt.subplots()

# Define the function that updates the plot
def update2(frames):
    global counter
    global ecg_num
    global timeArr
    data = readPortECG(ser)
    if not data:
        return
    processedData = 1000*float(data[0])/(2^17*160)

    if abs(processedData) < 100000:
        
        if len(ecg_num) >= 300:
            ecg_num = []
            timeArr = []
        ecg_num.append(processedData)
        timeArr.append(counter)
        counter += 1
        # Draw x and y lists
        # Format plot
        plt.xlabel = "Time"
        plt.ylabel = "ECG Data (mV)"
        
        ax.clear()
        ax.plot(timeArr, ecg_num, color='blue', linewidth=2, marker="o")
    else:
        pass


# Animate the plot
ani = FuncAnimation(fig, update2, frames=2, interval=0)
plt.show()
