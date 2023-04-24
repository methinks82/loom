import serial       # Communicating with board
import time         # for setting timeouts
import asyncio      # Threading for monitor
import json_minify  # compress json
import json         # validate json
import numpy as np  #used for generating hash


# send a signal that will cause the device to reset
def sendReset(connection: serial.Serial):
    connection.dtr = False
    connection.rts = False
    connection.dtr = True
    connection.rts = True


# load a config string from file
def loadFile(filename):
    try:
        with open(filename, 'r') as f:
            data=f.read()
            f.close()
    
    except (FileNotFoundError, IOError, OSError) as e: 
        raise Exception('Unable to load file "' + filename +'"') from e
        
    return data


# format and validate the data
def validateAndFormatJsonData(data):
    try:
        json.loads(data) #try loading to make sure it's valid
    except json.decoder.JSONDecodeError:
        raise Exception("Invalid json string")

    result = json_minify.json_minify(data)
    return result


# wait for a specified message and return if found
def getMessage(connection: serial.Serial, msg, timeout):
    start = time.time()
    
    while time.time() - start < timeout:
        rx = connection.readline().decode().strip()
        print(rx)
        if rx == msg:
            return True
    return False

def hashMessage(message):
    hash:np.uint8 = 0
    for c in message:
        hash += ord(c)
    return hash


# send a string to a device if requested
def uploadRequestedData(connection: serial.Serial, requestString, data):
    print("Uploading config: " + data)

    print("Resetting board")
    sendReset(connection)

    if getMessage(connection, requestString, 3):
        print("Request recieved")
        connection.write(data.encode())
        time.sleep(0.5)
        hash = "\0 42\0"
        connection.write(hash.encode())

        print("TX: " + data)


        hash:np.uint8 = hashMessage(data)
        print("Hash: " + str(hash))



        # send the hash
        #hashBytes:np.uint8 = []
        #hashBytes[1] = hash & 0xFF
        #hashBytes[0] = (hash >> 8) & 0xFF
        #bytes[1] = (hash >> 8) & 0xFF
        #bytes[0] = hash & 0xFF

        ##connection.write( hash & 0xFF )        
        #connection.write( (hash >> 8) & 0xFF )


        #connection.write(bytes[0])
        #connection.write(bytes[1])

        #if getMessage(connection, data, 3):
        #    print("Payload verified")
        #    return True
        #else:
        #    print("Upload failed")
        #    raise Exception("Upload not verified")
        
    else:
        print("Request not recieved")

    return False


# read a config file and send it to a device via serial
def uploadConfigFile(port, speed, filename):
    print("Loading file: " + filename)

    status = "running"

    try:
        # load the data from file
        msg = loadFile(filename)
        
        # format (and validate) string
        minified = validateAndFormatJsonData(msg)
        
        # create connection
        print("Connecting to device on port " + port + " at " + str(speed))
        connection = serial.Serial(port, speed, timeout=1)
        if connection.isOpen() == False:
            raise Exception("Unable to create connection")



        # send data to device
        if uploadRequestedData(connection, "CFG", minified):
            status = "Config file uploaded"
        else:
            status = "Config file not uploaded (no request)"
  
        #start the monitoring loop
        connection.close()
        
    except Exception as e:
        print("###### ERROR ######\nConfig upload failed: ")
        print(e)
        return e.args
    
    return status


#monitor
class SerialMonitor:
    running = False
    connection = serial.Serial()

    async def loop(self, connection):
            while self.running:
                msg = connection.read()
                print(msg)
            self.connection.close()

    def start(self):
        self.running = True
        print("monitor started")
        #self.connection = serial.Serial("COM5", 115200)
        self.connection.port("COM5")
        self.connection.baudrate(115200)
        self.connection.open()
        asyncio.run(self.loop(self.connection))

    def stop(self):
        self.running = False
