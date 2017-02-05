#ifndef _ZHA_DEVICEMANAGER_H_
#define _ZHA_DEVICEMANAGER_H_

#include "device.h"

/* ZDOs */
#define ZDO_SIMPLE_DESCRIPTOR_REQUEST             0x0004
#define ZDO_SIMPLE_DESCRIPTOR_RESPONSE            0x8004
#define ZDO_ACTIVE_ENDPOINTS_REQUEST              0x0005
#define ZDO_ACTIVE_ENDPOINTS_RESPONSE             0x8005
#define ZDO_MATCH_DESCRIPTOR_REQUEST              0x0006
#define ZDO_MATCH_DESCRIPTOR_RESPONSE             0x8006
#define ZDO_DEVICE_ANNOUNCE                       0x0013

class ZHA_DeviceManager : public XBeeWithCallbacks { 
public: 
  ZHA_DeviceManager();
  void setSerial(Stream &serial);
  void initializeModem();
  void loop();
  void addDevice(ZHA_Device *dev);
  static void atCommandCb(AtCommandResponse& at, uintptr_t data); 
  static void modemStatusCb(ModemStatusResponse& status, uintptr_t data);
  static void explicitRxCb(ZBExplicitRxResponse &resp, uintptr_t data);

private:
  LinkedList<ZHA_Device*> _deviceList;
  XBeeAddress64 _addr64;
  uint16_t _addr16;
  
  XBeeAddress64 _bcast64;
  uint16_t _bcast16;

  void sendAnnounce();
  void sendAT(String at);
  void processZDO(XBeeAddress64 remoteAddr64, uint16_t remoteAddr16, uint16_t clusterId, uint8_t *frameData, uint8_t frameDataLength);

  void processGeneralFrame(XBeeAddress64 remoteAddr64, uint16_t remoteAddr16, uint16_t clusterId, uint8_t dstEndpoint, uint8_t srcEndpoint, uint16_t profileId, uint8_t *frameData, uint8_t frameDataLength);

  ZHA_Device* getDeviceByEndpoint(uint8_t endpointId);
  
  /* reusable data payload */
  uint8_t _payload[MAX_FRAME_DATA_SIZE];
  uint8_t _payloadLength;

  void reportAttributes();

  Stream *_devSerial;
};

#endif
