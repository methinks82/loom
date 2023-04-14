/**
 * loom/SerialInterface.cpp
 * 
 * Interface to provide communication to and from the serial port
 * 
 * @author Alex Schlieck
 * @date 2023-04-07
*/

#ifndef LOOM_SERIAL_INTERFACE_HPP
#define LOOM_SERIAL_INTERFACE_HPP

#include <vector>

#include "Interface.hpp"

namespace loom
{
    /// @brief OutputChannel to transmit data via serial
    class SerialOutput : public OutputChannel
    {
        /// @brief Send data to serial port
        /// @param data data to be transmitted
        virtual void call(int data)
        {
            Serial.print(data);
        }
    };

    /// @brief Used to read and write serial port connection
    class SerialInterface : public Interface
    {
    public:
        /// @brief Set everything up
        /// @param config string containing settings
        virtual void init(JsonObject config) override
        {
            String msg = config["msg"];
            Serial.println(msg);
        }

        /// @brief Create an OutputChannel that sends data to serial port
        /// @param params settings to be applied to the output
        /// @return pointer to the instance of the new OutputChannel
        virtual OutputChannel* createOutput(JsonObject params) override
        {

            SerialOutput * output = new SerialOutput;
            String id = params["id"];
            return output;
        }

        /// @brief Create a channel that reads recieved data
        /// @param params configuration settings
        virtual void createInput(JsonObject params)
        {
            // stub
            // while there is only one channel the checkUpdate function can deal with everything
        }

        /// @brief Configure the flow between input and output channels
        /// @param inputId name of the input channel to be linked
        /// @param targetOutput pointer to the output channel to be sent to
        virtual void linkChannels(const String& inputId, OutputChannel* targetOutput)
        {
            // using single list of listeners as there are not individual inputs
            outputs.push_back(targetOutput);
        }

        /// @brief check if there is any recieved data, forward to the listeners if there is
        virtual void checkUpdate()
        {
            //Check if there's anything in the serial buffer
            if(Serial.available() > 0)
            {
                int data = Serial.read();

                // for now we only have one channel

                for(auto output: outputs)
                {
                    output->call(data);
                }
            }
        }


    private:
        std::vector<OutputChannel*> outputs;
    };
}

#endif
