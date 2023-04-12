#ifndef LOOM_GPIO_INTERFACE_HPP
#define LOOM_GPIO_INTERFACE_HPP

#include <Arduino.h>
#include "Interface.hpp"

namespace loom
{
    class GpioOutput: public OutputChannel
    {
    public:
        GpioOutput(int pin);
        virtual void call(int data);

    private:
        int pin;
    };

    class GpioInput: public InputChannel
    {
    public:
        GpioInput(int pin);
        virtual void update(int data);

    private:
        int pin;
        int lastVal;
    };

    class GpioInterface: public Interface
    {
    public:
        virtual void init(JsonObject config);
        virtual OutputChannel* createOutput(JsonObject params) override;
        virtual InputChannel* createInput(JsonObject param) override;
    };
}

#endif