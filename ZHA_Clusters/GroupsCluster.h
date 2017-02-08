//
// Created by apenner on 2/7/2017.
//

#ifndef XBEEZHA_GROUPSCLUSTER_H
#define XBEEZHA_GROUPSCLUSTER_H

#include "../ZHA_Clusters.h"

#define GROUPS_CLUSTER_ID                             0x0004
#define GROUPS_CLUSTER_ATTRIBUTE_NAME_SUPPORT         0x0000

class GroupsCluster : public ZHA_Cluster {
public:
    GroupsCluster();

private:
    bool processCommand(uint8_t *frameData, uint8_t frameDataLength) { };
};

GroupsCluster::GroupsCluster() {
    _attrs.add(new ZHA_Attribute(GROUPS_CLUSTER_ATTRIBUTE_NAME_SUPPORT, ZHA_TYPE_8BIT_BITMAP, 0x0000));
    _clusterId = GROUPS_CLUSTER_ID;
}
#endif //XBEEZHA_GROUPSCLUSTER_H
