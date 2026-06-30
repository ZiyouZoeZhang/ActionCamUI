#include <string.h>
#include <stdio.h>
#include "lvgl.h"
#include "ui_roller.h"
#include "camera_modes.h"
#include "my_demo_2.h"
#include "ui_menu_btnm.h"
#include "pic_converted_transparent/image_declares.h"

static  lv_obj_t * btnm = NULL;

static const menu_btn_info_t menu_buttons[] = {
    // 用户可以开关
    {CAM_MENU_WIFI,          "WiFi" ,  false},
    {CAM_MENU_AUTO_ROTATION, "Auto Rotations",   false},
    {CAM_MENU_LOCK,          "lock",    false},
    {CAM_MENU_SETTINGS,      "settings",     false},

    {CAM_MENU_VOICE_REC,     "voice rec",  false},
    {CAM_MENU_BLUETOOTH,     "B",     false},
    {CAM_MENU_TURN_OFF,      "turn off", false},
    {CAM_MENU_GRID_VIEW,     "GW",  false},
};


void menu_grid_click_cb(lv_event_t *e) {
    lv_obj_t *btn = lv_event_get_target(e);
    int id = (int)(intptr_t)lv_obj_get_user_data(btn);
    printf("clicked: %s\n", menu_buttons[id].name);
}

void create_menu_grid(lv_obj_t *parent) {
    // 创建容器
    lv_obj_t *cont = lv_obj_create(parent);
    lv_obj_set_size(cont, 780, 300);
    //lv_obj_center(cont);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, 0);
    //lv_obj_set_style_bg_color(cont, BG_COLOR_DARK_GREY, LV_PART_ANY);
    lv_obj_set_style_bg_color(cont, BG_COLOR_DARK_GREY, LV_PART_MAIN);

    // ✅ 设置网格：2行 × 4列
    static const int width = 100;
    static const int pad = 80;
    static lv_coord_t col_dsc[] = {width, width, width, width, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {width, width, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);
    lv_obj_set_style_pad_all(cont, 40, LV_PART_MAIN);
    lv_obj_set_style_pad_row(cont, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_column(cont, 40, LV_PART_MAIN);

    lv_obj_remove_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    for (int i = 0; i < 8; i++) {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, i % 4, 1, LV_GRID_ALIGN_STRETCH, i / 4, 1);
        lv_obj_set_style_radius(btn, 8, LV_PART_MAIN);

       /*

        // ✅ 图标
        lv_obj_t *img = lv_image_create(btn);
        lv_image_set_src(img, menu_items[i].icon);
        lv_obj_align(img, LV_ALIGN_TOP_MID, 0, 10);
        lv_obj_set_size(img, 30, 30);*/

        lv_obj_t *label = lv_label_create(btn);
        lv_label_set_text(label, menu_buttons[i].name);
        lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -10);
        lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_12, LV_PART_MAIN);

        lv_obj_set_user_data(btn, (void*)(intptr_t)i);
        lv_obj_add_event_cb(btn, menu_grid_click_cb, LV_EVENT_CLICKED, NULL);
    }
}

// TODO (gku#1#): create buttons function for menu scr ...
void create_menu_btnm(lv_obj_t * parent){
     create_menu_grid(parent);
}
