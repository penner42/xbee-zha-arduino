#include <SoftwareSerial.h>
#include <Bounce2.h>
#include <ZHA_DeviceManager.h>
#include <ZHA_Devices/OnOffOutputDevice.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif

#define DEBUG

void on() { digitalWrite(13, HIGH); }
void off() { digitalWrite(13, LOW); }
void toggle() { digitalWrite(13, digitalRead(13) ^ 1); }

Bounce debouncer = Bounce();

SoftwareSerial nss(12, 14);
ZHA_DeviceManager devmanager;
OnOffOutputDevice lightbulb(0x8);

void setup() {
#ifdef ESP8266
    WiFi.mode(WIFI_OFF);
#endif
    Serial.begin(115200);

    lightbulb.setOnCallback(on);
    lightbulb.setOffCallback(off);
    lightbulb.setToggleCallback(toggle);
    lightbulb.getBasicCluster()->setManufacturerName(String("Leviton"));
    lightbulb.getBasicCluster()->setModelIdentifier(String("ZSS-10"));
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
