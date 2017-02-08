#include <SoftwareSerial.h>
#include <Bounce2.h>
#include <ZHA_DeviceManager.h>
#include <ZHA_Devices/OnOffOutputDevice.h>
#include <ESP8266WiFi.h>

#define DEBUG

void on() { digitalWrite(13, HIGH); }
void off() { digitalWrite(13, LOW); }
void toggle() { digitalWrite(13, digitalRead(13) ^ 1); }

Bounce debouncer = Bounce();

SoftwareSerial nss(12, 14);
ZHA_DeviceManager devmanager;
OnOffOutputDevice lightswitch(0x8);

void setup() {
    WiFi.mode(WIFI_OFF);
    Serial.begin(115200);

    lightswitch.getOnOffCluster()->setOnCallback(on);
    lightswitch.getOnOffCluster()->setOffCallback(off);
    lightswitch.getOnOffCluster()->setToggleCallback(toggle);

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
        lightswitch.getOnOffCluster()->toggle();
    }
    devmanager.loop();
}
