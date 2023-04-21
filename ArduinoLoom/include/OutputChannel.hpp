/**
 * loom/OutputChannel.hpp
 * 
 * Sends data to output devices / interfaces
 * 
 * @author Alex Schlieck
 * @date 2023-04-07
*/

#ifndef LOOM_OUTPUTCHANNEL_HPP
#define LOOM_OUTPUTCHANNEL_HPP

#include <Arduino.h>

namespace loom
{
    /// @brief Used to send data from the arduino to a device
    class OutputChannel
    {
    public:
        /// @brief tell the channel to send new data to the device
        /// @param data what is to be sent to the device
        virtual void call(int data) = 0;

        /// @brief ID to access this channel for dynamic management
        String id;
    };
} // namespace loom

#endif
