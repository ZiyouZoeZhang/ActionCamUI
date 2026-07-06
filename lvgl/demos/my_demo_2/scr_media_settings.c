#include "scr_media_settings.h"
#include "my_demo_2.h"
#include "ui_components.h"
#include "ui_menu_btnm.h"

lv_obj_t * scr_media_settings = NULL;
lv_obj_t * scr_media_settings_select = NULL;

static bool media_set_pro = false;
static lv_style_t style_btn;
static int active_media_btn_count = 0;

void open_scr_media_settings_cb();
static void open_scr_media_selection_cb(lv_event_t * e);
static void create_scr_media_settings();
static void create_scr_media_selection(media_set_btn_info_t * btn);
static void create_settings_buttons_container(lv_obj_t * parent);
static void create_settings_buttons_grid(lv_obj_t * parent);
static void swipe_scr_media_settings_cb(lv_event_t *e);
static void swipe_scr_media_settings_select_cb(lv_event_t *e);
static void pro_btn_toogled_cb(lv_event_t * e);
static void roller_value_changed_cb(lv_event_t *e);
static int count_active_media_btn();

static const char *numer_states[] = {"3Photos", "5Photos", "10Photos", "15Photos"};
static const char *self_timer_states[] = {"3Sec", "5Sec", "10Sec", "30Sec"};
static const char *ldc_states[] = {"OFF", "ON"};
static const char *meter_states[] = {"Center", "Average", "Spot"};
static const char *exposure_states[] = {"+2", "+1.5", "+1", "+0.5", "0", "-0.5", "-1", "-1.5", "-2"};
static const char *shutter_states[] = {"Auto", "1Sec", "2Sec", "4Sec", "8Sec"};
static const char *iso_states[] = {"Auto", "6400 Max", "3200 Max", "1600 Max", "800 Max", "400 Max", "200 Max", "100 Max"};
static const char *awb_states[] = {"Auto", "Sunny", "Cloudy", "Tungsten", "FLUOR_L", "FLUOR_H"};
static const char *scene_mode_states[] = {"Auto", "Personage", "Scenery", "Defog"};
static const char *sharpness_states[] = {"High", "Medium", "Low"};
static const char *filter_states[] = {"Normal", "BW", "Colorful", "Brown", "Warm", "Cold"};

media_set_btn_info_t media_buttons[] = {
    {CAM_MEDIA_NUMBER,      "Number",        false, false, 0, numer_states,       4},
    {CAM_MEDIA_SELF_TIMER,  "Self Timer",    false, false, 0, self_timer_states,  4},
    {CAM_MEDIA_LDC,         "LDC",           true,  false, 0, ldc_states,         2},
    {CAM_MEDIA_METER_MODE,  "Meter Mode",    true,  true,  1, meter_states,       3},
    {CAM_MEDIA_EXPOSURE,    "Exposure",      true,  true,  0, exposure_states,    9},
    {CAM_MEDIA_SHUTTER,     "Shutter",       true,  true,  0, shutter_states,     5},
    {CAM_MEDIA_ISO,         "ISO",           true,  true,  0, iso_states,         8},
    {CAM_MEDIA_AWB,         "White Balance", true,  true,  0, awb_states,         6},
    {CAM_MEDIA_SCENE_MODE,  "Scene Mode",    true,  true,  0, scene_mode_states,  4},
    {CAM_MEDIA_SHARPNESS,   "Sharpness",     true,  true,  0, sharpness_states,   3},
    {CAM_MEDIA_FILTER,      "Filter",        true,  true,  0, filter_states,      6},
};


void open_scr_media_settings_cb(){
    create_scr_media_settings();
    lv_screen_load(scr_media_settings);
}

static void open_scr_media_selection_cb(lv_event_t * e){
    int btn_index = (int)(intptr_t)lv_event_get_user_data(e);

    media_set_btn_info_t *btn = &media_buttons[btn_index];

    printf("Button: %s\n", btn->name);
    printf("Current state: %s\n", btn->states[btn->cur_state]);

    create_scr_media_selection(btn);
    lv_screen_load(scr_media_settings_select);
}

static void create_scr_media_settings(){
    /**pre process**/
    //set style
    lv_style_init(&style_btn);
    lv_style_set_shadow_width(&style_btn, 0);
    lv_style_set_bg_color(&style_btn, BG_COLOR_DARK_GREY);
    lv_style_set_bg_opa(&style_btn, LV_OPA_100);
    lv_style_set_radius(&style_btn, 8);

    /**create screen**/
    scr_media_settings = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_media_settings, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);
    lv_obj_remove_flag(scr_media_settings, LV_OBJ_FLAG_SCROLLABLE);

    /**create cont for overall**/
    lv_obj_t * cont = lv_obj_create(scr_media_settings);
    lv_obj_set_size(cont, lv_pct(75), lv_pct(100));
    lv_obj_align(cont, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_style_bg_color(cont, BG_COLOR_DARK_GREY, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(cont, LV_OPA_50, LV_PART_MAIN);
    lv_obj_set_style_border_width(cont, 0, LV_PART_MAIN);

    /**create pro button**/
    lv_obj_t * pro_btn = lv_button_create(scr_media_settings);
    lv_obj_add_style(pro_btn, &style_btn, LV_PART_MAIN);
    lv_obj_set_style_bg_color(pro_btn, lv_palette_main(LV_PALETTE_BLUE), LV_STATE_CHECKED);
    lv_obj_align(pro_btn, LV_ALIGN_TOP_RIGHT, -25, 25);
    lv_obj_set_size(pro_btn, 120, 55);
    lv_obj_add_flag(pro_btn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_event_cb(pro_btn, pro_btn_toogled_cb, LV_EVENT_VALUE_CHANGED, NULL);
    if (media_set_pro) lv_obj_add_state(pro_btn, LV_STATE_CHECKED);

     lv_obj_t *pro_label = lv_label_create(pro_btn);
     lv_label_set_text(pro_label, "PRO");
     lv_obj_add_style(pro_label, &style_font_default_30, LV_PART_MAIN);
     lv_obj_center(pro_label);

    /**create grid**/
    create_settings_buttons_container(scr_media_settings);

    /**CB**/
    lv_obj_add_event_cb(scr_media_settings, swipe_scr_media_settings_cb, LV_EVENT_RELEASED, NULL);
    lv_obj_add_event_cb(scr_media_settings, open_scr_home_cb, LV_EVENT_CLICKED, NULL);
}

static void create_scr_media_selection(media_set_btn_info_t * btn){
     /**create screen**/
    scr_media_settings_select = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_media_settings_select, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);

    /**create cont for overall**/
    lv_obj_t * cont = lv_obj_create(scr_media_settings_select);
    lv_obj_set_size(cont, lv_pct(65), lv_pct(100));
    lv_obj_align(cont, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_style_bg_color(cont, BG_COLOR_DARK_GREY, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(cont, LV_OPA_50, LV_PART_MAIN);
    lv_obj_set_style_border_width(cont, 0, LV_PART_MAIN);

    /**CB**/
    lv_obj_add_event_cb(scr_media_settings_select, swipe_scr_media_settings_select_cb, LV_EVENT_RELEASED, NULL);
    lv_obj_add_event_cb(scr_media_settings_select, open_scr_media_settings_cb, LV_EVENT_CLICKED, NULL);

    char options[300] = ""; //format:  name\nname\nname\n etc
    for (int i = 0; i < btn->state_count; i++) {
        strcat(options, btn->states[i]);
        if (i < btn->state_count-1 ) {
            strcat(options, "\n");
        }
    }

    lv_obj_t * label = lv_label_create(scr_media_settings_select);
    lv_obj_add_style(label, &style_font_default_24, LV_PART_MAIN);
    lv_label_set_text(label, btn->name);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 15, 15);

    //create roller
    lv_obj_t * roller = lv_roller_create(cont);
    lv_obj_set_size(roller, 150, 250);
    lv_obj_align(roller, LV_ALIGN_RIGHT_MID, -40, 0);
    lv_roller_set_options(roller, options, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_selected(roller, btn->cur_state, LV_ANIM_OFF);
    lv_obj_set_style_bg_opa(roller, LV_OPA_0,  LV_PART_MAIN);
    lv_obj_set_style_bg_opa(roller, LV_OPA_0,  LV_PART_SELECTED);
    lv_obj_add_style(roller, &style_font_default_24, LV_PART_MAIN);
    lv_obj_set_style_text_color(roller, lv_palette_main(LV_PALETTE_BLUE), LV_PART_SELECTED);
    lv_obj_set_style_border_width(roller, 0, LV_PART_MAIN);
    lv_obj_add_flag(roller, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_text_line_space(roller, 50, LV_PART_MAIN);
    lv_obj_add_event_cb(roller, roller_value_changed_cb, LV_EVENT_VALUE_CHANGED, btn);

    //create roller indicator
    lv_obj_t * ind = lv_image_create(cont);
    lv_obj_align(ind, LV_ALIGN_RIGHT_MID, -15, 0);
    lv_image_set_src(ind, &Pattern_Select);
}

static void create_settings_buttons_container(lv_obj_t * parent){
    /**pre process**/
    //count active btn;
    active_media_btn_count = count_active_media_btn();

    /**create cont for grid**/
    lv_obj_t * cont_buttons = lv_obj_create(parent);
    lv_obj_set_size(cont_buttons, lv_pct(75), lv_pct(85));
    lv_obj_align(cont_buttons, LV_ALIGN_RIGHT_MID, 0, lv_pct(15));
    lv_obj_set_style_bg_opa(cont_buttons, 5, LV_PART_MAIN);
    lv_obj_set_style_border_width(cont_buttons, 0, LV_PART_MAIN);

    ///create row & columns
    static lv_coord_t row_dsc[10];
    int rows = (active_media_btn_count + 1) / 2;
    for (int i = 0; i < rows; i++)  {
            row_dsc[i] = 75;
    }
    row_dsc[rows] = LV_GRID_TEMPLATE_LAST;

    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(cont_buttons, col_dsc, row_dsc);

    lv_obj_set_style_pad_top(cont_buttons, 0, LV_PART_MAIN);     // ÉÏ±ß¾à
    lv_obj_set_style_pad_bottom(cont_buttons, 50, LV_PART_MAIN);
    lv_obj_set_style_pad_hor(cont_buttons, 30, LV_PART_MAIN);     // ×óÓÒ±ß¾à

 //   lv_obj_set_style_pad_all(cont_buttons, 40, LV_PART_MAIN);
   lv_obj_set_style_pad_column(cont_buttons, 25, LV_PART_MAIN);
   lv_obj_set_style_pad_row(cont_buttons, 25, LV_PART_MAIN);

   create_settings_buttons_grid(cont_buttons);
}

static void create_settings_buttons_grid(lv_obj_t * parent) {
    int cur_grid = 0;
    for (int i = 0; i < CAM_MEDIA_COUNT; i++) {
        if (!media_buttons[i].active) continue;
        if (!media_set_pro && media_buttons[i].pro) continue;

        lv_obj_t *btn = lv_btn_create(parent);
        lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, cur_grid % 2, 1, LV_GRID_ALIGN_STRETCH, cur_grid / 2, 1);
        lv_obj_add_style(btn, &style_btn, LV_PART_MAIN);
        lv_obj_add_event_cb(btn, open_scr_media_selection_cb, LV_EVENT_CLICKED, (void*)(intptr_t)i);

        //label of media setting name
        lv_obj_t *name_label = lv_label_create(btn);
        lv_label_set_text(name_label, media_buttons[i].name);
        lv_obj_add_style(name_label, &style_font_default_30, LV_PART_MAIN);
        lv_obj_align(name_label, LV_ALIGN_CENTER, 0, -18);

        //label of media setting state
        lv_obj_t *state_label = lv_label_create(btn);
        lv_label_set_text(state_label, media_buttons[i].states[media_buttons[i].cur_state]);
        lv_obj_add_style(state_label, &style_font_default_30, LV_PART_MAIN);
        lv_obj_align(state_label, LV_ALIGN_CENTER, 0, 17);
        lv_obj_set_style_text_opa(state_label, LV_OPA_100, LV_PART_MAIN);

        cur_grid ++;
    }
}

static void swipe_scr_media_settings_cb(lv_event_t *e){
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
    switch(dir) {
        case LV_DIR_RIGHT:
            open_scr_home_cb();
            break;
    }
}

static void swipe_scr_media_settings_select_cb(lv_event_t *e){
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
    switch(dir) {
        case LV_DIR_RIGHT:
            open_scr_media_settings_cb();
            break;
    }
}

static void pro_btn_toogled_cb(lv_event_t * e){
    media_set_pro = !media_set_pro;
    open_scr_media_settings_cb();
}

static void roller_value_changed_cb(lv_event_t *e) {
    lv_obj_t *roller_obj = lv_event_get_target(e);
    media_set_btn_info_t *btn = (media_set_btn_info_t*)lv_event_get_user_data(e);

    int selected = lv_roller_get_selected(roller_obj);
    btn->cur_state = selected;
}

static int count_active_media_btn(){
    int count = 0;
    for (int i = 0; i < CAM_MEDIA_COUNT; i++) {
        if (media_buttons[i].active && (media_set_pro || (!media_set_pro && !media_buttons[i].pro))) count ++;
    }
    return count;
}
