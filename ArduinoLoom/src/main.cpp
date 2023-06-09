#include <Arduino.h>
#include "Manager.hpp"
#include "logging.h"

using namespace loom;



Manager manager;

void setup() {
  Serial.begin(115200);
  LOG("Program started");

  //manager.loadConfig(jsonConfig);
  manager.setup();
}

void loop() {
  manager.mainLoop();
}