import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style

# Seriellen Port anpassen
ser = serial.Serial('COM3', 115200, timeout=1)

# Datenpuffer
MAX_POINTS = 200
pitch_data = []
gyro_data = []

# Initial Plot Setup
style.use('ggplot')
fig, ax = plt.subplots()
line_pitch, = ax.plot([], [], label='RAW', color='blue')
line_gyro,  = ax.plot([], [], label='GyroY (°/s)', color='red')
ax.set_xlim(0, MAX_POINTS)
ax.set_ylim(-100, 100)
ax.set_title("STM32 Live-Daten")
ax.set_xlabel("Samples")
ax.set_ylabel("Wert")
ax.legend(loc='upper right')
plt.tight_layout()

def update(frame):
    try:
        line = ser.readline().decode(errors='ignore').strip()
        print("RAW:", line)

        if "FusedPitch=" in line and "GyroY=" in line:
            parts = [p.strip() for p in line.split(',')]
            fused_str = next(p.split('=')[1].rstrip('°') for p in parts if p.startswith("FusedPitch="))
            gyro_str  = next(p.split('=')[1].split('°')[0] for p in parts if p.startswith("GyroY="))
            pitch = float(fused_str)
            gyro  = float(gyro_str)

            pitch_data.append(pitch)
            gyro_data.append(gyro)

            if len(pitch_data) > MAX_POINTS:
                pitch_data.pop(0)
                gyro_data.pop(0)

            x = list(range(len(pitch_data)))
            line_pitch.set_data(x, pitch_data)
            line_gyro.set_data(x, gyro_data)

            # Dynamisch X-Limit setzen (oder fix lassen wie oben)
            ax.set_xlim(max(0, len(pitch_data) - MAX_POINTS), len(pitch_data))

    except Exception as e:
        print("Fehler beim Parsen:", e)

    return line_pitch, line_gyro

ani = animation.FuncAnimation(fig, update, blit=True, interval=30)
plt.show()
