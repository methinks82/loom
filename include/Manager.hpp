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

#include "U8x8lib.h"

// list all the interfaces that should be used
#include "SerialInterface.hpp"
#include "GpioInterface.hpp"
//#include "ScreenInterface.hpp"

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
        void loadConfig();
        bool getConfigUpdate(String& config);
        bool loadLocalConfig(String& config);
        bool writeMemory(const String& data);
        void parseConfig(String& config);


        const int CFG_ADDRESS = 0;

/*

        /// @brief Check if there is a new configuration available, load if so
        /// @return was a new configuration loaded
        bool loadNewConfig();

        /// @brief Initialize and configure Interfaces and channels from saved configuration file
        void useExistingConfig();

        /// @brief Create and configure interfaces and channels as per given string
        /// @param config string containing settings
        /// @return was the configuration successful
        bool parseConfig(const String& config);

        /// @brief Write the given config to eeprom for future use
        /// @param config string to be saved
        void saveConfig(const String& config);
*/
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
