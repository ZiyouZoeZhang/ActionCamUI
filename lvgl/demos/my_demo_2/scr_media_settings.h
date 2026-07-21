#ifndef SCR_MEDIA_SETTINGS_H_INCLUDED
#define SCR_MEDIA_SETTINGS_H_INCLUDED

#include "lvgl.h"


typedef enum {
    CAM_MEDIA_NUMBER,
    CAM_MEDIA_SEGMENT,
    CAM_MEDIA_LOOP_TIME,
    CAM_MEDIA_DURATION,
    CAM_MEDIA_LAPSE,
    CAM_MEDIA_SELF_TIMER,
    CAM_MEDIA_AUDIO,
    CAM_MEDIA_LDC,
    CAM_MEDIA_VIDEO_ENCODE,
    CAM_MEDIA_WATER_AREA,
    CAM_MEDIA_PRE_RECORDING,
    CAM_MEDIA_METER_MODE,
    CAM_MEDIA_EXPOSURE,
    CAM_MEDIA_SHUTTER,
    CAM_MEDIA_ISO,
    CAM_MEDIA_AWB,
    CAM_MEDIA_SCENE_MODE,
    CAM_MEDIA_SHARPNESS,
    CAM_MEDIA_IMAGE_QUALITY,
    CAM_MEDIA_FILTER,
    CAM_MEDIA_BRIGHTNESS,
    CAM_MEDIA_CONTRAST,
    CAM_MEDIA_SATURATION,
    CAM_MEDIA_EIS_ANTI_BLUR,
    CAM_MEDIA_COUNT
} media_set_t;

/*
typedef enum {
    CAM_MEDIA_NUMBER,

    CAM_MEDIA_SELF_TIMER,
    CAM_MEDIA_LDC,
    CAM_MEDIA_METER_MODE,
    CAM_MEDIA_EXPOSURE,
    CAM_MEDIA_SHUTTER,
    CAM_MEDIA_ISO,
    CAM_MEDIA_AWB,           // White Balance
    CAM_MEDIA_SCENE_MODE,        // Focus Mode
    CAM_MEDIA_SHARPNESS,          // HDR
    CAM_MEDIA_FILTER,        // Flash
    CAM_MEDIA_COUNT
} media_set_t;

*/


typedef struct {
    int id;
    int name_id;
    int cur_state;
    const int *states;
    int state_count;
} media_set_btn_info_t;

extern lv_obj_t *scr_media_settings;
extern media_set_btn_info_t media_buttons[];
void open_scr_media_settings_cb();
void create_scr_media_settings();
void create_scr_media_selection();

#endif // SCR_MEDIA_SETTINGS_H_INCLUDED
