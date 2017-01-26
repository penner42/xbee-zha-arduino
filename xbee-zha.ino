#include <SoftwareSerial.h>
#include "device.h"

class arduinoLED : public OnOffCluster {
  void on() { digitalWrite(13, HIGH); } 
  void off() { digitalWrite(13, LOW); }
  void toggle() { digitalWrite(13, digitalRead(13) ^ 1); }
};

BasicCluster basic_cluster;
IdentifyCluster identify_cluster;
arduinoLED led_cluster;

ZHA_Endpoint lightswitch;
SoftwareSerial nss(8,9);
ZHA_Device device;

void sendAT(String at) {
  if (at == "+++") { 
    nss.print(at);
  } else {
    nss.println(at);
  }
  while (nss.available() == 0) {}
  Serial.println(nss.readString());
}

void initializeModem() {
  sendAT("+++");
  sendAT("ATEE0");
  sendAT("ATEO0");
  sendAT("ATAP0");
  sendAT("ATAO0");
  sendAT("ATWR");
  sendAT("ATAC");
  sendAT("ATCN");

  sendAT("+++");
  sendAT("ATZS2");
  sendAT("ATAP2");
  sendAT("ATAO3");
  sendAT("ATEE1");
  sendAT("ATEO2");
  sendAT("ATKY5A6967426565416C6C69616E63653039");
  sendAT("ATWR");
  sendAT("ATAC");
  sendAT("ATCN");
  Serial.println("setup done");
}

void setup() {
  lightswitch.addInCluster(&basic_cluster);
  lightswitch.addInCluster(&identify_cluster);
  lightswitch.addInCluster(&led_cluster);
  device.addEndpoint(&lightswitch);

  pinMode(13, OUTPUT);
  Serial.begin(9600);
  nss.begin(9600);
  initializeModem();
  device.setSerial(nss);
}

void loop() {
  // put your main code here, to run repeatedly:
  device.loop();
}
