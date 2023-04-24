import pytest
from unittest.mock import MagicMock
import communication_functions as cf
import serial

def test_loadFile():
    assert cf.loadFile("tests/validData.json") == '{"data":"valid"}'

    with pytest.raises(Exception):
        cf.loadFile()

    with pytest.raises(Exception):
        cf.loadFile("invalid")


def test_format():
    data = '{  "data":"something",\n "array": [\n "item1", \n "item2" \n]}'
    result = '{"data":"something","array":["item1","item2"]}'

    assert cf.validateAndFormatJsonData(data) == result

    with pytest.raises(Exception):
        cf.validateAndFormatJsonData("")

    with pytest.raises(Exception):
        cf.validateAndFormatJsonData("Invalid")


def test_getMessage():
    connection = serial.Serial()

    val = "valid"
    inval = "invalid"
    connection.readline = MagicMock(return_value= val.encode())

    #valid data sent
    assert cf.getMessage(connection, val, 1) == True    
    #invalid data sent
    assert cf.getMessage(connection, inval, 1) == False

    #multiple values sent
    multiCalls = MagicMock()
    multiCalls.side_effect = [inval.encode(), val.encode()]
    connection.readline = multiCalls
    assert cf.getMessage(connection, val, 1) == True

def test_hashMessage():
    msg = "This is a test message. I wonder how it will hash? Hopefully well."

    assert cf.hashMessage(msg) == 5984
    # TODO: Test overflow


def test_transmitData():

    connection = serial.Serial()
    connection.readline = MagicMock()
    cmd = "CFG"
    msg = "Valid"
    invalid = "Invalid"

    multiMock = MagicMock()
    multiMock.side_effect = [cmd.encode(), msg.encode()]


    # test with no response
    assert cf.uploadRequestedData(connection, cmd, msg) == False

    # test with invalid response
    connection.readline = MagicMock(invalid.encode())
    assert cf.uploadRequestedData(connection, cmd, msg) == False

    # test with valid response but invalid confirmation
    connection.readline = multiMock
    with pytest.raises(Exception):
        cf.uploadRequestedData(connection, cmd, invalid)

    # test with valid response and valid confirmation
    connection.readline = multiMock
    # TODO: fix the following test
    # it seems to have a problem with readline, but only in this valid case
    # AttributeError: 'str' object has no attribute 'decode'. Did you mean: 'encode'?
    # assert cf.transmitData(connection, msg) == True
