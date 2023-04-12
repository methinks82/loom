/**
 * loom/Interface.cpp
 * Base for the interface class
 * Includes definitions for input and output channels
 * 
 * @author Alex Schlieck
 * @date 2023-04-05
*/

#include "Interface.hpp"

using namespace loom;

// Create a stub of an output in case this interface doesn't require output
// Otherwise override this with the functionality that is required
OutputChannel* Interface::createOutput(JsonObject params)
{
    return nullptr;
}

// Creates a generic input channel
// Override for custom functionality
InputChannel* Interface::createInput(JsonObject params)
{
    String id = params["id"];
    
    InputChannel * input = new InputChannel;

    Serial.print(F("Creating Interface::InputChannel "));
    Serial.println(id);

//    input->id = id;
    inputs.push_back(input);

    return input;
}

void Interface::checkUpdate()
{
    for(auto i : inputs)
    {
        i->update(0);
    }
}
