#ifndef CAMERA_MODES_H_INCLUDED
#define CAMERA_MODES_H_INCLUDED

#include "../../lvgl.h"
#include <string.h>
#include <stdio.h>
#include "../../pic_converted_transparent/image_declares.h"

typedef enum {
    // 拍照模式
    CAM_MODE_RAW,                      // Raw拍照
    CAM_MODE_LONG_EXPOSURE,       // 长曝光拍照
    CAM_MODE_CONTINUOUS,            // 连续拍照
    CAM_MODE_TIMER,                         // 定时拍照
    CAM_MODE_TIMELAPSE,                 // 延时拍照
    CAM_MODE_PHOTO,                       // 普通拍照

    // 录像模式
    CAM_MODE_VIDEO,                          // 普通录像
    CAM_MODE_TIMELAPSE_VIDEO,      // 延时录像
    CAM_MODE_SLOW_MOTION,           // 慢速录像
    CAM_MODE_SHORT_VIDEO,             // 短视频

    // 特殊模式
    CAM_MODE_UNDERWATER,             // 水下模式
    CAM_MODE_CAR_LOOP,                   // 车载循环
    CAM_MODE_VIDEO_PHOTO,            // 录像+拍照
    CAM_MODE_NIGHT,                         // 夜景模式

    CAM_MODE_COUNT                        // 模式总数
} camera_mode_t;


typedef struct {
    camera_mode_t mode;
    const char *name;
    const void *icon_src;
} camera_mode_info_t;

const camera_mode_info_t* get_all_modes(void);
int get_mode_count(void);
const char* get_mode_name(camera_mode_t mode);
const void* get_mode_icon(camera_mode_t mode);

#endif // CAMERA_MODES_H_INCLUDED
