#ifndef CAMERA_MENU_H_INCLUDED
#define CAMERA_MENU_H_INCLUDED

#include "lvgl.h"

typedef enum {
    BTN_STATE_OFF = 0,
    BTN_STATE_ON = 1
} btn_state_t;

typedef enum{
    CAM_MENU_WIFI,
    CAM_MENU_AUTO_ROTATION,
    CAM_MENU_LOCK,
    CAM_MENU_SETTINGS,

    CAM_MENU_VOICE_REC,
    CAM_MENU_BLUETOOTH,
    CAM_MENU_TURN_OFF,
    CAM_MENU_GRID_VIEW,
} menu_btn_t;

typedef struct {
    menu_btn_t btn;
    const char *name;
    const void *icon_src_on;
    const void *icon_src_off;
    btn_state_t state;
    void (*action)(); ///
} menu_btn_info_t;

void open_scr_menu_cb();
void create_scr_menu();

extern menu_btn_info_t menu_buttons[];

#endif
