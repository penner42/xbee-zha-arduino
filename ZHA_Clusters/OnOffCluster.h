//
// Created by apenner on 2/7/2017.
//

#ifndef XBEEZHA_ONOFFCLUSTER_H
#define XBEEZHA_ONOFFCLUSTER_H

#include "../ZHA_Clusters.h"

#define ON_OFF_CLUSTER_ID                       0x0006
#define ON_OFF_CLUSTER_ATTRIBUTE_ON_OFF         0x0000
#define ON_OFF_CLUSTER_OFF_COMMAND              0x00
#define ON_OFF_CLUSTER_ON_COMMAND               0x01
#define ON_OFF_CLUSTER_TOGGLE_COMMAND           0x02

class OnOffCluster : public ZHA_Cluster {
public:
    OnOffCluster();
    bool processCommand(uint8_t *frameData, uint8_t frameDataLength);
    void on();
    void off();
    void toggle();

    void setOnCallback(void (*onCallback)()) { _onCallback = onCallback; }
    void setOffCallback(void (*offCallback)()) { _offCallback = offCallback; }
    void setToggleCallback(void (*toggleCallback)()) { _toggleCallback = toggleCallback; }

private:
    void (*_onCallback)();
    void (*_offCallback)();
    void (*_toggleCallback)();
};

void OnOffCluster::on() {
    getAttrById(ON_OFF_CLUSTER_ATTRIBUTE_ON_OFF)->set((uint64_t)true);
    if (_onCallback) {
        _onCallback();
    }
}

void OnOffCluster::off() {
    getAttrById(ON_OFF_CLUSTER_ATTRIBUTE_ON_OFF)->set((uint64_t)false);
    if (_offCallback) {
        _offCallback();
    }
}

void OnOffCluster::toggle() {
    getAttrById(ON_OFF_CLUSTER_ATTRIBUTE_ON_OFF)->set((uint64_t)(!(getAttrById(ON_OFF_CLUSTER_ATTRIBUTE_ON_OFF)->getValueBool())));
    if (_toggleCallback) {
        _toggleCallback();
    }
}

OnOffCluster::OnOffCluster() {
    /* mandatory attributes, default values */
    _attrs.add(new ZHA_Attribute(ON_OFF_CLUSTER_ATTRIBUTE_ON_OFF, ZHA_TYPE_BOOL, 0x0));
    _onCallback = _offCallback = _toggleCallback = NULL;
    _clusterId = ON_OFF_CLUSTER_ID;
}

bool OnOffCluster::processCommand(uint8_t* frameData, uint8_t frameDataLength) {
    switch(frameData[2]) {
        case ON_OFF_CLUSTER_ON_COMMAND:
            on();
            break;
        case ON_OFF_CLUSTER_OFF_COMMAND:
            off();
            break;
        case ON_OFF_CLUSTER_TOGGLE_COMMAND:
            toggle();
            break;
    }
    return false;
}

#endif //XBEEZHA_ONOFFOUTPUTCLUSTER_H
