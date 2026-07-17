#ifndef SCR_MEDIA_SETTINGS_H_INCLUDED
#define SCR_MEDIA_SETTINGS_H_INCLUDED

#include "lvgl.h"



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


typedef struct {
    int id;
    int name_id;
    bool active;
    bool pro;
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
