#include "my_demo_2.h"
#include "ui_components.h"
#include "scr_resolution.h"

#define CAM_RES_COUNT 7

lv_obj_t *scr_resolution = NULL;
static lv_obj_t * roller = NULL;
static lv_obj_t * heading;

const char* cam_resolution_table[] = {
    "48MP(4:3)",
    "20MP(4:3)",
    "16MP(4:3)",
    "12MP(4:3)",
    "8MP(4:3)",
    "5MP(4:3)",
    "3MP(4:3)"
};

int get_selected_resolution_from_roller(void){
    if(roller){
        return lv_roller_get_selected(roller);
    }
    return 0;
}

void open_scr_resolution_cb(){
    //create_scr_resolution();
    lv_label_set_text(heading, _(STRING_RESOLUTION));
    lv_screen_load(scr_resolution);
}

static void on_release_cb(){
     lv_obj_set_style_text_color(roller, lv_palette_main(LV_PALETTE_BLUE), LV_PART_SELECTED);
}

static void on_press_cb(){
     lv_obj_set_style_text_color(roller, lv_color_white(), LV_PART_SELECTED);
}

void create_scr_resolution(){
    scr_resolution = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_resolution, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);

    create_exit_icon(scr_resolution);

    heading = lv_label_create(scr_resolution);
    lv_label_set_text(heading, "Resolution");
    lv_obj_add_style(heading, &style_font_default_36, LV_PART_MAIN);
    lv_obj_align(heading, LV_ALIGN_CENTER, 0, -120);

    char options[500] = ""; //format:  name\nname\nname\n etc
    for (int i = 0; i < CAM_RES_COUNT; i++) {
        strcat(options, cam_resolution_table[i]);
        if (i < CAM_RES_COUNT-1 ) {
            strcat(options, "\n");
        }
    }

    //create roller
    roller = lv_roller_create(scr_resolution);
    lv_obj_set_size(roller, 600, 250);
    lv_obj_align(roller, LV_ALIGN_CENTER, 0, 50);

    lv_roller_set_options(roller, options, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_selected(roller, 0, LV_ANIM_OFF);

    lv_obj_set_style_bg_opa(roller, LV_OPA_0,  LV_PART_MAIN);
    lv_obj_set_style_bg_opa(roller, LV_OPA_0,  LV_PART_SELECTED);
    lv_obj_set_style_border_width(roller, 0, LV_PART_MAIN);

    lv_obj_add_style(roller, &style_font_default_36, LV_PART_MAIN);
    lv_obj_set_style_text_color(roller, lv_palette_main(LV_PALETTE_BLUE), LV_PART_SELECTED);
    lv_obj_set_style_text_line_space(roller, 50, LV_PART_MAIN);

    //roller cb
    lv_obj_add_event_cb(roller, on_release_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(roller, on_press_cb, LV_EVENT_PRESSED, NULL);
}
