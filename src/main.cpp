#include <Arduino.h>
#include "Manager.hpp"
#include "data.hpp"

using namespace loom;

Manager manager;

void setup() {
  Serial.begin(9600);
  Serial.println("Program started");

  manager.loadConfig(jsonConfig);

  pinMode(12, INPUT);
}

void loop() {
  manager.checkUpdates();
}