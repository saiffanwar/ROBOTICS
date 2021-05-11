# import serial
# from serial.tools import list_ports

# for com in list_ports.comports():
#     print('port',com)
# # while True:
#     # try:
# with serial.Serial('/dev/cu.usbmodem1421301') as ser:
#     for i in range(564):
#         x = ser.read(i)
#         if (len(x.decode())==0):
#             val = 0
#         else:
#             val = x
#         print(x.decode())
#         # s = ser.read()        # read up to ten bytes (timeout)
#         line = ser.readline()   # read a '\n' terminated line
#     # except:
#     #     pass

from coords import coords_list
from matplotlib import pyplot as plt
coords_list = [i for i in coords_list if (((i[0]) and (i[1])) !=0)]
print(coords_list)
xs = [i[0] for i in coords_list]
ys = [i[1] for i in coords_list]

plt.scatter(xs,ys)
plt.show()