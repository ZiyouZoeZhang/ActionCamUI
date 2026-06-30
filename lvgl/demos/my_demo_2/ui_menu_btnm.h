#ifndef UI_MENU_BTNM_H_INCLUDED
#define UI_MENU_BTNM_H_INCLUDED

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
    //const void *icon_src;
    btn_state_t state;
    //bool can_toggle;
} menu_btn_info_t;

const menu_btn_info_t * get_all_btns();

extern bool menu_wifi_on;
extern bool menu_auto_rotation_on;
extern bool menu_voice_rec_on;
extern bool menu_bluetooth_on;
extern bool menu_grid_view_on;

void create_menu_btnm(lv_obj_t *parent);

//getters
const char* get_menu_btn_name(menu_btn_t btn);
//const void* get_menu_btn_icon(menu_btn_t btn);
bool get_menu_btn_can_toggle(menu_btn_t btn);

//action
//void menu_btn_toggle(menu_btn_t btn);
//void menu_btn_execute(menu_btn_t btn);

#endif // UI_MENU_BTNM_H_INCLUDED
