/**
 * loom/Manager.cpp
 * 
 * Load and configure the interfaces and channels
 * 
 * @author Alex Schlieck
 * @date 2023-04-09
*/

#include "Manager.hpp"

using namespace loom;

// Initialize and configure Interfaces and channels from configuration file
void Manager::loadConfig(char* config)
{
    StaticJsonDocument<800> doc;
    DeserializationError error = deserializeJson(doc, config);
    if(error)
    { 
        Serial.print(F("Unable to load config: "));
        Serial.println(error.f_str());
        return;
    }

    loadInterfaces(doc["interfaces"]);
    loadOutputs(doc["outputs"]);
    loadInputs(doc["inputs"]);
}

// Check each Interface to see if it received any updates  
void Manager::checkUpdates()
{
    for(auto i : interfaces)
    {
        i->checkUpdate();
    }
}

// Load and configure the required Interfaces
void Manager::loadInterfaces(JsonArray interfaceList)
{
    if(!interfaceList.isNull())
    {
        for(JsonObject i : interfaceList)
        {
            String interfaceClass = i["class"];
            Interface * newInterface;

            // TODO: automate the loading of interfaces
            
            // check for each type of interface that is being used
            if(interfaceClass == "SerialInterface")
            {
                Serial.println(F("Loading serial interface"));
                newInterface = new SerialInterface;
            } 
            newInterface->init(i);
            String id = i["id"];
            newInterface->id = id;
            interfaces.push_back(newInterface);
        }
    }
}

// Create and configure output channels
void Manager::loadOutputs(JsonArray outputList)
{
    if(!outputList.isNull())
    {
        for(JsonObject outputConfig : outputList)
        {
            String parent = outputConfig["interface"];

            for(auto i : interfaces)
            {
                Serial.print("checking ");
                Serial.println(i->id);

                if(parent.compareTo(i->id) == 0)
                {
                    OutputChannel* output = i->createOutput(outputConfig);
                    if(output != nullptr)
                    {
                    outputs.push_back(output);
                    }
                }
            }
        }
    }
}

// Create and configure input channels
void Manager::loadInputs(JsonArray inputList)
{
    if(!inputList.isNull())
    {
        for(JsonObject inputConfig : inputList)
        {
            String parent = inputConfig["interface"];

            for(auto i : interfaces)
            {
                if(parent.compareTo(i->id) == 0)
                {
                    Serial.println("Loading input");
                    InputChannel* input = i->createInput(inputConfig);
                    if(input != nullptr)
                    {
                        linkChannels(input, inputConfig["outputs"]);
                    }
                }
            }
        }
    }
}

// Link an input channel to an output channel
void Manager::linkChannels(InputChannel* input, JsonArray outputList)
{
    for(String outputId : outputList)
    {
        for(OutputChannel* output : outputs)
        {
            Serial.print(outputId);
            Serial.print(" = ");
            Serial.println(output->id);

            if(output->id == outputId)
            {
                Serial.println("Linked");
                input->registerOutput(output);
            }
        }
    }
}