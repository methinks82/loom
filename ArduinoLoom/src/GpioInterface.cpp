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

///// GpioOutput /////
// Cstr
GpioOutput::GpioOutput(int pin):pin(pin) {}

// Writes data to gpio pin
void GpioOutput::call(int data)
{
    data==0 ? digitalWrite(pin,LOW) : digitalWrite(pin, HIGH);
}

///// GpioInput /////
// cstr
GpioInput::GpioInput(int pin): pin(pin), lastVal(0) {}

// read and process current value
void GpioInput::update()
{
    int currentVal = digitalRead(pin);
    if(currentVal != lastVal)
    {
        lastVal = currentVal;
        for(auto output : outputs)
        {
            output->call(currentVal);
        }
        delay(50); //debounce
    }
}

///// GpioInterface /////
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
    String id = params["id"];
    int pin = params["pin"];
    pinMode(pin, INPUT);
    GpioInput* input = new GpioInput(pin);
    input->id = id;
    inputs.push_back(input);
}

// Configure the channels for data to flow between them
void GpioInterface::linkChannels(const String& inputId, OutputChannel* targetOutput)
{
    for(GpioInput* input : inputs)
    {
        if(input->id == inputId)
        {
            input->outputs.push_back(targetOutput);
        }
    }
}

// Check if there are any updates and pass them on to the proper channels
void GpioInterface::checkUpdate()
{
    for(auto i : inputs)
    {
        i->update();
    }
}

loom::InterfaceFactory<loom::GpioInterface>gpio("GpioInterface");