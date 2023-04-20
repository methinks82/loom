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
    u8x8.drawString(0,0,"Waiting");
    loadConfig();
}

// Check each Interface to see if it received any updates  
void Manager::mainLoop()
{
    LOGA("*");
    delay(500);
    /*
    for(auto i : interfaces)
    {
        i->checkUpdate();
    }
    */
}

void Manager::loadConfig()
{
  const int BUFFER_SIZE = 500;
  EEPROM.begin(BUFFER_SIZE);

  String config;
  if(!getConfigUpdate(config))
  {
    if(!loadLocalConfig(config))
    {
      u8x8.drawString(0,0,"config not found");
      LOG("Config not found");
    }
  }
  parseConfig(config);
}


bool Manager::getConfigUpdate(String& config)
{  
  const char REQUEST_STRING[] = "CFG";
  
  Serial.println(); // make sure our request is on it's own line
  Serial.println(REQUEST_STRING);
  delay(500); // give host a chance to respond
  if( Serial.available() > 0) // host sent response
  {
    // get data from host
    String rx = Serial.readString();
    Serial.println(rx);
    config = rx;

    // save config to non-volotile memory
    writeMemory(config);

    u8x8.clear();
    u8x8.drawString(0,0,"RX:");
    u8x8.drawString(0,1, rx.c_str());

    return true;
  }
  return false;
}


bool Manager::loadLocalConfig(String& config)
{
  u8x8.clear();
  u8x8.drawString(0,0,"Loading local string");
  
  config = EEPROM.readString(CFG_ADDRESS);

  u8x8.drawString(0,1, config.c_str());
    if(config.length() == 0) // empty string, not loaded
    {
        return false;
    }
  return true;
}


void Manager::parseConfig(String& config)
{
  u8x8.clear();
  u8x8.drawString(0,0,"Parsing config:");
  u8x8.drawString(0,1, config.c_str());
}


bool Manager::writeMemory(const String& data)
{
  EEPROM.writeString(CFG_ADDRESS, data);
  EEPROM.commit();

  return true;
}







/*
// Check if there is a new configuration available, load if so
bool Manager::loadNewConfig()
{
    Serial.println("\nCFG");
    delay(500); // wait for reply

    // if there is a response
    if(Serial.available() > 0)
    {
        String response = Serial.readString();
        response.trim();
        Serial.print("New config: ");
        Serial.println(response);

        // save without checking validity
        saveConfig(response);

        // if valid config AND the user asked to save
        if(parseConfig(response)) // TODO: check if the user requested save
        {
            //saveConfig(response);
        }
        return true;
    }
    return false;
}

// Initialize and configure Interfaces and channels from saved configuration file
void Manager::useExistingConfig()
{
    String loadedConfig = EEPROM.readString(CONFIG_ADDRESS);
    LOG("Last config: ");
    LOGA(loadedConfig);
    parseConfig(loadedConfig);
}

// Create and configure interfaces and channels as per given string
bool Manager::parseConfig(const String& config)
{
    StaticJsonDocument<800> doc;
    DeserializationError error = deserializeJson(doc, config);
    if(error)
    { 
        LOG(F("Unable to load config: "));
        Serial.println(error.f_str());
        Serial.println(config);
        return false;
    }

    String id = doc["id"];
    LOG("Loading config ");
    LOGA(id);

    JsonArray interfaces = doc["interfaces"];

    loadInterfaces(doc["interfaces"]);
    loadInputs(doc["interfaces"]);
    return true;
}

// Write the given config to eeprom for future use
void Manager::saveConfig(const String& config)
{
    Serial.print("Starting save: ");
    if(config.length() < BLOCK_SIZE)
    {
        Serial.print(config);
        EEPROM.writeString(CONFIG_ADDRESS, config);
        EEPROM.commit();

        String readBack = EEPROM.readString(CONFIG_ADDRESS);
        LOG("Saved config ");
        LOGA(readBack);
    }
}
*/

// Load and configure the required Interfaces
void Manager::loadInterfaces(JsonArray interfaceList)
{
    for(JsonObject i : interfaceList)
    {
        String id = i["id"];
        String interfaceClass = i["class"];

        LOG(F("\nInterface <"));
        LOGA(interfaceClass);
        LOGA(F("> "));
        LOGA(id);
        LOG("--------");

        Interface * newInterface;

        // TODO: automate the loading of interfaces
        
        // check for each type of interface that is being used
        if(interfaceClass == "SerialInterface")
        {
            LOG(F("Loading Serial Interface"));
            newInterface = new SerialInterface;
        } 
        else if(interfaceClass == "GpioInterface")
        {
            LOG(F("Loading GPIO Interface"));
            newInterface = new GpioInterface;
        }
        /*
        else if(interfaceClass == "ScreenInterface")
        {
            LOG(F("Loading Screen Interface"));
            newInterface = new ScreenInterface;
        }*/
        else // no valid interface found
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

        LOG("- Output ");
        LOGA(id);

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
        LOG(F("\nInputs from "));
        LOGA(interfaceId);
        LOG(F("-------"));

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
                    LOG(inputId);
                    interface->createInput(inputInfo);

                    // once th input is created, we link it to any listening outputs
                    JsonArray listenerList = inputInfo["outputs"];
                    LOGA(F(" ("));
                    LOGA(listenerList.size());
                    LOGA(F(" listeners)"));

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
    JsonArray outputList = inputInfo["outputs"];

    for(String outputId : outputList)
    {
        LOG(F("- "));
        LOGA(outputId);
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