#ifndef MY_DEMO_2_H
#define MY_DEMO_2_H

#define BG_COLOR_DARK_GREY  lv_color_hex(0x333333)

#include "lvgl.h"
#include "camera_modes.h"
#include "ui_components.h"
#include "ui_roller.h"
#include "ui_menu_btnm.h"
#include "scr_resolution.h"
#include "scr_zoom.h"

/**variables for INPUT**/
extern int battery_level;
extern bool battery_charging;
extern int sd_status; //0 = none, 1 = available, -1 = error
extern float sd_storage;
extern int cur_cam_mode;
extern int cur_cam_resolution;
extern int cur_cam_zoom;
extern bool wifi_active;
extern lv_obj_t *scr_home;

/**main**/
void my_demo_2_create();

/**create scr**/
void create_scr_home();
void create_scr_poweroff();
void create_scr_mode_selection(int mode);
void create_scr_pic_library();
void create_scr_menu();

/**open scr**/
void open_scr_cam_modes_by_mode(int mode);
void open_scr_pic_lib_cb();
void open_scr_menu_cb();
void open_scr_poweroff_cb();

/**CB**/
void swipe_scr_main_cb(lv_event_t *e);
void swipe_scr_menu_cb(lv_event_t *e);
void swipe_scr_mode_selection_cb(lv_event_t *e);

///some how need to be global as well
void create_cam_mode_roller(lv_obj_t *parent, int cur_mode);
int get_selected_mode_from_roller(void);

#endif /* MY_DEMO_2_H */
