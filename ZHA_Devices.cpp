#include "ZHA_Devices.h"

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

ZHA_Cluster *ZHA_Device::getInCluster(uint8_t num) {
    return _inClusters.get(num);
}

ZHA_Cluster *ZHA_Device::getOutCluster(uint8_t num) {
    return _outClusters.get(num);
}

uint8_t ZHA_Device::getEndpointId() {
    return _endpointId;
}

uint16_t ZHA_Device::getDeviceId() {
    return _deviceId;
}

ZHA_Cluster *ZHA_Device::getInClusterById(uint16_t clusterId) {
    for (uint8_t i = 0; i < _inClusters.size(); i++) {
        if (_inClusters.get(i)->getClusterId() == clusterId) {
            return _inClusters.get(i);
        }
    }
    return NULL;
}

ZHA_Cluster *ZHA_Device::getOutClusterById(uint16_t clusterId) {
    for (uint8_t i = 0; i < _outClusters.size(); i++) {
        if (_outClusters.get(i)->getClusterId() == clusterId) {
            return _outClusters.get(i);
        }
    }
    return NULL;
}

bool
ZHA_Device::processGeneralCommand(uint8_t *frameData, uint8_t frameDataLength, uint16_t clusterId, uint8_t *payload,
                                  uint8_t &payloadLength) {
    /* Command and response ZCL header (not MFR specific)
       ------------------------------------------
       |    Bits: 8     |    8     |     8      |
       |-----------------------------------------
       | Frame Control  | Frame ID | Command ID |
       ------------------------------------------
       Response Frame Control Field
       -------------------------------------------------------------------------------
       |  Bits: 3 |            1             |     1     |       1      |      2     |
       |-----------------------------------------------------------------------------|
       | Reserved | Disable Default Response | Direction | MFR specific | Frame Type |
       |-----------------------------------------------------------------------------|
       |   000    |            1             |    0/1    |       0      |     00     |
       -------------------------------------------------------------------------------
    */
    uint8_t frameId = frameData[1];
    uint8_t command = frameData[2];
    ZHA_Cluster *cluster = getInClusterById(clusterId);
    if (!cluster) {
        return false;
    }
    if (command == ZCL_READ_ATTRIBUTES) {
        /*
          Command
          -------------------------------------------------------------------------
          |  Bits: 8   |    Bits: 16    |    Bits: 16    |  ...  |    Bits: 16    |
          |-----------------------------------------------------------------------|
          | ZCL Header | Attribute ID 1 | Attribute ID 2 |  ...  | Attribute ID n |
          -------------------------------------------------------------------------
          Response payload
          ---------------------------------------------------------------------------
          |  Bits: 8   |    Variable     |    Variable     | ... |     Variable     |
          |-------------------------------------------------------------------------|
          | ZCL Header | Read Attribute  | Read Attribute  | ... |  Read Attribute  |
          |            | Status Record 1 | Status Record 2 |     | Status Record n  |
          ---------------------------------------------------------------------------
          Read Attribute Status Record
          --------------------------------------------------
          |   Bits: 16   |   1    |    0/1    | 0/Variable |
          --------------------------------------------------
          | Attribute Id | Status | Data Type |   Value    |
          --------------------------------------------------
        */
        payloadLength = 3;
        payload[0] = 0b00011000;
        payload[1] = frameId;
        payload[2] = ZCL_READ_ATTRIBUTES_RESPONSE;
        for (uint8_t i = 3; i < frameDataLength; i += 2) {
            uint16_t attrId = ((uint16_t) frameData[i + 1] << 8) | frameData[i];
            ZHA_Attribute *attr = cluster->getAttrById(attrId);
            if (attr == NULL) {
                /* attribute is undefined */
                copyHexL(&payload[payloadLength], attrId);
                payload[payloadLength + 2] = STATUS_UNSUPPORTED_ATTRIBUTE;
                payloadLength += 3;
            } else {
                copyHexL(&payload[payloadLength], attrId);
                payload[payloadLength + 2] = STATUS_SUCCESS;
                payload[payloadLength + 3] = attr->getAttrType();
                payloadLength += 4;
                payloadLength += attr->copyPayload((uint8_t *) &payload[payloadLength]);
            }
        }
        return true;
    } else if (command == ZCL_DISCOVER_ATTRIBUTES) {
        payloadLength = 4;
        payload[0] = 0b00011000;
        payload[1] = frameId;
        payload[2] = ZCL_DISCOVER_ATTRIBUTES_RESPONSE;
        uint16_t first_attr = ((uint16_t) frameData[4] << 8) | frameData[3];
        uint8_t max_attrs = frameData[5];
        uint8_t attr_index = cluster->getAttrIndexById(first_attr);
        uint8_t num_attrs = cluster->numAttributes();
        bool done = false;
        ZHA_Attribute *attr;
        for (uint8_t i = 0; i < max_attrs; i++) {
            if (i + attr_index + 1 > num_attrs) {
                done = true;
                break;
            }
            attr = cluster->getAttrByIndex(i + attr_index);
            copyHexL(&payload[payloadLength], attr->getAttrId());
            payload[payloadLength + 2] = attr->getAttrType();
            payloadLength += 3;
        }
        payload[3] = done;
        return true;
    } else if (command == ZCL_CONFIGURE_REPORTING) {
        payloadLength = 3;
        payload[0] = 0b00011000;
        payload[1] = frameId;
        payload[2] = ZCL_CONFIGURE_REPORTING_RESPONSE;
        uint8_t i = 3;
        Serial.println(frameDataLength);
        while (i < frameDataLength) {
            if (frameData[i] == 0x00) {
                /* send reports */
                uint16_t attrId = ((uint16_t) frameData[i + 2] << 8) | frameData[i + 1];
                ZHA_Attribute *attr = cluster->getAttrById(attrId);
                if (attr) {
                    uint8_t datatype = frameData[i + 3];
                    uint16_t minimum_interval = ((uint16_t) frameData[i + 5] << 8) | frameData[i + 4];
                    uint16_t maximum_interval = ((uint16_t) frameData[i + 7] << 8) | frameData[i + 6];
                    uint16_t reportable_change = 0x0000;
                    uint16_t timeout_period = 0x0000;
                    attr->configureReporting(datatype, minimum_interval, maximum_interval, reportable_change,
                                             timeout_period);
                    i += 8;
                    payload[payloadLength++] = STATUS_SUCCESS;
                    payload[payloadLength++] = 0x00;
                    copyHexL(&payload[payloadLength++], attrId);
                }
            } else if (frameData[i] == 0x01) {
                /* receive reports */
            }
        }
        return true;
    } else {
        return false;
    }
}

bool ZHA_Device::processCommand(uint8_t *frameData, uint8_t frameDataLength, uint16_t clusterId, uint8_t *payload,
                                uint8_t &payloadLength) {
    /* Frame layout
      ---------------------------------------------------------------------
      |    Bits: 8     |   0/16   |    8     |    8       |   Variable    |
      |-------------------------------------------------------------------|
      | Frame Control  | MFR code | Frame ID | Command ID | Frame Payload |
      ---------------------------------------------------------------------
      Frame Control Field
      --------------------------------------------------------------------------------
      }  Bits: 3  |            1             |     1     |       1      |      2     |
      |------------------------------------------------------------------------------|
      | Reserverd | Disable Default Response | Direction | MFR specific | Frame Type |
      --------------------------------------------------------------------------------
    */
    uint8_t frametype = frameData[0] & 0b11;
    if (frametype == 0b00) {
        /* general command frame */
        Serial.println("general command");
        return processGeneralCommand(frameData, frameDataLength, clusterId, payload, payloadLength);
    } else if (frametype == 0b01) {
        /* cluster specific command frame */
        Serial.println("cluster specific command");
        ZHA_Cluster *cluster = getInClusterById(clusterId);
        cluster->processCommand(frameData, frameDataLength);
    }
}

