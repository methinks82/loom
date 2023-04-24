/**
 * loom/Interface.hpp
 * 
 * Base for the interface class
 * Inherit from this class in order to implement your own custom Interfaces
 * 
 * @author Alex Schlieck
 * @date 2023-04-05
*/



/*         How to create a new interface
------------------------------------------------------

While loom comes with a number of interfaces, you can easily create your own custom interfaces
as well in order to add support for new devices or extra functionality. All it takes is some basic
C++ knowlege.

Naming Interfaces: The convention is to come up with a short one to two word name that describes the function
of the interface (Servo, Serial, Thermometer, etc.). This name should be specific (if it's for a stepper call it stepper, not motor) and unique. This will be used as the ID of the interface class. The name of the class itself
will be the name of the interface followed by the word interface, such as StepperInterface.

For the example we will create an interface called Widget.

1. Add two files to the project: create a new header (.h or .hpp) and a declaration file (.cpp). Call these WidgetInterface.hpp and WidgetInterface.cpp
Note: The self-registration function in step 4 MUST be called from a .cpp file to work, otherwise the
compiler will not call it correctly!

2. Create your new Interface Class: The class must inherit the Interface class (found in Interface.hpp), which means overloading the following functions:
    1. init - Takes and use any configuration parameters that apply to this interface
    2. createOutput - Create a point to send data if outputs are required (See next step). If this interface doesn't have
        outputs, return nullptr
    3. createInput - Tell the interface that a new input channel is required.
    4. linkChannels - Set up what outputs get data from the specified input channel
    5. checkUpdate - Gets called during the main loop. This is where we check if there's any new data to send out,
        or if the input channels need to do anything.

3. Create Output Channels: If the interface sends data OUT of the Arduino, then you need to create one or more Output Channel classes. These classes take a piece of data and do something, such as sending it to a GPIO pin or a serial port.
The new output channel inherits the OutputChannel class and implements one function:
    - call - Takes a piece of data from an Input Channel and does something with it

4. Loom provides a self-registration mechanism so there's no need to modify any existing code to add your new interface.
    To register the interface simply add the following line to a cpp file in your project (To keep things clean it is recommended that you create a .cpp file for each Interface)
      loom:InterfaceRegistration<WidgetInterface> widget("WidgetInterface");

    Notice that there are 3 places to modify:
    - WidgetInterface - The class to add
    - widget - A unique name for the global variable that will do the registration for you
    - "WidgetInterface" - The name (id) by which the configuration file will find the interface

After compiling and uploading the project to the Arduino, you can now create new Widget interfaces from the config

*/




#ifndef LOOM_INTERFACE_HPP
#define LOOM_INTERFACE_HPP

#include <ArduinoJson.h>

namespace loom
{
    /// @brief An object used to send data from the arduino to a device
    class OutputChannel
    {
    public:
        /// @brief tell the channel to send new data to the device
        /// @param data what is to be sent to the device
        virtual void call(int data) = 0;

        /// @brief ID to access this channel for dynamic management
        String id;
    };


    /// @brief A collection of inputs and outputs that connect to the same device or share functionality
    class Interface
    {
    public:
        /// @brief The name by which the interface is found during configuration
        /// Set this to whatever you want to call the interface (best done in the constructor)
        String id;

        /// @brief set up this interface
        /// @param config settings to be applied to this interface
        virtual void init(JsonObject config) = 0;

        /// @brief Create an output channel appropriate to the Interface type
        /// @param params settings to apply to this output
        /// @return pointer to new output if needed
        /// @return nullptr if this interface has no outputs
        virtual OutputChannel* createOutput(JsonObject params) = 0;

        /// @brief Creates a generic input channel
        /// @param params settings to apply to the input
        virtual void createInput(JsonObject params) = 0;

        /// @brief Define the connection between input and output channels
        /// @param inputId Name of the input that we are connecting
        /// @param targetOutput Pointer to the output object that we want this input to send data to
        virtual void linkChannels(const String& inputId, OutputChannel* targetOutput) = 0;

        /// @brief If there are inputs, check with source to see if there are any updates
        virtual void checkUpdate() = 0;
    };
}

// Remember to add the following line to a cpp file for the interface to register itself:
// loom:InterfaceRegistration<InterfaceClass> name("InterfaceName");

#endif