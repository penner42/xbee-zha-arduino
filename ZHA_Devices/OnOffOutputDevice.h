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
    OnOffCluster* getOnOffCluster() { return &onoff_cluster; }

private:
    BasicCluster    basic_cluster;
    IdentifyCluster identify_cluster;
    OnOffCluster    onoff_cluster;
    GroupsCluster   groups_cluster;
    ScenesCluster   scenes_cluster;
};

OnOffOutputDevice::OnOffOutputDevice(uint8_t endpointId) : ZHA_Device(endpointId) {
    addInCluster(&basic_cluster);
    addInCluster(&identify_cluster);
    addInCluster(&onoff_cluster);
    addInCluster(&groups_cluster);
    addInCluster(&scenes_cluster);
}

#endif //XBEEZHA_ONOFFOUTPUTDEVICE_H
