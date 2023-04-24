# Loom
## Make your Arduino interact with the world around it without writing a single line of code

The Loom project is a zero-code solution that allows the user to use a graphic design tool to create an arduino project with multiple inputs and outputs including some of the following:
- GPIO pins
- Communication ports such as Serial, I2C, etc.
- Stepper and Servo motors
- Device hosted Web server
- Other devices that can be connected, such as sensors

>Please note that not all of these are available yet

## Getting Started
The project consists of two main parts:
- **Firmware**: A small program (or *sketch* in Arduino terms) that you upload to your device
- **InterfaceWeaver**: The application that you use on your computer to tell your board what to do, and to send this configuration to your board

Creating projects with Loom takes just a few simple steps:
1. **Upload the firmware:** Using a tool such as the Arduino IDE or PlatformIO, upload the firmware like you would any other project. **You only need to do this step once**
    > *In future we hope to automate this step directly from the InterfaceWeaver program*
2. **Describe how you want the data to flow:** You can do this either by writing a custom configuration file, or by using the graphical tools in InterfaceWeaver (*coming soon*)
3. **Send the configuration to the Arduino:** InterfaceWeaver does this for you. Simply select the serial settings like you would any other serial program (Port, Speed), Choose the file you created, and click upload.

The Arduino is now ready for you to build your creations!

## How it works
An Arduino using Loom communicates with the world around it using **Channels**.
There are three types of Channels:
- **Input Channels** that recieve data, such as button presses or sensor readings
- **Output Channels** that send data, such as lights, motors or display screens
- **Filters** take in input channel, modify the data, and send it on to an output channel
    > *Filters have not been implemented yet*

Channels are created and managed by **Interfaces** which group together channels that have something in common, such as using the same device (gpio pins or serial connection) or providing similar functionality (sending as formated signal to drive a specific device).

The user describes what inputs get sent to what outputs using a configuration file that is then sent to the Arduino. When the Arduino starts, it looks at the configuration file and connects the specified channels accordingly.

## Creating Configurations
Configuration files are writen using the JSON format.

In the following example the Arduino listens for data on the serial port, uses that data to turn an LED on or off, and sends the data right back on the serial port.

``` json
{
  "interfaces": [
    {
      "class": "SerialInterface",
      "id": "serial",
      "outputs": [
        {
          "id": "tx"
        }
      ],
      "inputs": [
        {
          "id": "rx",
          "links": [
            "led",
            "tx"
          ]
        }
      ]
    },
    {
      "class": "GpioInterface",
      "id": "serial",
      "outputs": [
        {
          "id": "led",
          "pin": 12
        }
      ]
    }
  ]
}
```
>*In future this may be changed to use YAML format*

We start with a list of Interfaces to use
- **class:** The type of interface to use
- **id:** What this instance of the interface is refered to
- **outputs:** A list of the output channels that are used
- **inputs:** A list of the input channels that are used
- **[Optional Parameters]** Any parameters that this type of interface needs to configure

Outputs
- **id:** What to refer to this channel as
- **[Optional Parameters]** Other values that this output needs to configure, such as pin number

Inputs
- **id:** What to refer to this channel as
- **links:** A list of the output channels to send data to
- **[Optional Parameters]** Other values that this Input needs to configure



### List of Interfaces

Interface Name | Description | Inputs | Outputs | Status
---|---|---|---|---
GpioInterface | Digital read and write the gpio pins on the board | Int | Int | Completed
SerialInterface | Read and write data via the serial port | RX | TX | Working, raw data only
ScreenInterface | Send data to an attached LED display using the U8x8 library | |Text| Missing config options
WebInterface | Simple webserver that provide HTML interface via WIFI ||| Planned
AnalogInterface | Analog read and write on GPIO pins |||Planned
ServoInterface | Send pwm signals via pin to set position of motor ||Position| Planned
StepperInterface | Send sequence of signals to move motor |[position]|position/speed|Planned


## How to create a new Interface
While loom comes with a number of interfaces, you can easily create your own custom interfaces
as well in order to add support for new devices or extra functionality. All it takes is some basic
C++ knowlege.

> **Naming Interfaces**  
The convention is to come up with a short one to two word name that describes the function
of the interface (Servo, Serial, Thermometer, etc.). This name should be specific (if it's for a stepper call it stepper, not motor) and unique. This will be used as the ID of the interface class. The name of the class itself will be the name of the interface followed by the word interface, such as StepperInterface.

For this example we will create an interface called Widget.

1. **Add two files to the project**  
Create a new header (.h or .hpp) and a declaration file (.cpp). Call these WidgetInterface.hpp and WidgetInterface.cpp
    >The self-registration function in step 4 MUST be called from a .cpp file to work, otherwise the
compiler will not call it correctly!  
2. **Create your new Interface Class**  
The class must inherit the Interface class (found in Interface.hpp), which means implementing the following functions:
    - init - Takes and use any configuration parameters that apply to this interface
    - createOutput - Create a point to send data if outputs are required (See next step). If this interface doesn't have
        outputs, return nullptr
    - createInput - Tell the interface that a new input channel is required.
    - linkChannels - Set up what outputs get data from the specified input channel
    - checkUpdate - Gets called during the main loop. This is where we check if there's any new data to send out, or if the input channels need to do anything.  

3. **Create Output Channels**  
If the interface sends data OUT of the Arduino, then you need to create one or more Output Channel classes. These classes take a piece of data and do something, such as sending it to a GPIO pin or a serial port.
The new output channel inherits the OutputChannel class and implements one function:
    - call - Takes a piece of data from an Input Channel and does something with it

4. **Register the new Interface**  
Loom provides a self-registration mechanism so there's no need to modify any existing code to add your new interface.
    To register the interface simply add the following line to a cpp file in your project (To keep things clean it is recommended that you create a .cpp file for each Interface)

    loom:InterfaceRegistration<***InterfaceClass***> **name** ( **"InterfaceName"** );  
    Notice that there are 3 values to fill in:
    - InterfaceClass - The class to register
    - name - A unique name for the global variable that will do the registration for you
    - "InterfaceName" - The name (id) by which the configuration file will find the interface  
    So registering our example Widget interface would look like this:
    ``` cpp
    loom:InterfaceRegistration<WidgetInterface> widget("WidgetInterface");
    ```

After compiling and uploading the project to the Arduino, you can now create new Widget interfaces from the config
