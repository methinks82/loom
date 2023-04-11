#ifndef LOOM_GPIO_INTERFACE_HPP
#define LOOM_GPIO_INTERFACE_HPP

#include <Arduino.h>
#include "Interface.hpp"

namespace loom
{
    class GpioOutput: public OutputChannel
    {
    public:
        GpioOutput(int pin):pin(pin) {}

        virtual void call(int data)
        {
            digitalWrite(pin, data);
        }

    private:
        int pin;
    };

    class GpioInput: public InputChannel
    {
    public:
        GpioInput(int pin): pin(pin) {}

        virtual void update(int data)
        {
            int currentVal = digitalRead(pin);
            if(lastVal != currentVal)
            {
                Serial.println("Reading input");
                InputChannel::update(currentVal);
                lastVal = currentVal;
            }
        }

    private:
        int pin;
        int lastVal;
    };

    class GpioInterface: public Interface
    {
    public:
        virtual void init(JsonObject config)
        {}

        virtual OutputChannel* createOutput(JsonObject params) override
        {
            int pin = params["pin"];
            pinMode(pin, OUTPUT);
            GpioOutput * output = new GpioOutput(pin);
            return output;
        }

        virtual InputChannel* createInput(JsonObject params) override
        {
            int pin = params["pin"];
            Serial.print("gpio::Input, pin ");
            Serial.println(pin);
            pinMode(pin, INPUT);
            GpioInput* input = new GpioInput(pin);
            return input;
        }

        virtual void checkUpdate()
        {
            for(auto i : inputs)
            {
                i->update(0);
            }
        }
    };
}

#endif