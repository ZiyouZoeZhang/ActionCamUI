#ifndef MY_DEMO_2_H
#define MY_DEMO_2_H

#define BG_COLOR_DARK_GREY  lv_color_hex(0x333333)

#include <string.h>
#include <stdio.h>

#include "lvgl.h"
#include "pic_converted_transparent/image_declares.h"

#include "camera_modes.h"
#include "ui_components.h"
#include "ui_roller.h"

/**variables for INPUT**/
extern int battery_level;
extern bool battery_charging;
extern int sd_status; //0 = none, 1 = available, -1 = error
extern float sd_storage;
extern int cur_cam_mode;

///somehow need to be global
extern lv_obj_t *scr_home;

/**functions**/
void my_demo_2_create();

/**scr**/
void create_scr_home();
void create_scr_poweroff();
void create_scr_mode_selection(int mode);
void create_scr_pic_library();

///some how need to be global as well
void create_cam_mode_roller(lv_obj_t *parent, int cur_mode);
int get_selected_mode_from_roller(void);

#endif /* MY_DEMO_2_H */
