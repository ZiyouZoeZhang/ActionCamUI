#include <string.h>
#include <stdio.h>
#include "ui_components.h"
#include "camera_modes.h"
#include "pic_converted_transparent/image_declares.h"
#include "my_demo_2.h"

static lv_obj_t * img_battery = NULL;
static lv_obj_t * img_sd = NULL;
static lv_obj_t * label_sd = NULL;
static lv_obj_t * img_pic_lib = NULL;
static lv_obj_t * img_cam_mode = NULL;
static lv_obj_t * img_exit = NULL;

/**creations**/

void create_sd_icon(lv_obj_t *parent, int status, float storage){
    img_sd = lv_image_create(parent);
    lv_obj_align(img_sd, LV_ALIGN_TOP_LEFT, 10, 10);

    label_sd = lv_label_create(parent);
    lv_obj_align(label_sd, LV_ALIGN_TOP_LEFT, 60, 30);
    lv_obj_set_style_text_color(label_sd, lv_color_white(), LV_PART_MAIN);

    update_sd_icon(status, storage);
}

void create_battery_icon(lv_obj_t *parent, bool charging, int level){
    img_battery = lv_image_create(parent);
    lv_obj_align(img_battery, LV_ALIGN_TOP_RIGHT, -20, 10);
    update_battery_icon(charging, level);
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
    lv_obj_align(img_cam_mode, LV_ALIGN_BOTTOM_LEFT, 0, 0);

    lv_obj_set_user_data(img_cam_mode, (void*)(intptr_t)mode);
    lv_obj_add_flag(img_cam_mode, LV_OBJ_FLAG_CLICKABLE);
    update_cam_mode_icon(mode);

    /*CB*/
    lv_obj_add_event_cb(img_cam_mode, update_cam_mode_cb, LV_EVENT_CLICKED, NULL);
}

void create_exit_icon(lv_obj_t * parent){
    img_exit = lv_image_create(parent);
    lv_obj_align(img_exit, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_image_set_src(img_exit, &common_return);

    /*CB*/
    lv_obj_add_flag(img_exit, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(img_exit, return_home_cb, LV_EVENT_CLICKED, NULL);
}


/**updates**/

void update_sd_icon(int status, float storage) {
    switch (status){
        case -1:
            lv_image_set_src(img_sd, &SD_error);
            lv_label_set_text(label_sd, "");
            printf("correct \n");
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

/**CB**/

void update_cam_mode_cb(lv_event_t * e){
    lv_obj_t *img = lv_event_get_target(e);
    camera_mode_t mode = (camera_mode_t)(intptr_t)lv_obj_get_user_data(img);
    create_scr_mode_selection(mode);
}

void return_home_cb(lv_event_t * e){
    cur_cam_mode = get_selected_mode_from_roller();
    create_scr_home();
    lv_screen_load(scr_home);
}

void open_scr_pic_lib_cb(lv_event_t * e){
    create_scr_pic_library();
}
