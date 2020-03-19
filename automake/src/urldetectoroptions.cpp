#include "urldetectoroptions.h"
UrlDetectorOptions UrlDetectorOptions_T::get(int index) {
    switch (index) {
    case 0:
        return Default;
    case 1:
        return QUOTE_MATCH;
    case 2:
        return SINGLE_QUOTE_MATCH;
    case 4:
        return BRACKET_MATCH;
    case 5:
        return JSON;
    case 7:
        return JAVASCRIPT;
    case 9:
        return XML;
    case 27:
        return HTML;
    case 32:
        return ALLOW_SINGLE_LEVEL_DOMAIN;
    default:
        return Default;
    }
}
int UrlDetectorOptions_T::getValue(UrlDetectorOptions flag) {
    int value;
    switch (flag) {
    case Default:
        value = 0;
        break;
    case QUOTE_MATCH:
        value = 1;
        break;
    case SINGLE_QUOTE_MATCH:
        value = 2;
        break;
    case BRACKET_MATCH:
        value = 4;
        break;
    case JSON:
        value = 5;
        break;
    case JAVASCRIPT:
        value = 7;
        break;
    case XML:
        value = 9;
        break;
    case HTML:
        value = 27;
        break;
    case ALLOW_SINGLE_LEVEL_DOMAIN:
        value = 32;
        break;
    default:
        value = 0;
        break;
    }
    return value;
}
/**
 * Creates a new Options enum
 * @param value The numeric value of the enum
 */
UrlDetectorOptions_T::UrlDetectorOptions_T(UrlDetectorOptions value) {
    _value = getValue(value);
}
UrlDetectorOptions_T::UrlDetectorOptions_T() {
    _value = 0;
}
/**
 * Checks if the current options have the specified flag.
 * @param flag The flag to check for.
 * @return True if this flag is active, else false.
 */
bool UrlDetectorOptions_T::hasFlag(UrlDetectorOptions flag) {
    int temp = getValue(flag);
    return (_value & temp) == temp;
}
