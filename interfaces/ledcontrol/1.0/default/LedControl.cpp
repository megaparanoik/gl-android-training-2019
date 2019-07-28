/*
*  Copyright (C) 2019 Androsov Alexander <eelleekk@gmail.com>
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License version 2 as
*  published by the Free Software Foundation.
*/

#include "LedControl.h"

#include <errno.h>
#include <fcntl.h>
#include <malloc.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define LOG_TAG "LedControl_HAL"
#include <log/log.h>

namespace vendor {
namespace globallogic {
namespace ledcontrol {
namespace V1_0 {

LedControl::LedControl()
{
    ALOGI("Constructor LedControl");    
}

Return<int32_t> LedControl::initializeLedControl(void)
{
    char file_str[50];
    int retcode;

    ALOGI("Initialization LedControl");    

    retcode = check_HW();
    if (retcode != 0) 
        return retcode;

    for (uint8_t led_num = 0; led_num < TOTAL_LEDS; led_num++)
       {
        snprintf(file_str, sizeof(file_str), "%s/led%d/%s", LED_DEVICE_PREFIX, led_num, LED_TRIGGER);
        retcode = write_value(file_str, LED_TARGET_TRIGGER);
        if (retcode != 0)
            {
            ALOGE("Unable to write into %s. Code %d", file_str, retcode);
            return retcode;
            }
        
        snprintf(file_str, sizeof(file_str), "%s/led%d/%s", LED_DEVICE_PREFIX, led_num, LED_BRIGHTNESS);                            
        retcode = write_value(file_str, LED_OFF_VALUE);
        if (retcode != 0)
            {
            ALOGE("Unable to write into %s. Code %d", file_str, retcode);
            return retcode;
            }
        }

    return 0;
}

Return<int32_t> LedControl::terminateLedControl(void)
{
    ALOGI("Terminate LedControl");
    return 0;
}

Return<int32_t> LedControl::setLedStatus(int32_t led, int32_t state)
{
    char file_str[50];
    int retcode;

    ALOGI("SetLedStatus LedControl [%d:%d]", led, state);

    snprintf(file_str, sizeof(file_str), "%s/led%d/%s", LED_DEVICE_PREFIX, (int)led + 1 , LED_BRIGHTNESS);
    if (state != 0) {
        retcode = write_value(file_str, LED_ON_VALUE);
    } else {
        retcode = write_value(file_str, LED_OFF_VALUE);
    }

    if (retcode != 0)
        ALOGE("Unable to write into %s. Code %d", file_str, retcode);

    return retcode;
}

Return<int32_t> LedControl::getLedStatus(int32_t led)
{
    char file_str[50];
    char buffer[10];
    int retcode;

    ALOGI("GetLedStatus LedControl [%d]", led);

    memset(&buffer, 0, sizeof(buffer));
    snprintf(file_str, sizeof(file_str), "%s/led%d/%s", LED_DEVICE_PREFIX, (led + 1) , LED_BRIGHTNESS);
    retcode = read_value(file_str, &buffer[0], sizeof(buffer));
    if (retcode < 0) {
        ALOGE("Unable to read from %s. Code %d", file_str, retcode); 
        return retcode;
    }

    if (atoi(&buffer[0]) == LED_STATUS_OFF)
        return LED_STATUS_OFF;
    else 
        return LED_STATUS_ON;        
}


Return<bool> LedControl::device_exists(const char *file) 
{
    int fd;

    fd = TEMP_FAILURE_RETRY(open(file, O_RDWR));
    if(fd < 0) {
        return false;
    }

    close(fd);
    return true;
}

Return<int32_t> LedControl::check_HW() 
{
    char file_str[50];

    for (uint8_t led_num = 0; led_num < TOTAL_LEDS; led_num++)
       {
        snprintf(file_str, sizeof(file_str), "%s/led%d/%s", LED_DEVICE_PREFIX, led_num, LED_BRIGHTNESS);
        if (!device_exists(file_str))
            ALOGE("Device %s doesn't exist", file_str);
            return -EIO;
       }

    return 0;
}

Return<int32_t> LedControl::write_value(const char *file, const char *value)
{
    int to_write, written, ret, fd;

    fd = TEMP_FAILURE_RETRY(open(file, O_WRONLY));
    if (fd < 0) {
        return -errno;
    }

    to_write = strlen(value) + 1;
    written = TEMP_FAILURE_RETRY(write(fd, value, to_write));
    if (written == -1) {
        ret = -errno;
    } else if (written != to_write) {
        ret = -EAGAIN;
    } else {
        ret = 0;
    }

    errno = 0;
    close(fd);

    return ret;
}

Return<int32_t> LedControl::read_value(const char *file, char *buf, uint8_t size)
{
    int ret, fd, read_size;

    fd = TEMP_FAILURE_RETRY(open(file, O_RDONLY));
    if (fd < 0) {
        return -errno;
    }

    read_size = TEMP_FAILURE_RETRY(read(fd, buf, size));
    if (read_size < 0) {
        ret = -errno;
    } else {
        ret = read_size;
    }
    
    errno = 0;
    close(fd);

    return ret;
}

} //namespace V1_0
} // namespace ledcontrol
} // namespace globallogic
} // namespace vendor