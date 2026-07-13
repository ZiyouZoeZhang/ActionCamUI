#include "my_demo_2.h"

static lv_obj_t * scr_menu_bluebooth;
static lv_obj_t * scr_menu_settings;

static lv_obj_t * menu;



static lv_obj_t * page_main;
static lv_obj_t * page_sub_wifi;
static lv_obj_t * page_sub_wifi_frequency;
static lv_obj_t * page_sub_wifi_connect;
static lv_obj_t * page_sub_auto_dormant;
static lv_obj_t * page_sub_auto_poweroff;
static lv_obj_t * page_sub_language;
static lv_obj_t * page_sub_video_format;
static lv_obj_t * page_sub_frequency;
static lv_obj_t * page_sub_voice_volume;
static lv_obj_t * page_sub_subscreen_play;
static lv_obj_t * page_sub_bluetooth;
static lv_obj_t * page_sub_date_time;
static lv_obj_t * page_sub_date_time_date;
static lv_obj_t * page_sub_date_time_time;
static lv_obj_t * page_sub_date_time_date_format;
static lv_obj_t * page_sub_voice_control;
static lv_obj_t * page_sub_voice_command;
static lv_obj_t * page_sub_factory_reset;
static lv_obj_t * page_sub_format_sd;
static lv_obj_t * page_sub_information;



static lv_style_t style_cont;

void settings_search_clicked_cb(lv_event_t * e);
char* center_string(const char *str, int length);

static lv_obj_t * create_switch(lv_obj_t * parent, const char * txt, bool chk);


/**lists**/
static const char *list_wifi_frequency[] = { "Red",  "Green", "Blue", "Yellow", "Purple",  "Orange" };
static const char *list_auto_poweroff[] = { "OFF",  "1min", "3min", "5min" };
static const char *list_auto_dormant[] = { "OFF",  "10Sec", "20Sec", "30Sec", "60Sec" };
static const char *list_language[] = { "English",  "Simplified Chinese", "Traditional Chinese", "30Sec", "60Sec" };
static const char *list_video_format[] = { "PAL",  "NTSC"};
static const char *list_frequency[] = { "50Hz",  "60Hz"};
static const char *list_voice_volume[] = { "Default",  "High"};
static const char *list_subscreen_play[] = { "Default",  "Full Display"};
static const char *list_date_format[] = { "YYYY-MM-DD",  "MM-DD-YYYY", "DD-MM-YYYY"};


static settings_roller_t date_format = {
    .states = list_date_format,
    .states_count = sizeof(list_date_format) / sizeof(list_date_format[0]),
    .roller = NULL,
    .state_label = NULL
};

static settings_roller_t wifi_frequency = {
    .states = list_wifi_frequency,
    .states_count = sizeof(list_wifi_frequency) / sizeof(list_wifi_frequency[0]),
    .roller = NULL,
    .state_label = NULL
};
static settings_roller_t frequency = {
    .states = list_frequency,
    .states_count = sizeof(list_frequency) / sizeof(list_frequency[0]),
    .roller = NULL,
    .state_label = NULL
};
static settings_roller_t voice_volume = {
    .states = list_voice_volume,
    .states_count = sizeof(list_voice_volume) / sizeof(list_voice_volume[0]),
    .roller = NULL,
    .state_label = NULL
};
static settings_roller_t subscreen_play= {
    .states = list_subscreen_play,
    .states_count = sizeof(list_subscreen_play) / sizeof(list_subscreen_play[0]),
    .roller = NULL,
    .state_label = NULL
};
static settings_roller_t auto_dormant = {
    .states = list_auto_dormant,
    .states_count = sizeof(list_auto_dormant) / sizeof(list_auto_dormant[0]),
    .roller = NULL,
    .state_label = NULL
};

static settings_roller_t auto_poweroff = {
    .states = list_auto_poweroff,
    .states_count = sizeof(list_auto_poweroff) / sizeof(list_auto_poweroff[0]),
    .roller = NULL,
    .state_label = NULL
};

static settings_roller_t video_format = {
    .states = list_video_format,
    .states_count = sizeof(list_video_format) / sizeof(list_video_format[0]),
    .roller = NULL,
    .state_label = NULL
};

static settings_roller_t language = {
    .states = list_language,
    .states_count = sizeof(list_language) / sizeof(list_language[0]),
    .roller = NULL,
    .state_label = NULL
};

/**functional**/
static lv_obj_t * cur_page;
static lv_obj_t * heading;
static void set_heading_cb(lv_event_t * e){
    cur_page = lv_menu_get_cur_main_page(menu);
    if (cur_page == page_main) {
        lv_label_set_text(heading, "Settings");
    } else if (cur_page == page_sub_wifi) {
        lv_label_set_text(heading, "WiFi");
    } else if (cur_page == page_sub_wifi_frequency) {
        lv_label_set_text(heading, "WiFi Frequency");
    } else if (cur_page == page_sub_wifi_connect) {
        lv_label_set_text(heading, "WiFi Connect");
        wifi_action();
    } else if (cur_page == page_sub_bluetooth) {
        lv_label_set_text(heading, "Bluetooth Device");
    } else if (cur_page == page_sub_auto_dormant) {
        lv_label_set_text(heading, "Auto Dormant");
    } else if (cur_page == page_sub_auto_poweroff) {
        lv_label_set_text(heading, "Auto Power Off");
    } else if (cur_page == page_sub_language) {
        lv_label_set_text(heading, "Language");
    } else if (cur_page == page_sub_video_format) {
        lv_label_set_text(heading, "Video Format");
    } else if (cur_page == page_sub_frequency) {
        lv_label_set_text(heading, "Frequency");
    } else if (cur_page == page_sub_voice_volume) {
        lv_label_set_text(heading, "Voice Volume");
    } else if (cur_page == page_sub_subscreen_play) {
        lv_label_set_text(heading, "Subscreen Play");
    } else if (cur_page == page_sub_date_time) {
        lv_label_set_text(heading, "Date Time");
    } else if (cur_page == page_sub_date_time_date) {
        lv_label_set_text(heading, "Date");
    } else if (cur_page == page_sub_date_time_time) {
        lv_label_set_text(heading, "Time");
    } else if (cur_page == page_sub_date_time_date_format) {
        lv_label_set_text(heading, "Date Format");
    } else if (cur_page == page_sub_voice_control) {
        lv_label_set_text(heading, "Voice Control");
    } else if (cur_page == page_sub_voice_command) {
        lv_label_set_text(heading, "Command List");
    } else if (cur_page == page_sub_format_sd) {
        lv_label_set_text(heading, "Format SD");
    } else if (cur_page == page_sub_factory_reset) {
        lv_label_set_text(heading, "Factory Reset");
    } else if (cur_page == page_sub_information) {
        lv_label_set_text(heading, "Information");
    } else if (cur_page == page_sub_auto_poweroff) {
        lv_label_set_text(heading, "Auto Power Off");
    } else if (cur_page == page_sub_language) {
        lv_label_set_text(heading, "Language");
    } else if (cur_page == page_sub_video_format) {
        lv_label_set_text(heading, "Video Format");
    } else if (cur_page == page_sub_frequency) {
        lv_label_set_text(heading, "Frequency");
    } else if (cur_page == page_sub_voice_volume) {
        lv_label_set_text(heading, "Voice Volume");
    } else if (cur_page == page_sub_subscreen_play) {
        lv_label_set_text(heading, "Subscreen Play");
    } else {
        printf("page_else\n");
    }
}
static const char* get_current_settings_state(settings_roller_t *target) {
    if (!target || !target->roller || !target->states) return NULL;

    int selected = lv_roller_get_selected(target->roller);
    if (selected < 0 || selected >= target->states_count) return NULL;

    return target->states[selected];
}

void settings_action(){
    printf("\nSettings\n");
    lv_menu_clear_history(menu);
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
        open_scr_menu_cb();
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
    lv_obj_set_size(overall_cont, 810, 80);

    lv_obj_t * cont = lv_obj_create(overall_cont);
    lv_obj_remove_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(cont, &style_cont, LV_PART_MAIN);
    lv_obj_set_size(cont, 810, 75);

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
        if (i <(target->states_count-1) ) {
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
    lv_obj_t * page = create_basics(parent, txt);

    target->state_label = lv_label_create(page);
    lv_obj_add_style(target->state_label, &style_font_default_36, LV_PART_MAIN);
    lv_obj_align(target->state_label, LV_ALIGN_RIGHT_MID, -70, 0);
    lv_label_set_text(target->state_label, target->states[0]);

    lv_obj_t * img = lv_image_create(page);
    lv_image_set_src(img, &set_have_sub_menu);
    lv_obj_align(img, LV_ALIGN_RIGHT_MID, -20, 0);


    return page;
}

void create_scr_menu_settings(){
    scr_menu_settings = lv_obj_create(NULL);

    /** create container**/
    menu = lv_menu_create(scr_menu_settings);
    lv_obj_add_style(menu, &style_font_default_36, LV_PART_MAIN);
    lv_obj_set_style_bg_color(menu, BG_COLOR_DARK_BLUE_GREY,LV_PART_MAIN);
    lv_obj_set_size(menu, lv_pct(100), lv_pct(100));
    lv_obj_center(menu);
    lv_obj_add_event_cb(menu, set_heading_cb, LV_EVENT_VALUE_CHANGED, lv_menu_get_cur_main_page(menu));

    /**manage styles**/
    //Back BTN
    lv_obj_t *back_btn = lv_menu_get_main_header_back_button(menu);
    lv_obj_t *back_icon = lv_obj_get_child(back_btn, 0);
    lv_image_set_src(back_icon, &Pattern_Return);
    lv_obj_align(lv_menu_get_main_header(menu), LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_add_event_cb(back_btn, settings_back_cb, LV_EVENT_CLICKED, lv_menu_get_cur_main_page(menu));

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
    page_sub_wifi= lv_menu_page_create(menu, "");
    lv_obj_set_scroll_dir(page_sub_wifi, LV_DIR_VER);
    temp_page = create_text(page_sub_wifi, "Wifi information");
    lv_menu_set_load_page_event(menu, temp_page, page_sub_wifi_connect);

    page_sub_wifi_connect = lv_menu_page_create(menu, "");
    lv_obj_t * scr_wifi_connect_btn = lv_obj_create(page_sub_wifi_connect);
    lv_obj_set_style_bg_color(scr_wifi_connect_btn, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);
    temp_page = create_switch(page_sub_wifi, "Wifi connect", false);
    lv_menu_set_load_page_event(menu, temp_page, page_sub_wifi_connect);

    page_sub_wifi_frequency = lv_menu_page_create(menu, "");
    lv_obj_set_scroll_dir(page_sub_wifi_frequency, LV_DIR_VER);
    create_scr_scrollbar(page_sub_wifi_frequency, & wifi_frequency);
    temp_page = create_scrollbar(page_sub_wifi, "Wifi frequency", &wifi_frequency);
    lv_menu_set_load_page_event(menu, temp_page, page_sub_wifi_frequency);


    /**bluetooth pages**/
    page_sub_bluetooth= lv_menu_page_create(menu, "");
    lv_obj_set_scroll_dir(page_sub_bluetooth, LV_DIR_VER);
    lv_obj_t * label = lv_label_create(page_sub_bluetooth);
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_width(label, 630);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(label,"\nnot connected to a device. Please ensure the Bluetoooth microphone is turned on.");
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    /**Auto Dormant pages**/
    /*Create a main page*/
    page_main = lv_menu_page_create(menu, "");
    lv_obj_add_style(page_main, &style_scrollbar, LV_PART_SCROLLBAR);
    lv_obj_set_scroll_dir(page_main, LV_DIR_VER);

    temp_page = create_text(page_main, "Wifi");
    lv_menu_set_load_page_event(menu, temp_page, page_sub_wifi);

    temp_page =  create_text(page_main, "Bluetooth Device");
    lv_menu_set_load_page_event(menu, temp_page, page_sub_bluetooth);

    page_sub_auto_dormant = lv_menu_page_create(menu, "");
    lv_obj_set_scroll_dir(page_sub_auto_dormant, LV_DIR_VER);
    create_scr_scrollbar(page_sub_auto_dormant, & auto_dormant);
    temp_page = create_scrollbar(page_main, "Auto Dormant", &auto_dormant);
    lv_menu_set_load_page_event(menu, temp_page, page_sub_auto_dormant);

    page_sub_auto_poweroff = lv_menu_page_create(menu, "");
    lv_obj_set_scroll_dir(page_sub_auto_poweroff, LV_DIR_VER);
    create_scr_scrollbar(page_sub_auto_poweroff, &auto_poweroff);
    temp_page = create_scrollbar(page_main, "Auto Poweroff", &auto_poweroff);
    lv_menu_set_load_page_event(menu, temp_page, page_sub_auto_poweroff);

    page_sub_language = lv_menu_page_create(menu, "");
    lv_obj_set_scroll_dir(page_sub_language, LV_DIR_VER);
    create_scr_scrollbar(page_sub_language, &language);
    temp_page = create_scrollbar(page_main, "Language", &language);
    lv_menu_set_load_page_event(menu, temp_page, page_sub_language);

    page_sub_video_format = lv_menu_page_create(menu, "");
    lv_obj_set_scroll_dir(page_sub_video_format, LV_DIR_VER);
    create_scr_scrollbar(page_sub_video_format, &video_format);
    temp_page = create_scrollbar(page_main, "Video Format", &video_format);
    lv_menu_set_load_page_event(menu, temp_page, page_sub_video_format);

    page_sub_frequency = lv_menu_page_create(menu, "");
    lv_obj_set_scroll_dir(page_sub_frequency, LV_DIR_VER);
    create_scr_scrollbar(page_sub_frequency, &frequency);
    temp_page = create_scrollbar(page_main, "Frequency", &frequency);
    lv_menu_set_load_page_event(menu, temp_page, page_sub_frequency);

    page_sub_voice_volume = lv_menu_page_create(menu, "");
    lv_obj_set_scroll_dir(page_sub_voice_volume, LV_DIR_VER);
    create_scr_scrollbar(page_sub_voice_volume, &voice_volume);
    temp_page = create_scrollbar(page_main, "Voice Volume", &voice_volume);
    lv_menu_set_load_page_event(menu, temp_page, page_sub_voice_volume);

    page_sub_subscreen_play = lv_menu_page_create(menu, "");
    lv_obj_set_scroll_dir(page_sub_subscreen_play, LV_DIR_VER);
    create_scr_scrollbar(page_sub_subscreen_play, &subscreen_play);
    temp_page = create_scrollbar(page_main, "SubScreen Play", &subscreen_play);
    lv_menu_set_load_page_event(menu, temp_page, page_sub_subscreen_play);

    create_switch(page_main, "LEDs", false);
    create_switch(page_main, "Date Stamp", false);
    create_switch(page_main, "Stamp", false);
    create_switch(page_main, "Power Tone", false);
    create_switch(page_main, "Key Tone", false);
    create_switch(page_main, "Clap Tone", false);
    create_switch(page_main, "Grid", false);
    create_switch(page_main, "Quick-Start Switch", false);


    /**date pages**/
    page_sub_date_time= lv_menu_page_create(menu, "");
    lv_obj_set_scroll_dir(page_sub_date_time, LV_DIR_VER);
    temp_page = create_text(page_main, "Date Time");
    lv_menu_set_load_page_event(menu, temp_page, page_sub_date_time);

    page_sub_date_time_date = lv_menu_page_create(menu, "");
    temp_page = create_text(page_sub_date_time, "Date");
    lv_menu_set_load_page_event(menu, temp_page, page_sub_date_time_date);


    page_sub_date_time_time = lv_menu_page_create(menu, "");
    temp_page = create_text(page_sub_date_time, "Time");
    lv_menu_set_load_page_event(menu, temp_page, page_sub_date_time_time);


    page_sub_date_time_date_format = lv_menu_page_create(menu, "");
    temp_page = create_text(page_sub_date_time, "Date Format");
    lv_obj_set_scroll_dir(page_sub_date_time_date_format, LV_DIR_VER);
    create_scr_scrollbar(page_sub_date_time_date_format, &date_format);
    lv_menu_set_load_page_event(menu, temp_page, page_sub_date_time_date_format);


    /**voice control **/
    page_sub_voice_control = lv_menu_page_create(menu, "");
    temp_page = create_text(page_main, "Voice Control");
    lv_obj_set_scroll_dir(page_sub_voice_control, LV_DIR_VER);
    lv_menu_set_load_page_event(menu, temp_page, page_sub_voice_control);

    create_switch(page_sub_voice_control, "Voice Control", false);

    page_sub_voice_command= lv_menu_page_create(menu, "");
    lv_obj_set_scroll_dir(page_sub_voice_command, LV_DIR_VER);
    label = lv_label_create(page_sub_voice_command);
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_width(label, 630);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(label,"command             command \ncommand             command \ncommand             command \ncommand             command \n");
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    temp_page =  create_text(page_sub_voice_control, "Command List");
    lv_menu_set_load_page_event(menu, temp_page, page_sub_voice_command);

    page_sub_format_sd = lv_menu_page_create(menu, "");
    temp_page = create_text(page_main, "Format SD");
    lv_menu_set_load_page_event(menu, temp_page, page_sub_format_sd);

    page_sub_factory_reset = lv_menu_page_create(menu, "");
    temp_page = create_text(page_main, "Factory Reset");
    lv_menu_set_load_page_event(menu, temp_page, page_sub_factory_reset);

    page_sub_information = lv_menu_page_create(menu, "");
    temp_page = create_text(page_main, "Information");
    lv_menu_set_load_page_event(menu, temp_page, page_sub_information);


    /*
    page_sub_wifi_frequency = lv_menu_page_create(menu, "");
    lv_obj_set_scroll_dir(page_sub_wifi_frequency, LV_DIR_VER);
    create_scr_scrollbar(page_sub_wifi_frequency, & wifi_frequency);
    temp_page = create_scrollbar(page_sub_date_time, "Wifi frequency", &wifi_frequency);
    lv_menu_set_load_page_event(menu, temp_page, page_sub_wifi_frequency);

    temp_page = create_text(page_sub_wifi, "Wifi information");
    lv_menu_set_load_page_event(menu, temp_page, page_sub_wifi_connect);
    create_switch(page_main, "Date Time", false);
    create_switch(page_main, "Oh", false);*/


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


 char* center_string(const char* str, int total_length) {
    if (str == NULL) return NULL;

    int str_len = strlen(str);

    if (str_len >= total_length) {
        char* result = (char*)malloc(str_len + 1);
        if (result) strcpy(result, str);
        return result;
    }

    int total_spaces = total_length - str_len;
    int left_spaces = total_spaces / 2;
    int right_spaces = total_spaces - left_spaces;

    char* result = (char*)malloc(total_length + 1);
    if (result == NULL) return NULL;

    int pos = 0;
    for (int i = 0; i < left_spaces; i++) {
        result[pos++] = ' ';
    }

    for (int i = 0; i < str_len; i++) {
        result[pos++] = str[i];
    }

    for (int i = 0; i < right_spaces; i++) {
        result[pos++] = ' ';
    }

    result[pos] = '\0';

    printf(result);
    return result;
}
