#ifndef _ENDPOINTS_H_
#define _ENDPOINTS_H_

#include "clusters.h"

class ZHA_Endpoint {
public:
//  ZHA_Endpoint(uint8_t endpointId, ZHA_Cluster **inClusters, uint8_t numInClusters, ZHA_Cluster **outClusters, uint8_t numOutClusters);
  ZHA_Endpoint(uint8_t endpointId) { _endpointId = endpointId; }
  void addInCluster(ZHA_Cluster *inCluster);
  void addOutCluster(ZHA_Cluster *outCluster);
  
  ZHA_Cluster* getInClusterById(uint16_t clusterId);
  ZHA_Cluster* getOutClusterById(uint16_t clusterId);
  uint8_t getEndpointId();
  uint8_t getNumInClusters();
  uint8_t getNumOutClusters();
  ZHA_Cluster* getInCluster(uint8_t num);
  ZHA_Cluster* getOutCluster(uint8_t num);  
  
private: 
  uint8_t _endpointId;
  
//  ZHA_Cluster **_inClusters;
//  uint8_t _numInClusters;
//  ZHA_Cluster **_outClusters;
//  uint8_t _numOutClusters;
  LinkedList<ZHA_Cluster*>_inClusters;
  LinkedList<ZHA_Cluster*>_outClusters;
};

#endif
