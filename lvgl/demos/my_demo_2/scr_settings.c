#include "my_demo_2.h"

static lv_obj_t * scr_menu_bluebooth;
static lv_obj_t * scr_menu_settings;

static lv_obj_t * menu;

static lv_obj_t * page_main;
static lv_obj_t * page_sub_wifi;
static lv_obj_t * page_sub_wifi_frequency;
static lv_obj_t * page_sub_wifi_connect;

static lv_obj_t * heading;

static lv_style_t style_cont;

void settings_search_clicked_cb(lv_event_t * e);
char* center_string(const char *str, int length);

static lv_obj_t * create_switch(lv_obj_t * parent, const char * txt, bool chk);


/**lists**/
static const char *list_wifi_frequency[] = {
    "Red",
    "Green",
    "Blue",
    "Yellow",
    "Purple",
    "Orange"
};

static settings_roller_t wifi_frequency = {
    .states = list_wifi_frequency,
    .states_count = sizeof(list_wifi_frequency) / sizeof(list_wifi_frequency[0]),
    .roller = NULL,
    .state_label = NULL
};

/**functional**/
static lv_obj_t * cur_page;
static void set_heading_cb(lv_event_t * e){
    cur_page = lv_menu_get_cur_main_page(menu);
    if (cur_page == page_main) lv_label_set_text(heading, "Main");
    if (cur_page == page_sub_wifi) lv_label_set_text(heading, "Wifi");
    if (cur_page == page_sub_wifi_frequency) lv_label_set_text(heading, "Wifi frequency");
    if (cur_page == page_sub_wifi_connect) wifi_action();
}

static const char* get_current_settings_state(settings_roller_t *target) {
    if (!target || !target->roller || !target->states) return NULL;

    int selected = lv_roller_get_selected(target->roller);
    if (selected < 0 || selected >= target->states_count) return NULL;

    return target->states[selected];
}

void settings_action(){
    printf("Settings\n");
    lv_menu_set_page(menu, page_main);
    lv_screen_load(scr_menu_settings);
}

static void roller_value_changed_cb(lv_event_t *e) {
    lv_obj_t *roller = lv_event_get_target(e);
    settings_roller_t *target = (settings_roller_t*)lv_event_get_user_data(e);

    if (!target) return;

    int selected = lv_roller_get_selected(roller);
    if (selected < 0 || selected >= target->states_count) return;

    if (target->state_label) {
        lv_label_set_text(target->state_label, target->states[selected]);
    }
}

static void settings_back_cb(lv_event_t *e) {
    if (lv_menu_back_button_is_root(menu, lv_event_get_target(e))){
        printf("AAA\n");
        open_scr_home_cb();
    }
}

static void settings_quit_cb(lv_event_t * e){
    open_scr_home_cb();
}

static void load_styles(){
    lv_style_init(&style_cont);
    lv_style_set_bg_color(&style_cont, BG_COLOR_DARK_GREY);
    lv_style_set_border_width(&style_cont, 0);
}

/**create**/
static lv_obj_t * create_basics(lv_obj_t * parent, const char * txt){
    lv_obj_t * overall_cont = lv_menu_cont_create(parent);
    lv_obj_remove_flag(overall_cont, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * cont = lv_obj_create(overall_cont);
    lv_obj_remove_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(cont, &style_cont, LV_PART_MAIN);
    lv_obj_set_size(cont, 800, 70);
    lv_obj_align(cont, LV_ALIGN_LEFT_MID, 50, 0);

    lv_obj_t * label = lv_label_create(cont);
    lv_obj_add_style(label, &style_font_default_36, LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 30, 0);
    lv_label_set_text(label, txt);

    return cont;
}
static lv_obj_t * create_text(lv_obj_t * parent, const char * txt){
    lv_obj_t * cont = create_basics(parent, txt);

    lv_obj_t * img = lv_image_create(cont);
    lv_image_set_src(img, &set_have_sub_menu);
    lv_obj_align(img, LV_ALIGN_RIGHT_MID, -20, 0);
    return cont;
}

static lv_obj_t * create_switch(lv_obj_t * parent, const char * txt, bool chk){
    lv_obj_t * cont = create_basics(parent, txt);

    lv_obj_t * sw = lv_switch_create(cont);
    lv_obj_add_state(sw, chk ? LV_STATE_CHECKED : 0);
    lv_obj_align(sw, LV_ALIGN_RIGHT_MID, -30, 0);
    return cont;
}

static lv_obj_t * create_scr_scrollbar(lv_obj_t * parent, settings_roller_t * target){ // return screen;
    lv_obj_t * scr =  parent;

    char options[300] = ""; //format:  name\nname\nname\n etc
    for (int i = 0; i < target->states_count; i++) {
        strcat(options, target->states[i]);
        if (i <=(target->states_count-1) ) {
            strcat(options, "\n");
        }
    }

    //create roller
    target->roller = lv_roller_create(scr);
    lv_obj_set_size(target->roller, lv_pct(100), 250);
    lv_obj_align(target->roller, LV_ALIGN_CENTER, 0, 0);

    lv_roller_set_options(target->roller, options, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_selected(target->roller, 0, LV_ANIM_OFF);

    lv_obj_set_style_bg_opa(target->roller, LV_OPA_0,  LV_PART_MAIN);
    lv_obj_set_style_bg_opa(target->roller, LV_OPA_0,  LV_PART_SELECTED);
    lv_obj_set_style_border_width(target->roller, 0, LV_PART_MAIN);

    lv_obj_add_style(target->roller, &style_font_default_36, LV_PART_MAIN);
    lv_obj_set_style_text_color(target->roller, lv_palette_main(LV_PALETTE_BLUE), LV_PART_SELECTED);
    lv_obj_set_style_text_line_space(target->roller, 30, LV_PART_MAIN);

    lv_obj_add_event_cb(target->roller, roller_value_changed_cb, LV_EVENT_VALUE_CHANGED, target);
    return scr;
}

static lv_obj_t * create_scrollbar(lv_obj_t * parent, const char * txt, settings_roller_t * target){
    lv_obj_t * cont = create_basics(parent, txt);

    target->state_label = lv_label_create(cont);
    lv_obj_add_style(target->state_label, &style_font_default_36, LV_PART_MAIN);
    lv_obj_align(target->state_label, LV_ALIGN_RIGHT_MID, -70, 0);
    lv_label_set_text(target->state_label, target->states[0]);

    lv_obj_t * img = lv_image_create(cont);
    lv_image_set_src(img, &set_have_sub_menu);
    lv_obj_align(img, LV_ALIGN_RIGHT_MID, -20, 0);
    return cont;
}

void create_scr_menu_settings(){
    scr_menu_settings = lv_obj_create(NULL);
    lv_obj_set_scroll_dir(scr_menu_settings, LV_DIR_VER);

    /** create container**/
    menu = lv_menu_create(scr_menu_settings);
    lv_obj_add_style(menu, &style_font_default_36, LV_PART_MAIN);
    lv_obj_set_style_bg_color(menu, BG_COLOR_DARK_BLUE_GREY,LV_PART_MAIN);
    lv_obj_set_size(menu, 810, lv_pct(100));
    lv_obj_center(menu);
    lv_obj_add_event_cb(menu, set_heading_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /**manage styles**/
    //Back BTN
    lv_obj_t *back_btn = lv_menu_get_main_header_back_button(menu);
    lv_obj_t *back_icon = lv_obj_get_child(back_btn, 0);
    lv_image_set_src(back_icon, &Pattern_Return);
    lv_obj_align(lv_menu_get_main_header(menu), LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_add_event_cb(back_btn, settings_back_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * heading_cont = lv_obj_create(lv_menu_get_main_header(menu));
    lv_obj_set_size(heading_cont, lv_pct(65), 60);
    lv_obj_set_style_bg_color(heading_cont, BG_COLOR_DARK_BLUE_GREY,LV_PART_MAIN);
    lv_obj_set_style_border_width(heading_cont, 0, LV_PART_MAIN);
    lv_obj_remove_flag(heading_cont, LV_OBJ_FLAG_SCROLLABLE);

    heading = lv_label_create(heading_cont);
    lv_obj_add_style(heading, &style_font_default_36, LV_PART_MAIN);
    lv_obj_center(heading);

    lv_obj_t * exit_btn = lv_menu_get_main_header(menu);
    lv_obj_t * exit_icon = lv_image_create(exit_btn);
    lv_image_set_src(exit_icon, &system_set_exit);
    lv_obj_align(exit_icon, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_add_flag(exit_icon, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(exit_icon, settings_quit_cb, LV_EVENT_CLICKED, NULL);

    load_styles();

    /**create pages**/
    lv_obj_t * temp_page;

    /**wifi pages**/
    /*Create wifi_frequency page*/
    page_sub_wifi_frequency = lv_menu_page_create(menu, "");//frquenc

    create_scr_scrollbar(page_sub_wifi_frequency, & wifi_frequency);

     /*Create sub wifi page*/
    page_sub_wifi_connect = lv_menu_page_create(menu, "");

    /*Create wifi_page*/
    page_sub_wifi= lv_menu_page_create(menu, "");


    temp_page = create_switch(page_sub_wifi, "Wifi", false);
    lv_menu_set_load_page_event(menu, temp_page, page_sub_wifi_connect);
    // cb add to new page -> wifi page

    temp_page = create_scrollbar(page_sub_wifi, "Wifi frequency", &wifi_frequency);
    lv_menu_set_load_page_event(menu, temp_page, page_sub_wifi_frequency);

    temp_page = create_text(page_sub_wifi, "Wifi information");
    lv_menu_set_load_page_event(menu, temp_page, page_sub_wifi_connect);

    /**bluetooth pages**/


    /**Auto Dormant pages**/

    /*Create a main page*/
    page_main = lv_menu_page_create(menu, "");
;
    temp_page = create_text(page_main, "Wifi");
    lv_menu_set_load_page_event(menu, temp_page, page_sub_wifi);

    create_text(page_main, "Bluetooth");

    create_scrollbar(page_main, "Auto Dormant", &wifi_frequency);

    create_scrollbar(page_main, "Auto Poweroff", &wifi_frequency);

    create_scrollbar(page_main, "Language", &wifi_frequency);

    create_scrollbar(page_main, "Video Format", &wifi_frequency);

    create_scrollbar(page_main, " Frequency", &wifi_frequency);

    create_scrollbar(page_main, "Voice Volume", &wifi_frequency);

    lv_menu_set_page(menu, page_main);

    lv_menu_set_mode_root_back_button(menu, LV_MENU_ROOT_BACK_BUTTON_ENABLED);
}


void settings_search_clicked_cb(lv_event_t * e){
    printf("searching settings\n");
}

///--------------------------------bluetooth ---------------------------------------------

void bluetooth_action(){
    printf("Bluetooth toggle\n");
    lv_screen_load(scr_menu_bluebooth);
}

void bluetooth_search_clicked_cb(lv_event_t * e){
    printf("searching bluetooth\n");
}

void create_scr_menu_bluetooth(){
    scr_menu_bluebooth = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_menu_bluebooth, BG_COLOR_DARK_BLUE_GREY,LV_PART_MAIN);

    lv_obj_t * exit_icon = create_exit_icon(scr_menu_bluebooth);
    lv_obj_remove_event_cb(exit_icon, open_scr_home_cb);
    lv_obj_add_event_cb(exit_icon, open_scr_menu_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * search_icon = create_search_icon(scr_menu_bluebooth);
    lv_obj_add_event_cb(search_icon, bluetooth_search_clicked_cb, LV_EVENT_CLICKED, NULL);

    create_label_top_center(scr_menu_bluebooth, "Bluetooth");
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Create a centered string by adding spaces to the front
 * @param str   Input string
 * @param length Total length to center within
 * @return Newly allocated string with leading spaces (caller must free)
 */
 char* center_string(const char* str, int total_length) {
    if (str == NULL) return NULL;

    int str_len = strlen(str);

    // 如果原始字符串长度大于等于总长度，则返回原始字符串的副本
    if (str_len >= total_length) {
        char* result = (char*)malloc(str_len + 1);
        if (result) strcpy(result, str);
        return result;
    }

    // 计算左右空格数量
    int total_spaces = total_length - str_len;
    int left_spaces = total_spaces / 2;
    int right_spaces = total_spaces - left_spaces;  // 如果总空格数为奇数，右侧多一个

    // 分配内存并构建居中字符串
    char* result = (char*)malloc(total_length + 1);
    if (result == NULL) return NULL;

    int pos = 0;

    // 添加左侧空格
    for (int i = 0; i < left_spaces; i++) {
        result[pos++] = ' ';
    }

    // 复制原始字符串
    for (int i = 0; i < str_len; i++) {
        result[pos++] = str[i];
    }

    // 添加右侧空格
    for (int i = 0; i < right_spaces; i++) {
        result[pos++] = ' ';
    }

    result[pos] = '\0';

    printf(result);
    return result;
}
