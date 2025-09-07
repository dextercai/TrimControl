//
// Created by dextercai on 2025/9/5.
//

#ifndef TRIMCONTROL_USER_H
#define TRIMCONTROL_USER_H

#include <stdint.h>
// #define MAX_AS5048A ((1<<14)-1)
void As5048Read(void *argument);

int16_t sensor_delta(uint16_t new_val, uint16_t old_val);

void HidReport(void *argument);
#endif //TRIMCONTROL_USER_H