/**
 * loom/InputChannel.cpp
 * 
 * Receives input from Interface and sends it
 * to any listening chanels
 * 
 * @author Alex Schlieck
 * @date 2023-04-07
*/

#include "InputChannel.hpp"

using namespace loom;

// add an output that the data will get sent on to
void InputChannel::registerOutput(OutputChannel* output)
{
    outputs.push_back(output);
}

// alert this channel that there is new data
void InputChannel::update(int data)
{
    for(auto out : outputs)
    {
        out->call(data);
    }
}