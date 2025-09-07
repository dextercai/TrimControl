//
// Created by dextercai on 2025/9/5.
//

#ifndef TRIMCONTROL_GLOBAL_H
#define TRIMCONTROL_GLOBAL_H

#include <stdint.h>
#include "../Src/as5048a/as5048a.h"
#include "../Src/fliter/median_filter.h"
#include "../Src/report/report_0x14.h"

// extern uint16_t as5048a_last_call;

extern AS5048A as5048a_1;

extern HID_Report14_t report14;

extern MedianFilter mf_filter;

#define DIAL_AXIS_MAX 1023
#define DIAL_AXIS_MIN 0
#endif //TRIMCONTROL_GLOBAL_H