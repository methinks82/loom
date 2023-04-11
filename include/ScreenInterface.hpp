#ifndef LOOM_SCREEN_INTERFACE_HPP
#define LOOM_SCREEN_INTERFACE_HPP

#include <U8x8lib.h>
#include "Interface.hpp"


namespace loom
{
    class ScreenOutput : public OutputChannel
    {
        virtual void call(int data) override;
        /*{
            String msg;
            msg = data;
            u8x8.clear();
            u8x8.drawString(0,0, msg.c_str());
        }*/
    };

    class ScreenInterface : public Interface
    {
    public:
        virtual void init(JsonObject config);
        /*
        {
            Serial.println("Initializing ScreenInterface");
            u8x8.begin();
            u8x8.setFont(u8x8_font_chroma48medium8_r);
            u8x8.drawString(0,0, "Initialized");
        }*/

        OutputChannel* createOutput(JsonObject params) override;
        /*
        {
            ScreenOutput* output = new ScreenOutput;
            return output;
        }*/

        // no inputs, not used
        virtual void checkUpdate();
    };
}

#endif