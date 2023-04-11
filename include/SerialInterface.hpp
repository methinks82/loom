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

#include "Interface.hpp"

namespace loom
{
    class SerialOutput : public OutputChannel
    {
        virtual void call(int data)
        {
            Serial.print(data);
        }
    };

    class SerialInterface : public Interface
    {
    public:
        virtual void init(JsonObject config) override
        {
            String msg = config["msg"];
            Serial.println(msg);
        }

        virtual void checkUpdate()
        {
            //Check if there's anything in the serial buffer
            if(Serial.available() > 0)
            {
                int data = Serial.read();

                // figure out how to sort data to channels.
                // for now we send to all
                for(auto i: inputs)
                {
                    i->update(data);
                }
            }
        }

        virtual OutputChannel* createOutput(JsonObject params) override
        {

            SerialOutput * output = new SerialOutput;
            String id = params["id"];
            output->id = id;

            Serial.print("Creating Serial::OutputChannel ");
            Serial.println(id);            

            return output;
        }
    };
} // namespace loom

#endif
