#include <Arduino.h>
#include "Manager.hpp"
#include "data.hpp"

using namespace loom;

Manager manager;

void setup() {
  Serial.begin(9600);
  Serial.println("Program started");

  manager.loadConfig(jsonConfig);
}

void loop() {
  manager.checkUpdates();
}