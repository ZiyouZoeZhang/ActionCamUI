#ifndef MY_DEMO_2_H
#define MY_DEMO_2_H

#define BG_COLOR_DARK_BLUE_GREY  lv_color_hex(0x1F3B4D)
#define BG_COLOR_DARK_GREY  lv_color_hex(0x333333)

#include "lvgl.h"
#include "camera_modes.h"
#include "ui_components.h"
#include "camera_menu.h"
#include "scr_resolution.h"
#include "scr_zoom.h"
#include "scr_media_settings.h"
#include "font/font_declares.h"
#include "image_storage.h"

/**variables for INPUT**/
extern int battery_level;
extern bool battery_charging;
extern int sd_status; //0 = none, 1 = available, -1 = error
extern float sd_storage;
extern int cur_cam_mode;
extern int cur_cam_resolution;
extern int cur_cam_zoom;
extern bool wifi_active;
extern bool bluetooth_active;
extern bool grid_active;

extern lv_style_t style_font_default_36;
extern lv_style_t style_font_default_30;
extern lv_style_t style_font_default_24;
extern lv_style_t style_swipe_icon;
extern lv_style_t style_scrollbar;

/** screen**/
extern lv_obj_t *scr_home;
extern lv_obj_t * scr_poweroff;
/*
extern lv_obj_t *scr_menu;
extern lv_obj_t *scr_pic_library;
extern lv_obj_t *scr_media_settings;
extern lv_obj_t *scr_mode_selection;*/


void open_scr_home_cb();
void open_scr_poweroff_cb();
/*
void open_scr_menu_cb(void);
void open_scr_pic_lib_cb(void);
void open_scr_media_settings_cb(void);
void open_scr_mode_selection_cb(void);*/

void create_scr_poweroff();
void create_scr_home();
/*
lv_obj_t* create_scr_menu(void);
lv_obj_t* create_scr_pic_library(void);
lv_obj_t* create_scr_media_settings(void);
lv_obj_t* create_scr_mode_selection(void);*/

/**main**/
void my_demo_2_create();

/**open scr**/
void open_scr_cam_modes_by_mode(int mode);
void open_scr_pic_lib_cb();
void open_scr_menu_cb();
void open_scr_poweroff_cb();

/**others -> to be changed!!**/
int get_selected_mode_from_roller(void);

/**CB**/
void swipe_scr_menu_cb(lv_event_t *e);

#endif /* MY_DEMO_2_H */
