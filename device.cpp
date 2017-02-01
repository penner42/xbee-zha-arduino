#include "device.h"
#include "Printers.h"

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

void copyHexL(uint8_t *d, uint32_t v) {
  /* little endian */
  copyHexL(d, (uint16_t)(v));
  copyHexL(d+2, (uint16_t)(v >> 16));
}

void ZHA_Device::sendAT(String at) {
  if (at == "+++") { 
    _devSerial->print(at);
  } else {
    _devSerial->println(at);
  }
  while (_devSerial->available() == 0) { 
#ifdef ESP8266
    yield(); 
#endif
  }
  Serial.println(_devSerial->readString());
}

void ZHA_Device::initializeModem() {
  sendAT("+++");
//  sendAT("ATEE0");
//  sendAT("ATEO0");
  sendAT("ATNR0");
  sendAT("ATAP0");
//  sendAT("ATAO0");
//  sendAT("ATWR");
//  sendAT("ATAC");
//  sendAT("ATCN");
//
//  sendAT("+++");
//  sendAT("ATBD3");
//  sendAT("ATZS2");
  sendAT("ATAP2");
  sendAT("ATSH");
//  sendAT("ATAO3");
//  sendAT("ATEE1");
//  sendAT("ATEO2");
//  sendAT("ATKY5A6967426565416C6C69616E63653039");
  sendAT("ATWR");
  sendAT("ATAC");
//  sendAT("ATCN");
  Serial.println("setup done");
}

ZHA_Device::ZHA_Device() : _addr64(0), _bcast64(0) {
  _addr16 = 0;
  _bcast16 = 0;

  onResponse(printResponseCb, (uintptr_t)(Print*)&Serial);
  onModemStatusResponse(modemStatusCb, (uintptr_t)this);
  onAtCommandResponse(atCommandCb, (uintptr_t)this);
  onPacketError(printErrorCb, (uintptr_t)(Print*)&Serial);
}

void ZHA_Device::addEndpoint(ZHA_Endpoint* endpoint) {
  _endpoints.add(endpoint);
}

void ZHA_Device::setSerial(Stream &serial) {
  XBeeWithCallbacks::setSerial(serial);
  _devSerial = &serial;
}

/* Send the device announce command */
void ZHA_Device::sendAnnounce() {
  uint8_t capability = 0b00001000; /* For now, just receiver on during idle times */
  uint8_t announce_cluster = 0x13;
  _payload[0] = getNextFrameId();
  copyHexL(&_payload[1], _addr16);
  copyHexL(&_payload[3], _addr64.getLsb());
  copyHexL(&_payload[7], _addr64.getMsb());
  _payload[11] = capability;

  ZBExplicitTxRequest announce(_bcast64, 0xFFFC, 0, 0, (uint8_t*)&_payload, 12, _payload[0], 0, 0, announce_cluster, 0);
  send(announce);
}

void ZHA_Device::processZDO(XBeeAddress64 dst64, uint16_t dst16, uint16_t clusterId, uint8_t *frameData, uint8_t frameDataLength) {
  if (clusterId == 0x0004) {
    _addr16 = ((uint16_t)frameData[2] << 8) | frameData[1];
    ZHA_Endpoint* ep = getEndpointById(frameData[3]);
    if (ep) { 
      uint8_t numInClusters = ep->getNumInClusters();
      uint8_t numOutClusters = ep->getNumOutClusters();
      _payloadLength = 12; //13 + (2 * (numInClusters + numOutClusters));
      _payload[0] = frameData[0];
      _payload[1] = STATUS_SUCCESS;
      copyHexL(&_payload[2], _addr16);
//      _payload[4] = _payloadLength - 5;
      _payload[5] = ep->getEndpointId();
      copyHexL(&_payload[6], (uint16_t)0x0104);
      copyHexL(&_payload[8], (uint16_t)0x0002);
      _payload[10] = 0x40; 
      _payload[11] = numInClusters;
      for (uint8_t i = 0; i < numInClusters; i++) {
        copyHexL(&_payload[_payloadLength], ep->getInCluster(i)->getClusterId());
        _payloadLength += 2;
      }
      _payload[_payloadLength] = numOutClusters;
      _payloadLength++;
      for (uint8_t i = 0; i < numOutClusters; i++) {
        copyHexL(&_payload[_payloadLength], ep->getOutCluster(i)->getClusterId());
        _payloadLength += 2;
      }
      _payload[4] = _payloadLength - 5;
      ZBExplicitTxRequest SDR(dst64, dst16, 0, 0, (uint8_t*)&_payload, _payloadLength, getNextFrameId(), 0, 0, (uint16_t)0x8004, 0);
      send(SDR);
    }
  } else if (clusterId == 0x0005) {
    _addr16 = ((uint16_t)frameData[2] << 8) | frameData[1];
    _payload[0] = frameData[0];
    _payload[1] = STATUS_SUCCESS;
    copyHexL(&_payload[2], _addr16);
    _payload[4] = _endpoints.size();
    for (uint8_t i = 0; i < _endpoints.size(); i++) {
      _payload[i + 5] = _endpoints.get(i)->getEndpointId();
    }
    _payloadLength = 5 + _endpoints.size();
    ZBExplicitTxRequest endpoints(dst64, dst16, 0, 0, (uint8_t*)&_payload, _payloadLength, getNextFrameId(), 0, 0, (uint16_t)0x8005, 0);
    send(endpoints);
  } else if (clusterId == 0x0006) {
    _addr16 = ((uint16_t)frameData[2] << 8) | frameData[1];
    uint16_t profile_id = ((uint16_t)frameData[4] << 8) | frameData[3];
    uint8_t numInClusters = frameData[5];
    for (uint8_t i = 1; i <= numInClusters; i++) {
      uint16_t cluster = ((uint16_t)frameData[5+2*i] << 8) | frameData[4+(2*i)];
      Serial.println(cluster, HEX);
    }
    uint8_t numOutClusters = frameData[6+2*numInClusters];
    for (uint8_t i = 1; i <= numOutClusters; i++) {
      uint16_t cluster = ((uint16_t)frameData[7+2*(i+numInClusters)] << 8) | frameData[6+2*(i+numInClusters)];
      Serial.println(cluster, HEX);
    }
    _payload[0] = frameData[0];
    _payload[1] = STATUS_SUCCESS;
    copyHexL(&_payload[2], _addr16);
    _payload[4] = 0;
    ZBExplicitTxRequest endpoints(dst64, dst16, 0, 0, (uint8_t*)&_payload, 5, getNextFrameId(), 0, 0, (uint16_t)0x8006, 0);
    send(endpoints);
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
  Serial.println("asdf");
  if (strncmp((char*)cmd, "SH", 2) == 0) {
    /* set high 32 bits of 64 bit address */
    XBeeDevice->_addr64.setMsb( ((uint32_t)value[0]) << 24 | ((uint32_t)value[1]) << 16 | ((uint32_t)value[2]) << 8 | (uint32_t)value[3]);
    req.setCommand((uint8_t*)"SL");
    req.setFrameId(XBeeDevice->getNextFrameId());
    XBeeDevice->send(req);    
  } else if (strncmp((char*)cmd, "SL", 2) == 0) {
    /* set low 32 bits of 64 bit address */
    XBeeDevice->_addr64.setLsb( ((uint32_t)value[0]) << 24 | ((uint32_t)value[1]) << 16 | ((uint32_t)value[2]) << 8 | (uint32_t)value[3]);
    req.setCommand((uint8_t*)"MY");
    req.setFrameId(XBeeDevice->getNextFrameId());
    XBeeDevice->send(req);    
  } else if (strncmp((char*)cmd, "MY", 2) == 0) {
    /* set 16 bit address */
    XBeeDevice->_addr16 = ((uint16_t)value[0] << 8) | value[1];
    /* send announce */
    //XBeeDevice->sendAnnounce();
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
      req.setCommand((uint8_t*)"SH");
      req.setFrameId(XBeeDevice->getNextFrameId());
      XBeeDevice->send(req);
      XBeeDevice->onZBExplicitRxResponse(explicitRxCb, (uintptr_t)XBeeDevice);
      break;
    default:
      printResponseCb(status, (uintptr_t)(Print*)&Serial);
  }
}

void ZHA_Device::processGeneralFrame(XBeeAddress64 dst64, uint16_t dst16, uint16_t clusterId, uint8_t dstEndpoint, uint8_t srcEndpoint, uint16_t profileId, uint8_t *frameData, uint8_t frameDataLength) {
  uint8_t frameId = frameData[1];
  uint8_t command = frameData[2];
  ZHA_Cluster *cluster = getEndpointById(srcEndpoint)->getInClusterById(clusterId);
  if (command == ZCL_READ_ATTRIBUTES) {
    _payloadLength = 3;
    _payload[0] = 0b00011000;
    _payload[1] = frameId;
    _payload[2] = ZCL_READ_ATTRIBUTES_RESPONSE;
    for (uint8_t i = 3; i < frameDataLength; i+=2) {
      uint16_t attrId = ((uint16_t)frameData[i+1] << 8) | frameData[i];
      Attribute *attr = cluster->getAttrById(attrId);
      if (attr == NULL) {
        /* attribute is undefined */
        copyHexL(&_payload[_payloadLength], attrId);
        _payload[_payloadLength + 2] = STATUS_UNSUPPORTED_ATTRIBUTE;
        _payloadLength += 3;
      } else {
        /* Staples Connect won't recognize device without manufacturer name and model ID */
        if (attr->getAttrType() == ZHA_TYPE_CHARACTER_STRING) {
          copyHexL(&_payload[_payloadLength], attrId);
          _payload[_payloadLength + 2] = STATUS_SUCCESS;
          _payload[_payloadLength + 3] = ZHA_TYPE_CHARACTER_STRING;
          _payload[_payloadLength + 4] = 0x1;
          _payload[_payloadLength + 5] = 0x41;
          _payloadLength += 6;
        }
      }
    }
    ZBExplicitTxRequest attrresp(dst64, dst16, 0, 0, (uint8_t*)&_payload, _payloadLength, getNextFrameId(), srcEndpoint, dstEndpoint, clusterId, profileId);
    send(attrresp);
  } else if (command == ZCL_DISCOVER_ATTRIBUTES) {
    _payloadLength = 4;
    _payload[0] = 0b00011000;
    _payload[1] = frameId;
    _payload[2] = ZCL_DISCOVER_ATTRIBUTES_RESPONSE;
    uint16_t first_attr = ((uint16_t)frameData[4] << 8) | frameData[3];
    uint8_t max_attrs = frameData[5];
    uint8_t attr_index = cluster->getAttrIndexById(first_attr);
    uint8_t num_attrs = cluster->numAttributes();
    bool done = false;
    Attribute *attr;
    for (uint8_t i = 0; i < max_attrs; i++) {
      if (i + attr_index + 1 > num_attrs) {
        done = true;
        break;
      }
      attr = cluster->getAttrByIndex(i + attr_index);
      copyHexL(&_payload[_payloadLength], attr->getAttrId());
      _payload[_payloadLength + 2] = attr->getAttrType();
      _payloadLength += 3;
    }
    _payload[3] = done;
    ZBExplicitTxRequest dar(dst64, dst16, 0, 0, (uint8_t*)&_payload, _payloadLength, getNextFrameId(), srcEndpoint, dstEndpoint, clusterId, profileId);
    send(dar);
  } else if (command == ZCL_CONFIGURE_REPORTING) {
    _payloadLength = 3;
    _payload[0] = 0b00011000;
    _payload[1] = frameId;
    _payload[2] = ZCL_CONFIGURE_REPORTING_RESPONSE;
    uint8_t i = 3;
    Serial.println(frameDataLength);
    while (i < frameDataLength) {
      if (frameData[i] == 0x00) {
        /* send reports */
        uint16_t attrId = ((uint16_t)frameData[i+2] << 8) | frameData[i+1];
        Attribute *attr = cluster->getAttrById(attrId);
        Serial.println("here1");
        if (attr) {
          Serial.println("here2");
          uint8_t datatype = frameData[i+3];
          uint16_t minimum_interval = ((uint16_t)frameData[i+5] << 8) | frameData[i+4];
          uint16_t maximum_interval = ((uint16_t)frameData[i+7] << 8) | frameData[i+6];
          uint16_t reportable_change = 0x0000;
          uint16_t timeout_period = 0x0000;
          attr->configureReporting(datatype, minimum_interval, maximum_interval, reportable_change, timeout_period);
          i += 8;
          _payload[_payloadLength++] = STATUS_SUCCESS;
          _payload[_payloadLength++] = 0x00;
          copyHexL(&_payload[_payloadLength++], attrId);
        }
      } else if (frameData[i] == 0x01) {
        /* receive reports */
      }
    }
    ZBExplicitTxRequest crr(dst64, dst16, 0, 0, (uint8_t*)&_payload, _payloadLength, getNextFrameId(), srcEndpoint, dstEndpoint, clusterId, profileId);
    send(crr);
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
    XBeeDevice->processZDO(resp.getRemoteAddress64(), resp.getRemoteAddress16(), clusterId, frameData, frameDataLength);
  } else if (profileId == 0x0104) {
    uint8_t frametype = frameData[0] & 0b11;
    if (frametype == 0b00) {
      /* general command frame */ 
      Serial.println("general command");
      XBeeDevice->processGeneralFrame(resp.getRemoteAddress64(), resp.getRemoteAddress16(), clusterId, srcEndpoint, dstEndpoint, profileId, frameData, frameDataLength);
    } else if (frametype == 0b01) {
      /* cluster specific command frame */
      Serial.println("cluster specific command");
      ZHA_Cluster *cluster = XBeeDevice->getEndpointById(dstEndpoint)->getInClusterById(clusterId);
      cluster->processCommand(frameData, frameDataLength);
    }
  }
}

void ZHA_Device::reportAttributes() {
  for (uint8_t eps = 0; eps < _endpoints.size(); eps++) {
    ZHA_Endpoint *ep = _endpoints.get(eps);
    for (uint8_t ics = 0; ics < ep->getNumInClusters(); ics++) {
      ZHA_Cluster *ic = ep->getInCluster(ics);
      for (uint8_t ats = 0; ats < ic->numAttributes(); ats++) {
        Attribute *at = ic->getAttrByIndex(ats);
        if (at->needsReporting()) {
          _payload[0] = 0b00011000;
          _payload[1] = 0;
          _payload[2] = ZCL_REPORT_ATTRIBUTES;
          copyHexL(&_payload[3], at->getAttrId());
          _payload[5] = at->getAttrType();
          _payload[6] = at->getValueBool();
          _payloadLength = 7;
          ZBExplicitTxRequest crr(_bcast64, _bcast16, 0, 0, (uint8_t*)&_payload, _payloadLength, getNextFrameId(), 0x8, 0x1, 0x0006, 0x0104);
          send(crr);
          at->markReported();
        }
      }
    }
  }
}

void ZHA_Device::loop() {
  XBeeWithCallbacks::loop();
  reportAttributes();
#ifdef ESP8266
  yield(); 
#endif
}

