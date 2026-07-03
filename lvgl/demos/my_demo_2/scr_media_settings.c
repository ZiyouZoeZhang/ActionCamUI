#include "scr_media_settings.h"
#include "my_demo_2.h"
#include "ui_components.h"

lv_obj_t * scr_media_settings = NULL;
static void swipe_scr_media_settings_cb(lv_event_t *e);

static void create_scr_media_settings(){
    scr_media_settings = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_media_settings, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);

    lv_obj_t * cont = lv_obj_create(scr_media_settings);
    lv_obj_set_size(cont, lv_pct(60), lv_pct(100));
    lv_obj_set_align(cont, LV_ALIGN_RIGHT_MID);
    //lv_obj_set_style_bg_color()

    /**CB**/
    lv_obj_add_event_cb(scr_media_settings, swipe_scr_media_settings_cb, LV_EVENT_GESTURE, NULL);
}

void open_scr_media_settings_cb(){
    create_scr_media_settings();
    lv_screen_load(scr_media_settings);
}

static void swipe_scr_media_settings_cb(lv_event_t *e){
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
    switch(dir) {
        case LV_DIR_RIGHT:
            open_scr_home_cb();
            break;
    }
}

