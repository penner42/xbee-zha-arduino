#ifndef _ZHA_CLUSTERS_H_
#define _ZHA_CLUSTERS_H_

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "ZHA_Attributes.h"
#include <LinkedList.h>

class ZHA_Cluster {
public:
    ZHA_Cluster();

	uint16_t getClusterId();
	ZHA_Attribute* getAttrById(uint16_t attrId);
    int getAttrIndexById(uint16_t attrId);
    ZHA_Attribute* getAttrByIndex(int index);
    uint8_t numAttributes() { return _attrs.size(); }

	void setAttribute(uint16_t attrId, uint8_t datatype, uint64_t value);
	void setAttribute(uint16_t attrId, uint8_t datatype, String value);

    virtual bool processCommand(uint8_t *frameData, uint8_t frameDataLength) = 0;

protected:
    LinkedList<ZHA_Attribute*> _attrs;
	uint16_t _clusterId;
};

#endif
