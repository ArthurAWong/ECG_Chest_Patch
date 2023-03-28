import serial.tools.list_ports
import serial
import time
import matplotlib.pyplot as plt
import random
from matplotlib.animation import FuncAnimation
from mpl_toolkits.mplot3d import Axes3D
import re
import warnings
import numpy 
warnings.simplefilter('ignore')
ports = serial.tools.list_ports.comports()
x, y, z = [],[],[]
xs, ys = [], []

for index,port in enumerate(ports):
    if "JLink" in port.description:
        ser = serial.Serial(port.device,115200) #115200 for bmd
        ser.port = port.device
        print("Recognized JLink")
        break
    if index == len(ports) - 1:
        print("JLink not found")
        
        
def readPort(ser):
    data = ser.readline() # Read data from the UART device
    data = data.decode('utf-8')
    coordinates = re.findall(r'-?\d+\.\d+|-?\d+',data)
    #print(f"coordinates: {coordinates}")
    return coordinates

#'x:  0.138, y: -0.001, z:  9.708\r\r\n'

fig = plt.figure()
ax = Axes3D(fig)

# Plot the initial data
scatter = ax.scatter(x, y, z, c='r', marker='o')

readPort(ser)


# Define the function that updates the plot
def update(frames, xs, ys):
    data = readPort(ser)
    #print(f'x:{data[0]},y:{data[1]},z:{data[2]}')
    if len(data) == 3:
        print(f'x:{data[0]},y:{data[1]},z:{data[2]}')
        x.append(float(data[0]))
        y.append(float(data[1]))
        z.append(float(data[2]))
        scatter._offsets3d = (x, y, z)
        ax.relim()
        ax.autoscale_view()

    xs = xs[-20:]
    ys = ys[-20:]
    # Draw x and y lists
    ax.clear()
    ax.scatter3D(x, y, z, c=z, cmap='Greens')

    # Format plot
    plt.xticks(rotation=45, ha='right')
    plt.subplots_adjust(bottom=0.30)
    plt.title('Accelerometer data')
    plt.ylabel('Y')


# Animate the plot
ani = FuncAnimation(fig, update, fargs= (xs, ys), frames=2, interval=0)
plt.show()
