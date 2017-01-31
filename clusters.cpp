#include "clusters.h"

ZHA_Cluster::ZHA_Cluster() {}

uint16_t ZHA_Cluster::getClusterId() {
  return _clusterId;
}

//void ZHA_Cluster::sortAttributes() {
//  _attrs.sort(Attribute::cmp);
//}

Attribute* ZHA_Cluster::getAttrById(uint16_t attrId) {
  for (int i = 0; i < _attrs.size(); i++) {
    Attribute *cur = _attrs.get(i);
    if (cur->getAttrId() == attrId) {
      return cur;
    }
  }
  return NULL;
}

int ZHA_Cluster::getAttrIndexById(uint16_t attrId) {
  for (int i = 0; i < _attrs.size(); i++) {
    Attribute *cur = _attrs.get(i);
    if (cur->getAttrId() == attrId) {
      return i;
    }
  }
  return -1;
  
}

Attribute* ZHA_Cluster::getAttrByIndex(int index) {
  return _attrs.get(index);
}

BasicCluster::BasicCluster() {
  /* mandatory attributes, default values */
  _attrs.add(new Attribute(BASIC_CLUSTER_ATTRIBUTE_ZCL_VERSION, ZHA_TYPE_UINT8, 0x1));
  _attrs.add(new Attribute(BASIC_CLUSTER_ATTRIBUTE_POWER_SOURCE, ZHA_TYPE_8BIT_ENUMERATION, 0x0));
  _attrs.add(new Attribute(BASIC_CLUSTER_ATTRIBUTE_MANUFACTURER_NAME, ZHA_TYPE_CHARACTER_STRING, String("test1")));
  _attrs.add(new Attribute(BASIC_CLUSTER_ATTRIBUTE_MODEL_IDENTIFIER, ZHA_TYPE_CHARACTER_STRING, String("test2")));

  _clusterId = BASIC_CLUSTER_ID;
}

IdentifyCluster::IdentifyCluster() {
  _attrs.add(new Attribute(IDENTITY_CLUSTER_ATTRIBUTE_IDENTIFY_TIME, ZHA_TYPE_UINT16, 0x0000));
  _clusterId = IDENTITY_CLUSTER_ID;
}

OnOffCluster::OnOffCluster() {
  /* mandatory attributes, default values */
  _attrs.add(new Attribute(ON_OFF_CLUSTER_ATTRIBUTE_ON_OFF, ZHA_TYPE_BOOL, 0x0));
  _clusterId = ON_OFF_CLUSTER_ID;
}

void OnOffCluster::processCommand(uint8_t* frameData, uint8_t frameDataLength) {
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
}

