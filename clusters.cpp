#include "clusters.h"

ZHA_Cluster::ZHA_Cluster() {}

uint16_t ZHA_Cluster::getClusterId() {
  return _clusterId;
}

ZHA_Attribute* ZHA_Cluster::getAttrById(uint16_t attrId) {
  for (int i = 0; i < _attrs.size(); i++) {
    ZHA_Attribute *cur = _attrs.get(i);
    if (cur->getAttrId() == attrId) {
      return cur;
    }
  }
  return NULL;
}

int ZHA_Cluster::getAttrIndexById(uint16_t attrId) {
  for (int i = 0; i < _attrs.size(); i++) {
    ZHA_Attribute *cur = _attrs.get(i);
    if (cur->getAttrId() == attrId) {
      return i;
    }
  }
  return -1;
  
}

ZHA_Attribute* ZHA_Cluster::getAttrByIndex(int index) {
  return _attrs.get(index);
}

BasicCluster::BasicCluster() {
  /* mandatory attributes, default values */
  _attrs.add(new ZHA_Attribute(BASIC_CLUSTER_ATTRIBUTE_ZCL_VERSION, ZHA_TYPE_UINT8, 0x1));
  _attrs.add(new ZHA_Attribute(BASIC_CLUSTER_ATTRIBUTE_POWER_SOURCE, ZHA_TYPE_8BIT_ENUMERATION, 0x0));
  _attrs.add(new ZHA_Attribute(BASIC_CLUSTER_ATTRIBUTE_MANUFACTURER_NAME, ZHA_TYPE_CHARACTER_STRING, String("test1")));
  _attrs.add(new ZHA_Attribute(BASIC_CLUSTER_ATTRIBUTE_MODEL_IDENTIFIER, ZHA_TYPE_CHARACTER_STRING, String("test2")));

  _clusterId = BASIC_CLUSTER_ID;
}

IdentifyCluster::IdentifyCluster() {
  _attrs.add(new ZHA_Attribute(IDENTITY_CLUSTER_ATTRIBUTE_IDENTIFY_TIME, ZHA_TYPE_UINT16, 0x0000));
  _clusterId = IDENTITY_CLUSTER_ID;
}

GroupsCluster::GroupsCluster() {
  _attrs.add(new ZHA_Attribute(GROUPS_CLUSTER_ATTRIBUTE_NAME_SUPPORT, ZHA_TYPE_8BIT_BITMAP, 0x0000));
  _clusterId = GROUPS_CLUSTER_ID;
}

ScenesCluster::ScenesCluster() {
  _attrs.add(new ZHA_Attribute(SCENES_CLUSTER_ATTRIBUTE_SCENE_COUNT,   ZHA_TYPE_UINT8,  0x00));
  _attrs.add(new ZHA_Attribute(SCENES_CLUSTER_ATTRIBUTE_CURRENT_SCENE, ZHA_TYPE_UINT8,  0x00));
  _attrs.add(new ZHA_Attribute(SCENES_CLUSTER_ATTRIBUTE_CURRENT_GROUP, ZHA_TYPE_UINT16, 0x0000));
  _attrs.add(new ZHA_Attribute(SCENES_CLUSTER_ATTRIBUTE_SCENE_VALID,   ZHA_TYPE_BOOL,   0x00));
  _attrs.add(new ZHA_Attribute(SCENES_CLUSTER_ATTRIBUTE_NAME_SUPPORT,  ZHA_TYPE_8BIT_BITMAP, 0x0000));
  _clusterId = SCENES_CLUSTER_ID;
}

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

