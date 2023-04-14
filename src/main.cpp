#include <Arduino.h>
#include "Manager.hpp"
#include "data.hpp"
#include "logging.h"

using namespace loom;



Manager manager;

void setup() {
  Serial.begin(115200);
  LOG("Program started");

  manager.loadConfig(jsonConfig);
}

void loop() {



  manager.checkUpdates();
}