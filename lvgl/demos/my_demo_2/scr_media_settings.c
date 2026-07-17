#include "my_demo_2.h"

lv_obj_t * scr_media_settings = NULL;
lv_obj_t * scr_media_settings_select = NULL;

static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
static lv_coord_t row_dsc[10];
int rows = 0;

static bool media_set_pro = false;
static int active_media_btn_count = 0;
static lv_obj_t * media_select_label;
static lv_obj_t * cont_buttons;
static lv_obj_t * media_select_roller;

static lv_style_t style_btn;

static void update_scr_media_settings();
static void update_scr_media_selection(media_set_btn_info_t * btn);

static void open_scr_media_selection_cb(lv_event_t * e);
void open_scr_media_settings_cb();
void create_scr_media_selection();

static void create_settings_buttons_container(lv_obj_t * parent);
static void create_settings_buttons_grid(lv_obj_t * parent);
static void swipe_scr_media_settings_cb(lv_event_t *e);
static void swipe_scr_media_settings_select_cb(lv_event_t *e);
static void pro_btn_toogled_cb(lv_event_t * e);
static void roller_value_changed_cb(lv_event_t *e);
static int count_active_media_btn();


static const int list_numer_states[] = {
    STRING_3FPS,
    STRING_5FPS,
    STRING_10FPS,
    STRING_15FPS
};

static const int list_self_timer_states[] = {
    STRING_3S,
    STRING_5S,
    STRING_10S,
    STRING_30S
};

static const int list_ldc_states[] = {
    STRING_OFF,
    STRING_ON
};

static const int list_meter_states[] = {
    STRING_CENTRE,
    STRING_AVERAGE,
    STRING_SPOT
};

static const int list_exposure_states[] = {
    STRING_EV_2,
    STRING_EV_1_5,
    STRING_EV_1,
    STRING_EV_0_5,
    STRING_EV_0,
    STRING_EV_B0_5,
    STRING_EV_B1,
    STRING_EV_B1_5,
    STRING_EV_B2
};

static const int list_shutter_states[] = {
    STRING_AUTO,
    STRING_1S,
    STRING_2S,
    STRING_4S,
    STRING_8S
};

static const int list_iso_states[] = {
    STRING_AUTO,
    STRING_ISO6400_MAX,
    STRING_ISO3200_MAX,
    STRING_ISO1600_MAX,
    STRING_ISO800_MAX,
    STRING_ISO400_MAX,
    STRING_ISO200_MAX,
    STRING_ISO100_MAX
};

static const int list_awb_states[] = {
    STRING_AUTO,
    STRING_DAYLIGHT,
    STRING_CLOUDY,
    STRING_TUNGSTEN,
    STRING_FLUOR_L,
    STRING_FLUOR_H
};

static const int list_scene_mode_states[] = {
    STRING_AUTO,
    STRING_SCENE_Personage,
    STRING_SCENE_Scenery,
    STRING_SCENE_Defog
};

static const int list_sharpness_states[] = {
    STRING_HIGH,
    STRING_MEDIUM,
    STRING_LOW
};

static const int list_filter_states[] = {
    STRING_FILTER_NORMAL,
    STRING_BLACK_AND_WHITE,
    STRING_COLORFUL,
    STRING_BROWN,
    STRING_WARM_TONES,
    STRING_COLD_TONES
};

media_set_btn_info_t media_buttons[] = {
    {CAM_MEDIA_NUMBER,      STRING_BUTST_TYPE,        false, false, 0, list_numer_states,       4},
    {CAM_MEDIA_SELF_TIMER,  STRING_DELAY_TIME,    false, false, 0, list_self_timer_states,  4},
    {CAM_MEDIA_LDC,         STRING_LDC,           true,  false, 0, list_ldc_states,         2},
    {CAM_MEDIA_METER_MODE,  STRING_METER,         true,  true,  1, list_meter_states,       3},
    {CAM_MEDIA_EXPOSURE,    STRING_EXPOSURE_EV,   true,  true,  0, list_exposure_states,    9},
    {CAM_MEDIA_SHUTTER,     STRING_EXPOSURE_TIME, true,  true,  0, list_shutter_states,     5},
    {CAM_MEDIA_ISO,         STRING_ISO,           true,  true,  0, list_iso_states,         8},
    {CAM_MEDIA_AWB,         STRING_WB,            true,  true,  0, list_awb_states,         6},
    {CAM_MEDIA_SCENE_MODE,  STRING_SCENE_MODE,    true,  true,  0, list_scene_mode_states,  4},
    {CAM_MEDIA_SHARPNESS,   STRING_SHARPNESS,     true,  true,  0, list_sharpness_states,   3},
    {CAM_MEDIA_FILTER,      STRING_FILTER,        true,  true,  0, list_filter_states,      6},
};

void open_scr_media_settings_cb(){
    update_scr_media_settings();
    lv_screen_load(scr_media_settings);
}

static void open_scr_media_selection_cb(lv_event_t * e){
    int btn_index = (int)(intptr_t)lv_event_get_user_data(e);
    media_set_btn_info_t *btn = &media_buttons[btn_index];

    //printf("Button: %s\n", btn->name);
    //printf("Current state: %s\n", btn->states[btn->cur_state]);

    update_scr_media_selection(btn);
    lv_screen_load(scr_media_settings_select);
}

static void update_scr_media_selection(media_set_btn_info_t * btn){///TBD
    char options[300] = ""; //format:  name\nname\nname\n etc
    for (int i = 0; i < btn->state_count; i++) {
            strcat(options, _(btn->states[i]));
        if (i < btn->state_count-1 ) {
            strcat(options, "\n");
        }
    }
    lv_label_set_text(media_select_label, _(btn->name_id));
    lv_roller_set_options(media_select_roller, options, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_selected(media_select_roller, btn->cur_state, LV_ANIM_OFF);
    lv_obj_remove_event_cb(media_select_roller, roller_value_changed_cb);
    lv_obj_add_event_cb(media_select_roller, roller_value_changed_cb, LV_EVENT_VALUE_CHANGED, btn);
    lv_obj_add_event_cb(media_select_roller, roller_value_changed_cb, LV_EVENT_VALUE_CHANGED, btn);
}

static void update_scr_media_settings(){
    ///create row & columns
    active_media_btn_count = count_active_media_btn();
    rows = (active_media_btn_count + 1) / 2;
    for (int i = 0; i < rows; i++)  {
            row_dsc[i] = 75;
    }
    row_dsc[rows] = LV_GRID_TEMPLATE_LAST;
    lv_obj_set_grid_dsc_array(cont_buttons, col_dsc, row_dsc);

   create_settings_buttons_grid(cont_buttons);
}

void create_scr_media_settings(){
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

void create_scr_media_selection(){
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

    media_select_label = lv_label_create(scr_media_settings_select);
    lv_obj_add_style(media_select_label, &style_font_default_24, LV_PART_MAIN);
    lv_obj_align(media_select_label, LV_ALIGN_TOP_LEFT, 15, 15);

    //create roller
    media_select_roller = lv_roller_create(cont);
    lv_obj_set_size(media_select_roller, 150, 250);
    lv_obj_align(media_select_roller, LV_ALIGN_RIGHT_MID, -40, 0);
    lv_obj_set_style_bg_opa(media_select_roller, LV_OPA_0,  LV_PART_MAIN);
    lv_obj_set_style_bg_opa(media_select_roller, LV_OPA_0,  LV_PART_SELECTED);
    lv_obj_add_style(media_select_roller, &style_font_default_24, LV_PART_MAIN);
    lv_obj_set_style_text_color(media_select_roller, lv_palette_main(LV_PALETTE_BLUE), LV_PART_SELECTED);
    lv_obj_set_style_border_width(media_select_roller, 0, LV_PART_MAIN);
    lv_obj_add_flag(media_select_roller, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_text_line_space(media_select_roller, 50, LV_PART_MAIN);

    //create roller indicator
    lv_obj_t * ind = lv_image_create(cont);
    lv_obj_align(ind, LV_ALIGN_RIGHT_MID, -15, 0);
    lv_image_set_src(ind, &Pattern_Select);
}

static void create_settings_buttons_container(lv_obj_t * parent){
    /**create cont for grid**/
    cont_buttons = lv_obj_create(parent);
    lv_obj_set_size(cont_buttons, lv_pct(75), lv_pct(85));
    lv_obj_align(cont_buttons, LV_ALIGN_RIGHT_MID, 0, lv_pct(15));
    lv_obj_set_style_bg_opa(cont_buttons, 5, LV_PART_MAIN);
    lv_obj_set_style_border_width(cont_buttons, 0, LV_PART_MAIN);

    lv_obj_set_style_pad_top(cont_buttons, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(cont_buttons, 50, LV_PART_MAIN);
    lv_obj_set_style_pad_hor(cont_buttons, 30, LV_PART_MAIN);
   lv_obj_set_style_pad_column(cont_buttons, 25, LV_PART_MAIN);
   lv_obj_set_style_pad_row(cont_buttons, 25, LV_PART_MAIN);
}

static void create_settings_buttons_grid(lv_obj_t * parent) {
    lv_obj_clean(parent);
    int cur_grid = 0;
    for (int i = 0; i < CAM_MEDIA_COUNT; i++) {
        if (!media_buttons[i].active) continue;
        if (!media_set_pro && media_buttons[i].pro) continue;

        lv_obj_t * btn = lv_btn_create(parent);
        lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, cur_grid % 2, 1, LV_GRID_ALIGN_STRETCH, cur_grid / 2, 1);
        lv_obj_add_style(btn, &style_btn, LV_PART_MAIN);
        lv_obj_add_event_cb(btn, open_scr_media_selection_cb, LV_EVENT_CLICKED, (void*)(intptr_t)i);

        //label of media setting name
        lv_obj_t *name_label = lv_label_create(btn);
        lv_label_set_text(name_label, _(media_buttons[i].name_id));
        lv_obj_add_style(name_label, &style_font_default_30, LV_PART_MAIN);
        lv_obj_align(name_label, LV_ALIGN_CENTER, 0, -18);

        //label of media setting state
        lv_obj_t *state_label = lv_label_create(btn);
        lv_label_set_text(state_label, _(media_buttons[i].states[media_buttons[i].cur_state]));
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
        default:
            break;
    }
}

static void swipe_scr_media_settings_select_cb(lv_event_t *e){
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
    switch(dir) {
        case LV_DIR_RIGHT:
            open_scr_media_settings_cb();
            break;
        default:
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
