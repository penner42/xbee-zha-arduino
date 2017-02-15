#ifndef _ZHA_ATTRIBUTES_H_
#define _ZHA_ATTRIBUTES_H_

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#define ZHA_TYPE_NULL   0x00
#define ZHA_TYPE_BOOL   0x10
#define ZHA_TYPE_UINT8  0x20
#define ZHA_TYPE_UINT16 0x21
#define ZHA_TYPE_UINT24 0x22
#define ZHA_TYPE_UINT32 0x23
#define ZHA_TYPE_UINT40 0x24
#define ZHA_TYPE_UINT48 0x25
#define ZHA_TYPE_UINT56 0x26
#define ZHA_TYPE_UINT64 0x27
#define ZHA_TYPE_8BIT_BITMAP      0x18
#define ZHA_TYPE_8BIT_ENUMERATION 0x30
#define ZHA_TYPE_CHARACTER_STRING 0x42

class ZHA_Attribute {
  public:
    ZHA_Attribute(uint16_t attrId, uint8_t datatype, uint64_t value);
    ZHA_Attribute(uint16_t attrId, uint8_t datatype, String value);
    uint16_t getAttrId();
    uint8_t  getValueUINT8();
    bool     getValueBool();
    uint8_t  getAttrType();
    bool     needsReporting();
    void     markReported();
    void set(uint64_t value);
    void set(String value);
    uint8_t copyPayload(uint8_t *payload);
    String toString();

    uint16_t getAttrSize();

    uint8_t configureReporting(uint8_t datatype, uint16_t minimum_interval, uint16_t maximum_interval, uint8_t reportable_change, uint16_t timeout_period);

  private:
    uint8_t _datatype;
    uint16_t _attrId;

    uint64_t _value;
    String _strValue;

    /* reporting configuration */
    bool _reporting;
    bool _unreported;
    uint16_t _minimum_reporting_interval;
    uint16_t _maximum_reporting_interval;
    uint64_t _reportable_change;
    uint16_t _timeout_period;

    unsigned long _last_reported;
};

#endif
