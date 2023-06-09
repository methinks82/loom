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
#include <EEPROM.h>
#include "logging.h"
#include "InterfaceRegistry.hpp"

#include <U8x8lib.h>

namespace loom
{
    class Manager
    {
    public:
        /// @brief Get configuration and set up interfaces and channels accordingly
        /// @param config string containing the default config
        void setup();

        /// @brief Check each Interface to see if it received any updates
        void mainLoop();

    private:
        /// @brief get the valid coniguration file and set up the program accordingly
        void loadConfig();

        /// @brief Check if there is a new configuration available, load if so
        /// @return was a new configuration loaded        
        bool getConfigUpdate(String& config);

        /// @brief read the configuration file that is stored in memory
        /// @param config 
        /// @return 
        bool loadLocalConfig(String& config);

        /// @brief Create a simple numeric hash to represent the string. Used to verify integrity
        /// This will just be adding the values of each character. We don't care if it overflows
        /// @param msg String to be hashed
        /// @return 16bit unsigned number generated from the string
        uint16_t hashMessage(String& msg);

        /// @brief Write the given config to eeprom for future use
        /// @param config string to be saved        
        bool writeMemory(const String& data);

        /// @brief Create and configure interfaces and channels as per given string
        /// @param config string containing settings
        /// @return was the configuration successful        
        bool parseConfig(const String& config);

        /// @brief Load and configure the required Interfaces
        /// @param interfaceList List of all the required Interfaces and their settings
        void loadInterfaces(JsonArray interfaceList);

        /// @brief Create and configure output channels
        /// @param outputList List of all the required output channels and their settings
        void loadOutputs(JsonArray outputList, Interface* interface);
 
        /// @brief Create and configure input channels
        /// @param inputList List of all the required input channels and their settings
        void loadInputs(JsonArray inputList);

        /// @brief Link all inputs from given interface to required outputs
        /// @param input pointer to the input channel
        /// @param outputList list of all outputs to be linked to this input
        void createLinks(Interface* interface, JsonObject inputInfo);
   
        std::vector<Interface*> interfaces;
        std::vector<OutputChannel*> outputs;

        const unsigned int BLOCK_SIZE = 500;
        const unsigned int CONFIG_ADDRESS = 0;
    };
}

#endif
