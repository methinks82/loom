/**
 * loom/ScreenInterface.cpp
 * 
 * Send data to an attached screen
 * Currently limited to text
 * Based on the U8x8 library
 * 
 * @author Alex Schlieck
 * @date 2023-04-08
*/


#include "ScreenInterface.hpp"

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(15, 4, 16); // configure screen

using namespace loom;

// Send data to screen
void ScreenOutput::call(int data) 
{
    String msg;
    msg = data;
    u8x8.clear();
    // TODO: instead of clearing screen, overwrite the line, and add spaces at end
    u8x8.drawString(0,0, msg.c_str());
}

// configure any setting required for the screen to work
void ScreenInterface::init(JsonObject config)
{
    u8x8.begin();
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0,0, "Initialized");
}

// create a channel that will output to the screen
OutputChannel* ScreenInterface::createOutput(JsonObject params)
{
    ScreenOutput* output = new ScreenOutput;
    return output;
}


loom::InterfaceFactory<loom::ScreenInterface>screen("ScreenInterface");