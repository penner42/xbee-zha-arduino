#include "ZHA_Clusters.h"

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
