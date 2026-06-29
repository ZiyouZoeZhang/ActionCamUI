#include "my_demo_2.h"
#define BG_COLOR_DARK_GREY  lv_color_hex(0x333333)

/**VARIABLES FOR INPUT**/
int battery_level = 30;
bool battery_charging = true;
int sd_status = 1; //0 = none, 1 = available, -1 = error
float sd_storage = 30.9;
int cur_cam_mode = 1;
const camera_mode_info_t * cam_modes;

/**globals**/
static lv_obj_t * scr_home = NULL;
static lv_obj_t * scr_poweroff = NULL;
static lv_obj_t * scr_mode_selection = NULL;
static lv_obj_t * img_xtu_poweroff = NULL;
static lv_obj_t * img_battery = NULL;
static lv_obj_t * img_sd = NULL;
static lv_obj_t * label_sd = NULL;
static lv_obj_t * img_pic_lib = NULL;
static lv_obj_t * img_cam_mode = NULL;
static lv_obj_t * img_exit = NULL;
static lv_obj_t * cam_modes_roller = NULL;

/**functions**/
void my_demo_2_create();

void create_scr_home();
void create_scr_poweroff();
void create_scr_mode_selection(int mode);

void create_sd_icon(lv_obj_t *parent, int status, float storage);
void create_battery_icon(lv_obj_t *parent, bool charging, int level);
void create_pic_lib_icon(lv_obj_t *parent);
void create_cam_mode_icon(lv_obj_t * parent, int mode);
void create_cam_mode_roller(lv_obj_t * parent, int cur_mode);
void create_exit_icon(lv_obj_t * parent);

void update_battery_icon(bool charging, int level);
void update_sd_icon(int status, float storage);
void update_cam_mode_icon(int mode);

 void update_cam_mode_cb(lv_event_t * e);
 void cb_test(lv_event_t *e);
 void return_home_cb(lv_event_t * e);


/**TEST**/

void cb_test(lv_event_t *e) {
    lv_obj_t *roller = lv_event_get_target(e);

    uint16_t selected_idx = lv_roller_get_selected(roller); ///index of seldcted mode

//    print selected text
    /*
    char selected_text[32];
    lv_roller_get_selected_str(roller, selected_text, sizeof(selected_text));
    printf("%s\n", selected_text);
    */
}


void create_cam_mode_roller(lv_obj_t * parent, int cur_mode){

    int count = get_mode_count();
    camera_mode_info_t * cam_modes = get_all_modes();

    /// using tileview
    /*
    lv_obj_t * tv = lv_tileview_create(lv_screen_active());
    lv_obj_set_size(tv, 300, 200);
    lv_obj_set_style_bg_color(tv,BG_COLOR_DARK_GREY, LV_PART_MAIN);
    lv_obj_center(tv);


    lv_obj_t * tile = lv_tileview_add_tile(tv, 0, 0,  LV_DIR_RIGHT);
    lv_obj_t * img = lv_image_create(tile);
    lv_obj_set_size(tile, 100, 200);
    lv_obj_set_align(tile, LV_ALIGN_LEFT_MID);
    lv_image_set_src(img, modes[0].icon_src);
   // lv_obj_center(img);


    lv_obj_t * tile1= lv_tileview_add_tile(tv, 1, 0,  LV_DIR_LEFT | LV_DIR_RIGHT);
    lv_obj_t * img1 = lv_image_create(tile1);
    lv_obj_set_size(tile1, 100, 200);
    lv_image_set_src(img1, modes[1].icon_src);
    //lv_obj_center(img1);
    */

    ///using a roller
    char options[300] = ""; //name\nname\nname\n etc
    for (int i = 0; i < count; i++) {
        strcat(options, cam_modes[i].name);
       //printf(cam_modes[i].name);
        if (i < count-1 ) {
            strcat(options, "\n");
        }
    }

    cam_modes_roller = lv_roller_create(parent);
    lv_obj_set_size(cam_modes_roller, 600, 150);
    lv_obj_center(cam_modes_roller);

    lv_roller_set_options(cam_modes_roller, options, LV_ROLLER_MODE_INFINITE);
    lv_roller_set_visible_row_count(cam_modes_roller, 3);
    //lv_roller_set_selected(cam_modes_roller, 0, LV_ANIM_OFF);
    lv_obj_set_style_bg_color(cam_modes_roller, BG_COLOR_DARK_GREY, LV_PART_MAIN);
    lv_obj_set_style_text_color(cam_modes_roller, lv_color_white(), LV_PART_MAIN);

    //lv_obj_add_event_cb(cam_modes_roller, cb_test, LV_EVENT_VALUE_CHANGED, NULL);
}


/**TEST END**/

/**HOME**/

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

    lv_obj_add_flag(img_exit, LV_OBJ_FLAG_CLICKABLE);

}

void create_pic_lib_icon(lv_obj_t *parent){
    img_pic_lib = lv_image_create(parent);
    lv_obj_align(img_pic_lib, LV_ALIGN_LEFT_MID, -20, 0);
    lv_image_set_src(img_pic_lib, &main_Picture_library);
}

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

void update_cam_mode_cb(lv_event_t * e){
    lv_obj_t *img = lv_event_get_target(e);
    camera_mode_t mode = (camera_mode_t)(intptr_t)lv_obj_get_user_data(img);
    printf(get_mode_name(mode)); ///PRINT

    create_scr_mode_selection(mode);
}

void return_home_cb(lv_event_t * e){
        /*
    uint16_t selected_idx = lv_roller_get_selected(roller); ///index of seldcted mode

//    print selected tex

    char selected_text[32];
    lv_roller_get_selected_str(roller, selected_text, sizeof(selected_text));*/

    printf("AAAAAAA");

    cur_cam_mode = lv_roller_get_selected(cam_modes_roller);
    create_scr_home();
    lv_screen_load(scr_home);


}

void create_scr_home(){
    /**background**/
    scr_home = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_home,BG_COLOR_DARK_GREY, LV_PART_MAIN);

    /**create & update battery icon**/
    create_battery_icon(scr_home, battery_charging, battery_level);  // 传入 scr_home

    /**create & update SD**/
    create_sd_icon(scr_home, sd_status, sd_storage);  // 传入 scr_home

    /**create pic lib**/
    create_pic_lib_icon(scr_home);

    create_cam_mode_icon(scr_home, cur_cam_mode);

    lv_scr_load(scr_home);
}

void create_scr_poweroff(){
     scr_poweroff = lv_obj_create(NULL);
    img_xtu_poweroff = lv_image_create(scr_poweroff);
    lv_image_set_src(img_xtu_poweroff, &xtu_poweroff);
}

/**TO BE FINISHED**/
void create_scr_mode_selection(int mode){
    scr_mode_selection = lv_obj_create(NULL);


    lv_obj_set_style_bg_color(scr_mode_selection,BG_COLOR_DARK_GREY, LV_PART_MAIN);
    create_cam_mode_roller(scr_mode_selection, mode);
    create_exit_icon(scr_mode_selection);

    lv_obj_add_event_cb(img_exit, return_home_cb, LV_EVENT_CLICKED, NULL);

    lv_screen_load(scr_mode_selection);

}

void my_demo_2_create() {
    create_scr_home();
    create_scr_poweroff();

    /**driver code**/
    lv_screen_load(scr_home);
}


