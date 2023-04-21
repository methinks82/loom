/**
 * loom/logging.hpp
 * 
 * Provides logging to the serial port while debugging
 * 
 * @author Alex Schlieck
 * @date 2023-04-14
*/

#ifndef LOOM_LOGGING_HPP
#define LOOM_LOGGING_HPP

#include <Arduino.h>

#define LOGGING // temp, move to appropriate location

#ifdef LOGGING
    #define LOG(x) Serial.println(); Serial.print(x);
    #define LOGA(x) Serial.print(x);

#else
    #define LOG(x)
    #define LOGA(x)
#endif

#endif  //LOOM_LOGGING_HPP