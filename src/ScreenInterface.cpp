#include "ScreenInterface.hpp"

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(15, 4, 16);

using namespace loom;
 
void ScreenOutput::call(int data) 
{
    String msg;
    msg = data;
    u8x8.clear();
    u8x8.drawString(0,0, msg.c_str());
}

   
void ScreenInterface::init(JsonObject config)
{
    Serial.println("Initializing ScreenInterface");
    u8x8.begin();
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0,0, "Initialized");
}

OutputChannel* ScreenInterface::createOutput(JsonObject params)
{
    ScreenOutput* output = new ScreenOutput;
    return output;
}

void ScreenInterface::checkUpdate()
{}