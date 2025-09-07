//
// Created by dextercai on 25-8-6.
//

#ifndef REPORT_0X14_H
#define REPORT_0X14_H
#include <stdint.h>
#pragma pack(push, 1)
typedef struct {
    uint8_t report_id; // 0x14
    uint16_t dial   : 10;
    uint8_t unused : 6;
} HID_Report14_t;
#pragma pack(pop)

#endif //REPORT_0X14_H
