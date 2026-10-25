import pyqtgraph as pg
from pyqtgraph.Qt import QtCore, QtWidgets
import serial
import sys

# Setup Serial
ser = serial.Serial('COM3', 115200, timeout=1)
pg.setConfigOption('background', 'w')  # Hintergrund: Weiß
# Setup Qt Application
app = QtWidgets.QApplication([])
win = pg.GraphicsLayoutWidget(title="STM32 Live-Daten")
win.resize(800, 400)
plot = win.addPlot(title="FusedPitch (°) & GyroY (°/s)")
plot.showGrid(x=True, y=True)

curve_pitch = plot.plot(pen='b', name='Pitch')
curve_gyro  = plot.plot(pen='r', name='Gyro')

pitch_data = []
gyro_data = []
max_points = 200

def update():
    line = ser.readline().decode(errors='ignore').strip()
    print("RAW:", line)
    if "FusedPitch=" in line and "GyroY=" in line:
        try:
            parts = [p.strip() for p in line.split(',')]
            pitch = float(next(p.split('=')[1].rstrip('°') for p in parts if p.startswith("FusedPitch=")))
            gyro  = float(next(p.split('=')[1].split('°')[0] for p in parts if p.startswith("GyroY=")))

            pitch_data.append(pitch)
            gyro_data.append(gyro)
            if len(pitch_data) > max_points:
                pitch_data.pop(0)
                gyro_data.pop(0)

            x = list(range(len(pitch_data)))
            curve_pitch.setData(x, pitch_data)
            curve_gyro.setData(x, gyro_data)
        except Exception as e:
            print("Fehler beim Parsen:", e)

timer = QtCore.QTimer()
timer.timeout.connect(update)
timer.start(30)  # 30 ms → ca. 33 FPS

win.show()
sys.exit(app.exec_())
