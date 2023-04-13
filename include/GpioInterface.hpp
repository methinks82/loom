/**
 * loom/GpioInterface.hpp
 * 
 * Interface to read and write the arduino's gpio pins
 * 
 * @author Alex Schlieck
 * @date 2023-04-12
*/

#ifndef LOOM_GPIO_INTERFACE_HPP
#define LOOM_GPIO_INTERFACE_HPP

#include <vector>
#include <Arduino.h>
#include "Interface.hpp"

namespace loom
{
    /// @brief Output that writes to gpio pin
    class GpioOutput: public OutputChannel
    {
    public:
        /// @brief Constructor
        /// @param pin pin to write to
        GpioOutput(int pin);

        /// @brief Writes data to gpio pin
        /// @param data value to write to pin
        virtual void call(int data);

    private:
        int pin; // pin to write to
    };

    // TODO: Input class will be removed
    class GpioInput
    {
    public:
        GpioInput(int pin);
        void update(int data);

    private:
        int pin;
        int lastVal;
    };

    /// @brief Read from and write to arduino's gpio pins
    class GpioInterface: public Interface
    {
    public:
        /// @brief Set up this interface to use the parameters
        /// @param config string containing the parameters
        virtual void init(JsonObject config);

        /// @brief create an output that writes to gpio pin
        /// @param params Settings to apply to this output (pin number)
        /// @return pointer to the new output
        virtual OutputChannel* createOutput(JsonObject params) override;

        /// @brief create an input that reads from gpio pin
        /// @param param Setting to apply to the input (pin number)
        /// @return pointer to the new input
        virtual void createInput(JsonObject param) override;

    private:
        std::vector<GpioInput*> inputs; // replace with array. You can find size at runtime
    };
}

#endif