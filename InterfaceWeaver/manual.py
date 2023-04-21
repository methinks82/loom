import communication_functions as cf
from unittest.mock import MagicMock
import unittest.mock
import serial

conn = serial.Serial("COM1", 115200, timeout=0.5)
#conn.readline = MagicMock("Valid")
inval = "invalid"
val = "valid"
#conn.readline = MagicMock(return_value=val.encode())
mock = MagicMock()
mock.side_effect = [inval.encode(), val.encode()]
conn.readline = mock
#conn.readline.return_value = val.encode()

print ("Starting")

#if cf.getMessage(conn, "valid", 2):
#    print("Wait Success")

if cf.uploadRequestedData(conn, "valid"):
    print ("Tx success")