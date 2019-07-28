/*
*  Copyright (C) 2019 Androsov Alexander <eelleekk@gmail.com>
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License version 2 as
*  published by the Free Software Foundation.
*/

#define LOG_TAG "vendor.globallogic.ledcontrol@1.0-service-hikey960"

#include <hidl/HidlSupport.h>
#include <hidl/HidlTransportSupport.h>
#include "LedControl.h"
#include <log/log.h>

using ::android::hardware::configureRpcThreadpool;
using ::android::hardware::joinRpcThreadpool;
using ::android::OK;
using ::android::sp;
using namespace vendor::globallogic::ledcontrol::V1_0;

int main(int /* argc */, char* /* argv */ []) {
    sp<LedControl> ledcontrol = new LedControl();
    configureRpcThreadpool(1, true /* will join */);
    if (ledcontrol->registerAsService() != OK) {
        ALOGE("Could not register LedControl 1.0 service.");
        return 1;
    } else {
        ALOGI("LedControl 1.0 registered.");
    }
    joinRpcThreadpool();

    ALOGE("Service exited!");
    return 1;
}
