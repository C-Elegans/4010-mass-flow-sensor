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
line_dt, = ax1.plot(ydata, c='g', label='delta t')
ax_heater = ax1.twinx()
line_heater, = ax_heater.plot(ydata, c='r', label='heater %')
line_massflow, = ax2.plot(massdata)
ax1.set_ylim([-ymax, ymax])
ax_heater.set_ylim([0, 1])
ax2.set_ylim([-4, 4])
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
    heater_data.append(data['heater_mult'])
    line_dt.set_ydata(ydata)
    line_dt.set_xdata(np.arange(len(ydata)) * delay)
    line_heater.set_ydata(heater_data)
    line_heater.set_xdata(np.arange(len(ydata)) * delay)
    line_massflow.set_ydata(massdata)
    line_massflow.set_xdata(np.arange(len(massdata)) * delay)

    maxtime = len(ydata) * delay
    ax1.set_xlim([max(0, maxtime - max_entries), maxtime])
    ax2.set_xlim([max(0, maxtime - max_entries), maxtime])
    plt.pause(0.0001)





