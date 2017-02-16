#define DEBUG

#include <SoftwareSerial.h>
#include <Bounce2.h>
#include <ZHA_DeviceManager.h>
#include <ZHA_Devices/LevelControlOutputDevice.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif

void on() { digitalWrite(13, HIGH); }
void off() { digitalWrite(13, LOW); }
void toggle() { digitalWrite(13, digitalRead(13) ^ 1); }

void setLevel(uint8_t level, uint16_t transition_time) {
    analogWrite(13,level);
}

Bounce debouncer = Bounce();

SoftwareSerial nss(12, 14);
ZHA_DeviceManager devmanager;
LeveLControlOutputDevice lightbulb(0x8);

void setup() {
#ifdef ESP8266
    WiFi.mode(WIFI_OFF);
#endif
    Serial.begin(115200);

    lightbulb.setOnCallback(on);
    lightbulb.setOffCallback(off);
    lightbulb.setToggleCallback(toggle);
    lightbulb.setMoveToLevelWithOnOffCallback(setLevel);
    lightbulb.getBasicCluster()->setManufacturerName(String("GE_Appliances"));
    lightbulb.getBasicCluster()->setModelIdentifier(String("ZLL Light"));
    devmanager.addDevice(&lightbulb);

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
        lightbulb.toggle();
    }
    devmanager.loop();
}
