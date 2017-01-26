#ifndef _ZHA_DEVICE_H_
#define _ZHA_DEVICE_H_

#include "XBee.h"
#include "endpoints.h"

class ZHA_Device : public XBeeWithCallbacks { 
public: 
//  ZHA_Device(ZHA_Endpoint *endpoints, uint8_t numEndpoints);
  ZHA_Device();
  void addEndpoint(ZHA_Endpoint* endpoint);
  void setSerial(Stream &serial);

  static void atCommandCb(AtCommandResponse& at, uintptr_t data); 
  static void modemStatusCb(ModemStatusResponse& status, uintptr_t data);
  static void explicitRxCb(ZBExplicitRxResponse &resp, uintptr_t data);
  
private: 
//  ZHA_Endpoint* _endpoints;
  LinkedList<ZHA_Endpoint*> _endpoints;
  uint8_t _numEndpoints;
  
  XBeeAddress64 _addr64;
  uint16_t _addr16;
  
  XBeeAddress64 _bcast64;

  void sendAnnounce();
  void processZDO(uint16_t clusterId, uint8_t *frameData, uint8_t frameDataLength);
  void processGeneralFrame(XBeeAddress64 remoteAddr64, uint16_t remoteAddr16, uint16_t clusterId, uint8_t dstEndpoint, uint8_t srcEndpoint, uint16_t profileId, uint8_t *frameData, uint8_t frameDataLength);
  ZHA_Endpoint* getEndpointById(uint8_t endpointId);

  /* reusable data payload */
  uint8_t _payload[255];
  uint8_t _payloadLength;
};

#endif
