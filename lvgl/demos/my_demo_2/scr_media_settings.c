#include "scr_media_settings.h"
#include "my_demo_2.h"
#include "ui_components.h"
#include "ui_menu_btnm.h"

static int media_buttons_total = 5;
lv_obj_t * scr_media_settings = NULL;
lv_obj_t * scr_media_settings_select = NULL;
static lv_obj_t * cont;
static lv_obj_t *cont_grid;
static lv_obj_t * roller;

void open_scr_media_settings_cb();

static int active_media_btn_count = 0;
static void swipe_scr_media_settings_cb(lv_event_t *e);
static int count_active_media_btn();
static void create_cont_grid(lv_obj_t * parent);
static void create_media_set_grid(lv_obj_t * parent);
static void create_scr_media_settings();
static void open_media_selection_cb(lv_event_t * e);


static const char *ldc_states[] = {"Off", "On"};
static const char *meter_states[] = {"Center", "Average", "Spot"};
static const char *exposure_states[] = {"Auto", "Manual"};
static const char *shutter_states[] = {"Auto", "1/4000", "1/2000", "1/1000", "1/500", "1/250", "1/125", "1/60", "1/30", "1/15", "1/8", "1/4", "1/2", "1\"", "2\"", "4\"", "8\"", "15\"", "30\""};
static const char *iso_states[] = {"Auto", "100", "200", "400", "800", "1600", "3200", "6400"};

media_set_btn_info_t media_buttons[] = {
    {CAM_MEDIA_LDC,         "LDC",          true,  false, 0, ldc_states,     2},
    {CAM_MEDIA_METER_MODE,  "Meter Mode",   true,  false, 1, meter_states,   3},
    {CAM_MEDIA_EXPOSURE,    "Exposure",     true,  false,  0, exposure_states, 2},
    {CAM_MEDIA_SHUTTER,     "Shutter",      true,  false,  0, shutter_states,  19},
    {CAM_MEDIA_ISO,         "ISO",          true,  true,  0, iso_states,      8},
};

static void create_media_set_grid(lv_obj_t * parent) {
    int cur_grid = 0;
        for (int i = 0; i < media_buttons_total; i++) {
            if (!media_buttons[i].active) continue;

            lv_obj_t *btn = lv_btn_create(parent);
            lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, cur_grid % 2, 1, LV_GRID_ALIGN_STRETCH, cur_grid / 2, 1);

            lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
            lv_obj_set_style_bg_color(btn, BG_COLOR_DARK_GREY, LV_PART_MAIN);
            lv_obj_set_style_bg_opa(btn, LV_OPA_100, LV_PART_MAIN);
            lv_obj_set_style_radius(btn, 8, LV_PART_MAIN);
            lv_obj_add_event_cb(btn, open_media_selection_cb, LV_EVENT_CLICKED, (void*)(intptr_t)i);

            //label of media setting name
            lv_obj_t *name_label = lv_label_create(btn);
            lv_label_set_text(name_label, media_buttons[i].name);
            lv_obj_add_style(name_label, &style_font_default_24, LV_PART_MAIN);
            lv_obj_align(name_label, LV_ALIGN_CENTER, 0, -15);

            //label of media setting state
            lv_obj_t *state_label = lv_label_create(btn);
            lv_label_set_text(state_label, media_buttons[i].states[media_buttons[i].cur_state]);
            lv_obj_add_style(state_label, &style_font_default_24, LV_PART_MAIN);
            lv_obj_align(state_label, LV_ALIGN_CENTER, 0, 15);
            lv_obj_set_style_text_opa(state_label, LV_OPA_100, LV_PART_MAIN);


            cur_grid ++;
    }
}

static void create_scr_media_settings(){
    /**pre process**/
    //count active btn;
    active_media_btn_count = count_active_media_btn();

    /**create screen**/
    scr_media_settings = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_media_settings, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);

    /**create cont for overall**/
    cont = lv_obj_create(scr_media_settings);
    lv_obj_set_size(cont, lv_pct(65), lv_pct(100));
    lv_obj_align(cont, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_style_bg_color(cont, BG_COLOR_DARK_GREY, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(cont, LV_OPA_50, LV_PART_MAIN);
    lv_obj_set_style_border_width(cont, 0, LV_PART_MAIN);

    /**create grid**/
    //crreate title
    create_cont_grid(scr_media_settings);

    /**CB**/
    lv_obj_add_event_cb(scr_media_settings, swipe_scr_media_settings_cb, LV_EVENT_RELEASED, NULL);
    lv_obj_add_event_cb(scr_media_settings, open_scr_home_cb, LV_EVENT_CLICKED, NULL);
}

static void create_cont_grid(lv_obj_t * parent){
    /**create cont for grid**/
    cont_grid = lv_obj_create(parent);
    lv_obj_set_size(cont_grid, lv_pct(65), lv_pct(75));
    lv_obj_align(cont_grid, LV_ALIGN_RIGHT_MID, 0, lv_pct(10));
    lv_obj_set_style_bg_opa(cont_grid, LV_OPA_0, LV_PART_MAIN);
    lv_obj_set_style_border_width(cont_grid, 0, LV_PART_MAIN);

    ///create row & columns
    static lv_coord_t row_dsc[10];
    int rows = (active_media_btn_count + 1) / 2;
    for (int i = 0; i < rows; i++)  {
            row_dsc[i] = 70;
    }
    row_dsc[rows] = LV_GRID_TEMPLATE_LAST;

    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(cont_grid, col_dsc, row_dsc);

    lv_obj_set_style_pad_all(cont_grid, 40, LV_PART_MAIN);
    lv_obj_set_style_pad_column(cont_grid, 50, LV_PART_MAIN);
    lv_obj_set_style_pad_row(cont_grid, 40, LV_PART_MAIN);

   create_media_set_grid(cont_grid);
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

static void swipe_scr_media_settings_select_cb(lv_event_t *e){
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
    switch(dir) {
        case LV_DIR_RIGHT:
            open_scr_media_settings_cb();
            break;
    }
}

static int count_active_media_btn(){
    int count = 0;
    for (int i = 0; i < media_buttons_total; i++) {
        if (media_buttons[i].active) count ++;
    }
    return count;
}

static void roller_value_changed_cb(lv_event_t *e) {
    lv_obj_t *roller_obj = lv_event_get_target(e);
    media_set_btn_info_t *btn = (media_set_btn_info_t*)lv_event_get_user_data(e);

    int selected = lv_roller_get_selected(roller_obj);
    btn->cur_state = selected;
}

static void create_media_selection(media_set_btn_info_t * btn){
     /**create screen**/
    scr_media_settings_select = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_media_settings_select, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);

    /**create cont for overall**/
    lv_obj_t * co = lv_obj_create(scr_media_settings_select);
    lv_obj_set_size(co, lv_pct(65), lv_pct(100));
    lv_obj_align(co, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_style_bg_color(co, BG_COLOR_DARK_GREY, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(co, LV_OPA_50, LV_PART_MAIN);
    lv_obj_set_style_border_width(co, 0, LV_PART_MAIN);

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
    roller = lv_roller_create(co);
    lv_obj_set_size(roller, 150, 250);
    lv_obj_align(roller, LV_ALIGN_RIGHT_MID, -40, 0);

    lv_roller_set_options(roller, options, LV_ROLLER_MODE_NORMAL);
 //   lv_roller_set_visible_row_count(roller, 5);
    lv_roller_set_selected(roller, 0, LV_ANIM_OFF);

    lv_obj_set_style_bg_opa(roller, LV_OPA_0,  LV_PART_MAIN);
    lv_obj_set_style_bg_opa(roller, LV_OPA_0,  LV_PART_SELECTED);

    lv_obj_add_style(roller, &style_font_default_24, LV_PART_MAIN);
    lv_obj_set_style_text_color(roller, lv_palette_main(LV_PALETTE_BLUE), LV_PART_SELECTED);
    lv_obj_set_style_border_width(roller, 0, LV_PART_MAIN);
    lv_obj_add_flag(roller, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_text_line_space(roller, 50, LV_PART_MAIN);

   lv_obj_add_event_cb(roller, roller_value_changed_cb, LV_EVENT_VALUE_CHANGED, btn);

    lv_obj_t * ind = lv_image_create(co);
    lv_obj_align(ind, LV_ALIGN_RIGHT_MID, -15, 0);
    lv_image_set_src(ind, &Pattern_Select);
}

static void open_media_selection_cb(lv_event_t * e){
    int btn_index = (int)(intptr_t)lv_event_get_user_data(e);

    media_set_btn_info_t *btn = &media_buttons[btn_index];

    printf("Button: %s\n", btn->name);
    printf("Current state: %s\n", btn->states[btn->cur_state]);

    create_media_selection(btn);
    lv_screen_load(scr_media_settings_select);
}
