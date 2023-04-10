#include <Arduino.h>
#include "Manager.hpp"
#include "data.hpp"

using namespace loom;

Manager manager;

void setup() {
  Serial.begin(115200);
  Serial.println("Program started");

  manager.loadConfig(config);
}

void loop() {
  manager.checkUpdates();
}