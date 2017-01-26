#include "device.h"
//#include "Printers.h"

void copyHex(uint8_t *d, uint16_t v) {
  d[1] = (uint8_t)v;
  d[0] = (uint8_t)(v >> 8);
}

void copyHex(uint8_t *d, uint32_t v) {
  copyHex(d, (uint16_t)(v >> 16));
  copyHex(d+2, (uint16_t)v);
}

void copyHexL(uint8_t *d, uint16_t v) {
  /* little endian */
  d[0] = (uint8_t)v;
  d[1] = (uint8_t)(v >> 8);
}

ZHA_Device::ZHA_Device() : _addr64(0), _bcast64(0) {
  _addr16 = 0;

#ifdef DEBUG
  onResponse(printResponseCb, (uintptr_t)(Print*)&Serial);
  onModemStatusResponse(modemStatusCb, (uintptr_t)this);
#endif
  onAtCommandResponse(atCommandCb, (uintptr_t)this);
  onZBExplicitRxResponse(explicitRxCb, (uintptr_t)this);
  
  AtCommandRequest req;
  req.setCommand((uint8_t*)"SH");
  req.setFrameId(getNextFrameId());
  send(req);
}

void ZHA_Device::addEndpoint(ZHA_Endpoint* endpoint) {
  _endpoints.add(endpoint);
}

void ZHA_Device::setSerial(Stream &serial) {
  XBeeWithCallbacks::setSerial(serial);
  /* Get device 64 bit address on startup */
  AtCommandRequest req;
  req.setCommand((uint8_t*)"SH");
  req.setFrameId(getNextFrameId());
  send(req);
}

/* Send the device announce command */
void ZHA_Device::sendAnnounce() {
  uint8_t capability = 0b00001000; /* For now, just receiver on during idle times */
  uint8_t announce_cluster = 0x13;
  uint8_t payload[12];
  payload[0] = getNextFrameId();
  copyHex(&payload[1], _addr16);
  copyHex(&payload[3], _addr64.getMsb());
  copyHex(&payload[7], _addr64.getLsb());
  payload[11] = capability;

  ZBExplicitTxRequest announce(_bcast64, 0xFFFC, 0, 0, (uint8_t*)&payload, 12, payload[0], 0, 0, announce_cluster, 0);
  send(announce);
}

void ZHA_Device::processZDO(uint16_t clusterId, uint8_t *frameData, uint8_t frameDataLength) {
  if (clusterId == 0x0004) {
    _addr16 = ((uint16_t)frameData[2] << 8) | frameData[1];
    ZHA_Endpoint* ep = getEndpointById(frameData[3]);
    if (ep) { 
      uint8_t numInClusters = ep->getNumInClusters();
      uint8_t numOutClusters = ep->getNumOutClusters();
      _payloadLength = 13 + (2 * (numInClusters + numOutClusters));
      _payload[0] = frameData[0];
      _payload[1] = SUCCESS;
      copyHexL(&_payload[2], _addr16);
      _payload[4] = _payloadLength - 5;
      _payload[5] = ep->getEndpointId();
      copyHexL(&_payload[6], (uint16_t)0x0104);
      copyHexL(&_payload[8], (uint16_t)0x0002);
      _payload[10] = 0x40;
      _payload[11] = numInClusters;
      for (uint8_t i = 0; i < numInClusters; i++) {
        copyHexL(&_payload[12+(2*i)], ep->getInCluster(i)->getClusterId());
      }
      _payload[12 + (2 * numInClusters)] = numOutClusters;
      for (uint8_t i = 0; i < numOutClusters; i++) {
        copyHexL(&_payload[13+2*(numInClusters+i)], ep->getOutCluster(i)->getClusterId());
      }
      ZBExplicitTxRequest SDR(_bcast64, 0, 0, 0, (uint8_t*)&_payload, _payloadLength, getNextFrameId(), 0, 0, (uint16_t)0x8004, 0);
      send(SDR);
    }
  } else if (clusterId == 0x0005) {
    _addr16 = ((uint16_t)frameData[2] << 8) | frameData[1];
    _payload[0] = frameData[0];
    _payload[1] = SUCCESS;
    copyHexL(&_payload[2], _addr16);
    _payload[4] = _endpoints.size();
    for (uint8_t i = 0; i < _endpoints.size(); i++) {
      _payload[i + 5] = _endpoints.get(i)->getEndpointId();
    }
    ZBExplicitTxRequest endpoints(_bcast64, (uint16_t)0, (uint8_t)0, (uint8_t)0, (uint8_t*)&_payload, (uint8_t)5 + _endpoints.size(), getNextFrameId(), (uint8_t)0, (uint8_t)0, (uint16_t)0x8005, (uint16_t)0);
    send(endpoints);
    Serial.println("sent endpoints");
  }
}

ZHA_Endpoint* ZHA_Device::getEndpointById(uint8_t endpointId) {
  for (uint8_t i = 0; i < _endpoints.size(); i++) {
    if (_endpoints.get(i)->getEndpointId() == endpointId) {
      return _endpoints.get(i);
    }
  }
  return NULL;
}


void ZHA_Device::atCommandCb(AtCommandResponse &at, uintptr_t data) {
  ZHA_Device *XBeeDevice = (ZHA_Device*)data;

  uint8_t *cmd = at.getCommand();
  uint8_t *value = at.getValue();
  AtCommandRequest req;

  if (strncmp((char*)cmd, "SH", 2) == 0) {
    /* set high 32 bits of 64 bit address */
    XBeeDevice->_addr64.setMsb( ((uint32_t)value[0]) << 24 | ((uint32_t)value[1]) << 16 | ((uint32_t)value[2]) << 8 | (uint32_t)value[3]);
    req.setCommand((uint8_t*)"SL");
    req.setFrameId(XBeeDevice->getNextFrameId());
    XBeeDevice->send(req);    
  } else if (strncmp((char*)cmd, "SL", 2) == 0) {
    /* set low 32 bits of 64 bit address */
    XBeeDevice->_addr64.setLsb( ((uint32_t)value[0]) << 24 | ((uint32_t)value[1]) << 16 | ((uint32_t)value[2]) << 8 | (uint32_t)value[3]);
  } else if (strncmp((char*)cmd, "MY", 2) == 0) {
    /* set 16 bit address */
    XBeeDevice->_addr16 = ((uint16_t)value[0] << 8) | value[1];
    /* send announce */
    XBeeDevice->sendAnnounce();
  }
}

void ZHA_Device::modemStatusCb(ModemStatusResponse& status, uintptr_t data) {
  ZHA_Device *XBeeDevice = (ZHA_Device*)data;
  AtCommandRequest req;
    
  switch(status.getStatus()) {
    case HARDWARE_RESET:
      Serial.println("Modem reset.");
      break;
    case ASSOCIATED: 
      Serial.println("Joined network.");
      /* Get 16 bit address */
      req.setCommand((uint8_t*)"MY");
      req.setFrameId(XBeeDevice->getNextFrameId());
      XBeeDevice->send(req);
      break;
    default:
      break;
//      printResponseCb(status, (uintptr_t)(Print*)&Serial);
  }
}

void ZHA_Device::processGeneralFrame(XBeeAddress64 dst64, uint16_t dst16, uint16_t clusterId, uint8_t dstEndpoint, uint8_t srcEndpoint, uint16_t profileId, uint8_t *frameData, uint8_t frameDataLength) {
  uint8_t frameId = frameData[1];
  uint8_t command = frameData[2];
  ZHA_Cluster *cluster = getEndpointById(srcEndpoint)->getInClusterById(clusterId);
  if (command == 0x00) {
    uint8_t payloadLength = 3;
    _payload[0] = 0b00011000;
    _payload[1] = frameId;
    _payload[2] = 0x1;
    Serial.print("fdl ");
    Serial.println(frameDataLength);
    for (uint8_t i = 3; i < frameDataLength; i+=2) {
      uint16_t attrId = ((uint16_t)frameData[i+1] << 8) | frameData[i];
      Attribute *attr = cluster->getAttrById(attrId);
      if (attr == NULL) {
        payloadLength += 3;
        copyHexL(&_payload[3*((i-1)/2)], attrId);
        _payload[(3*((i-1)/2)) + 2] = 0x86;
      }
    }
    ZBExplicitTxRequest attrresp(dst64, dst16, 0, 0, (uint8_t*)&_payload, payloadLength, getNextFrameId(), srcEndpoint, dstEndpoint, clusterId, profileId);
    send(attrresp);
  } else if (command == 0x0c) {
    Serial.println("discover attributes");
//    cluster->sortAttributes();
    uint8_t payloadLength = 4;
    _payload[0] = 0b00011000;
    _payload[1] = frameId;
    _payload[2] = 0x0d;
    Serial.println("discover attributes");
    uint16_t first_attr = ((uint16_t)frameData[4] << 8) | frameData[3];
    uint8_t max_attrs = frameData[5];
    uint8_t attr_index = cluster->getAttrIndexById(first_attr);
    uint8_t num_attrs = cluster->numAttributes();
    bool done = false;
    Attribute *attr;
    Serial.println("discover attributes");
    for (uint8_t i = 0; i < max_attrs; i++) {
      if (i + attr_index + 1 > num_attrs) {
        done = true;
        break;
      }
      payloadLength += 3;
      attr = cluster->getAttrByIndex(i + attr_index);
      copyHexL(&_payload[4+(i*3)], attr->getAttrId());
      _payload[6+(i*3)] = attr->getAttrType();
    }
    _payload[3] = done;
    Serial.println("discover attributes");
    ZBExplicitTxRequest dar(dst64, dst16, 0, 0, (uint8_t*)&_payload, payloadLength, getNextFrameId(), srcEndpoint, dstEndpoint, clusterId, profileId);
    send(dar);
  }
}

void ZHA_Device::explicitRxCb(ZBExplicitRxResponse &resp, uintptr_t data) {
  ZHA_Device *XBeeDevice = (ZHA_Device*)data;
  uint16_t profileId      = resp.getProfileId();
  uint16_t clusterId      = resp.getClusterId();
  uint8_t  srcEndpoint    = resp.getSrcEndpoint();
  uint8_t  dstEndpoint    = resp.getDstEndpoint();  
  uint8_t *frameData      = resp.getFrameData() + resp.getDataOffset();
  uint8_t frameDataLength = resp.getFrameDataLength() - resp.getDataOffset();

  if (profileId == 0x0000 && srcEndpoint == 0x00 && dstEndpoint == 0x00) {
    /* ZDO command */
    XBeeDevice->processZDO(clusterId, frameData, frameDataLength);
  } else if (profileId == 0x0104) {
    uint8_t frametype = frameData[0] & 0b11;
    if (frametype == 0b00) {
      /* general command frame */ 
      XBeeDevice->processGeneralFrame(resp.getRemoteAddress64(), resp.getRemoteAddress16(), clusterId, srcEndpoint, dstEndpoint, profileId, frameData, frameDataLength);
    } else if (frametype == 0b01) {
      /* cluster specific command frame */
      ZHA_Cluster *cluster = XBeeDevice->getEndpointById(dstEndpoint)->getInClusterById(clusterId);
      cluster->processCommand(frameData, frameDataLength);
    }
  }
}


