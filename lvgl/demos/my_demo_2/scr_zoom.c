#include "my_demo_2.h"
#include "ui_components.h"
#include "scr_zoom.h"

#define CAM_ZOOM_COUNT 7

lv_obj_t *scr_zoom = NULL;
static lv_obj_t * roller = NULL;

const char* cam_zoom_table[] = {
    "1.0X",
    "1.5X",
    "2.0X",
    "2.5X",
    "3.0X",
    "3.5X",
    "4.0X"
};

int get_selected_zoom_from_roller(void){
    if(roller){
        return lv_roller_get_selected(roller);
    }
    return 0;
}

void open_scr_zoom_cb(){
    lv_screen_load(scr_zoom);
}

static void on_release_cb(){
     lv_obj_set_style_text_color(roller, lv_palette_main(LV_PALETTE_BLUE), LV_PART_SELECTED);
}

static void on_press_cb(){
     lv_obj_set_style_text_color(roller, lv_color_white(), LV_PART_SELECTED);
}

 void create_scr_zoom(){
    scr_zoom = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_zoom, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);

    create_exit_icon(scr_zoom);

    lv_obj_t * heading = lv_label_create(scr_zoom);
    lv_label_set_text(heading, "ZOOM");
    lv_obj_add_style(heading, &style_font_default_36, LV_PART_MAIN);
    lv_obj_align(heading, LV_ALIGN_TOP_LEFT, 95, 20);

    char options[300] = ""; //format:  name\nname\nname\n etc
    for (int i = 0; i < CAM_ZOOM_COUNT; i++) {
        strcat(options, cam_zoom_table[i]);
        if (i < CAM_ZOOM_COUNT-1 ) {
            strcat(options, "\n");
        }
    }

    //create roller
    roller = lv_roller_create(scr_zoom);
    lv_obj_align(roller, LV_ALIGN_RIGHT_MID, -60, 0);

    lv_roller_set_options(roller, options, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller, 9);
    lv_roller_set_selected(roller, 0, LV_ANIM_OFF);

    lv_obj_set_style_bg_opa(roller, LV_OPA_0,  LV_PART_MAIN);
    lv_obj_set_style_bg_opa(roller, LV_OPA_0,  LV_PART_SELECTED);

    lv_obj_add_style(roller, &style_font_default_36, LV_PART_MAIN);
    lv_obj_set_style_text_color(roller, lv_palette_main(LV_PALETTE_BLUE), LV_PART_SELECTED);
    lv_obj_set_style_border_width(roller, 0, LV_PART_MAIN);

    lv_obj_add_event_cb(roller, on_release_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(roller, on_press_cb, LV_EVENT_PRESSED, NULL);

    //create indicator
    lv_obj_t * ind = lv_image_create(scr_zoom);
    lv_obj_align(ind, LV_ALIGN_RIGHT_MID, -15, 0);
    lv_image_set_src(ind, &Pattern_Select);
}
