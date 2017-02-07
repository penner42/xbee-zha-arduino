#ifndef _ZHA_CLUSTERS_H_
#define _ZHA_CLUSTERS_H_

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "attributes.h"
#include <LinkedList.h>

/* ZHA profile cluster IDs */
#define BASIC_CLUSTER_ID 								              0x0000
#define BASIC_CLUSTER_ATTRIBUTE_ZCL_VERSION 		  	  0x0000
#define BASIC_CLUSTER_ATTRIBUTE_APPLICATION_VERSION 	0x0001
#define BASIC_CLUSTER_ATTRIBUTE_STACK_VERSION			    0x0002
#define BASIC_CLUSTER_ATTRIBUTE_HW_VERSION				    0x0003
#define BASIC_CLUSTER_ATTRIBUTE_MANUFACTURER_NAME		  0x0004
#define BASIC_CLUSTER_ATTRIBUTE_MODEL_IDENTIFIER    	0x0005
#define BASIC_CLUSTER_ATTRIBUTE_DATE_CODE				      0x0006
#define BASIC_CLUSTER_ATTRIBUTE_POWER_SOURCE			    0x0007
#define BASIC_CLUSTER_ATTRIBUTE_LOCATION_DESCRIPTION	0x0010
#define BASIC_CLUSTER_ATTRIBUTE_PHYSICAL_ENVIRONMENT	0x0011
#define BASIC_CLUSTER_ATTRIBUTE_DEVICE_ENABLED			  0x0012
#define BASIC_CLUSTER_ATTRIBUTE_ALARM_MASK				    0x0013
#define BASIC_CLUSTER_ATTRIBUTE_DISABLE_LOCAL_CONFIG	0x0014

#define IDENTITY_CLUSTER_ID                           0x0003
#define IDENTITY_CLUSTER_ATTRIBUTE_IDENTIFY_TIME      0x0000

#define GROUPS_CLUSTER_ID                             0x0004
#define GROUPS_CLUSTER_ATTRIBUTE_NAME_SUPPORT         0x0000

#define SCENES_CLUSTER_ID                             0x0005
#define SCENES_CLUSTER_ATTRIBUTE_SCENE_COUNT          0x0000
#define SCENES_CLUSTER_ATTRIBUTE_CURRENT_SCENE        0x0001
#define SCENES_CLUSTER_ATTRIBUTE_CURRENT_GROUP        0x0002
#define SCENES_CLUSTER_ATTRIBUTE_SCENE_VALID          0x0003
#define SCENES_CLUSTER_ATTRIBUTE_NAME_SUPPORT         0x0004

#define ON_OFF_CLUSTER_ID                           	0x0006
#define ON_OFF_CLUSTER_ATTRIBUTE_ON_OFF               0x0000
#define ON_OFF_CLUSTER_OFF_COMMAND                    0x00
#define ON_OFF_CLUSTER_ON_COMMAND                     0x01
#define ON_OFF_CLUSTER_TOGGLE_COMMAND                 0x02

class ZHA_Cluster { 
public:
	ZHA_Cluster();
	
	uint16_t getClusterId();
	ZHA_Attribute* getAttrById(uint16_t attrId);
  int getAttrIndexById(uint16_t attrId);
  ZHA_Attribute* getAttrByIndex(int index);
  uint8_t numAttributes() { return _attrs.size(); } 
  virtual bool processCommand(uint8_t *frameData, uint8_t frameDataLength) = 0;
      
protected: 
  LinkedList<ZHA_Attribute*> _attrs;
	uint16_t _clusterId;

};

class BasicCluster : public ZHA_Cluster {
public:
	BasicCluster();
  
private:
  bool processCommand(uint8_t *frameData, uint8_t frameDataLength) { };
};

class IdentifyCluster : public ZHA_Cluster {
public:
  IdentifyCluster();

private:
  bool processCommand(uint8_t *frameData, uint8_t frameDataLength) { };
};

class GroupsCluster : public ZHA_Cluster {
public:
  GroupsCluster();

private:
  bool processCommand(uint8_t *frameData, uint8_t frameDataLength) { };
};

class ScenesCluster : public ZHA_Cluster {
public:
  ScenesCluster();

private:
  bool processCommand(uint8_t *frameData, uint8_t frameDataLength) { };
};

class OnOffCluster : public ZHA_Cluster {
public:
  OnOffCluster();
  bool processCommand(uint8_t *frameData, uint8_t frameDataLength);
  virtual void on() = 0;
  virtual void off() = 0;
  virtual void toggle() = 0;

//.private:
  void _on();
  void _off();
  void _toggle();  
};

class LevelControlCluster : public ZHA_Cluster {
public:
  LevelControlCluster();

private:
  bool processCommand(uint8_t *frameData, uint8_t frameDataLength) { };
};

class ZLLCluster : public ZHA_Cluster {
public:
  ZLLCluster();

private:
  bool processCommand(uint8_t *frameData, uint8_t frameDataLength) { };
};

#endif
