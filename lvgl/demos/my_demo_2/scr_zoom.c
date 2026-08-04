#include "my_demo_2.h"
#include "ui_components.h"
#include "scr_zoom.h"

#define CAM_ZOOM_COUNT 7

lv_obj_t *scr_zoom = NULL;
static lv_obj_t * roller = NULL;
static lv_obj_t * heading;

static const int cam_zoom_table[] = {
    STRING_ZOOM_1,
    STRING_ZOOM_1_5,
    STRING_ZOOM_2,
    STRING_ZOOM_2_5,
    STRING_ZOOM_3,
    STRING_ZOOM_3_5,
    STRING_ZOOM_4
};

int get_selected_zoom_from_roller(void){
    if(roller){
        return cam_zoom_table[lv_roller_get_selected(roller)];
    }
    return 0;
}

void open_scr_zoom_cb(){
    lv_label_set_text(heading, _(STRING_ZOOM));
    lv_screen_load(scr_zoom);
}


 void create_scr_zoom(){
    scr_zoom = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_zoom, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);

    create_exit_icon(scr_zoom);

    heading = lv_label_create(scr_zoom);
    lv_label_set_text(heading, "ZOOM");
    lv_obj_add_style(heading, &style_font_default_36, LV_PART_MAIN);
    lv_obj_align(heading, LV_ALIGN_TOP_LEFT, 95, 20);

    roller = create_roller_align_right(scr_zoom, cam_zoom_table, CAM_ZOOM_COUNT, 0, 630, 240, &style_font_default_36, false, true);
}
