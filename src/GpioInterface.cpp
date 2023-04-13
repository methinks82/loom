/**
 * loom/GpioInterface.cpp
 * 
 * Interface to read and write the arduino's gpio pins
 * 
 * @author Alex Schlieck
 * @date 2023-04-12
*/

#include "GpioInterface.hpp"

using namespace loom;

GpioOutput::GpioOutput(int pin):pin(pin) {}

void GpioOutput::call(int data)
{
    data==0 ? digitalWrite(pin,LOW) : digitalWrite(pin, HIGH);
}

GpioInput::GpioInput(int pin): pin(pin), lastVal(0) {}

void GpioInput::update(int data)
{
    int currentVal = digitalRead(pin);
    if(currentVal != lastVal)
    {
        lastVal = currentVal;
        delay(50); // debounce
        //InputChannel::update(currentVal);
        //TODO: update all listeners
    }
}

// Set up this interface to use the parameters
void GpioInterface::init(JsonObject config)
{}

// create an output that writes to gpio pin
OutputChannel* GpioInterface::createOutput(JsonObject params)
{
    int pin = params["pin"];
    pinMode(pin, OUTPUT);
    GpioOutput* out = new GpioOutput(pin);
    return out;
}

// create an input that reads from gpio pin
void GpioInterface::createInput(JsonObject params)
{
    int pin = params["pin"];
    pinMode(pin, INPUT);
    GpioInput* input = new GpioInput(pin);
    inputs.push_back(input);
}