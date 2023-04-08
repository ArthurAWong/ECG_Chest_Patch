from PyQt5 import QtWidgets, uic, QtCore
from pyqtgraph import PlotWidget
import pyqtgraph as pg
import sys
import os
import numpy as np
import serial_data
from multiprocessing import Queue

class AccelMainWindow(QtWidgets.QMainWindow):
    bottom_plot_data = np.random.normal(size=(10, 3))
    axis = ('x','y','z')
    accel_data = [0,0,9.81]

    def __init__(self, xQueue: Queue, yQueue: Queue, zQueue: Queue):
        super(AccelMainWindow, self).__init__()
        self.xQueue = xQueue
        self.yQueue = yQueue
        self.zQueue = zQueue
        uic.loadUi(os.path.join(os.path.dirname(__file__), "window.ui"), self)

        screen_rect = QtWidgets.QDesktopWidget().screenGeometry(-1)
        width = int(screen_rect.width() * 0.6)
        frame_width = int(width)

        height = int(screen_rect.height() * 0.6)
        frame_height = int(height)
        self.setGeometry(0, 0, width, height)
        self.setMinimumSize(QtCore.QSize(width, height))

        central_layout = QtWidgets.QVBoxLayout(self.centralwidget)
        central_layout.setContentsMargins(0, 0, 0, 0)

        self.plot_widget = pg.PlotWidget()
        central_layout.addWidget(self.plot_widget)

        self.plot_widget.setBackground("white")
        self.plot_widget.setLabel("left", "Acceleration", units="m/s^2")
        self.plot_widget.hideAxis('bottom')

        self.accel_graph = pg.BarGraphItem(
            x=range(3), height=[0, 0, 0], width=0.3, brush="green"
        )
        
        self.plot_widget.addItem(self.accel_graph)

        self.timer = pg.QtCore.QTimer()
        self.timer.timeout.connect(self.update)
        self.timer.start(0)

    def update(self):
        changed = False     
        if not self.xQueue.empty():
            self.accel_data[0] = self.xQueue.get_nowait() 
            changed = True
        if not self.yQueue.empty():
            self.accel_data[1] = self.yQueue.get_nowait() 
            changed = True
        if not self.zQueue.empty():
            self.accel_data[2] = self.zQueue.get_nowait() 
            changed = True

        if changed == True:
            self.accel_graph.setOpts(height=self.accel_data, x=range(3), brush='blue', labels={i: f"Bar {i}" for i in self.axis})
        else:
            return

class GyroMainWindow(QtWidgets.QMainWindow):
    top_plot_data = np.zeros(100)
    bottom_plot_data = np.random.normal(size=(10, 3))
    axis = ('x','y','z')
    gyro_data = [0,0,0]

    def __init__(self, xQueue: Queue, yQueue: Queue, zQueue: Queue):
        super(GyroMainWindow, self).__init__()
        self.xQueue = xQueue
        self.yQueue = yQueue
        self.zQueue = zQueue
        uic.loadUi(os.path.join(os.path.dirname(__file__), "window.ui"), self)

        screen_rect = QtWidgets.QDesktopWidget().screenGeometry(-1)
        width = int(screen_rect.width() * 0.6)
        frame_width = int(width)

        height = int(screen_rect.height() * 0.6)
        frame_height = int(height)
        self.setGeometry(0, 0, width, height)
        self.setMinimumSize(QtCore.QSize(width, height))

        central_layout = QtWidgets.QVBoxLayout(self.centralwidget)
        central_layout.setContentsMargins(0, 0, 0, 0)

        self.plot_widget = pg.PlotWidget()
        central_layout.addWidget(self.plot_widget)

        self.plot_widget.setBackground("black")
        self.plot_widget.setLabel("left", "Gyroscope", units="°/s")
        # self.plot_widget.hideAxis('bottom')

        self.gyro_graph = pg.BarGraphItem(
            x=range(3), height=[0, 0, 0], width=0.3, brush="green"
        )
        
        self.plot_widget.addItem(self.gyro_graph)

        self.timer = pg.QtCore.QTimer()
        self.timer.timeout.connect(self.update)
        self.timer.start(0)

    def update(self): 
        changed = False     
        if not self.xQueue.empty():
            self.gyro_data[0] = self.xQueue.get_nowait() 
            changed = True
        if not self.yQueue.empty():
            self.gyro_data[1] = self.yQueue.get_nowait() 
            changed = True
        if not self.zQueue.empty():
            self.gyro_data[2] = self.zQueue.get_nowait() 
            changed = True

        if changed == True:
            self.gyro_graph.setOpts(height=self.gyro_data, x=range(3), brush='blue', labels={i: f"Bar {i}" for i in self.axis})
        else:
            return

class ECGMainWindow(QtWidgets.QMainWindow):
    ecg_plot_data = np.zeros(100)
    ecg_data = 0

    def __init__(self, ecgQueue: Queue):
        super(ECGMainWindow, self).__init__()
        self.ecgQueue = ecgQueue
        uic.loadUi(os.path.join(os.path.dirname(__file__), "window.ui"), self)

        screen_rect = QtWidgets.QDesktopWidget().screenGeometry(-1)
        width = int(screen_rect.width() * 0.6)
        frame_width = int(width)

        height = int(screen_rect.height() * 0.6)
        frame_height = int(height)
        self.setGeometry(0, 0, width, height)
        self.setMinimumSize(QtCore.QSize(width, height))

        central_layout = QtWidgets.QVBoxLayout(self.centralwidget)
        central_layout.setContentsMargins(0, 0, 0, 0)

        self.plot_widget = pg.PlotWidget()
        central_layout.addWidget(self.plot_widget)

        self.plot_widget.setBackground("white")
        self.plot_widget.setLabel("left", "Amplitude", units="V")
        self.plot_widget.setLabel("bottom", "Time", units="s")

        self.plot_curve = self.plot_widget.plot(self.ecg_plot_data,pen=pg.mkPen('black',width=3))


        self.timer = pg.QtCore.QTimer()
        self.timer.timeout.connect(self.update)
        self.timer.start(0)

    def update(self):
        if not self.ecgQueue.empty():
            self.ecg_plot_data[:-1] = self.ecg_plot_data[1:]
            self.ecg_plot_data[-1] = self.ecgQueue.get_nowait()
            self.plot_curve.setData(self.ecg_plot_data)
        else:
            return

class MainWindow(QtWidgets.QMainWindow):
    top_plot_data = np.zeros(100)
    bottom_plot_data = np.random.normal(size=(10, 3))
    axis = ('x','y','z')
    ecg_data = 0
    accel_data = [0,0,9.81]
    gyro_data = [0,0,0]

    def __init__(self):
        super(MainWindow, self).__init__()
        uic.loadUi(os.path.join(os.path.dirname(__file__), "window.ui"), self)

        screen_rect = QtWidgets.QDesktopWidget().screenGeometry(-1)
        width = int(screen_rect.width() * 0.6)
        frame_width = int(width)

        height = int(screen_rect.height() * 0.6)
        frame_height = int(height / 2)
        self.setGeometry(0, 0, width, height)
        self.setMinimumSize(QtCore.QSize(width, height))

        central_layout = QtWidgets.QVBoxLayout(self.centralwidget)
        central_layout.setContentsMargins(0, 0, 0, 0)

        self.top_plot_widget = pg.PlotWidget()
        central_layout.addWidget(self.top_plot_widget)

        self.top_plot_widget.setBackground("white")
        self.top_plot_widget.setLabel("left", "Amplitude", units="V")
        self.top_plot_widget.setLabel("bottom", "Time", units="s")

        self.top_plot_curve = self.top_plot_widget.plot(self.top_plot_data,pen=pg.mkPen('black',width=3))

        self.bottom_plot_widget = pg.PlotWidget()
        central_layout.addWidget(self.bottom_plot_widget)

        self.bottom_plot_widget.setBackground("white")
        self.bottom_plot_widget.setLabel("left", "Acceleration", units="m/s^2")
        self.bottom_plot_widget.hideAxis('bottom')

        self.bot_plot_widget = pg.PlotWidget()
        central_layout.addWidget(self.bot_plot_widget)

        self.bot_plot_widget.setBackground("black")
        self.bot_plot_widget.setLabel("left", "Gyroscope", units="°/s")
        self.bot_plot_widget.hideAxis('bottom')

        self.accel_graph = pg.BarGraphItem(
            x=range(3), height=[0, 0, 0], width=0.3, brush="green"
        )
        
        self.bottom_plot_widget.addItem(self.accel_graph)

        self.top_plot_widget.setMinimumSize(400, 400)
        self.bottom_plot_widget.setMinimumHeight(frame_height)



        self.timer = pg.QtCore.QTimer()
        self.timer.timeout.connect(self.update)
        self.timer.start(0)

    def update(self):
         # Generate new data for the 2D plot
        readval = serial_data.readPort()

        print(readval)
        if len(readval) > 4 or len(readval) < 2:
            pass
        
        elif readval[0] == "Accel":
            readval.pop(0)
            if len(readval) == 3:
                self.accel_data = list(map(float, readval))

        elif readval[0] == "Gyro":
            readval.pop(0)
            if len(readval) == 3:
                self.gyro_data = list(map(float, readval))             
                   
        elif readval[0] == "ECG":
            readval.pop(0)
            self.ecg_data = float(readval[-1])
            

        self.top_plot_data[:-1] = self.top_plot_data[1:]
        self.top_plot_data[-1] = self.ecg_data
        self.top_plot_curve.setData(self.top_plot_data)

        # heights = np.random.normal(self.accel_data)
        
        self.accel_graph.setOpts(height=self.accel_data, x=range(3), brush='blue', labels={i: f"Bar {i}" for i in self.axis})

if __name__ == "__main__":

    app = QtWidgets.QApplication(sys.argv)

    window = MainWindow()  # Create an instance of our class

    screen_size = QtWidgets.QDesktopWidget().screenGeometry(-1)
    window_size = window.geometry()
    window.move(
        (screen_size.width() - window_size.width()) // 2,
        (screen_size.height() - window_size.height()) // 2,
    )
    window.show()
    sys.exit(app.exec_())
