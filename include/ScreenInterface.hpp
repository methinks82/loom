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

namespace loom
{
    class ScreenOutput : public OutputChannel
    {
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
        /// @param params 
        /// @return pointer to the newly created ScreenOutput
        OutputChannel* createOutput(JsonObject params) override;

        void CreateInput(JsonObject params) {} // stub, not used

        // no inputs, not used
        virtual void checkUpdate();
    };
}

#endif