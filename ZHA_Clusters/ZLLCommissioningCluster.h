//
// Created by apenner on 2/7/2017.
//

#ifndef XBEEZHA_ZLLCOMMISSIONINGCLUSTER_H
#define XBEEZHA_ZLLCOMMISSIONINGCLUSTER_H

#include "../ZHA_Clusters.h"


class ZLLCommissioningCluster : public ZHA_Cluster {
public:
    ZLLCommissioningCluster();

private:
    bool processCommand(uint8_t *frameData, uint8_t frameDataLength) { };
};

ZLLCommissioningCluster::ZLLCommissioningCluster() {
    _clusterId = 0x1000;
}
#endif //XBEEZHA_ZLLCOMMISSIONINGCLUSTER_H
