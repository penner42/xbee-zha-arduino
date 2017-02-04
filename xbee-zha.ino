#include <SoftwareSerial.h>
#include <Bounce2.h>
#include "device.h"

#define DEBUG

class arduinoLED : public OnOffCluster {
public:
  void on() { digitalWrite(13, HIGH); } 
  void off() { digitalWrite(13, LOW); }
  void toggle() { digitalWrite(13, digitalRead(13) ^ 1); }
};

BasicCluster basic_cluster;
IdentifyCluster identify_cluster;
GroupsCluster groups_cluster;
ScenesCluster scenes_cluster;
arduinoLED led_cluster;

Bounce debouncer = Bounce();

ZHA_Device lightswitch(0x08);
SoftwareSerial nss(12,14);
//ZHA_Device device;

void setup() {
  Serial.begin(115200);

  lightswitch.addInCluster(&basic_cluster);
  lightswitch.addInCluster(&identify_cluster);
  lightswitch.addInCluster(&groups_cluster);
  lightswitch.addInCluster(&scenes_cluster);
  lightswitch.addInCluster(&led_cluster);

  pinMode(13, OUTPUT);
  pinMode(16, INPUT);
  debouncer.attach(16);
  debouncer.interval(5);
  nss.begin(57600);
  lightswitch.setSerial(nss);
  lightswitch.initializeModem();
}

void loop() {
  if (debouncer.update() && debouncer.read() == LOW) {
    led_cluster._toggle();
  }
  lightswitch.loop();
}
