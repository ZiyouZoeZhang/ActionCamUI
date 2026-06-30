#include "my_demo_2.h"

/**VARIABLES FOR INPUT**/
int battery_level = 30;
bool battery_charging = false;
int sd_status = 1; //0 = none, 1 = available, -1 = error
float sd_storage = 30.9;
int cur_cam_mode = 8;

//global
lv_obj_t *scr_home = NULL;

/**static SCREENS**/
static lv_obj_t * scr_poweroff = NULL;
static lv_obj_t * scr_mode_selection = NULL;
static lv_obj_t * img_xtu_poweroff = NULL;
static lv_obj_t * scr_pic_library = NULL;
static lv_obj_t * scr_menu = NULL;

/**TEST**/
 void test_error_check(); //UNUSED

/**TEST**/
void  test_error_check(){
    printf("\nERROR\n");
}

/**CB functions**/
void open_scr_menu_cb(){
    printf("CB called\n");
    create_scr_menu();
}

void swipe_scr_main_cb(lv_event_t * e){
     lv_obj_t * screen = lv_event_get_current_target(e);
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
    switch(dir) {
        case LV_DIR_LEFT:
            printf("SWIPE -> enter screen PRO selection");
            break;
        case LV_DIR_RIGHT:
            open_scr_pic_lib_cb(); //MAYBE ERROR
            break;
        case LV_DIR_TOP:
            open_scr_cam_modes_by_mode(cur_cam_mode);
            break;
        case LV_DIR_BOTTOM:
            open_scr_menu_cb();
            break;
    }
}

void swipe_scr_menu_cb(lv_event_t *e){
    lv_obj_t * screen = lv_event_get_current_target(e);
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
    switch(dir) {
        case LV_DIR_TOP:
            open_scr_home_cb();
            break;
    }
}

void swipe_scr_mode_selection_cb(lv_event_t *e){
    lv_obj_t * screen = lv_event_get_current_target(e);
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
    switch(dir) {
        case LV_DIR_BOTTOM:
            open_scr_home_cb();
            break;
    }
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

    /**CB**/
    lv_obj_add_event_cb(scr_home, swipe_scr_main_cb, LV_EVENT_GESTURE, NULL);

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

    /**CB**/
    lv_obj_add_event_cb(scr_mode_selection, swipe_scr_mode_selection_cb, LV_EVENT_GESTURE, NULL);

    /**load screen**/
    lv_screen_load(scr_mode_selection);
}

void create_scr_pic_library(){
    /**background**/
    scr_pic_library = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_pic_library,BG_COLOR_DARK_GREY, LV_PART_MAIN);
    create_exit_icon(scr_pic_library);

     /**load screen**/
    lv_screen_load(scr_pic_library);
}

void create_scr_menu(){
    /**background**/
    scr_menu = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_menu, BG_COLOR_DARK_GREY, LV_PART_MAIN);

    create_menu_btnm(scr_menu);
    create_battery_icon(scr_menu, battery_charging, battery_level);
    create_exit_icon(scr_menu);

    /**CB**/
    lv_obj_add_event_cb(scr_menu, swipe_scr_menu_cb, LV_EVENT_GESTURE, NULL);

    /**load screen**/
    lv_screen_load(scr_menu);
}

void my_demo_2_create() {
    create_scr_home();

    /**driver code**/
    lv_screen_load(scr_home);
}


