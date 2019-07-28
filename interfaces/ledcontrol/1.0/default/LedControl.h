/*
*  Copyright (C) 2019 Androsov Alexander <eelleekk@gmail.com>
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License version 2 as
*  published by the Free Software Foundation.
*/

#ifndef VENDOR_GLOBALLOGIC_LEDCONTROL_V1_0_LEDCONTROL_H_
#define VENDOR_GLOBALLOGIC_LEDCONTROL_V1_0_LEDCONTROL_H_

#include <vendor/globallogic/ledcontrol/1.0/ILedControl.h>

namespace vendor {
namespace globallogic {
namespace ledcontrol {
namespace V1_0 {

using namespace android::hardware;

static const char LED_DEVICE_PREFIX[] = "/sys/class/leds";
static const char LED_TRIGGER[] = "trigger";
static const char LED_TARGET_TRIGGER[] = "none";
static const char LED_BRIGHTNESS[] = "bightness";
static const char LED_ON_VALUE[] = "255";
static const char LED_OFF_VALUE[] = "0";
static const uint8_t LED_STATUS_ON = 1;
static const uint8_t LED_STATUS_OFF = 0;
static const uint8_t TOTAL_LEDS = 4;

class LedControl: public ILedControl {
public:
    LedControl();
    Return<int32_t> initializeLedControl(void) override;
    Return<int32_t> terminateLedControl(void) override;
    Return<int32_t> setLedStatus(int32_t led, int32_t state) override;
	Return<int32_t> getLedStatus(int32_t led) override;

private:
    Return<bool> device_exists(const char *file);
    Return<int32_t> check_HW();
    Return<int32_t> write_value(const char *file, const char *value);    
    Return<int32_t> read_value(const char *file, char *buf, uint8_t size);    
};
} // namespace V1_0
} // namespace ledcontrol
} // namespace globallogic
} // namespace vendor

#endif //VENDOR_GLOBALLOGIC_LEDCONTROL_V1_0_LEDCONTROL_H_