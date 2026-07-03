#include <string.h>
#include <stdio.h>
#include "ui_components.h"
#include "camera_modes.h"
#include "pic_converted_transparent/image_declares.h"
#include "scr_media_settings.h"
#include "my_demo_2.h"

static lv_obj_t * img_battery = NULL;
static lv_obj_t * img_sd = NULL;
static lv_obj_t * label_sd = NULL;
static lv_obj_t * img_pic_lib = NULL;
static lv_obj_t * img_cam_mode = NULL;
static lv_obj_t * img_exit = NULL;
static lv_obj_t * img_res = NULL;
static lv_obj_t * img_zoom = NULL;
static lv_obj_t * img_spot_met = NULL;
static lv_obj_t * img_media_set = NULL;


/**creations**/

void create_media_set_icon(lv_obj_t * parent){
    img_media_set = lv_image_create(parent);
    lv_image_set_src(img_media_set, &main_Media_set);
    lv_obj_align(img_media_set, LV_ALIGN_RIGHT_MID, 10, 0);
    lv_obj_add_flag(img_media_set, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_add_event_cb(img_media_set, open_scr_media_settings_cb, LV_EVENT_CLICKED, NULL);
}

void create_resolution_icon(lv_obj_t * parent, int cur_res){
    img_res = lv_image_create(parent);
    lv_obj_align(img_res, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_add_flag(img_res, LV_OBJ_FLAG_CLICKABLE);

    if (strlen(cam_resolution_table[cur_res])<=7){
        lv_image_set_src(img_res, &main_Res_short);
    } else if  (strlen(cam_resolution_table[cur_res])<=9){
         lv_image_set_src(img_res, &main_Res_mid);
    } else {
         lv_image_set_src(img_res, &main_Res_bg);
    }

    lv_obj_t * label = lv_label_create(img_res);
    lv_label_set_text(label, cam_resolution_table[cur_res]);
    lv_obj_add_style(label, &style_font_default_36, LV_PART_MAIN);

    lv_obj_add_event_cb(img_res, open_scr_resolution_cb, LV_EVENT_CLICKED, NULL);
}

void create_zoom_icon(lv_obj_t * parent, int cur_zoom){
    img_zoom = lv_image_create(parent);
    lv_obj_align(img_zoom, LV_ALIGN_BOTTOM_RIGHT, -15, -20);
    lv_obj_add_flag(img_zoom, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(img_zoom, &main_Zoom_bg);

    lv_obj_t * label = lv_label_create(img_zoom);
    lv_label_set_text(label, cam_zoom_table[cur_zoom]);
    lv_obj_add_style(label, &style_font_default_36, LV_PART_MAIN);

    lv_obj_add_event_cb(img_zoom, open_scr_zoom_cb, LV_EVENT_CLICKED, NULL);
}

void create_sd_icon(lv_obj_t *parent, int status, float storage){
    img_sd = lv_image_create(parent);
    lv_obj_align(img_sd, LV_ALIGN_TOP_LEFT, 10, 10);

    label_sd = lv_label_create(parent);
    lv_obj_add_style(label_sd, &style_font_default_36, LV_PART_MAIN);
    lv_obj_align(label_sd, LV_ALIGN_TOP_LEFT, 65, 15);

    update_sd_icon(status, storage);
}

void create_battery_icon(lv_obj_t *parent, bool charging, int level){
    img_battery = lv_image_create(parent);
    lv_obj_align(img_battery, LV_ALIGN_TOP_RIGHT, -20, 10);
    update_battery_icon(charging, level);
}


void create_grid(lv_obj_t * parent){
    if (menu_buttons[CAM_MENU_GRID_VIEW].state == BTN_STATE_OFF){
        return;
    }

    static lv_point_precise_t line_points[][2] = {
        {{260, 0}, {260, 480}},
        {{520, 0}, {520, 480}},
        {{0, 145}, {800, 145}},
        {{0, 295}, {800, 295}}
    };

    for (int i=0; i<4; i++){
        lv_obj_t * line;
        line = lv_line_create(parent);
        lv_line_set_points(line, line_points[i], 2);
        lv_obj_set_style_line_color(line, lv_color_white(), LV_PART_MAIN);
    }
}

void create_pic_lib_icon(lv_obj_t *parent){
    img_pic_lib = lv_image_create(parent);
    lv_obj_align(img_pic_lib, LV_ALIGN_LEFT_MID, -20, 0);
    lv_image_set_src(img_pic_lib, &main_Picture_library);

    /*CB*/
    lv_obj_add_flag(img_pic_lib, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(img_pic_lib, open_scr_pic_lib_cb, LV_EVENT_CLICKED, NULL);

}

void create_cam_mode_icon(lv_obj_t * parent, int mode){
    img_cam_mode = lv_image_create(parent);
    lv_obj_align(img_cam_mode, LV_ALIGN_BOTTOM_LEFT, 10, -20);
    lv_obj_set_user_data(img_cam_mode, (void*)(intptr_t)mode);

    update_cam_mode_icon(mode);

    /*CB*/
    lv_obj_add_flag(img_cam_mode, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(img_cam_mode, open_scr_cam_modes_cb, LV_EVENT_CLICKED, NULL);
}

void create_exit_icon(lv_obj_t * parent){
    img_exit = lv_image_create(parent);
    lv_obj_align(img_exit, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_image_set_src(img_exit, &common_return);

    /*CB*/
    lv_obj_add_flag(img_exit, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(img_exit, open_scr_home_cb, LV_EVENT_CLICKED, NULL);
}

void create_spot_metering(lv_obj_t * parent){
    img_spot_met = lv_image_create(parent);
    lv_obj_center(img_spot_met);
    lv_image_set_src(img_spot_met, &spot_metering);
}


/**updates**/
void update_sd_icon(int status, float storage) {
    switch (status){
        case -1:
            lv_image_set_src(img_sd, &SD_error);
            lv_label_set_text(label_sd, "");
            break;
        case 0:
             lv_image_set_src(img_sd, &SD_none);
             lv_label_set_text(label_sd, "");
             break;
        case 1:
             lv_image_set_src(img_sd, &SD_available);
             char buffer[64];
             snprintf(buffer, sizeof(buffer), "%.1fG", storage);
             lv_label_set_text(label_sd, buffer);
             break;
    }
}

void update_battery_icon(bool charging, int level) {
    if (charging == true){
        lv_image_set_src(img_battery, &battery_Charging);
        return;
    }
    if (level >= 80) {
        lv_image_set_src(img_battery, &Battery_full);
    } else if (level >= 60) {
        lv_image_set_src(img_battery, &Battery_3);
    } else if (level >= 40) {
        lv_image_set_src(img_battery, &Battery_2);
    } else if (level >= 20) {
        lv_image_set_src(img_battery, &Battery_1);
    } else if (level > 0) {
        lv_image_set_src(img_battery, &Battery_low);
    } else {
        printf("ERROR: battery invalid input\n");
    }
}

void update_cam_mode_icon(int mode){
    lv_image_set_src(img_cam_mode, get_mode_icon(mode));
}

void update_spot_metering_cb(lv_event_t *e) {
    lv_indev_t *indev = lv_indev_get_act();

    lv_point_t point;
    lv_indev_get_point(indev, &point);

    lv_obj_set_align(img_spot_met, LV_ALIGN_TOP_LEFT);
    lv_coord_t img_w = lv_obj_get_width(img_spot_met);
    lv_coord_t img_h = lv_obj_get_height(img_spot_met);

    lv_obj_set_pos(img_spot_met,
                   point.x - img_w / 2,
                   point.y - img_h / 2);
}

/**CB**/

void open_scr_cam_modes_by_mode(int mode){
    create_scr_mode_selection(mode);
}


void open_scr_cam_modes_cb(lv_event_t * e){
    lv_obj_t *img = lv_event_get_target(e);
    camera_mode_t mode = (camera_mode_t)(intptr_t)lv_obj_get_user_data(img);
    open_scr_cam_modes_by_mode(mode);
}


void open_scr_home_cb(){
    cur_cam_mode = get_selected_mode_from_roller();
    cur_cam_resolution = get_selected_resolution_from_roller();
    cur_cam_zoom = get_selected_zoom_from_roller();
    create_scr_home();
    lv_screen_load(scr_home);
}

void open_scr_pic_lib_cb(){
    create_scr_pic_library();
}
