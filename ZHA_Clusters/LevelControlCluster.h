//
// Created by apenner on 2/7/2017.
//

#ifndef XBEEZHA_LEVELCONTROLCLUSTER_H
#define XBEEZHA_LEVELCONTROLCLUSTER_H

#include "../ZHA_Clusters.h"

#define LEVEL_CONTROL_CLUSTER_ID 0x0008
#define LEVEL_CONTROL_CLUSTER_ATTRIBUTE_CURRENT_LEVEL 0x0000
#define LEVEL_CONTROL_CLUSTER_ATTRIBUTE_REMAINING_TIME 0x0001
#define LEVEL_CONTROL_CLUSTER_ATTRIBUTE_ONOFF_TRANSITION_TIME 0x0010
#define LEVEL_CONTROL_CLUSTER_ATTRIBUTE_ON_LEVEL 0x0011
#define LEVEL_CONTROL_CLUSTER_COMMAND_MOVE_TO_LEVEL 0x00
#define LEVEL_CONTROL_CLUSTER_COMMAND_MOVE 0x01
#define LEVEL_CONTROL_CLUSTER_COMMAND_STEP 0x02
#define LEVEL_CONTROL_CLUSTER_COMMAND_STOP 0x03
#define LEVEL_CONTROL_CLUSTER_COMMAND_MOVE_TO_LEVEL_WITH_ON_OFF 0x04
#define LEVEL_CONTROL_CLUSTER_COMMAND_MOVE_WITH_ON_OFF 0x05
#define LEVEL_CONTROL_CLUSTER_COMMAND_STEP_WITH_ON_OFF 0x06
#define LEVEL_CONTROL_CLUSTER_COMMAND_STOP_WITH_ON_OFF 0x06

class LevelControlCluster : public ZHA_Cluster {
public:
    LevelControlCluster();
    void moveToLevelWithOnOff(uint8_t level, uint16_t transition_time);

    void setMoveToLevelWithOnOffCallback(void (*movetoLevelWithOnOffCallback)(uint8_t level, uint16_t transition_time)) {
        _moveToLevelWithOnOffCallback = movetoLevelWithOnOffCallback;
    }

private:
    bool processCommand(uint8_t *frameData, uint8_t frameDataLength);

    void (*_moveToLevelWithOnOffCallback)(uint8_t, uint16_t);
};

LevelControlCluster::LevelControlCluster() {
    _attrs.add(new ZHA_Attribute(LEVEL_CONTROL_CLUSTER_ATTRIBUTE_CURRENT_LEVEL, ZHA_TYPE_UINT8, 0x0));
    _moveToLevelWithOnOffCallback = NULL;
    _clusterId = LEVEL_CONTROL_CLUSTER_ID;
}

void LevelControlCluster::moveToLevelWithOnOff(uint8_t level, uint16_t transition_time) {
    getAttrById(LEVEL_CONTROL_CLUSTER_ATTRIBUTE_CURRENT_LEVEL)->set(level);
    if (_moveToLevelWithOnOffCallback) {
        _moveToLevelWithOnOffCallback(level, transition_time);
    }
}

bool LevelControlCluster::processCommand(uint8_t* frameData, uint8_t frameDataLength) {
    switch(frameData[2]) {
        case LEVEL_CONTROL_CLUSTER_COMMAND_MOVE_TO_LEVEL_WITH_ON_OFF:
            uint8_t level = frameData[3];
            moveToLevelWithOnOff(level, 0x0000);
            break;
    }
    return false;
}

#endif //XBEEZHA_LEVELCONTROLCLUSTER_H
