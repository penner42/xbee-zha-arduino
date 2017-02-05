#include "device.h"

ZHA_Device::ZHA_Device(uint8_t endpointId) {
  _endpointId = endpointId;
}

void ZHA_Device::addInCluster(ZHA_Cluster *inCluster) {
  _inClusters.add(inCluster);  
}

void ZHA_Device::addOutCluster(ZHA_Cluster *outCluster) {
  _outClusters.add(outCluster);  
}

uint8_t ZHA_Device::getNumInClusters() {
  return _inClusters.size();
}

uint8_t ZHA_Device::getNumOutClusters() {
  return _outClusters.size();
}

ZHA_Cluster* ZHA_Device::getInCluster(uint8_t num) {
  return _inClusters.get(num);
}

ZHA_Cluster* ZHA_Device::getOutCluster(uint8_t num) {
  return _outClusters.get(num);
}

uint8_t ZHA_Device::getEndpointId() {
  return _endpointId;
}

ZHA_Cluster* ZHA_Device::getInClusterById(uint16_t clusterId) {
  for (uint8_t i = 0; i < _inClusters.size(); i++) {
      if (_inClusters.get(i)->getClusterId() == clusterId) {
        return _inClusters.get(i);
      }
  }
  return NULL;
}

ZHA_Cluster* ZHA_Device::getOutClusterById(uint16_t clusterId) {
  for (uint8_t i = 0; i < _outClusters.size(); i++) {
      if (_outClusters.get(i)->getClusterId() == clusterId) {
        return _outClusters.get(i);
      }
  }
  return NULL;
}

