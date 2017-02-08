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
    virtual void on() = 0;
    virtual void off() = 0;
    virtual void toggle() = 0;

//.private:
    void _on();
    void _off();
    void _toggle();
};

OnOffCluster::OnOffCluster() {
    /* mandatory attributes, default values */
    _attrs.add(new ZHA_Attribute(ON_OFF_CLUSTER_ATTRIBUTE_ON_OFF, ZHA_TYPE_BOOL, 0x0));
    _clusterId = ON_OFF_CLUSTER_ID;
}

bool OnOffCluster::processCommand(uint8_t* frameData, uint8_t frameDataLength) {
    switch(frameData[2]) {
        case ON_OFF_CLUSTER_ON_COMMAND:
            getAttrById(ON_OFF_CLUSTER_ATTRIBUTE_ON_OFF)->set((uint64_t)true);
            _on();
            break;
        case ON_OFF_CLUSTER_OFF_COMMAND:
            getAttrById(ON_OFF_CLUSTER_ATTRIBUTE_ON_OFF)->set((uint64_t)false);
            _off();
            break;
        case ON_OFF_CLUSTER_TOGGLE_COMMAND:
            getAttrById(ON_OFF_CLUSTER_ATTRIBUTE_ON_OFF)->set((uint64_t)(!(getAttrById(ON_OFF_CLUSTER_ATTRIBUTE_ON_OFF)->getValueBool())));
            _toggle();
            break;
    }
    return false;
}

void OnOffCluster::_on()     {
    getAttrById(ON_OFF_CLUSTER_ATTRIBUTE_ON_OFF)->set((uint64_t)true);
    on();
}

void OnOffCluster::_off()    {
    getAttrById(ON_OFF_CLUSTER_ATTRIBUTE_ON_OFF)->set((uint64_t)false);
    off();
}

void OnOffCluster::_toggle() {
    getAttrById(ON_OFF_CLUSTER_ATTRIBUTE_ON_OFF)->set((uint64_t)(!(getAttrById(ON_OFF_CLUSTER_ATTRIBUTE_ON_OFF)->getValueBool())));
    toggle();
}


#endif //XBEEZHA_ONOFFOUTPUTCLUSTER_H
