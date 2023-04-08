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
try:
    for index,port in enumerate(ports):
        if "JLink" in port.description:
            global ser
            ser = serial.Serial(port.device,115200,xonxoff=True,rtscts=True) #115200 for bmd
            ser.port = port.device
            
            print("Recognized JLink")
            break
        if index == len(ports) - 1:
            print("JLink not found")
except serial.SerialException:
    print("COM port is currently in use!")


def readPort():
    data = ser.readline() # Read data from the UART device
    data = data.decode('utf-8')
    if not data:
        pass
    else:
  
        retval = []
        if "Accel" in data:
            retval.append("Accel")
            retval += re.findall(r'-?\d+\.\d+|-?\d+',data)
            
        elif "Gyro" in data:
            retval.append("Gyro")
            retval += re.findall(r'-?\d+\.\d+|-?\d+',data)
            
        else:
            retval.append("ECG")
            retval += re.findall(r'^[-]?[0-9]+', data) 
            

    
    return retval


# while 1:
#     print(readPort())


