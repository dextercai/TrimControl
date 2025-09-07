//
// Created by dextercai on 2025/9/5.
//
#include "global.h"




AS5048A as5048a_1;

// uint16_t led_delay_ms = 255;

MedianFilter mf_filter;

// uint16_t as5048a_last_call = 0;

HID_Report14_t report14 = {
    0x14, 512, 0
};