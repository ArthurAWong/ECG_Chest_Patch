from PyQt5 import QtWidgets, uic, QtCore
from pyqtgraph import PlotWidget
import pyqtgraph as pg
import sys
import os
import numpy as np


class MainWindow(QtWidgets.QMainWindow):
    top_plot_data = np.zeros(100)
    bottom_plot_data = np.random.normal(size=(10, 3))
    axis = ('x','y','z')
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

        self.top_plot_widget.setLabel("left", "Amplitude", units="V")
        self.top_plot_widget.setLabel("bottom", "Time", units="s")
        self.top_plot_widget.setBackground("white")
        self.top_plot_curve = self.top_plot_widget.plot(self.top_plot_data,pen=pg.mkPen('black',width=3))

        self.bottom_plot_widget = pg.PlotWidget()
        central_layout.addWidget(self.bottom_plot_widget)

        self.bottom_plot_widget.setLabel("left", "Acceleration", units="m/s^2")
        self.bottom_plot_widget.hideAxis('bottom')
        self.bottom_plot_widget.setBackground("white")
        self.bar_graph_item = pg.BarGraphItem(
            x=range(3), height=[0, 0, 0], width=0.3, brush="r"
        )
        
        self.bottom_plot_widget.addItem(self.bar_graph_item)

        self.top_plot_widget.setMinimumSize(400, 400)
        self.bottom_plot_widget.setMinimumHeight(frame_height)



        self.timer = pg.QtCore.QTimer()
        self.timer.timeout.connect(self.update)
        self.timer.start(50)

    def update(self):
        # Generate new data for the 2D plot
        self.top_plot_data[:-1] = self.top_plot_data[1:]
        self.top_plot_data[-1] = np.random.normal()
        self.top_plot_curve.setData(self.top_plot_data)

        heights = np.random.normal(size=(3,))
        x = np.arange(3)
        
        self.bar_graph_item.setOpts(height=heights, x=range(3), brush='black', labels={i: f"Bar {i}" for i in self.axis})

        

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
