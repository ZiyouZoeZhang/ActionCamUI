#include "my_demo_2.h"

/**VARIABLES FOR INPUT**/
int battery_level = 30;
bool battery_charging = false;
int sd_status = 1; //0 = none, 1 = available, -1 = error
float sd_storage = 30.9;
int cur_cam_mode = 8;
int cur_cam_resolution = 0;
int cur_cam_zoom = 0;
bool wifi_active = false;

//global
lv_obj_t *scr_home = NULL;
lv_style_t style_font_default_36;
lv_style_t style_swipe_icon;

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
    create_scr_menu();
}


void swipe_scr_main_cb(lv_event_t * e){
    lv_event_code_t code = lv_event_get_code(e);
   // if (code == LV_EVENT_GESTURE) {
        switch(lv_indev_get_gesture_dir(lv_indev_active())) {
            case LV_DIR_LEFT:
                open_scr_media_settings_cb();
                //printf("SWIPE -> enter screen PRO selection");
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
   //}
}


void swipe_scr_menu_cb(lv_event_t *e){
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
    switch(dir) {
        case LV_DIR_TOP:
            open_scr_home_cb();
            break;
    }
}

void swipe_scr_mode_selection_cb(lv_event_t *e){
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
    switch(dir) {
        case LV_DIR_BOTTOM:
            open_scr_home_cb();
            break;
    }
}


/**Access of Screens**/

void open_scr_poweroff_cb(){
    create_scr_poweroff();
    lv_screen_load(scr_poweroff);
}

/**SCREENS**/
void create_scr_home(){
    /**background**/
    scr_home = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_home,BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);

    lv_obj_remove_flag(scr_home, LV_OBJ_FLAG_SCROLLABLE);

    /**create icons**/
    create_battery_icon(scr_home, battery_charging, battery_level);
    create_sd_icon(scr_home, sd_status, sd_storage);
    create_pic_lib_icon(scr_home);
    create_cam_mode_icon(scr_home, cur_cam_mode);
    create_resolution_icon(scr_home, cur_cam_resolution);
    create_zoom_icon(scr_home, cur_cam_zoom);
    create_grid(scr_home);
    create_media_set_icon(scr_home);
    create_spot_metering(scr_home);

    /**CB**/
    lv_obj_add_event_cb(scr_home, swipe_scr_main_cb, LV_EVENT_RELEASED, NULL);
    lv_obj_add_event_cb(scr_home, update_spot_metering_cb, LV_EVENT_CLICKED, NULL);

    /**load screen**/
    lv_scr_load(scr_home);
}

void create_scr_poweroff(){
    scr_poweroff = lv_obj_create(NULL);
    img_xtu_poweroff = lv_image_create(scr_poweroff);
    lv_image_set_src(img_xtu_poweroff, &xtu_poweroff);

    lv_obj_remove_flag(scr_poweroff, LV_OBJ_FLAG_SCROLLABLE);
}

void create_scr_mode_selection(int mode){
     /**background**/
    scr_mode_selection = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_mode_selection,BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);

    /**ui components**/
    create_cam_mode_roller(scr_mode_selection, mode);
    create_exit_icon(scr_mode_selection);

    lv_obj_t * swipe_icon = lv_obj_create(scr_mode_selection);
    lv_obj_add_style(swipe_icon, &style_swipe_icon, LV_PART_MAIN);
    lv_obj_remove_flag(swipe_icon, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(swipe_icon, LV_ALIGN_TOP_MID, 0, 40);

    /**CB**/
    lv_obj_add_event_cb(scr_mode_selection, swipe_scr_mode_selection_cb, LV_EVENT_RELEASED, NULL);

    /**load screen**/
    lv_screen_load(scr_mode_selection);
}

void create_scr_pic_library(){
    /**background**/
    scr_pic_library = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_pic_library,BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);
    create_exit_icon(scr_pic_library);

     /**load screen**/
    lv_screen_load(scr_pic_library);
}

void create_scr_menu(){

    /**background**/
    scr_menu = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_menu, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);

    create_menu_btnm(scr_menu);
    create_battery_icon(scr_menu, battery_charging, battery_level);
    create_exit_icon(scr_menu);

    /**CB**/
    lv_obj_add_event_cb(scr_menu, swipe_scr_menu_cb, LV_EVENT_RELEASED, NULL);

    /**load screen**/
    lv_screen_load(scr_menu);
}

/**initialize**/

void initializ_styles(){
    lv_style_init(&style_font_default_36);
    lv_style_set_text_font(&style_font_default_36, &font_36);
    lv_style_set_text_color(&style_font_default_36, lv_color_white());
    lv_style_set_align(&style_font_default_36, LV_ALIGN_CENTER);

    lv_style_init(&style_swipe_icon);
    lv_style_set_size(&style_swipe_icon, 100, 10);
    lv_style_set_align(&style_swipe_icon, LV_ALIGN_BOTTOM_MID);
    lv_style_set_bg_color(&style_swipe_icon, BG_COLOR_DARK_GREY);
    lv_style_set_border_width(&style_swipe_icon, 0);
}
void my_demo_2_create() {
    initializ_styles();
   create_scr_home();

    /**driver code**/
    lv_screen_load(scr_home);

    ///TEST
    //open_scr_menu_cb();

}


