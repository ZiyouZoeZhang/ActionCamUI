#include <string.h>
#include <stdio.h>
#include "lvgl.h"
#include "ui_roller.h"
#include "camera_modes.h"
#include "my_demo_2.h"
#include "ui_menu_btnm.h"
#include "pic_converted_transparent/image_declares.h"

static  lv_obj_t * btnm = NULL;
lv_obj_t *cont = NULL;

static menu_btn_info_t menu_buttons[] = {
    {CAM_MENU_WIFI,          "WiFi" , &quick_wifi_enable, &quick_wifi_disable,  BTN_STATE_OFF},
    {CAM_MENU_AUTO_ROTATION, "Auto Rotations", &quick_autorotate_enable, &quick_autorotate_disable,   BTN_STATE_OFF},
    {CAM_MENU_LOCK,          "lock", &quick_lock_enable, &quick_lock_disable,  BTN_STATE_OFF},
    {CAM_MENU_SETTINGS,      "settings",   &quick_system_set, &quick_system_set, BTN_STATE_OFF},

    {CAM_MENU_VOICE_REC,     "voice rec", &quick_voice_enable, &quick_voice_disable, BTN_STATE_OFF},
    {CAM_MENU_BLUETOOTH,     "B",  &quick_bt_enable, &quick_bt_disable,   BTN_STATE_OFF},
    {CAM_MENU_TURN_OFF,      "turn off", &quick_poweroff, &quick_poweroff, BTN_STATE_OFF},
    {CAM_MENU_GRID_VIEW,     "GW",  &quick_grid_enable, &quick_grid_disable, BTN_STATE_OFF},
};

void menu_btn_toggle_state_cb(lv_event_t *e){
    lv_obj_t *btn_obj = lv_event_get_target(e);
    int btn = (int)(intptr_t)lv_obj_get_user_data(btn_obj);

    if (menu_buttons[btn].state == BTN_STATE_ON) {
        menu_buttons[btn].state = BTN_STATE_OFF;
    } else {
        menu_buttons[btn].state = BTN_STATE_ON;
    }
    create_menu_grid();
}

void create_menu_grid() {
    for (int i = 0; i < 8; i++) {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, i % 4, 1, LV_GRID_ALIGN_STRETCH, i / 4, 1);
        lv_obj_set_style_radius(btn, 8, LV_PART_MAIN);

        lv_obj_t *img = lv_image_create(btn);
        if (menu_buttons[i].state == BTN_STATE_ON){
            lv_image_set_src(img, menu_buttons[i].icon_src_on);
        } else {
            lv_image_set_src(img, menu_buttons[i].icon_src_off);
        }

        lv_obj_center(img);
        lv_obj_set_style_bg_color(img, BG_COLOR_DARK_GREY, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(img, LV_OPA_COVER, LV_PART_MAIN);

        lv_obj_set_user_data(btn, (void*)(intptr_t)i);
        lv_obj_add_event_cb(btn, menu_btn_toggle_state_cb, LV_EVENT_CLICKED, NULL);
    }
}

void create_menu_btnm(lv_obj_t * parent){
    cont = lv_obj_create(parent);
    lv_obj_set_size(cont, 780, 350);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(cont, BG_COLOR_DARK_GREY, LV_PART_MAIN);

    static const int width = 147;
    static const int pad = 80;
    static lv_coord_t col_dsc[] = {width, width, width, width, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {width, width, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);
    lv_obj_set_style_pad_all(cont, 30, LV_PART_MAIN);
    lv_obj_set_style_pad_row(cont, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_column(cont, 40, LV_PART_MAIN);

    lv_obj_remove_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
     create_menu_grid();
}
