/**
 * loom/Manager.cpp
 *
 * Load and configure the interfaces and channels
 * 
 * @author Alex Schlieck
 * @date 2023-04-09
*/

#ifndef LOOM_MANAGER_HPP
#define LOOM_MANAGER_HPP

#include <ArduinoJson.h>
#include <vector>

// list all the interfaces that should be used
#include "SerialInterface.hpp"
//#include "GpioInterface.hpp"
//#include "ScreenInterface.hpp"

namespace loom
{
    class Manager
    {
    public:

        /// @brief Initialize and configure Interfaces and channels from configuration file
        /// @param config String containing the configuration
        void loadConfig(char* config);
        
        /// @brief Check each Interface to see if it received any updates
        void checkUpdates();

    private:
        /// @brief Load and configure the required Interfaces
        /// @param interfaceList List of all the required Interfaces and their settings
        void loadInterfaces(JsonArray interfaceList);

        /// @brief Create and configure output channels
        /// @param outputList List of all the required output channels and their settings
        void loadOutputs(JsonArray outputList, Interface* interface);
 
        /// @brief Create and configure input channels
        /// @param inputList List of all the required input channels and their settings
        void loadInputs(JsonArray inputList);

        /// @brief Link an input channel to an output channel
        /// @param input pointer to the input channel
        /// @param outputList list of all outputs to be linked to this input
        //void linkChannels(InputChannel* input, JsonArray outputList);
   
        std::vector<Interface*> interfaces;
        std::vector<OutputChannel*> outputs;
    };
}

#endif
