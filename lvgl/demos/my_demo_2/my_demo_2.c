#include "my_demo_2.h"

/**VARIABLES FOR INPUT**/
int battery_level = 30;
bool battery_charging = false;
int sd_status = 1; //0 = none, 1 = available, -1 = error
float sd_storage = 30.9;
int cur_cam_mode = 8;

lv_obj_t *img_exit = NULL;
lv_obj_t *scr_home = NULL;

/**static SCREENS**/
static lv_obj_t * scr_poweroff = NULL;
static lv_obj_t * scr_mode_selection = NULL;
static lv_obj_t * img_xtu_poweroff = NULL;

/**TEST**/
 void cb_test(lv_event_t *e); //UNUSED

/**TEST**/
void cb_test(lv_event_t *e) {
    lv_obj_t *roller = lv_event_get_target(e);
    uint16_t selected_idx = lv_roller_get_selected(roller); ///index of seldcted mode
}


/**SCREENS**/

void create_scr_home(){
    /**background**/
    scr_home = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_home,BG_COLOR_DARK_GREY, LV_PART_MAIN);

    /**create icons**/
    create_battery_icon(scr_home, battery_charging, battery_level);
    create_sd_icon(scr_home, sd_status, sd_storage);
    create_pic_lib_icon(scr_home);
    create_cam_mode_icon(scr_home, cur_cam_mode);

    /**load screen**/
    lv_scr_load(scr_home);
}

void create_scr_poweroff(){
     scr_poweroff = lv_obj_create(NULL);
    img_xtu_poweroff = lv_image_create(scr_poweroff);
    lv_image_set_src(img_xtu_poweroff, &xtu_poweroff);
}

void create_scr_mode_selection(int mode){
     /**background**/
    scr_mode_selection = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_mode_selection,BG_COLOR_DARK_GREY, LV_PART_MAIN);

    /**ui components**/
    create_cam_mode_roller(scr_mode_selection, mode);
    create_exit_icon(scr_mode_selection);

    /**clicked on exit CB**/
    lv_obj_add_event_cb(img_exit, return_home_cb, LV_EVENT_CLICKED, NULL);

    /**load screen**/
    lv_screen_load(scr_mode_selection);
}

void my_demo_2_create() {
    create_scr_home();

    /**driver code**/
    lv_screen_load(scr_home);
}


