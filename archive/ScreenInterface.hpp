/**
 * loom/ScreenInterface.hpp
 * 
 * Send data to an attached screen
 * Currently limited to text
 * Based on the U8x8 library
 * 
 * @author Alex Schlieck
 * @date 2023-04-08
*/

#ifndef LOOM_SCREEN_INTERFACE_HPP
#define LOOM_SCREEN_INTERFACE_HPP

#include <U8x8lib.h>
#include "Interface.hpp"
#include "InterfaceRegistry.hpp"

namespace loom
{
    /// @brief OutputChannel to send data to screen
    class ScreenOutput : public OutputChannel
    {
        /// @brief Send data to screen
        /// @param data Message to be displayed on screen
        virtual void call(int data) override;
    };

    /// @brief Used to send text to an attached screen
    class ScreenInterface : public Interface
    {
    public:
        /// @brief configure any setting required for the screen to work
        /// @param config a list of required settings
        virtual void init(JsonObject config);

        /// @brief create a channel that will output to the screen
        /// @param params settings to be applied to the output channel
        /// @return pointer to the newly created ScreenOutput
        virtual OutputChannel* createOutput(JsonObject params) override;

        /// @brief STUB: Screen does not have inputs, this is not used
        /// @param params Settings for inputs, not used
        virtual void createInput(JsonObject params) {}

        /// @brief STUB:
        /// @param  
        /// @param  
        virtual void linkChannels(const String&, OutputChannel*) {}

        /// @brief STUB:
        virtual void checkUpdate() {}
    };
}

#endif