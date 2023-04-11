#ifndef LOOM_SCREEN_INTERFACE_HPP
#define LOOM_SCREEN_INTERFACE_HPP

#include <U8x8lib.h>
#include "Interface.hpp"


namespace loom
{
    class ScreenOutput : public OutputChannel
    {
        virtual void call(int data) override;
    };

    class ScreenInterface : public Interface
    {
    public:
        virtual void init(JsonObject config);

        OutputChannel* createOutput(JsonObject params) override;

        // no inputs, not used
        virtual void checkUpdate();
    };
}

#endif