//
// Created by apenner on 2/7/2017.
//

#ifndef XBEEZHA_IDENTIFYCLUSTER_H
#define XBEEZHA_IDENTIFYCLUSTER_H

#include "../ZHA_Clusters.h"

#define IDENTITY_CLUSTER_ID                           0x0003
#define IDENTITY_CLUSTER_ATTRIBUTE_IDENTIFY_TIME      0x0000

class IdentifyCluster : public ZHA_Cluster {
public:
    IdentifyCluster();

private:
    bool processCommand(uint8_t *frameData, uint8_t frameDataLength) { };
};

IdentifyCluster::IdentifyCluster() {
    _attrs.add(new ZHA_Attribute(IDENTITY_CLUSTER_ATTRIBUTE_IDENTIFY_TIME, ZHA_TYPE_UINT16, 0x0000));
    _clusterId = IDENTITY_CLUSTER_ID;
}
#endif //XBEEZHA_IDENTIFYCLUSTER_H
