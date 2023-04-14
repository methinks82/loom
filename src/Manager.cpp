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

    JsonArray interfaces = doc["interfaces"];

    Serial.print(interfaces.size());
    Serial.println(" interfaces found");

    loadInterfaces(doc["interfaces"]);
    //loadOutputs(doc["outputs"]); // now get created with interface
    loadInputs(doc["interfaces"]);

    Serial.println ("Initialization complete");
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
    for(JsonObject i : interfaceList)
    {
        String id = i["id"];
        String interfaceClass = i["class"];

        Serial.print("Interface <");
        Serial.print(interfaceClass);
        Serial.print("> ");
        Serial.println(id);
        Serial.println("--------");

        Interface * newInterface;

        // TODO: automate the loading of interfaces
        
        // check for each type of interface that is being used
        if(interfaceClass == "SerialInterface")
        {
            Serial.println(F("Loading Serial Interface"));
            newInterface = new SerialInterface;
        } 
        else if(interfaceClass == "GpioInterface")
        {
            Serial.println(F("Loading GPIO Interface"));
            newInterface = new GpioInterface;
        }/*
        else if(interfaceClass == "ScreenInterface")
        {
            Serial.println(F("Loading Screen Interface"));
            newInterface = new ScreenInterface;
        }*/
        else // no valid interface found
        {
            return;
        }
        newInterface->init(i);
        newInterface->id = id;
        loadOutputs(i["outputs"], newInterface);
        interfaces.push_back(newInterface);
    }
    
}

// Create and configure output channels
void Manager::loadOutputs(JsonArray outputList, Interface* interface)
{
    for(JsonObject outputConfig : outputList)
    {
        String id = outputConfig["id"];

        Serial.print(" Output ");
        Serial.println(id);

        OutputChannel * output = interface->createOutput(outputConfig);
        if(output != nullptr)
        {
            output->id = id;
            outputs.push_back(output);
        }
    }
}

// Create and configure input channels
void Manager::loadInputs(JsonArray interfaceList)
{   
    // check each interface
    for(JsonObject interfaceInfo : interfaceList)
    {
        String interfaceId = interfaceInfo["id"];
        Serial.print(F("Checking interface "));
        Serial.print(interfaceId);
        Serial.println(F(" for inputs"));

        // find the matching interface object
        for(Interface* interface : interfaces)
        {
            if(interface->id == interfaceId)
            {
                Serial.println("Instance found");

                // create each input
                JsonArray inputs = interfaceInfo["inputs"];
                for(JsonObject inputInfo : inputs)
                {
                    String inputId = inputInfo["id"];
                    Serial.print("Found input ");
                    Serial.println(inputId);
                    interface->createInput(inputInfo);

                    // once th input is created, we link it to any listening outputs
                    JsonArray listenerList = inputInfo["outputs"];
                    Serial.print(F("Listeners found: "));
                    Serial.println(listenerList.size());

                    // cycle through each listener in the list
                    createLinks(interface, inputInfo);
                }
                break;
            }
        }
    }
}

void Manager::createLinks(Interface* interface, JsonObject inputInfo)
{
    // for each output
    String inputId = inputInfo["id"];
    JsonArray outputList = inputInfo["outputs"];

    for(String outputId : outputList)
    {
        Serial.println(outputId);
        //find the instance
        for(OutputChannel* output : outputs)
        {
            if(output->id == outputId) // we've found the correct output
            {
                Serial.println("Linking");
                interface->linkChannels(inputId, output);
            }
        }
    }
}