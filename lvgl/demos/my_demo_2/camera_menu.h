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

typedef struct {
    const char **states;   // Array of string options
    int states_count;             // Number of options
    lv_obj_t * roller;           // Current selected index
    lv_obj_t * state_label;
} settings_roller_t;

void open_scr_menu_cb();
void create_scr_menu();

extern menu_btn_info_t menu_buttons[];
void settings_action();
void wifi_action(void);

///actions
void create_scr_menu_bluetooth();
void create_scr_menu_settings();

#endif
