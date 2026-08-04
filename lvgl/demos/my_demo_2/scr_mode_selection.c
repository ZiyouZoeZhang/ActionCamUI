#include <string.h>
#include <stdio.h>
#include "camera_modes.h"
#include "my_demo_2.h"


static lv_obj_t * scr_mode_selection = NULL;
static lv_obj_t * container;
static lv_obj_t * label;
static lv_obj_t * exit_icon;

static void swipe_scr_mode_selection_cb(lv_event_t *e);
static void create_cam_mode_roller(lv_obj_t * parent, int cur_mode);


static void return_home_cb(){

    printf("UPDATE RESOUTION ACOODINGLY\n");
    open_scr_home_cb();

}

 void create_scr_mode_selection(int mode){
     /**background**/
    scr_mode_selection = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_mode_selection,BG_COLOR_DARK_GREY, LV_PART_MAIN);

    /**ui components**/
    create_cam_mode_roller(scr_mode_selection, mode);
    exit_icon = create_exit_icon(scr_mode_selection);
    lv_obj_remove_event_cb(exit_icon, open_scr_home_cb);
    lv_obj_add_event_cb(exit_icon, return_home_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * swipe_icon = lv_obj_create(scr_mode_selection);
    lv_obj_add_style(swipe_icon, &style_swipe_icon, LV_PART_MAIN);
    lv_obj_remove_flag(swipe_icon, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(swipe_icon, lv_pct(17), lv_pct(2));
    lv_obj_set_style_bg_color(swipe_icon, BG_COLOR_LIGHT_GREY, LV_PART_MAIN);
    lv_obj_align(swipe_icon, LV_ALIGN_TOP_MID, 0, 40);

    label = lv_label_create(scr_mode_selection);
    lv_obj_add_style(label, &style_font_default_36, LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_TOP_MID, lv_pct(0), lv_pct(20));

    /**CB**/

    lv_obj_add_event_cb(scr_mode_selection, swipe_scr_mode_selection_cb, LV_EVENT_RELEASED, NULL);

    /**load screen**/
    lv_screen_load(scr_mode_selection);
}

static void value_changed_cb(){
    lv_label_set_text(label, _(mode_names[get_selected_mode_from_roller(0)]));
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

    lv_obj_add_event_cb(container, value_changed_cb, LV_EVENT_SCROLL, NULL);
}

int get_selected_mode_from_roller(int cur) {
    int index = (lv_obj_get_scroll_x(container)+298)/141;
    return index;
}

void open_scr_cam_modes(){
    lv_label_set_text(label, _(mode_names[get_selected_mode_from_roller(0)]));
    lv_screen_load(scr_mode_selection);
}

static void swipe_scr_mode_selection_cb(lv_event_t *e){
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
    switch(dir) {
        case LV_DIR_BOTTOM:
            return_home_cb();
            break;
        default:
            break;
    }
}

