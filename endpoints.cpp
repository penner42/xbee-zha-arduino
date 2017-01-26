#include "endpoints.h"

//ZHA_Endpoint::ZHA_Endpoint(uint8_t endpointId, ZHA_Cluster **inClusters, uint8_t numInClusters, ZHA_Cluster **outClusters, uint8_t numOutClusters) {
//  _endpointId     = endpointId;
//  _inClusters     = inClusters;
//  _numInClusters  = numInClusters;
//  _outClusters    = outClusters;
//  _numOutClusters = numOutClusters;
//}

void ZHA_Endpoint::addInCluster(ZHA_Cluster *inCluster) {
  _inClusters.add(inCluster);  
}

void ZHA_Endpoint::addOutCluster(ZHA_Cluster *outCluster) {
  _outClusters.add(outCluster);  
}

uint8_t ZHA_Endpoint::getNumInClusters() {
  return _inClusters.size();
}

uint8_t ZHA_Endpoint::getNumOutClusters() {
  return _outClusters.size();
}

ZHA_Cluster* ZHA_Endpoint::getInCluster(uint8_t num) {
  return _inClusters.get(num);
}

ZHA_Cluster* ZHA_Endpoint::getOutCluster(uint8_t num) {
  return _outClusters.get(num);
}

uint8_t ZHA_Endpoint::getEndpointId() {
  return _endpointId;
}

ZHA_Cluster* ZHA_Endpoint::getInClusterById(uint16_t clusterId) {
  for (uint8_t i = 0; i < _inClusters.size(); i++) {
      if (_inClusters.get(i)->getClusterId() == clusterId) {
        return _inClusters.get(i);
      }
  }
  return NULL;
}

ZHA_Cluster* ZHA_Endpoint::getOutClusterById(uint16_t clusterId) {
  for (uint8_t i = 0; i < _outClusters.size(); i++) {
      if (_outClusters.get(i)->getClusterId() == clusterId) {
        return _outClusters.get(i);
      }
  }
  return NULL;
}

