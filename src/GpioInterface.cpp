#include "GpioInterface.hpp"

using namespace loom;

GpioInput::GpioInput(int pin): pin(pin), lastVal(0)
{}

void GpioInput::update(int data)
{
    int currentVal = digitalRead(pin);
    if(currentVal != lastVal)
    {
        lastVal = currentVal;
        delay(100); // debounce
        InputChannel::update(currentVal);
    }
}


void GpioInterface::init(JsonObject config)
{}

OutputChannel* GpioInterface::createOutput(JsonObject params)
{
    return nullptr;
}

InputChannel* GpioInterface::createInput(JsonObject params)
{
    int pin = params["pin"];
    GpioInput* input = new GpioInput(pin);
    inputs.push_back(input);
    return input;
}