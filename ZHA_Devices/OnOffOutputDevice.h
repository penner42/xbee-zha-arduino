//
// Created by apenner on 2/8/2017.
//
#ifndef XBEEZHA_ONOFFOUTPUTDEVICE_H
#define XBEEZHA_ONOFFOUTPUTDEVICE_H

#include "../ZHA_Devices.h"

/* Clusters required for device. */
#include "../ZHA_Clusters/BasicCluster.h"
#include "../ZHA_Clusters/IdentifyCluster.h"
#include "../ZHA_Clusters/OnOffCluster.h"
#include "../ZHA_Clusters/ScenesCluster.h"
#include "../ZHA_Clusters/GroupsCluster.h"

class OnOffOutputDevice:public ZHA_Device {
public:
    OnOffOutputDevice(uint8_t endpointId);

    BasicCluster *getBasicCluster() { return &basic_cluster; }

    void on() { onoff_cluster.on(); }
    void off() { onoff_cluster.off(); }
    void toggle() { onoff_cluster.toggle(); }

    void setOnCallback(void (*onCallback)()) { onoff_cluster.setOnCallback(onCallback); }
    void setOffCallback(void (*offCallback)()) { onoff_cluster.setOffCallback(offCallback); }
    void setToggleCallback(void (*toggleCallback)()) { onoff_cluster.setToggleCallback(toggleCallback); }


private:
    BasicCluster    basic_cluster;
    IdentifyCluster identify_cluster;
    OnOffCluster    onoff_cluster;
    GroupsCluster   groups_cluster;
    ScenesCluster   scenes_cluster;
};

OnOffOutputDevice::OnOffOutputDevice(uint8_t endpointId) : ZHA_Device(endpointId) {
    _deviceId = ZHA_ON_OFF_OUTPUT_DEVICE_ID;
    addInCluster(&basic_cluster);
    addInCluster(&identify_cluster);
    addInCluster(&onoff_cluster);
    addInCluster(&groups_cluster);
    addInCluster(&scenes_cluster);
}

#endif //XBEEZHA_ONOFFOUTPUTDEVICE_H
