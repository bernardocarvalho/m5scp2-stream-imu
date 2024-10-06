# import socket
import numpy as np
import pyqtgraph as pg
# from pyqtgraph.Qt import QtCore, QtWidgets
from PyQt6 import QtNetwork, QtWidgets


# bind all IP
HOST = '0.0.0.0'
# Listen on Port
PORT = 44444
# Size of receive buffer
BUFFER_SIZE = 32  # 2 int  + 6 float32 -axis IMU
N_AXIS = 3
N_SAMPLES = 2
COLORS = ("r","g","b")
# Create a TCP/IP socket
socket = QtNetwork.QUdpSocket()

# Always start by initializing Qt (only once per application)
app = QtWidgets.QApplication([])

# Define a top-level widget to hold everything
w = QtWidgets.QWidget()
w.setWindowTitle('M5Stick IMU Data')

# Create some widgets to be placed inside
btn = QtWidgets.QPushButton('press me')
text = QtWidgets.QLineEdit('enter text')
# listWidget = QtWidgets.QListWidget()
plota = pg.PlotWidget()
plotg = pg.PlotWidget(title="Gyro Data")

# Create a grid layout to manage the widgets size and position
layout = QtWidgets.QGridLayout()
w.setLayout(layout)

# Add widgets to the layout in their proper positions
layout.addWidget(btn, 0, 0)  # button goes in upper-left
layout.addWidget(text, 1, 0)  # text edit goes in middle-left
# layout.addWidget(listWidget, 2, 0)  # list widget goes in bottom-left
layout.addWidget(plota, 0, 1)
layout.addWidget(plotg, 1, 1)

data_a = np.zeros((3, 300))
curve_a = []
data_g = np.zeros((3, 300))
curve_g = []
for i in range(N_AXIS):
    curve_a.append(plota.plot(data_a[i], pen=COLORS[i]))
    curve_g.append(plotg.plot(data_g[i], pen=COLORS[i]))
ptr1 = 0


def process_datagrams():
    global socket, data_a, data_g, ptr1
    try:
        while socket.hasPendingDatagrams():
            datagram = socket.receiveDatagram()
            udat = np.frombuffer(datagram.data(), dtype=np.float32)
            idat = np.frombuffer(datagram.data(), dtype=np.int32)
            data_a = np.roll(data_a, -N_SAMPLES, axis=1)  # rotate circular buffer
            data_g = np.roll(data_g, -N_SAMPLES, axis=1)  # rotate circular buffer
            ptr1 += 1
            print(f"I: {idat[0]/1000.0:.2f}")
            for i in range(N_AXIS):
                data_a[i, -2] = udat[2 + i]
                data_a[i, -1] = udat[2 + 2 * N_AXIS + i]
                curve_a[i].setData(data_a[i])
                data_g[i, -2] = udat[2 + N_AXIS + i]
                data_g[i, -1] = udat[2 + 3 * N_AXIS + i]
                curve_g[i].setData(data_g[i])
    except KeyboardInterrupt:
        print("Interrupt 0 ")


socket.bind(QtNetwork.QHostAddress.SpecialAddress.Any, PORT)
socket.readyRead.connect(process_datagrams)


# Display the widget as a new window
w.show()

# Start the Qt event loop
app.exec()  # or app.exec_() for PyQt5 / PySide2

