#include <Arduino.h>
//#include <LittleFS.h>
#include "radio.h"
#include "ir.h"
#include "web.h"
#include "commands.h"

void setup() {
  Serial.begin(115200);
  //LittleFS.begin();
  wifiSetup();
  delay(2000);
  setupRadio();
  setupIR();
  Serial.printf("Free heap after all: %u bytes\n", ESP.getFreeHeap());
}

bool asyncWait(unsigned long intervalMs) {
  static unsigned long lastTime = 0;
  unsigned long now = millis();

  if (now - lastTime >= intervalMs) {
    lastTime = now;
    return true;   
  }
  return false;    
}


void loop() {
  wifiLoop();

  if (radioReceiveEnabled) {
    radioLoop();
  }

  if (IRReceiveEnabled) {
    IRLoop();
  }

  commandsLoop(); 
}
