#include "my_demo_2.h"

static lv_obj_t * img_cam_mode = NULL;
static lv_obj_t * label_zoom = NULL;
static lv_obj_t * img_res = NULL;
static lv_obj_t * label_res = NULL;
static lv_obj_t * grid_container = NULL;

void update_scr_home(){
    //update sd
    //update battery

    reset_spot_metering();

    if (menu_buttons[CAM_MENU_GRID_VIEW].state == BTN_STATE_ON){
        lv_obj_remove_flag(grid_container, LV_OBJ_FLAG_HIDDEN);
    } else {
         lv_obj_add_flag(grid_container, LV_OBJ_FLAG_HIDDEN);
    }
    cur_cam_mode = get_selected_mode_from_roller();
    lv_image_set_src(img_cam_mode, get_mode_icon(cur_cam_mode));

    cur_cam_zoom = get_selected_zoom_from_roller();
    lv_label_set_text(label_zoom, cam_zoom_table[cur_cam_zoom]);

    cur_cam_resolution = get_selected_resolution_from_roller();
    set_resolution_icon_bg(img_res, cur_cam_resolution);
    lv_label_set_text(label_res, cam_resolution_table[cur_cam_resolution]);
}

static void swipe_scr_main_cb(lv_event_t * e){
        switch(lv_indev_get_gesture_dir(lv_indev_active())) {
            case LV_DIR_LEFT:
                open_scr_media_settings_cb(false);
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
            default:
                break;
        }
}

void create_scr_home(){
    /**background**/
    scr_home = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_home,BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);
    lv_obj_remove_flag(scr_home, LV_OBJ_FLAG_SCROLLABLE);

    /**create icons**/
    grid_container = create_grid(scr_home);
    create_battery_icon(scr_home, battery_charging, battery_level);
    create_sd_icon(scr_home, sd_status, sd_storage);
    create_pic_lib_icon(scr_home);
    img_cam_mode = create_cam_mode_icon(scr_home, cur_cam_mode);
    img_res = create_resolution_icon(scr_home, cur_cam_resolution);
    label_res =lv_obj_get_child(img_res, 0);
    label_zoom = create_zoom_icon(scr_home, cur_cam_zoom);
    create_media_set_icon(scr_home);
    create_spot_metering(scr_home);

    /**CB**/
    lv_obj_add_event_cb(scr_home, swipe_scr_main_cb, LV_EVENT_RELEASED, NULL);
    lv_obj_add_event_cb(grid_container, swipe_scr_main_cb, LV_EVENT_RELEASED, NULL);
    lv_obj_add_event_cb(grid_container, update_spot_metering_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(scr_home, update_spot_metering_cb, LV_EVENT_CLICKED, NULL);

    return;
}

void open_scr_home_cb(){
    update_scr_home();
    lv_screen_load(scr_home);
}
