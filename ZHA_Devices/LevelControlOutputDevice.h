//
// Created by apenner on 2/8/2017.
//

#ifndef XBEEZHA_LEVELCONTROLOUTPUTDEVICE_H
#define XBEEZHA_LEVELCONTROLOUTPUTDEVICE_H

#include "../ZHA_Devices.h"

/* Clusters required for device. */
#include "../ZHA_Clusters/BasicCluster.h"
#include "../ZHA_Clusters/IdentifyCluster.h"
#include "../ZHA_Clusters/OnOffCluster.h"
#include "../ZHA_Clusters/ScenesCluster.h"
#include "../ZHA_Clusters/GroupsCluster.h"
#include "../ZHA_Clusters/LevelControlCluster.h"

class LeveLControlOutputDevice : public ZHA_Device {
public:
    LeveLControlOutputDevice(uint8_t endpointId);

    BasicCluster *getBasicCluster() { return &basic_cluster; }
    OnOffCluster *getOnOffCluster() { return &onoff_cluster; }
    LevelControlCluster *getLevelControlCluster() { return &level_cluster; }

    void on() { onoff_cluster.on(); }
    void off() { onoff_cluster.off(); }
    void toggle() { onoff_cluster.toggle(); }

    void setOnCallback(void (*onCallback)()) { onoff_cluster.setOnCallback(onCallback); }
    void setOffCallback(void (*offCallback)()) { onoff_cluster.setOffCallback(offCallback); }
    void setToggleCallback(void (*toggleCallback)()) { onoff_cluster.setToggleCallback(toggleCallback); }
    void
    setMoveToLevelWithOnOffCallback(void (*movetoLevelWithOnOffCallback)(uint8_t level, uint16_t transition_time)) {
        level_cluster.setMoveToLevelWithOnOffCallback(movetoLevelWithOnOffCallback);
    }


private:
    BasicCluster basic_clsuter;
    IdentifyCluster identify_cluster;
    OnOffCluster onoff_cluster;
    GroupsCluster groups_cluster;
    ScenesCluster scenes_cluster;
    LevelControlCluster level_cluster;
};

LeveLControlOutputDevice::LeveLControlOutputDevice(uint8_t endpointId) : ZHA_Device(endpointId) {
    _deviceId = ZHA_LEVEL_CONTROLLABLE_OUTPUT_DEVICE_ID;
    addInCluster(&basic_cluster);
    addInCluster(&identify_cluster);
    addInCluster(&onoff_cluster);
    addInCluster(&groups_cluster);
    addInCluster(&scenes_cluster);
    addInCluster(&level_cluster);
}

#endif //XBEEZHA_LEVELCONTROLOUTPUTDEVICE_H
