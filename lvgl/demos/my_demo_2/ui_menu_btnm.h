#ifndef UI_MENU_BTNM_H_INCLUDED
#define UI_MENU_BTNM_H_INCLUDED

#include "lvgl.h"

void create_menu_grid() ;

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

extern menu_btn_info_t menu_buttons[];


/**CREATE**/
void create_menu_btnm(lv_obj_t *parent);

#endif // UI_MENU_BTNM_H_INCLUDED
