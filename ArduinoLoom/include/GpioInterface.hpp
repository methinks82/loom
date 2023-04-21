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
#include "InterfaceRegistry.hpp"

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

    /// @brief Read values from pins and send them to listeners
    class GpioInput
    {
    public:
        /// @brief Constructor
        /// @param pin number of pin to read
        GpioInput(int pin);

        /// @brief read and process current value
        void update();

        /// @brief identifier string used for configuration
        String id;

        /// @brief list of outputChannels to send data to
        std::vector<OutputChannel*> outputs;

    private:
        /// @brief The pin to read
        int pin;
        /// @brief previously read value so that we only trigger on change
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

        /// @brief Configure the channels for data to flow between them
        /// @param inputId name of the input from where the data comes
        /// @param targetOutput instance of the output channel to send data to
        virtual void linkChannels(const String& inputId, OutputChannel* targetOutput);

        /// @brief Check if there are any updates and pass them on to the proper channels
        virtual void checkUpdate();

    private:
        std::vector<GpioInput*> inputs; // replace with array. You can find size during configuration
    };
}

#endif