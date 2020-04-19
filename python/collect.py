import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import socket

delay = 0.1
ymax = 4

savefile = open('data.txt', 'w')

plt.ion()
fig = plt.figure()
ax1 = fig.add_subplot(2, 1, 1)
ax2 = fig.add_subplot(2, 1, 2)

ydata = [0]
massdata = [0]
line1, = ax1.plot(ydata)
line2, = ax2.plot(massdata)
ax1.set_ylim([-ymax, ymax])
ax2.set_ylim([-4, 4])
plt.show()

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

s.connect(("192.168.6.2", 8081))

s.send(b"start\n")

while True:
    chunk = s.recv(32)
    chunk = chunk.decode('utf-8')

    y = [float(s) for s in chunk.split(' ')]
    savefile.write(chunk)
    print(y)
    ydata.append(y[0])
    massdata.append(y[1])
    line1.set_ydata(ydata)
    line1.set_xdata(np.arange(len(ydata)) * delay)
    line2.set_ydata(massdata)
    line2.set_xdata(np.arange(len(massdata)) * delay)
    ax1.set_xlim([0, len(ydata) * delay])
    ax2.set_xlim([0, len(ydata) * delay])
    plt.pause(0.0001)





