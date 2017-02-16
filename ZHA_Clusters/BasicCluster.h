//
// Created by apenner on 2/7/2017.
//

#ifndef XBEEZHA_BASICCLUSTER_H
#define XBEEZHA_BASICCLUSTER_H

#include "../ZHA_Clusters.h"

#define BASIC_CLUSTER_ID                              0x0000
#define BASIC_CLUSTER_ATTRIBUTE_ZCL_VERSION           0x0000
#define BASIC_CLUSTER_ATTRIBUTE_APPLICATION_VERSION   0x0001
#define BASIC_CLUSTER_ATTRIBUTE_STACK_VERSION         0x0002
#define BASIC_CLUSTER_ATTRIBUTE_HW_VERSION            0x0003
#define BASIC_CLUSTER_ATTRIBUTE_MANUFACTURER_NAME     0x0004
#define BASIC_CLUSTER_ATTRIBUTE_MODEL_IDENTIFIER      0x0005
#define BASIC_CLUSTER_ATTRIBUTE_DATE_CODE             0x0006
#define BASIC_CLUSTER_ATTRIBUTE_POWER_SOURCE          0x0007
#define BASIC_CLUSTER_ATTRIBUTE_LOCATION_DESCRIPTION  0x0010
#define BASIC_CLUSTER_ATTRIBUTE_PHYSICAL_ENVIRONMENT  0x0011
#define BASIC_CLUSTER_ATTRIBUTE_DEVICE_ENABLED        0x0012
#define BASIC_CLUSTER_ATTRIBUTE_ALARM_MASK            0x0013
#define BASIC_CLUSTER_ATTRIBUTE_DISABLE_LOCAL_CONFIG  0x0014

class BasicCluster : public ZHA_Cluster {
public:
    BasicCluster();

    void setZCLVersion(uint8_t version);
    void setApplicationVersion(uint8_t version);
    void setStackVersion(uint8_t version);
    void setHWVersion(uint8_t version);
    void setManufacturerName(String manufacturerName);
    void setModelIdentifier(String modelId);
    void setDateCode(String dateCode);
    void setPowerSource(uint8_t powerSource);
    void setLocationDescription(String locationDescription);
    void setPhysicalEnvironment(uint8_t physicalEnvironment);
    void setDeviceEnabled(bool deviceEnabled);
    void setAlarmMask(uint8_t alarmMask);
    void setDisableLocalConfig(uint8_t disableLocalConfig);

private:
    bool processCommand(uint8_t *frameData, uint8_t frameDataLength);
};

BasicCluster::BasicCluster() {
    /* mandatory attributes, default values */
    setZCLVersion(0x01);
    setPowerSource(0x00);

    _clusterId = BASIC_CLUSTER_ID;
}

bool BasicCluster::processCommand(uint8_t *frameData, uint8_t frameDataLength) {}

void BasicCluster::setZCLVersion(uint8_t version) {
    setAttribute(BASIC_CLUSTER_ATTRIBUTE_ZCL_VERSION, ZHA_TYPE_UINT8, version);
}

void BasicCluster::setApplicationVersion(uint8_t version) {
    setAttribute(BASIC_CLUSTER_ATTRIBUTE_APPLICATION_VERSION, ZHA_TYPE_UINT8, version);
}

void BasicCluster::setStackVersion(uint8_t version) {
    setAttribute(BASIC_CLUSTER_ATTRIBUTE_STACK_VERSION, ZHA_TYPE_UINT8, version);
}

void BasicCluster::setHWVersion(uint8_t version) {
    setAttribute(BASIC_CLUSTER_ATTRIBUTE_HW_VERSION, ZHA_TYPE_UINT8, version);
}

void BasicCluster::setManufacturerName(String manufacturerName) {
    setAttribute(BASIC_CLUSTER_ATTRIBUTE_MANUFACTURER_NAME, ZHA_TYPE_CHAR_STRING, manufacturerName);
}

void BasicCluster::setModelIdentifier(String modelId) {
    setAttribute(BASIC_CLUSTER_ATTRIBUTE_MODEL_IDENTIFIER, ZHA_TYPE_CHAR_STRING, modelId);
}

void BasicCluster::setDateCode(String dateCode) {
    setAttribute(BASIC_CLUSTER_ATTRIBUTE_DATE_CODE, ZHA_TYPE_CHAR_STRING, dateCode);
}

void BasicCluster::setPowerSource(uint8_t powerSource) {
    setAttribute(BASIC_CLUSTER_ATTRIBUTE_POWER_SOURCE, ZHA_TYPE_ENUMERATION8, powerSource);
}

void BasicCluster::setLocationDescription(String locationDescription) {
    setAttribute(BASIC_CLUSTER_ATTRIBUTE_LOCATION_DESCRIPTION, ZHA_TYPE_CHAR_STRING, locationDescription);
}

void BasicCluster::setPhysicalEnvironment(uint8_t physicalEnvironment) {
    setAttribute(BASIC_CLUSTER_ATTRIBUTE_PHYSICAL_ENVIRONMENT, ZHA_TYPE_ENUMERATION8, physicalEnvironment);
}

void BasicCluster::setDeviceEnabled(bool deviceEnabled) {
    setAttribute(BASIC_CLUSTER_ATTRIBUTE_DEVICE_ENABLED, ZHA_TYPE_BOOL, deviceEnabled);
}

void BasicCluster::setAlarmMask(uint8_t alarmMask) {
    setAttribute(BASIC_CLUSTER_ATTRIBUTE_ALARM_MASK, ZHA_TYPE_BITMAP8, alarmMask);
}

void BasicCluster::setDisableLocalConfig(uint8_t disableLocalConfig) {
    setAttribute(BASIC_CLUSTER_ATTRIBUTE_DISABLE_LOCAL_CONFIG, ZHA_TYPE_BITMAP8, disableLocalConfig);
}

#endif //XBEEZHA_BASICCLUSTER_H
