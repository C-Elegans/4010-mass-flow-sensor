import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import socket
import json

delay = 0.1
ymax = 2.5
max_entries = 100

savefile = open('data.txt', 'w')

plt.ion()
fig = plt.figure()
ax1 = fig.add_subplot(2, 1, 1)
ax2 = fig.add_subplot(2, 1, 2)

ydata = [0]
heater_data = [0]
massdata = [0]
angle_data = [0]

ax1.set_ylabel('delta T (C)')
ax1.set_xlabel('time (s)')
line_dt, = ax1.plot(ydata, c='g', label='delta t')
ax_heater = ax1.twinx()
ax_heater.set_ylabel('Heater power (%)')
line_heater, = ax_heater.plot(ydata, c='r', label='heater')
line_massflow, = ax2.plot(massdata, c='g', label='massflow')
ax2.set_ylabel('Mass flow rate')
ax2.set_xlabel('time (s)')
ax_angle = ax2.twinx()
line_angle, = ax_angle.plot(ydata, c='b', label='angle')
ax_angle.set_xlabel('Angle (deg)')
ax1.set_ylim([-ymax, ymax])
ax_heater.set_ylim([0, 100])
ax2.set_ylim([-4, 4])
ax_angle.set_ylim([0, 300])

ax1_lines = [line_dt, line_heater]
ax1_labels = [l.get_label() for l in ax1_lines]
ax1.legend(ax1_lines, ax1_labels)
ax2.legend()
ax2_lines = [line_heater, line_massflow]
ax2_labels = [l.get_label() for l in ax2_lines]
ax2.legend(ax2_lines, ax2_labels)
plt.show()

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

s.connect(("192.168.6.2", 8081))

s.send(b"start\n")

while True:
    chunk = s.recv(128)
    chunk = chunk.decode('utf-8')
    data = json.loads(chunk)

    print(data)
    ydata.append(data['dt'])
    massdata.append(data['massflow'])
    heater_data.append(data['heater_mult'] * 100)
    angle_data.append(data['angle'])
    line_dt.set_ydata(ydata)
    line_dt.set_xdata(np.arange(len(ydata)) * delay)
    line_heater.set_ydata(heater_data)
    line_heater.set_xdata(np.arange(len(ydata)) * delay)
    line_massflow.set_ydata(massdata)
    line_massflow.set_xdata(np.arange(len(massdata)) * delay)
    line_angle.set_ydata(angle_data)
    line_angle.set_xdata(np.arange(len(angle_data)) * delay)

    maxtime = len(ydata) * delay
    ax1.set_xlim([max(0, maxtime - max_entries), maxtime])
    ax2.set_xlim([max(0, maxtime - max_entries), maxtime])
    plt.pause(0.0001)





