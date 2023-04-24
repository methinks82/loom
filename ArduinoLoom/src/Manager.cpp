/**
 * loom/Manager.cpp
 * 
 * Load and configure the interfaces and channels
 * 
 * @author Alex Schlieck
 * @date 2023-04-09
*/

#include "Manager.hpp"


U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(15, 4, 16); // configure screen

using namespace loom;

// Get configuration and set up interfaces and channels accordingly
void Manager::setup()
{
    u8x8.begin();
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.clear();
    u8x8.drawString(0,0, "Start");

    loadConfig();
}

// Check each Interface to see if it received any updates  
void Manager::mainLoop()
{
    for(auto i : interfaces)
    {
        i->checkUpdate();
    }
}

// get the valid coniguration file and set up the program accordingly
void Manager::loadConfig()
{
    //u8x8.drawString(0,0,"Loading");
    EEPROM.begin(BLOCK_SIZE);

    String config;
    if(getConfigUpdate(config))
    {
        parseConfig(config);     
    }
    else if(loadLocalConfig(config))
    {
        parseConfig(config);
    }
    else
    {
        LOG("Config not found");
    }
}

// Check if there is a new configuration available, load if so
bool Manager::getConfigUpdate(String& config)
{  
    u8x8.drawString(0,0,"Waiting for update");
    const char REQUEST_STRING[] = "CFG";
    
    Serial.println(); // make sure our request is on it's own line
    Serial.println(REQUEST_STRING);
    delay(800); // give host a chance to respond
    if( Serial.available() > 0) // host sent response
    {
        // get data from host
        String rx = Serial.readString();
        //uint8_t recievedHash = Serial.readBytes();
        //String hash = Serial.readString();

        
        
        // check message validity
        uint16_t calcHash = hashMessage(rx);
          
        u8x8.drawString(0,1,"Recv Hash:");
        //u8x8.drawString(11, 1, hash.c_str());
        u8x8.drawString(0,2,"Calc Hash:");
        u8x8.drawString(11, 2, std::to_string(calcHash).c_str());
        u8x8.drawString(0,0,rx.c_str());

        //Serial.println(rx); 

/*
        uint16_t recievedHash = 0;
        recievedHash = Serial.read();
        recievedHash = recievedHash<<8;
        recievedHash += Serial.read();

        u8x8.drawString(0,0,rx.c_str());
        u8x8.drawString(0,2,"Calc Hash:");
        u8x8.drawString(11, 2, std::to_string(calcHash).c_str());
        u8x8.drawString(0,1,"Recv Hash:");
        u8x8.drawString(11, 1, std::to_string(recievedHash).c_str());
*/
        config = rx;

        // save config to non-volotile memory
        writeMemory(config);

        return true;
    }
    return false;
}

// read the configuration file that is stored in memory
bool Manager::loadLocalConfig(String& config)
{ 
    u8x8.clear();
    u8x8.drawString(0,0,"Load saved config");
    config = EEPROM.readString(CONFIG_ADDRESS);
    u8x8.drawString(0,1, config.c_str());
  
    if(config.length() == 0) // empty string, not loaded
    {
        u8x8.drawString(0,2, "Invalid");
        return false;
    }
    u8x8.drawString(0,2, "Valid");
    return true;
}

// Create a simple numeric hash to represent the string. Used to verify integrity
uint16_t Manager::hashMessage(String& msg)
{
    uint16_t hash = 0;
    for(int i = 0; i < msg.length(); i++)
    {
        hash += (int)msg[i];
    }
    return hash;
}

// Write the given config to eeprom for future use
bool Manager::writeMemory(const String& data)
{
    EEPROM.writeString(CONFIG_ADDRESS, data);
    EEPROM.commit();

    return true;
}

// Create and configure interfaces and channels as per given string
bool Manager::parseConfig(const String& config)
{
    StaticJsonDocument<800> doc;
    DeserializationError error = deserializeJson(doc, config);
    if(error)
    { 
        LOG(F("Unable to configure board: "));
        Serial.println(error.f_str());
        Serial.println(config);
        return false;
    }

    String id = doc["id"];

    JsonArray interfaces = doc["interfaces"];

    loadInterfaces(doc["interfaces"]);
    loadInputs(doc["interfaces"]);
    return true;
}


// Load and configure the required Interfaces
void Manager::loadInterfaces(JsonArray interfaceList)
{
    for(JsonObject i : interfaceList)
    {
        String id = i["id"];
        String interfaceClass = i["class"];

        Interface * newInterface = InterfaceRegistry::getInterface(interfaceClass);

        if (newInterface == nullptr) // no valid interface found
        {
            LOG("Unknown interface: ");
            LOGA(interfaceClass);
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

        // find the matching interface object
        for(Interface* interface : interfaces)
        {
            if(interface->id == interfaceId)
            {
                // create each input
                JsonArray inputs = interfaceInfo["inputs"];
                for(JsonObject inputInfo : inputs)
                {
                    String inputId = inputInfo["id"];
                    
                    interface->createInput(inputInfo);

                    // once th input is created, we link it to any listening outputs
                    JsonArray listenerList = inputInfo["outputs"];

                    // cycle through each listener in the list
                    createLinks(interface, inputInfo);
                }
                break;
            }
        }
    }
}

// Link all inputs from given interface to required outputs
void Manager::createLinks(Interface* interface, JsonObject inputInfo)
{
    // for each output
    String inputId = inputInfo["id"];
    JsonArray outputList = inputInfo["links"];

    for(String outputId : outputList)
    {
        //find the instance
        for(OutputChannel* output : outputs)
        {
            if(output->id == outputId) // we've found the correct output
            {
                interface->linkChannels(inputId, output);
            }
        }
    }
}