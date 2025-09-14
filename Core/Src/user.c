//
// Created by dextercai on 2025/9/5.
//

#include "user.h"

#include "cmsis_os.h"
#include "stm32f0xx_hal.h"
#include "usbd_customhid.h"
#include "usb_device.h"
#include "global.h"


void As5048Read(void *argument)
{
    uint16_t res = 0;
    while (1)
    {
        taskENTER_CRITICAL();
        ReadAs5048aReg(&as5048a_1, AS5048_CMD_ANGLE, &res);
        taskEXIT_CRITICAL();
        const float asFs = MedianFilter_Update(&mf_filter, res);
        vTaskDelay(pdMS_TO_TICKS(2));
    }
}

int16_t sensor_delta(uint16_t new_val, uint16_t old_val) {
    const uint16_t M = (1<<14);        // 传感器量程
    const uint16_t half = M / 2;      // 一半量程 = 8192
    int32_t diff = (int32_t)new_val - (int32_t)old_val;
    // 把差值归一化到 (-M/2, M/2]
    if (diff > half) {
        diff -= M;
    } else if (diff <= -half) {
        diff += M;
    }
    return (int16_t)diff;
}


void HidReport(void *argument)
{
    uint16_t as5048a_last_call = MedianFilter_Fetch(&mf_filter);
    while (1)
    {
        uint16_t dial = MedianFilter_Fetch(&mf_filter);
        int16_t delta = sensor_delta(dial, as5048a_last_call) >> 6;
        if (abs(delta) > 2)
        {
            int16_t rd = report14.dial + delta;
            if (rd > (1<<10)-1)
            {
                rd = (1<<10)-1;
            }else if (rd < 0)
            {
                rd = 0;
            }
            as5048a_last_call = dial;
            report14.dial = rd;
        }
        USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t*)&report14, sizeof(report14));
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}


