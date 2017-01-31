#include <SoftwareSerial.h>
#include <Bounce2.h>
#include "device.h"

#define DEBUG

class arduinoLED : public OnOffCluster {
  void on() { digitalWrite(13, HIGH); } 
  void off() { digitalWrite(13, LOW); }
  void toggle() { digitalWrite(13, digitalRead(13) ^ 1); }
};

BasicCluster basic_cluster;
IdentifyCluster identify_cluster;
arduinoLED led_cluster;

Bounce debouncer = Bounce();

ZHA_Endpoint lightswitch(0x08);
SoftwareSerial nss(12,14);
ZHA_Device device;

void setup() {
  Serial.begin(115200);

  lightswitch.addInCluster(&basic_cluster);
  lightswitch.addInCluster(&identify_cluster);
  lightswitch.addInCluster(&led_cluster);
  device.addEndpoint(&lightswitch);

  pinMode(13, OUTPUT);
  pinMode(16, INPUT);
  debouncer.attach(16);
  debouncer.interval(5);
  nss.begin(57600);
  device.setSerial(nss);
  device.initializeModem();
}

void loop() {
  if (debouncer.update() && debouncer.read() == LOW) {
    digitalWrite(13, digitalRead(13) ^ 1);
  }
  device.loop();
}
