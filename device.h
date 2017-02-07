#ifndef _ZHA_DEVICE_H_
#define _ZHA_DEVICE_H_

#include "XBee.h"
#include "clusters.h"
#include "Printers.h"
#include "utils.h"

/* ZCL Status Fields */
#define STATUS_SUCCESS                            0x00
#define STATUS_FAILURE                            0x01
#define STATUS_UNSUPPORTED_ATTRIBUTE              0x86

/* General Command Frames */
#define ZCL_READ_ATTRIBUTES                       0x00
#define ZCL_READ_ATTRIBUTES_RESPONSE              0x01
#define ZCL_WRITE_ATTRIBUTES                      0x02
#define ZCL_WRITE_ATTRIBUTES_UNDIVIDED            0x03
#define ZCL_WRITE_ATTRIBUTES_RESPONSE             0x04
#define ZCL_WRITE_ATTRIBYTES_NO_RESPONSE          0x05
#define ZCL_CONFIGURE_REPORTING                   0x06
#define ZCL_CONFIGURE_REPORTING_RESPONSE          0x07
#define ZCL_READ_REPORTING_CONFIGURATION          0x08
#define ZCL_READ_REPORTING_CONFIGURATION_RESPONSE 0x09
#define ZCL_REPORT_ATTRIBUTES                     0x0A
#define ZCL_DEFAULT_RESPONSE                      0x0B
#define ZCL_DISCOVER_ATTRIBUTES                   0x0C
#define ZCL_DISCOVER_ATTRIBUTES_RESPONSE          0x0D
#define ZCL_READ_ATTRBIUTES_STRUCTURED            0x0E
#define ZCL_WRITE_ATTRIBUTES_STRUCTURED           0x0F
#define ZCL_WRITE_ATTRIBUTES_STRUCTURED_RESPONSE  0x10

class ZHA_Device {
public: 
  ZHA_Device(uint8_t endpointId);
//  void setSerial(Stream &serial);
//  void initializeModem();
//  
//  void loop();
  void addInCluster(ZHA_Cluster *inCluster);
  void addOutCluster(ZHA_Cluster *outCluster);
  ZHA_Cluster* getInClusterById(uint16_t clusterId);
  ZHA_Cluster* getOutClusterById(uint16_t clusterId);
  uint8_t getEndpointId();
  uint8_t getNumInClusters();
  uint8_t getNumOutClusters();
  ZHA_Cluster* getInCluster(uint8_t num);
  ZHA_Cluster* getOutCluster(uint8_t num);    
  
  bool processCommand(uint8_t *frameData, uint8_t frameDataLength, uint16_t clusterId, uint8_t *payload, uint8_t& _payload);    
  bool processGeneralCommand(uint8_t *frameData, uint8_t frameDataLength, uint16_t clusterId, uint8_t *payload, uint8_t& _payload);    

private: 
  uint8_t _endpointId;
  LinkedList<ZHA_Cluster*>_inClusters;
  LinkedList<ZHA_Cluster*>_outClusters;
};

#endif
