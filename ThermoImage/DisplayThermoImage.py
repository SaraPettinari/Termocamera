import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import serial
import re

ser = serial.Serial("COM3", 9600)

#lettura del seriale di arduino
def readSerial():
    pixels = []
    while len(pixels) != 8:
        cc = str(ser.readline())
        cc = cc[2:][:-5]
        array = str.split(cc, ',')
        if len(array) == 8:
            pixels.append(array)
    print(pixels)
    return pixels


pixels = readSerial()

fig = plt.figure()
values = np.array(pixels, dtype=float)
# inferno: tipo di gradazione di colore
# interpolation: quanto sono sfuocati i pixel
im = plt.imshow(values, vmin = 20, vmax = 35, interpolation='spline36',
                cmap='inferno', animated='True')


def updatefig(*args):
    im.set_array(np.array(readSerial(), dtype=float))
    return im,


ani = animation.FuncAnimation(
    fig, updatefig, interval=10, blit=True)

plt.show()
