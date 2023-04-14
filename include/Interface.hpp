/**
 * loom/Interface.hpp
 * Base for the interface class
 * Includes definitions for input and output channels
 * 
 * @author Alex Schlieck
 * @date 2023-04-05
*/

#ifndef LOOM_INTERFACE_HPP
#define LOOM_INTERFACE_HPP

//#include <vector>
#include <ArduinoJson.h>
#include "OutputChannel.hpp"
//#include <string.h>

namespace loom
{
    /// @brief A collection of inputs and outputs that connect to the same device or ui
    class Interface
    {
    public:
        String id;

        /// @brief set up this interface
        /// @param config settings to be applied to this interface
        virtual void init(JsonObject config) = 0;

        /// @brief Create a stub of an output in case this interface doesn't require output
        /// To use a full output, overload this function
        /// @param params settings to apply to this output
        /// @return nullptr
        /// @return pointer to new output if using an overloaded function 
        virtual OutputChannel* createOutput(JsonObject params) = 0;

        /// @brief Creates a generic input channel
        /// Override to use custom input channels
        /// @param params settings to apply to the input
        virtual void createInput(JsonObject params) = 0;

        virtual void linkChannels(const String& inputId, OutputChannel* targetOutput) = 0;

        /// @brief If there are inputs, check with source to see if there are any updates
        virtual void checkUpdate() = 0;
    };
}

#endif