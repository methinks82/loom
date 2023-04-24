import numpy as np
import serial

connection = serial.Serial("COM5", 115200, timeout=1)


# connection.dtr = False
# connection.rts = False
# connection.dtr = True
# connection.rts = True
val : np.uint16 = 1337
connection.write(str(val).encode())