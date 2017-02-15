#include "ZHA_DeviceManager.h"
#define DEBUG
ZHA_DeviceManager::ZHA_DeviceManager() : _addr64(0), _bcast64(0) {
    _addr16 = 0;
    _bcast16 = 0;

//    onResponse(printResponseCb, (uintptr_t) (Print * ) & Serial);
    onModemStatusResponse(modemStatusCb, (uintptr_t) this);
    onAtCommandResponse(atCommandCb, (uintptr_t) this);
    onPacketError(printErrorCb, (uintptr_t) (Print * ) & Serial);
}

void ZHA_DeviceManager::sendAT(String at) {
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

void ZHA_DeviceManager::initializeModem() {
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

void ZHA_DeviceManager::setSerial(Stream &serial) {
    XBeeWithCallbacks::setSerial(serial);
    _devSerial = &serial;
}

/* Send the device announce command */
void ZHA_DeviceManager::sendAnnounce() {
    uint8_t capability = 0b00001000; /* For now, just receiver on during idle times */
    uint8_t announce_cluster = 0x13;
    _payload[0] = getNextFrameId();
    copyHexL(&_payload[1], _addr16);
    copyHexL(&_payload[3], _addr64.getLsb());
    copyHexL(&_payload[7], _addr64.getMsb());
    _payload[11] = capability;

    ZBExplicitTxRequest announce(_bcast64, 0xFFFC, 0, 0, (uint8_t *) &_payload, 12, _payload[0], 0, 0, announce_cluster,
                                 0);
    send(announce);
}

ZHA_Device *ZHA_DeviceManager::getDeviceByEndpoint(uint8_t endpointId) {
    for (uint8_t i = 0; i < _deviceList.size(); i++) {
        if (endpointId == _deviceList.get(i)->getEndpointId()) {
            return _deviceList.get(i);
        }
    }
    return NULL;
}

void ZHA_DeviceManager::processZDO(XBeeAddress64 dst64, uint16_t dst16, uint16_t clusterId, uint8_t *frameData,
                                   uint8_t frameDataLength) {
    if (clusterId == ZDO_SIMPLE_DESCRIPTOR_REQUEST) {
#ifdef DEBUG
        Serial.println("ZDO Simple Descriptor Request");
#endif
        _addr16 = ((uint16_t) frameData[2] << 8) | frameData[1];
        ZHA_Device *dev = getDeviceByEndpoint(frameData[3]);
        if (dev) {
            uint8_t numInClusters = dev->getNumInClusters();
            uint8_t numOutClusters = dev->getNumOutClusters();
            _payloadLength = 12; //13 + (2 * (numInClusters + numOutClusters));
            _payload[0] = frameData[0];
            _payload[1] = STATUS_SUCCESS;
            copyHexL(&_payload[2], _addr16);
            _payload[5] = dev->getEndpointId();
            copyHexL(&_payload[6], (uint16_t) 0x0104);
            copyHexL(&_payload[8], (uint16_t) dev->getDeviceId());
            _payload[10] = 0x40;
            _payload[11] = numInClusters;
            for (uint8_t i = 0; i < numInClusters; i++) {
                copyHexL(&_payload[_payloadLength], dev->getInCluster(i)->getClusterId());
                _payloadLength += 2;
            }
            _payload[_payloadLength] = numOutClusters;
            _payloadLength++;
            for (uint8_t i = 0; i < numOutClusters; i++) {
                copyHexL(&_payload[_payloadLength], dev->getOutCluster(i)->getClusterId());
                _payloadLength += 2;
            }
            _payload[4] = _payloadLength - 5;
            ZBExplicitTxRequest SDR(dst64, dst16, 0, 0, (uint8_t *) &_payload, _payloadLength, getNextFrameId(), 0, 0,
                                    ZDO_SIMPLE_DESCRIPTOR_RESPONSE, 0);
            send(SDR);
        }
    } else if (clusterId == ZDO_ACTIVE_ENDPOINTS_REQUEST) {
#ifdef DEBUG
        Serial.println("ZDO Active Endpoints Request");
#endif
        _addr16 = ((uint16_t) frameData[2] << 8) | frameData[1];
        _payload[0] = frameData[0];
        _payload[1] = STATUS_SUCCESS;
        copyHexL(&_payload[2], _addr16);
        _payload[4] = 1; //_endpoints.size();
        for (uint8_t i = 0; i < _deviceList.size(); i++) {
            _payload[i + 5] = _deviceList.get(i)->getEndpointId();
        }
        _payloadLength = 5 + _deviceList.size();
        ZBExplicitTxRequest endpoints(dst64, dst16, 0, 0, (uint8_t *) &_payload, _payloadLength, getNextFrameId(), 0, 0,
                                      ZDO_ACTIVE_ENDPOINTS_RESPONSE, 0);
        send(endpoints);
    } else if (clusterId == ZDO_MATCH_DESCRIPTOR_REQUEST) {
#ifdef DEBUG
        Serial.println("ZDO Match Descriptor Request");
#endif
        _addr16 = ((uint16_t) frameData[2] << 8) | frameData[1];
        uint16_t profile_id = ((uint16_t) frameData[4] << 8) | frameData[3];
        uint8_t numInClusters = frameData[5];
        for (uint8_t i = 1; i <= numInClusters; i++) {
            uint16_t cluster = ((uint16_t) frameData[5 + 2 * i] << 8) | frameData[4 + (2 * i)];
            Serial.println(cluster, HEX);
        }
        uint8_t numOutClusters = frameData[6 + 2 * numInClusters];
        for (uint8_t i = 1; i <= numOutClusters; i++) {
            uint16_t cluster =
                    ((uint16_t) frameData[7 + 2 * (i + numInClusters)] << 8) | frameData[6 + 2 * (i + numInClusters)];
            Serial.println(cluster, HEX);
        }
        _payload[0] = frameData[0];
        _payload[1] = STATUS_SUCCESS;
        copyHexL(&_payload[2], _addr16);
        _payload[4] = 0;
        ZBExplicitTxRequest endpoints(dst64, dst16, 0, 0, (uint8_t *) &_payload, 5, getNextFrameId(), 0, 0,
                                      (uint16_t) ZDO_MATCH_DESCRIPTOR_RESPONSE, 0);
        send(endpoints);
    }
}

void ZHA_DeviceManager::atCommandCb(AtCommandResponse &at, uintptr_t data) {
    ZHA_DeviceManager *XBeeDevice = (ZHA_DeviceManager *) data;

    uint8_t *cmd = at.getCommand();
    uint8_t *value = at.getValue();
    AtCommandRequest req;
    if (strncmp((char *) cmd, "SH", 2) == 0) {
        /* set high 32 bits of 64 bit address */
        XBeeDevice->_addr64.setMsb(
                ((uint32_t) value[0]) << 24 | ((uint32_t) value[1]) << 16 | ((uint32_t) value[2]) << 8 |
                (uint32_t) value[3]);
        req.setCommand((uint8_t *) "SL");
        req.setFrameId(XBeeDevice->getNextFrameId());
        XBeeDevice->send(req);
    } else if (strncmp((char *) cmd, "SL", 2) == 0) {
        /* set low 32 bits of 64 bit address */
        XBeeDevice->_addr64.setLsb(
                ((uint32_t) value[0]) << 24 | ((uint32_t) value[1]) << 16 | ((uint32_t) value[2]) << 8 |
                (uint32_t) value[3]);
        req.setCommand((uint8_t *) "MY");
        req.setFrameId(XBeeDevice->getNextFrameId());
        XBeeDevice->send(req);
    } else if (strncmp((char *) cmd, "MY", 2) == 0) {
        /* set 16 bit address */
        XBeeDevice->_addr16 = ((uint16_t) value[0] << 8) | value[1];
        /* send announce */
        //XBeeDevice->sendAnnounce();
    }
}

void ZHA_DeviceManager::modemStatusCb(ModemStatusResponse &status, uintptr_t data) {
    ZHA_DeviceManager *XBeeDevice = (ZHA_DeviceManager *) data;
    AtCommandRequest req;

    switch (status.getStatus()) {
        case HARDWARE_RESET:
            Serial.println("Modem reset.");
            break;
        case ASSOCIATED:
            Serial.println("Joined network.");
            /* Get 16 bit address */
            req.setCommand((uint8_t *) "SH");
            req.setFrameId(XBeeDevice->getNextFrameId());
            XBeeDevice->send(req);
            XBeeDevice->onZBExplicitRxResponse(explicitRxCb, (uintptr_t) XBeeDevice);
            break;
        default:
            printResponseCb(status, (uintptr_t) (Print * ) & Serial);
    }
}

void ZHA_DeviceManager::explicitRxCb(ZBExplicitRxResponse &resp, uintptr_t data) {
    ZHA_DeviceManager *XBeeDevice = (ZHA_DeviceManager *) data;
    uint16_t profileId = resp.getProfileId();
    uint16_t clusterId = resp.getClusterId();
    uint8_t srcEndpoint = resp.getSrcEndpoint();
    uint8_t dstEndpoint = resp.getDstEndpoint();
    uint8_t *frameData = resp.getFrameData() + resp.getDataOffset();
    uint8_t frameDataLength = resp.getFrameDataLength() - resp.getDataOffset();

    if (profileId == 0x0000 && srcEndpoint == 0x00 && dstEndpoint == 0x00) {
        /* ZDO command */
        XBeeDevice->processZDO(resp.getRemoteAddress64(), resp.getRemoteAddress16(), clusterId, frameData,
                               frameDataLength);
    } else if (profileId == ZHA_PROFILE_ID) {
        ZHA_Device *dev = XBeeDevice->getDeviceByEndpoint(dstEndpoint);
        if (dev) {
            bool result = dev->processCommand(frameData, frameDataLength, clusterId, (uint8_t *) &XBeeDevice->_payload,
                                              XBeeDevice->_payloadLength);
            if (result) {
                ZBExplicitTxRequest response(resp.getRemoteAddress64(), resp.getRemoteAddress16(), 0, 0,
                                             (uint8_t *) &XBeeDevice->_payload, XBeeDevice->_payloadLength,
                                             XBeeDevice->getNextFrameId(), dstEndpoint, srcEndpoint, clusterId,
                                             profileId);
                XBeeDevice->send(response);
            }
        }
    }
}

void ZHA_DeviceManager::reportAttributes() {
    for (uint8_t devs = 0; devs < _deviceList.size(); devs++) {
        ZHA_Device *dev = _deviceList.get(devs);
        for (uint8_t ics = 0; ics < dev->getNumInClusters(); ics++) {
            ZHA_Cluster *ic = dev->getInCluster(ics);
            for (uint8_t ats = 0; ats < ic->numAttributes(); ats++) {
                ZHA_Attribute *at = ic->getAttrByIndex(ats);
                if (at->needsReporting()) {
                    _payload[0] = 0b00011000;
                    _payload[1] = 0;
                    _payload[2] = ZCL_REPORT_ATTRIBUTES;
                    copyHexL(&_payload[3], at->getAttrId());
                    _payload[5] = at->getAttrType();
                    _payloadLength = 6 + at->copyPayload((uint8_t*)&_payload[6]);
                    ZBExplicitTxRequest crr(_bcast64, _bcast16, 0, 0, (uint8_t *) &_payload, _payloadLength,
                                            getNextFrameId(), 0x8, 0x1, ic->getClusterId(), 0x0104);
                    send(crr);
                    at->markReported();
                }
            }
        }
    }
}

void ZHA_DeviceManager::addDevice(ZHA_Device *dev) {
    _deviceList.add(dev);
}

void ZHA_DeviceManager::loop() {
    XBeeWithCallbacks::loop();
    reportAttributes();
#ifdef ESP8266
    yield();
#endif
}

