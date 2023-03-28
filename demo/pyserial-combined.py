import serial.tools.list_ports
import serial
import time
import matplotlib.pyplot as plt
import random
from matplotlib.animation import FuncAnimation
from mpl_toolkits.mplot3d import Axes3D
from functools import partial
import re
import warnings
import numpy 
warnings.simplefilter('ignore')
ports = serial.tools.list_ports.comports()

ecg_num = []
timeArr = []
counter = 0
voltage_th = 500000
x, y, z = [],[],[]
xs, ys = [], []
for index,port in enumerate(ports):
    if "JLink" in port.description:
        ser = serial.Serial(port.device,115200,xonxoff=True,rtscts=True) #115200 for bmd
        ser.port = port.device
        
        print("Recognized JLink")
        break
    if index == len(ports) - 1:
        print("JLink not found")

def readPort(ser,peripheral):
    data = ser.readline() # Read data from the UART device
    data = data.decode('utf-8')
    if peripheral == 'MAX30003':
        retval = re.findall(r'^[-]?[0-9]+', data)
    elif peripheral == 'LSM6DSM':
        retval = re.findall(r'-?\d+\.\d+|-?\d+',data)
    else:
        print("Peripheral not defined")
    return retval


fig, ax = plt.subplots()
fig3d = plt.figure()
ax3d = Axes3D(fig3d)
# Define the function that updates the plot
def updateECG(frames):
    global ecg_num
    global counter
    global timeArr
    data = readPort(ser,'MAX30003')
    if not data:
        return
    processedData =  float(data[0])/(2^17*160) # mV
    if len(ecg_num) >= 300:
        ecg_num = []
        timeArr = []
    if abs(processedData) < voltage_th:  
        #print(processedData)
        ecg_num.append(processedData)


    else:
        if processedData > 0:
            ecg_num.append(voltage_th)

        else:
            ecg_num.append(-1*voltage_th)

    timeArr.append(counter)
    counter += 1
    # Draw x and y lists
    # Format plot
    plt.xlabel = "Time"
    plt.ylabel = "ECG Data (mV)"
    
    ax.clear()
    ax.plot(timeArr, ecg_num, color='blue', linewidth=2, marker="o")


scatter = ax3d.scatter(x, y, z, c='r', marker='o')
def updateAccel(frames, xs, ys):
    data = readPort(ser,'LSM6DSM')
    print(data)
    x.append(float(data[0]))
    y.append(float(data[1]))
    z.append(float(data[2]))
    
    scatter._offsets3d = (x, y, z)
    ax3d.relim()
    ax3d.autoscale_view()
    xs = xs[-20:]
    ys = ys[-20:]
    # Draw x and y lists
    ax3d.clear()
    ax3d.scatter3D(x, y, z, c=z, cmap='Greens')

    # Format plot
    plt.xticks(rotation=45, ha='right')
    plt.subplots_adjust(bottom=0.30)
    plt.title('Accelerometer data')
    plt.ylabel('Y')


# Animate the plot
def plotData(peripheral):
    if peripheral == "MAX30003":
        ani = FuncAnimation(fig, updateECG, frames=2, interval=0)
    elif peripheral == "LSM6DSM":
        ani = FuncAnimation(fig, updateAccel, fargs= (xs, ys), frames=1, interval=0)
    plt.show()

plotData('MAX30003')

while 1:
    readPort(ser,'LSM6DSM')

