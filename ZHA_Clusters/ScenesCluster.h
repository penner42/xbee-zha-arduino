//
// Created by apenner on 2/7/2017.
//

#ifndef XBEEZHA_SCENESCLUSTER_H
#define XBEEZHA_SCENESCLUSTER_H

#include "../ZHA_Clusters.h"

#define SCENES_CLUSTER_ID                             0x0005
#define SCENES_CLUSTER_ATTRIBUTE_SCENE_COUNT          0x0000
#define SCENES_CLUSTER_ATTRIBUTE_CURRENT_SCENE        0x0001
#define SCENES_CLUSTER_ATTRIBUTE_CURRENT_GROUP        0x0002
#define SCENES_CLUSTER_ATTRIBUTE_SCENE_VALID          0x0003
#define SCENES_CLUSTER_ATTRIBUTE_NAME_SUPPORT         0x0004

class ScenesCluster : public ZHA_Cluster {
public:
    ScenesCluster();

private:
    bool processCommand(uint8_t *frameData, uint8_t frameDataLength) { };
};

ScenesCluster::ScenesCluster() {
    _attrs.add(new ZHA_Attribute(SCENES_CLUSTER_ATTRIBUTE_SCENE_COUNT,   ZHA_TYPE_UINT8,  0x00));
    _attrs.add(new ZHA_Attribute(SCENES_CLUSTER_ATTRIBUTE_CURRENT_SCENE, ZHA_TYPE_UINT8,  0x00));
    _attrs.add(new ZHA_Attribute(SCENES_CLUSTER_ATTRIBUTE_CURRENT_GROUP, ZHA_TYPE_UINT16, 0x0000));
    _attrs.add(new ZHA_Attribute(SCENES_CLUSTER_ATTRIBUTE_SCENE_VALID,   ZHA_TYPE_BOOL,   0x00));
    _attrs.add(new ZHA_Attribute(SCENES_CLUSTER_ATTRIBUTE_NAME_SUPPORT,  ZHA_TYPE_8BIT_BITMAP, 0x0000));
    _clusterId = SCENES_CLUSTER_ID;
}

#endif //XBEEZHA_SCENESCLUSTER_H
