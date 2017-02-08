//
// Created by apenner on 2/7/2017.
//

#ifndef XBEEZHA_LEVELCONTROLCLUSTER_H
#define XBEEZHA_LEVELCONTROLCLUSTER_H

#include "../ZHA_Clusters.h"

class LevelControlCluster : public ZHA_Cluster {
public:
    LevelControlCluster();

private:
    bool processCommand(uint8_t *frameData, uint8_t frameDataLength) { };
};

LevelControlCluster::LevelControlCluster() {
    _clusterId = 0x0008;
}

#endif //XBEEZHA_LEVELCONTROLCLUSTER_H
