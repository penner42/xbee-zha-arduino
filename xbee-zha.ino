#include <SoftwareSerial.h>
#include <Bounce2.h>
#include "devicemanager.h"
#include <ESP8266WiFi.h>

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
LevelControlCluster level_cluster;
ZLLCluster zll_cluster;
arduinoLED led_cluster;

Bounce debouncer = Bounce();

ZHA_Device lightswitch(0x08);
SoftwareSerial nss(12,14);
ZHA_DeviceManager devmanager;

void setup() {
  WiFi.mode(WIFI_OFF);
  Serial.begin(115200);

  lightswitch.addInCluster(&basic_cluster);
  lightswitch.addInCluster(&identify_cluster);
  lightswitch.addInCluster(&groups_cluster);
  lightswitch.addInCluster(&scenes_cluster);
  lightswitch.addInCluster(&led_cluster);
  lightswitch.addInCluster(&level_cluster);
  lightswitch.addInCluster(&zll_cluster);
  devmanager.addDevice(&lightswitch);
  
  pinMode(13, OUTPUT);
  pinMode(16, INPUT);
  debouncer.attach(16);
  debouncer.interval(5);
  nss.begin(57600);
  devmanager.setSerial(nss);
  devmanager.initializeModem();
}

void loop() {
  if (debouncer.update() && debouncer.read() == LOW) {
    led_cluster._toggle();
  }
  devmanager.loop();
}
