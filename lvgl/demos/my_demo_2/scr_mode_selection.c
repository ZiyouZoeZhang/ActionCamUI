#include <string.h>
#include <stdio.h>
#include "camera_modes.h"
#include "my_demo_2.h"

static lv_obj_t * cam_modes_roller = NULL;
static lv_obj_t * scr_mode_selection = NULL;
static lv_obj_t * container;

static void swipe_scr_mode_selection_cb(lv_event_t *e);
static void create_cam_mode_roller(lv_obj_t * parent, int cur_mode);

 void create_scr_mode_selection(int mode){
     /**background**/
    scr_mode_selection = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_mode_selection,BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);

    /**ui components**/
    create_cam_mode_roller(scr_mode_selection, mode);
    create_exit_icon(scr_mode_selection);

    lv_obj_t * swipe_icon = lv_obj_create(scr_mode_selection);
    lv_obj_add_style(swipe_icon, &style_swipe_icon, LV_PART_MAIN);
    lv_obj_remove_flag(swipe_icon, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(swipe_icon, LV_ALIGN_TOP_MID, 0, 40);

    /**CB**/
    lv_obj_add_event_cb(scr_mode_selection, swipe_scr_mode_selection_cb, LV_EVENT_RELEASED, NULL);

    /**load screen**/
    lv_screen_load(scr_mode_selection);
}

static void create_cam_mode_roller(lv_obj_t * parent, int mode){
    lv_obj_t * img = lv_image_create(parent);
    lv_image_set_src(img, &mode_scrollview);
    lv_obj_center(img);

    int count = get_mode_count();
    const camera_mode_info_t * cam_modes = get_all_modes();

    container = lv_obj_create(parent);
    lv_obj_set_size(container, lv_pct(90), lv_pct(50));

    lv_obj_add_style(container, &style_cont_transparent, LV_PART_MAIN);
   lv_obj_set_style_flex_track_place(container, LV_FLEX_ALIGN_CENTER, 0);

    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_column(container, 60, 0);
    lv_obj_set_scroll_snap_x(container, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(container, LV_OBJ_FLAG_SCROLL_ELASTIC);

    for (int i=0; i<count; i++){
        img = lv_image_create(container);
        lv_image_set_src(img, cam_modes[i].icon_src);
        lv_obj_add_flag(img, LV_OBJ_FLAG_SNAPPABLE);
    }

    lv_obj_scroll_by(container, 298, 0, LV_ANIM_OFF);
}

int get_selected_mode_from_roller(int cur) {
    int index = (lv_obj_get_scroll_x(container)+298)/141;
    return index;
}

void open_scr_cam_modes(){
    lv_screen_load(scr_mode_selection);
}

static void swipe_scr_mode_selection_cb(lv_event_t *e){
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
    switch(dir) {
        case LV_DIR_BOTTOM:
            open_scr_home_cb();
            break;
        default:
            break;
    }
}

