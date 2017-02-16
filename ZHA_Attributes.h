#ifndef _ZHA_ATTRIBUTES_H_
#define _ZHA_ATTRIBUTES_H_

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#define ZHA_TYPE_NULL   0x00

#define ZHA_TYPE_GENERALDATA8  0x08
#define ZHA_TYPE_GENERALDATA16 0x09
#define ZHA_TYPE_GENERALDATA24 0x0a
#define ZHA_TYPE_GENERALDATA32 0x0b
#define ZHA_TYPE_GENERALDATA40 0x0c
#define ZHA_TYPE_GENERALDATA48 0x0d
#define ZHA_TYPE_GENERALDATA56 0x0e
#define ZHA_TYPE_GENERALDATA64 0x0f

#define ZHA_TYPE_BOOL 0x10

#define ZHA_TYPE_BITMAP8  0x18
#define ZHA_TYPE_BITMAP16 0x19
#define ZHA_TYPE_BITMAP24 0x1a
#define ZHA_TYPE_BITMAP32 0x1b
#define ZHA_TYPE_BITMAP40 0x1c
#define ZHA_TYPE_BITMAP48 0x1d
#define ZHA_TYPE_BITMAP56 0x1e
#define ZHA_TYPE_BITMAP64 0x1f

#define ZHA_TYPE_UINT8  0x20
#define ZHA_TYPE_UINT16 0x21
#define ZHA_TYPE_UINT24 0x22
#define ZHA_TYPE_UINT32 0x23
#define ZHA_TYPE_UINT40 0x24
#define ZHA_TYPE_UINT48 0x25
#define ZHA_TYPE_UINT56 0x26
#define ZHA_TYPE_UINT64 0x27

#define ZHA_TYPE_INT8  0x28
#define ZHA_TYPE_INT16 0x29
#define ZHA_TYPE_INT24 0x2a
#define ZHA_TYPE_INT32 0x2b
#define ZHA_TYPE_INT40 0x2c
#define ZHA_TYPE_INT48 0x2d
#define ZHA_TYPE_INT56 0x2e
#define ZHA_TYPE_INT64 0x2f

#define ZHA_TYPE_ENUMERATION8  0x30
#define ZHA_TYPE_ENUMERATION16 0x31

#define ZHA_TYPE_FLOAT_SEMI   0x38
#define ZHA_TYPE_FLOAT_SINGLE 0x39
#define ZHA_TYPE_FLOAT_DOUBLE 0x3a

#define ZHA_TYPE_OCTET_STRING      0x41
#define ZHA_TYPE_CHAR_STRING       0x42
#define ZHA_TYPE_LONG_OCTET_STRING 0x43
#define ZHA_TYPE_LONG_CHAR_STRING  0x44

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
    uint16_t copyPayload(uint8_t *payload);
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
