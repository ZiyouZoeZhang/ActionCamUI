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
static lv_obj_t * img_spot_met = NULL;
static lv_obj_t * img_media_set = NULL;
static lv_obj_t * img_pic_select = NULL;
static lv_obj_t * img_search = NULL;

/**creations**/

void create_media_set_icon(lv_obj_t * parent){
    img_media_set = lv_image_create(parent);
    lv_image_set_src(img_media_set, &main_Media_set);
    lv_obj_align(img_media_set, LV_ALIGN_RIGHT_MID, 10, 0);
    lv_obj_add_flag(img_media_set, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_add_event_cb(img_media_set, open_scr_media_settings_cb, LV_EVENT_CLICKED, NULL);
}

void set_resolution_icon_bg(lv_obj_t *img, int res) {
    if (strlen(_(res)) <= 7) {
        lv_image_set_src(img, &main_Res_short);
    } else if (strlen(_(res)) <= 9) {
        lv_image_set_src(img, &main_Res_mid);
    } else {
        lv_image_set_src(img, &main_Res_bg);
    }
}

lv_obj_t * create_resolution_icon(lv_obj_t * parent, int cur_res){
    lv_obj_t * img_res = lv_image_create(parent);
    lv_obj_align(img_res, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_add_flag(img_res, LV_OBJ_FLAG_CLICKABLE);

    set_resolution_icon_bg(img_res, cur_res);

    lv_obj_t * label = lv_label_create(img_res);
    lv_label_set_text(label, _(cur_res));
    lv_obj_add_style(label, &style_font_default_36, LV_PART_MAIN);

    lv_obj_add_event_cb(img_res, open_scr_resolution_cb, LV_EVENT_CLICKED, NULL);
    return img_res;
}

lv_obj_t * create_zoom_icon(lv_obj_t * parent, int cur_zoom){
    lv_obj_t * img_zoom = lv_image_create(parent);
    lv_obj_align(img_zoom, LV_ALIGN_BOTTOM_RIGHT, -15, -20);
    lv_obj_add_flag(img_zoom, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(img_zoom, &main_Zoom_bg);

    lv_obj_t * label = lv_label_create(img_zoom);
    lv_label_set_text(label, _(cur_zoom));
    lv_obj_add_style(label, &style_font_default_36, LV_PART_MAIN);

    lv_obj_add_event_cb(img_zoom, open_scr_zoom_cb, LV_EVENT_CLICKED, NULL);

    return label;
}

void create_sd_icon(lv_obj_t *parent, int status, float storage){
    img_sd = lv_image_create(parent);
    lv_obj_align(img_sd, LV_ALIGN_TOP_LEFT, 10, 10);

    label_sd = lv_label_create(parent);
    lv_obj_add_style(label_sd, &style_font_default_36, LV_PART_MAIN);
    lv_obj_align(label_sd, LV_ALIGN_TOP_LEFT, 65, 15);

    update_sd_icon(status, storage);

    return;
}

void create_battery_icon(lv_obj_t *parent, bool charging, int level){
    img_battery = lv_image_create(parent);
    lv_obj_align(img_battery, LV_ALIGN_TOP_RIGHT, -20, 10);
    update_battery_icon(charging, level);
}

lv_obj_t * create_grid(lv_obj_t * parent){
    lv_obj_t *grid_container = lv_obj_create(parent);
    lv_obj_set_size(grid_container, 800, 480);
    lv_obj_set_style_bg_opa(grid_container, LV_OPA_0, LV_PART_MAIN);
    lv_obj_set_style_border_width(grid_container, 0, LV_PART_MAIN);
    lv_obj_align(grid_container, LV_ALIGN_CENTER, 0, 0);
    lv_obj_remove_flag(grid_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(grid_container, LV_OBJ_FLAG_HIDDEN);

    static lv_point_precise_t line_points[][2] = {
        {{260, 0}, {260, 480}},
        {{520, 0}, {520, 480}},
        {{0, 145}, {800, 145}},
        {{0, 295}, {800, 295}}
    };

    for (int i=0; i<4; i++){
        lv_obj_t * line;
        line = lv_line_create(grid_container);
        lv_line_set_points(line, line_points[i], 2);
        lv_obj_set_style_line_color(line, lv_color_white(), LV_PART_MAIN);
    }

    return grid_container;
}

void create_pic_lib_icon(lv_obj_t *parent){
    img_pic_lib = lv_image_create(parent);
    lv_obj_align(img_pic_lib, LV_ALIGN_LEFT_MID, -20, 0);
    lv_image_set_src(img_pic_lib, &main_Picture_library);

    /*CB*/
    lv_obj_add_flag(img_pic_lib, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(img_pic_lib, open_scr_pic_lib_cb, LV_EVENT_CLICKED, NULL);

}

lv_obj_t *  create_search_icon(lv_obj_t * parent){
    img_search = lv_image_create(parent);
    lv_obj_align(img_search, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_image_set_src(img_search, &Pattern_Search);
    lv_obj_add_flag(img_search, LV_OBJ_FLAG_CLICKABLE);
    return img_search;
}

lv_obj_t * create_label_top_center(lv_obj_t * parent, char * str){
    lv_obj_t * label = lv_label_create(parent);
    lv_label_set_text(label, str);
    lv_obj_add_style(label, &style_font_default_36, LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 22);
    return label;
}

lv_obj_t * create_btn_cancle(lv_obj_t * parent){
    lv_obj_t * btn_cancle  = lv_button_create(parent);
    lv_obj_set_style_bg_color(btn_cancle, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN);
    lv_obj_set_size(btn_cancle, lv_pct(35), lv_pct(20));
    lv_obj_align(btn_cancle, LV_ALIGN_BOTTOM_MID, lv_pct(-20), lv_pct(-25));

    lv_obj_t * label_cancle= lv_label_create(btn_cancle);
    lv_label_set_text(label_cancle, _(STRING_CANCEL));
    lv_obj_add_style(label_cancle, &style_font_default_36, LV_PART_MAIN);

    return btn_cancle;
}

lv_obj_t * create_btn_confirm(lv_obj_t * parent){
    lv_obj_t * btn_confirm  = lv_button_create(parent);
    lv_obj_set_style_bg_color(btn_confirm, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);
    lv_obj_set_size(btn_confirm, lv_pct(35), lv_pct(20));
    lv_obj_align(btn_confirm, LV_ALIGN_BOTTOM_MID, lv_pct(20), lv_pct(-25));

    lv_obj_t * label_confirm = lv_label_create(btn_confirm);
    lv_label_set_text(label_confirm, _(STRING_CONFIRM));
    lv_obj_add_style(label_confirm, &style_font_default_36, LV_PART_MAIN);

    return btn_confirm;
}

lv_obj_t * create_cam_mode_icon(lv_obj_t * parent, int mode){
    lv_obj_t * img_cam_mode = lv_image_create(parent);
    lv_obj_align(img_cam_mode, LV_ALIGN_BOTTOM_LEFT, 10, -20);
    lv_obj_set_user_data(img_cam_mode, (void*)(intptr_t)mode);
    lv_image_set_src(img_cam_mode, get_mode_icon(mode));
    /*CB*/
    lv_obj_add_flag(img_cam_mode, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(img_cam_mode, open_scr_cam_modes, LV_EVENT_CLICKED, NULL);
    return img_cam_mode;
}

lv_obj_t* create_exit_icon(lv_obj_t * parent){
    lv_obj_t * img_exit = lv_image_create(parent);
    lv_obj_align(img_exit, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_image_set_src(img_exit, &common_return);

    /*CB*/
    lv_obj_add_flag(img_exit, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(img_exit, open_scr_home_cb, LV_EVENT_CLICKED, NULL);

    return img_exit;
}

void create_spot_metering(lv_obj_t * parent){
    img_spot_met = lv_image_create(parent);
    lv_obj_center(img_spot_met);
    lv_image_set_src(img_spot_met, &spot_metering);
}

lv_obj_t *create_pic_select_icon(lv_obj_t * parent){
    img_pic_select = lv_image_create(parent);
    lv_obj_align(img_pic_select, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_image_set_src(img_pic_select, &filelist_multiselect);
    lv_obj_add_flag(img_pic_select, LV_OBJ_FLAG_CLICKABLE);
    return img_pic_select;
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

void reset_spot_metering(){
    lv_obj_center(img_spot_met);
}


static void on_roller_release_cb(lv_event_t * e){
     lv_obj_set_style_text_color(lv_event_get_target(e), lv_palette_main(LV_PALETTE_BLUE), LV_PART_SELECTED);
}

static void on_roller_press_cb(lv_event_t * e){
     lv_obj_set_style_text_color(lv_event_get_target(e), lv_color_white(), LV_PART_SELECTED);
}

lv_obj_t * create_roller(
    lv_obj_t *parent,
    const int *options,
    int option_count,
    int default_selected,
    int width,
    int height,
    const lv_style_t *font_style,
    bool special_options_only_int
){
    char temp_buffer[32];
    char options_str[1024] = "";
    for (int i = 0; i < option_count; i++) {
        if (special_options_only_int) {
            snprintf(temp_buffer, sizeof(temp_buffer), "%d", options[i]);
            strcat(options_str, temp_buffer);
        } else strcat(options_str, _(options[i]));
        if (i < option_count - 1) {
            strcat(options_str, "\n");
        }
    }

    lv_obj_t  *roller = lv_roller_create(parent);
    lv_obj_set_size(roller, width, height);
    lv_obj_align(roller, LV_ALIGN_CENTER, 0, 50);

    lv_roller_set_options(roller, options_str, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_selected(roller, default_selected, LV_ANIM_OFF);

    lv_obj_set_style_bg_opa(roller, LV_OPA_0, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(roller, LV_OPA_0, LV_PART_SELECTED);
    lv_obj_set_style_border_width(roller, 0, LV_PART_MAIN);

    lv_obj_add_style(roller, font_style, LV_PART_MAIN);
    lv_obj_set_style_text_color(roller, lv_palette_main(LV_PALETTE_BLUE), LV_PART_SELECTED);
    lv_obj_set_style_text_line_space(roller, 50, LV_PART_MAIN);

    lv_obj_add_event_cb(roller, on_roller_release_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(roller, on_roller_press_cb, LV_EVENT_PRESSED, NULL);

    return roller;
}

lv_obj_t * create_roller_align_right(
    lv_obj_t *parent,
    const int *options,
    int option_count,
    int default_selected,
    int width,
    int height,
    const lv_style_t *font_style,
    bool special_options_only_int,
    bool more_visible_rows
) {
    lv_obj_t * cont =  lv_obj_create(parent);
    lv_obj_add_style(cont, &style_cont_transparent, LV_PART_MAIN);
    lv_obj_align(cont, LV_ALIGN_RIGHT_MID, -40, 0);
    lv_obj_set_size(cont, width+50, height+50);
    lv_obj_remove_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    //create roller
    lv_obj_t * roller = create_roller(cont,options, option_count, default_selected, width, height,font_style, special_options_only_int);
    lv_obj_align(roller, LV_ALIGN_RIGHT_MID, 0, 0);
    if (more_visible_rows){
            lv_obj_set_style_text_line_space(roller, 15, LV_PART_MAIN);
            lv_roller_set_visible_row_count(roller, 9);
    }
    lv_obj_set_style_text_align(roller, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);

    //create indicator
    lv_obj_t * ind = lv_image_create(parent);
    lv_obj_align(ind, LV_ALIGN_RIGHT_MID, -15, 0);
    lv_image_set_src(ind, &Pattern_Select);

    return roller;
}



void update_roller_options(
    lv_obj_t * roller_obj,
    const int *options,
    int option_count,
    bool special_options_only_int
){
    char temp_buffer[32];
    char options_str[1024] = "";
    for (int i = 0; i < option_count; i++) {
        if (special_options_only_int) {
            snprintf(temp_buffer, sizeof(temp_buffer), "%d", options[i]);
            strcat(options_str, temp_buffer);
        } else strcat(options_str, _(options[i]));
        if (i < option_count - 1) {
            strcat(options_str, "\n");
        }
    }

    int selected = lv_roller_get_selected(roller_obj);

    lv_roller_set_options(roller_obj, options_str, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_selected(roller_obj, selected, LV_ANIM_OFF);

    return;
}

/**CB**/

void delete_parent_cb(lv_event_t *e) {
    lv_obj_t *current = lv_event_get_target(e);
    lv_obj_t *parent = lv_obj_get_parent(current);

    lv_obj_del(parent);
}
