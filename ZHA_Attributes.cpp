#include "ZHA_Attributes.h"
#include "ZHA_Utils.h"
#include "Printers.h"

ZHA_Attribute::ZHA_Attribute(uint16_t attrId, uint8_t datatype, uint64_t value) {
  _attrId = attrId;
  _datatype = datatype;
  _value = value;
  _reporting = false;
  _unreported = false;
};

ZHA_Attribute::ZHA_Attribute(uint16_t attrId, uint8_t datatype, String value) {
  _attrId = attrId;
  _datatype = datatype;
  _strValue = String(value);
  _reporting = false;
  _unreported = false;
};

uint16_t ZHA_Attribute::getAttrId() {
  return _attrId;
}

uint8_t ZHA_Attribute::getValueUINT8() {
  return (uint8_t)_value;
}

bool ZHA_Attribute::getValueBool() {
  return (bool)_value;
}

uint8_t ZHA_Attribute::getAttrType() {
  return _datatype;
}

bool ZHA_Attribute::needsReporting() {
  return _reporting && _unreported;
}

void ZHA_Attribute::markReported() {
  _unreported = false;
}

void ZHA_Attribute::set(uint64_t value) {
  _value = value;
  if (_reporting) {
    _unreported = true;
  }
}

void ZHA_Attribute::set(String value) {
  _strValue = value;
  if (_reporting) {
    _unreported = true;
  }
}

uint16_t ZHA_Attribute::getAttrSize() {
  switch (_datatype) {
    case ZHA_TYPE_NULL:
      return 0;
      break;
    case ZHA_TYPE_BOOL:
    case ZHA_TYPE_UINT8:
    case ZHA_TYPE_8BIT_ENUMERATION:
    case ZHA_TYPE_8BIT_BITMAP:
      return 1;
      break;
    case ZHA_TYPE_UINT16:
      return 2;
      break;
    default:
      return 0;
  }
}

uint8_t ZHA_Attribute::copyPayload(uint8_t *payload) {
  /* Copy attribute to payload field, return size */
  switch(_datatype) {
    case ZHA_TYPE_NULL:
      return 0;
      break;
    case ZHA_TYPE_BOOL:
    case ZHA_TYPE_UINT8:
    case ZHA_TYPE_8BIT_ENUMERATION:
    case ZHA_TYPE_8BIT_BITMAP:
      /* for simple types, size is known, payload is just value */
      payload[0] = (uint8_t)_value;
      return 1;
      break;
    case ZHA_TYPE_UINT16:
      copyHexL(payload, (uint16_t)_value);
      return 2;
      break;
    case ZHA_TYPE_CHARACTER_STRING:
      /* payload is length/size of value, followed by value */
      payload[0] = (uint8_t)_strValue.length();
      _strValue.getBytes((byte*)&payload[1], _strValue.length()+1);
      return _strValue.length() + 1;
      break;
    default:
      return 0;
  }
}

String ZHA_Attribute::toString() {
  switch(_datatype) {
    case ZHA_TYPE_NULL:
      return String("NULL");
          break;
    case ZHA_TYPE_BOOL:
    case ZHA_TYPE_UINT8:
    case ZHA_TYPE_8BIT_ENUMERATION:
    case ZHA_TYPE_8BIT_BITMAP:
      /* for simple types, size is known, payload is just value */
      return (String)((uint8_t)_value);
          break;
    case ZHA_TYPE_UINT16:
      return (String)((uint16_t)_value);
          break;
    case ZHA_TYPE_CHARACTER_STRING:
      /* payload is length/size of value, followed by value */
      return _strValue;
          break;
    default:
      return String("Attribute type not implemented yet.");
  }
}

uint8_t ZHA_Attribute::configureReporting(uint8_t datatype, uint16_t minimum_interval, uint16_t maximum_interval, uint8_t reportable_change, uint16_t timeout_period) {
  _last_reported = 0;
  _minimum_reporting_interval = minimum_interval;
  _maximum_reporting_interval = maximum_interval;
  _reportable_change = reportable_change;
  _timeout_period = timeout_period;
  _reporting = true;
}
