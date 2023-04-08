import sys
from multiprocessing import Process, Queue
from PyQt5 import QtWidgets

from ble_data2 import runBleakConnector

from demo import AccelMainWindow, GyroMainWindow, ECGMainWindow

class AccelGraphWindowProcess(Process):
    def __init__(self, xQueue, yQueue, zQueue):
        Process.__init__(self)
        self.xQueue = xQueue
        self.yQueue = yQueue
        self.zQueue = zQueue

    def run(self):
        self.app = QtWidgets.QApplication(sys.argv)
        self.window = AccelMainWindow(self.xQueue, self.yQueue, self.zQueue)  # Create an instance of our class

        self.screen_size = QtWidgets.QDesktopWidget().screenGeometry(-1)
        self.window_size = self.window.geometry()
        self.window.move(0, 0)
        self.window.show()
        sys.exit(self.app.exec_())

class GyroGraphWindowProcess(Process):
    def __init__(self, xQueue, yQueue, zQueue):
        Process.__init__(self)
        self.xQueue = xQueue
        self.yQueue = yQueue
        self.zQueue = zQueue

    def run(self):
        self.app = QtWidgets.QApplication(sys.argv)
        self.window = GyroMainWindow(self.xQueue, self.yQueue, self.zQueue)  # Create an instance of our class

        self.screen_size = QtWidgets.QDesktopWidget().screenGeometry(-1)
        self.window_size = self.window.geometry()
        self.window.move(700, 0)
        self.window.show()
        sys.exit(self.app.exec_())

class ECGGraphWindowProcess(Process):
    def __init__(self, ecgQueue):
        Process.__init__(self)
        self.ecgQueue = ecgQueue

    def run(self):
        self.app = QtWidgets.QApplication(sys.argv)
        self.window = ECGMainWindow(self.ecgQueue)  # Create an instance of our class

        self.screen_size = QtWidgets.QDesktopWidget().screenGeometry(-1)
        self.window_size = self.window.geometry()
        self.window.move(350, 400)
        self.window.show()
        sys.exit(self.app.exec_())

if __name__ == "__main__":
    accelXQueue = Queue()
    accelYQueue = Queue()
    accelZQueue = Queue()
    gyroXQueue = Queue()
    gyroYQueue = Queue()
    gyroZQueue = Queue()
    ecgQueue = Queue()
    queueDict = {
        "accelXQueue": accelXQueue,
        "accelYQueue": accelYQueue,
        "accelZQueue": accelZQueue,
        "gyroXQueue": gyroXQueue,
        "gyroYQueue": gyroYQueue,
        "gyroZQueue": gyroZQueue,
        "ecgQueue": ecgQueue
    }

    AccelGraphProcess = AccelGraphWindowProcess(accelXQueue, accelYQueue, accelZQueue)
    AccelGraphProcess.daemon = True
    AccelGraphProcess.start()
    GyroGraphProcess = GyroGraphWindowProcess(gyroXQueue, gyroYQueue, gyroZQueue)
    GyroGraphProcess.daemon = True
    GyroGraphProcess.start()
    ECGGraphProcess = ECGGraphWindowProcess(ecgQueue)
    ECGGraphProcess.daemon = True
    ECGGraphProcess.start()

    runBleakConnector(queueDict)

    AccelGraphProcess.kill()
    GyroGraphProcess.kill()
    ECGGraphProcess.kill()

    accelXQueue.close()
    accelYQueue.close()
    accelZQueue.close()
    gyroXQueue.close()
    gyroYQueue.close()
    gyroZQueue.close()
    ecgQueue.close()