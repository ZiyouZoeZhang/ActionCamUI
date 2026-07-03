#include <string.h>
#include <stdio.h>
#include "ui_roller.h"
#include "camera_modes.h"
#include "my_demo_2.h"

static lv_obj_t * cam_modes_roller = NULL;

void create_cam_mode_roller(lv_obj_t * parent, int cur_mode){

    int count = get_mode_count();
    const camera_mode_info_t * cam_modes = get_all_modes();

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
    char options[300] = ""; //format:  name\nname\nname\n etc
    for (int i = 0; i < count; i++) {
        strcat(options, cam_modes[i].name);
        if (i < count-1 ) {
            strcat(options, "\n");
        }
    }

    cam_modes_roller = lv_roller_create(parent);
    lv_obj_set_size(cam_modes_roller, 600, 250);
    lv_obj_center(cam_modes_roller);

    lv_roller_set_options(cam_modes_roller, options, LV_ROLLER_MODE_INFINITE);
    lv_roller_set_selected(cam_modes_roller, cur_mode, LV_ANIM_OFF);

    lv_obj_set_style_bg_opa(cam_modes_roller, LV_OPA_0,  LV_PART_MAIN);
    lv_obj_set_style_bg_opa(cam_modes_roller, LV_OPA_0,  LV_PART_SELECTED);
    lv_obj_set_style_border_width(cam_modes_roller, 0, LV_PART_MAIN);

    lv_obj_add_style(cam_modes_roller, &style_font_default_36, LV_PART_MAIN);
    lv_obj_set_style_text_color(cam_modes_roller, lv_palette_main(LV_PALETTE_BLUE), LV_PART_SELECTED);
    lv_obj_set_style_text_line_space(cam_modes_roller, 50, LV_PART_MAIN);
}

int get_selected_mode_from_roller(void) {
    if (cam_modes_roller) {
        return lv_roller_get_selected(cam_modes_roller);
    }
    return 0;
}
