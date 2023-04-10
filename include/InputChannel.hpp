/**
 * loom/InputChannel.hpp
 * 
 * Receives input from Interface and sends it
 * to any listening chanels
 * 
 * @author Alex Schlieck
 * @date 2023-04-07
*/

#ifndef LOOM_INPUTCHANNEL_HPP
#define LOOM_INPUTCHANNEL_HPP

#include <Arduino.h> // temp, serial for debugging

#include <vector>
#include "OutputChannel.hpp"

namespace loom
{
    /// @brief Allows the Arduino to recieve data from devices
    /// and forward it to the proper output devices
    class InputChannel
    {
    public:
        //InputChannel(String ) {Serial.println(F("Input Constructor"));}
        InputChannel() {};
        InputChannel(const String& id):id(id) {}


        /// @brief add an output that the data will get sent on to
        /// @param output channel to call when the data is updated
        void registerOutput(OutputChannel* output);

        /// @brief alert this channel that there is new data
        /// @param data data to be processed
        void update(int data);

        /// @brief ID to access this channel for dynamic management
        //char* id;
        String id;

    private:
        /// @brief a list of all the channels to forward the data to
        std::vector<OutputChannel*> outputs;
    };
}

#endif
