#include "my_demo_2.h"

void refresh_all_rollers(void);
#define _(id) lv_lang_string[id][current_lang]

#define INIT_ROLLER(name, list_array) \
    static settings_roller_t name = { \
        .ids = list_array, \
        .states_count = sizeof(list_array) / sizeof(list_array[0]), \
        .roller = NULL, \
        .state_label = NULL \
    }

#define CREATE_ROLLER_PAGE(parent, page_var, roller_ptr, title) \
    do { \
        page_var = lv_menu_page_create(menu, ""); \
        lv_obj_set_scroll_dir(page_var, LV_DIR_VER); \
        create_scr_scrollbar(page_var, roller_ptr); \
        temp_page = create_scrollbar(parent, title, roller_ptr); \
        lv_menu_set_load_page_event(menu, temp_page, page_var); \
    } while(0)


#define CREATE_SIMPLE_PAGE(parent, page_var, title) \
    do { \
        page_var = lv_menu_page_create(menu, ""); \
        lv_obj_set_scroll_dir(page_var, LV_DIR_VER); \
        temp_page = create_text(parent, title); \
        lv_menu_set_load_page_event(menu, temp_page, page_var); \
    } while(0)


typedef struct {
    lv_obj_t* switch_obj;
    lv_obj_t* cont_obj;
    const char* name;
    void (*toggle_cb)(bool state);
} switch_item_t;

typedef struct {
    const int* ids;
    int states_count;
    lv_obj_t* roller;
    lv_obj_t* state_label;
} settings_roller_t;

//const char * language_id[9] = {"English", "日本語", "简体中文", "繁體中文", "한국어","Français","El español", "Deutsch","Italiano"}

static lv_obj_t* scr_menu_bluetooth;
static lv_obj_t* scr_menu_settings;
static lv_obj_t* menu;
static lv_obj_t* cur_page;
static lv_obj_t* heading;
lv_style_t style_cont;


static void switch_leds_toggle(bool state);
static void switch_date_stamp_toggle(bool state);
static void switch_stamp_toggle(bool state);
static void switch_power_tone_toggle(bool state);
static void switch_key_tone_toggle(bool state);
static void switch_clap_tone_toggle(bool state);
static void switch_grid_toggle(bool state);
static void switch_quick_start_toggle(bool state);
static void switch_voice_control_toggle(bool state);


static lv_obj_t* grid_switch = NULL;

static lv_obj_t* page_main;
static lv_obj_t* page_sub_wifi;
static lv_obj_t* page_sub_wifi_frequency;
static lv_obj_t* page_sub_wifi_connect;
static lv_obj_t* page_sub_bluetooth;
static lv_obj_t* page_sub_auto_dormant;
static lv_obj_t* page_sub_auto_poweroff;
static lv_obj_t* page_sub_language;
static lv_obj_t* page_sub_video_format;
static lv_obj_t* page_sub_frequency;
static lv_obj_t* page_sub_voice_volume;
static lv_obj_t* page_sub_subscreen_play;
static lv_obj_t* page_sub_date_time;
static lv_obj_t* page_sub_date_time_date;
static lv_obj_t* page_sub_date_time_time;
static lv_obj_t* page_sub_date_time_date_format;
static lv_obj_t* page_sub_voice_control;
static lv_obj_t* page_sub_voice_command;
static lv_obj_t* page_sub_format_sd;
static lv_obj_t* page_sub_factory_reset;
static lv_obj_t* page_sub_information;

static const int list_wifi_frequency[] = {
    STRING_LOW,
    STRING_MEDIUM,
    STRING_HIGH
};

static const int list_auto_poweroff[] = {
    STRING_OFF,
    STRING_1MIN,
    STRING_3MIN,
    STRING_5MIN
};

static const int list_auto_dormant[] = {
    STRING_OFF,
    STRING_10S,
    STRING_15S,
    STRING_30S,
    STRING_60S
};

static const int list_language[] = {
    STRING_ENGLISH,
    STRING_ja,
    STRING_CHINESE,
    STRING_zh_HK,
    STRING_KR,
    STRING_fr,
    STRING_es,
    STRING_de,
    STRING_it
};

static const int list_video_format[] = {
    STRING_PAL,
    STRING_NTSC
};

static const int list_frequency[] = {
    STRING_50HZ,
    STRING_60HZ
};

static const int list_voice_volume[] = {
    STRING_DEFAULT,
    STRING_HIGH
};

static const int list_subscreen_play[] = {
    STRING_DEFAULT,
    STRING_FULLDISPLAY
};

static const int list_date_format[] = {
    STRING_YYYY_MM_DD,
    STRING_MM_DD_YYYY,
    STRING_DD_MM_YYYY
};

INIT_ROLLER(date_format, list_date_format);
INIT_ROLLER(wifi_frequency, list_wifi_frequency);
INIT_ROLLER(frequency, list_frequency);
INIT_ROLLER(voice_volume, list_voice_volume);
INIT_ROLLER(subscreen_play, list_subscreen_play);
INIT_ROLLER(auto_dormant, list_auto_dormant);
INIT_ROLLER(auto_poweroff, list_auto_poweroff);
INIT_ROLLER(video_format, list_video_format);
INIT_ROLLER(language, list_language);


static void roller_value_changed_cb(lv_event_t* e) {
    lv_obj_t* roller = lv_event_get_target(e);
    settings_roller_t* target = (settings_roller_t*)lv_event_get_user_data(e);
    if (!target) return;
    int selected = lv_roller_get_selected(roller);

    if (target->state_label) {
        const char* text = lv_lang_string[target->ids[selected]][current_lang];
        lv_label_set_text(target->state_label, text);
    }

    cur_page = lv_menu_get_cur_main_page(menu);
}


static void switch_cont_click_cb(lv_event_t* e) {
    switch_item_t* item = (switch_item_t*)lv_event_get_user_data(e);
    if (!item || !item->switch_obj) return;

    bool current_state = lv_obj_has_state(item->switch_obj, LV_STATE_CHECKED);
    if (current_state) {
        lv_obj_remove_state(item->switch_obj, LV_STATE_CHECKED);
    } else {
        lv_obj_add_state(item->switch_obj, LV_STATE_CHECKED);
    }

    if (item->toggle_cb) {
        item->toggle_cb(!current_state);
    }
}

static void switch_value_changed_cb(lv_event_t* e) {
    lv_obj_t* sw = lv_event_get_target(e);
    switch_item_t* item = (switch_item_t*)lv_event_get_user_data(e);

    if (!item || !item->toggle_cb) return;

    bool state = lv_obj_has_state(sw, LV_STATE_CHECKED);
    item->toggle_cb(state);
}


static void switch_leds_toggle(bool state) {
    printf("LEDs: %s\n", state ? "ON" : "OFF");
}

static void switch_date_stamp_toggle(bool state) {
    printf("Date Stamp: %s\n", state ? "ON" : "OFF");
}

static void switch_stamp_toggle(bool state) {
    printf("Stamp: %s\n", state ? "ON" : "OFF");
}

static void switch_power_tone_toggle(bool state) {
    printf("Power Tone: %s\n", state ? "ON" : "OFF");
}

static void switch_key_tone_toggle(bool state) {
    printf("Key Tone: %s\n", state ? "ON" : "OFF");
}

static void switch_clap_tone_toggle(bool state) {
    printf("Clap Tone: %s\n", state ? "ON" : "OFF");
}

static void switch_grid_toggle(bool state) {
    printf("Grid: %s\n", state ? "ON" : "OFF");
    grid_active = state;
}

static void switch_quick_start_toggle(bool state) {
    printf("Quick-Start Switch: %s\n", state ? "ON" : "OFF");
}

static void switch_voice_control_toggle(bool state) {
    printf("Voice Control: %s\n", state ? "ON" : "OFF");
}

void switch_language(uint8_t new_lang);

static void roller_language(){
    current_lang = lv_roller_get_selected(language.roller);

    lv_obj_clean(scr_menu_settings);
    create_scr_menu_settings();
    settings_action();
}

static void set_heading_cb(lv_event_t* e) {
    //use "cur_page" as "last_page"
    if (cur_page && cur_page == page_sub_language) roller_language();

    //update cur page & headings accordingly
    cur_page = lv_menu_get_cur_main_page(menu);

    if (cur_page == page_main) {
        lv_label_set_text(heading, _(STRING_SET_INFO));
    } else if (cur_page == page_sub_wifi) {
        lv_label_set_text(heading, _(STRING_WIFI));
    } else if (cur_page == page_sub_wifi_frequency) {
        lv_label_set_text(heading, _(STRING_WIFI_FREQUENCY));
    } else if (cur_page == page_sub_wifi_connect) {
        lv_label_set_text(heading, _(STRING_WIFI_INFO));
    } else if (cur_page == page_sub_bluetooth) {
        lv_label_set_text(heading, _(STRING_BT_DEVICE));
    } else if (cur_page == page_sub_auto_dormant) {
        lv_label_set_text(heading, _(STRING_Auto_Dormant));
    } else if (cur_page == page_sub_auto_poweroff) {
        lv_label_set_text(heading, _(STRING_AUTO_OFF));
    } else if (cur_page == page_sub_language) {
        lv_label_set_text(heading, _(STRING_LANGUAGE));
    } else if (cur_page == page_sub_video_format) {
        lv_label_set_text(heading, _(STRING_VIDEO_STANDARD));
    } else if (cur_page == page_sub_frequency) {
        lv_label_set_text(heading, _(STRING_FREQ));
    } else if (cur_page == page_sub_voice_volume) {
        lv_label_set_text(heading, _(STRING_MIC_VOLUME));
    } else if (cur_page == page_sub_subscreen_play) {
        lv_label_set_text(heading, _(STRING_SUBDISPLAY));
    } else if (cur_page == page_sub_date_time) {
        lv_label_set_text(heading, _(STRING_DATE_TIME));
    } else if (cur_page == page_sub_date_time_date) {
        lv_label_set_text(heading, _(STRING_DATE_FORMAT));
    } else if (cur_page == page_sub_date_time_time) {
        lv_label_set_text(heading, _(STRING_TIME));
    } else if (cur_page == page_sub_date_time_date_format) {
        lv_label_set_text(heading, _(STRING_DATE_FORMAT));
    } else if (cur_page == page_sub_voice_control) {
        lv_label_set_text(heading, _(STRING_VOICE_CTR));
    } else if (cur_page == page_sub_voice_command) {
        lv_label_set_text(heading, _(STRING_VOICE_INFO));
    } else if (cur_page == page_sub_format_sd) {
        lv_label_set_text(heading, _(STRING_FORMAT_SD));
    } else if (cur_page == page_sub_factory_reset) {
        lv_label_set_text(heading, _(STRING_DEFAULT_SET));
    } else if (cur_page == page_sub_information) {
        lv_label_set_text(heading, _(STRING_INFO));
    } else {
        printf("page_else\n");
    }
}

static void settings_back_cb(lv_event_t* e) {
    if (lv_menu_back_button_is_root(menu, lv_event_get_target(e))) {
        open_scr_menu_cb();
    }
}

static void settings_quit_cb(lv_event_t* e) {
    open_scr_home_cb();
}

static void load_styles(void) {
    lv_style_init(&style_cont);
    lv_style_set_bg_color(&style_cont, BG_COLOR_DARK_GREY);
    lv_style_set_border_width(&style_cont, 0);
}

static lv_obj_t* create_basics(lv_obj_t* parent, const char* txt) {
    lv_obj_t* overall_cont = lv_menu_cont_create(parent);
    lv_obj_remove_flag(overall_cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(overall_cont, 810, 80);

    lv_obj_t* cont = lv_obj_create(overall_cont);
    lv_obj_remove_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(cont, &style_cont, LV_PART_MAIN);
    lv_obj_set_size(cont, 810, 75);

    lv_obj_t* label = lv_label_create(cont);
    lv_obj_add_style(label, &style_font_default_36, LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 30, 0);
    lv_label_set_text(label, txt);

    return cont;
}

static lv_obj_t* create_text(lv_obj_t* parent, const char* txt) {
    lv_obj_t* cont = create_basics(parent, txt);
    lv_obj_t* img = lv_image_create(cont);
    lv_image_set_src(img, &set_have_sub_menu);
    lv_obj_align(img, LV_ALIGN_RIGHT_MID, -30, 0);
    return cont;
}

static lv_obj_t* create_switch(lv_obj_t* parent, const char* txt, bool chk, void (*toggle_cb)(bool)) {
    lv_obj_t* cont = create_basics(parent, txt);
    lv_obj_t* sw = lv_switch_create(cont);
    lv_obj_set_size(sw, 100, 50);
    lv_obj_add_state(sw, chk ? LV_STATE_CHECKED : 0);
    lv_obj_align(sw, LV_ALIGN_RIGHT_MID, -40, 0);

    switch_item_t* item = (switch_item_t*)lv_malloc(sizeof(switch_item_t));
    if (item) {
        item->switch_obj = sw;
        item->cont_obj = cont;
        item->name = txt;
        item->toggle_cb = toggle_cb;
    }

    lv_obj_add_flag(cont, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(sw, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_add_event_cb(cont, switch_cont_click_cb, LV_EVENT_CLICKED, item);

    if (toggle_cb) {
        lv_obj_add_event_cb(sw, switch_value_changed_cb, LV_EVENT_VALUE_CHANGED, item);
    }

    if (toggle_cb == switch_grid_toggle) grid_switch = sw;

    return cont;
}

static lv_obj_t* create_scrollbar(lv_obj_t* parent, const char* txt, settings_roller_t* target) {
    lv_obj_t* page = create_basics(parent, txt);

    target->state_label = lv_label_create(page);
    lv_obj_add_style(target->state_label, &style_font_default_36, LV_PART_MAIN);
    lv_obj_align(target->state_label, LV_ALIGN_RIGHT_MID, -80, 0);

    if (target->ids && target->states_count > 0) {
        const char* text = lv_lang_string[target->ids[0]][current_lang];
        lv_label_set_text(target->state_label, text);
    }

    lv_obj_t* img = lv_image_create(page);
    lv_image_set_src(img, &set_have_sub_menu);
    lv_obj_align(img, LV_ALIGN_RIGHT_MID, -30, 0);

    return page;
}

static lv_obj_t* create_scr_scrollbar(lv_obj_t* parent, settings_roller_t* target) {
    char options[300] = "";

    for (int i = 0; i < target->states_count; i++) {
        strcat(options, lv_lang_string[target->ids[i]][current_lang]);
        if (i < (target->states_count - 1)) {
            strcat(options, "\n");
        }
    }

    target->roller = lv_roller_create(parent);
    lv_obj_set_size(target->roller, lv_pct(100), 250);
    lv_obj_align(target->roller, LV_ALIGN_CENTER, 0, 0);
    lv_roller_set_options(target->roller, options, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_selected(target->roller, 0, LV_ANIM_OFF);

    lv_obj_set_style_bg_opa(target->roller, LV_OPA_0, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(target->roller, LV_OPA_0, LV_PART_SELECTED);
    lv_obj_set_style_border_width(target->roller, 0, LV_PART_MAIN);
    lv_obj_add_style(target->roller, &style_font_default_36, LV_PART_MAIN);
    lv_obj_set_style_text_color(target->roller, lv_palette_main(LV_PALETTE_BLUE), LV_PART_SELECTED);
    lv_obj_set_style_text_line_space(target->roller, 30, LV_PART_MAIN);

    lv_obj_add_event_cb(target->roller, roller_value_changed_cb, LV_EVENT_VALUE_CHANGED, target);
    return parent;
}

void settings_action(void) {
    lv_menu_clear_history(menu);
    lv_menu_set_page(menu, page_main);
    if (grid_switch) grid_active ?  lv_obj_add_state(grid_switch, LV_STATE_CHECKED) : lv_obj_remove_state(grid_switch, LV_STATE_CHECKED);
    lv_roller_set_selected(language.roller, current_lang, LV_ANIM_OFF);
    lv_label_set_text(language.state_label, lv_lang_string[language.ids[lv_roller_get_selected(language.roller)]][current_lang]);
    lv_screen_load(scr_menu_settings);
}

void settings_search_clicked_cb(lv_event_t* e) {
    printf("searching settings\n");
}

void bluetooth_action(void) {
    printf("Bluetooth toggle\n");
    lv_screen_load(scr_menu_bluetooth);
}

void bluetooth_search_clicked_cb(lv_event_t* e) {
    printf("searching bluetooth\n");
}

void create_scr_menu_settings(void) {
    scr_menu_settings = lv_obj_create(NULL);
    lv_obj_t* temp_page;

    menu = lv_menu_create(scr_menu_settings);
    lv_obj_add_style(menu, &style_font_default_36, LV_PART_MAIN);
    lv_obj_set_style_bg_color(menu, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);
    lv_obj_set_size(menu, lv_pct(100), lv_pct(100));
    lv_obj_center(menu);

    load_styles();

    // main page
    page_main = lv_menu_page_create(menu, "");
    lv_obj_add_style(page_main, &style_scrollbar, LV_PART_SCROLLBAR);
    lv_obj_set_scroll_dir(page_main, LV_DIR_VER);

    // wifi page
    CREATE_SIMPLE_PAGE(page_main, page_sub_wifi,  _(STRING_WIFI));

    page_sub_wifi_connect = lv_menu_page_create(menu, "");
    lv_obj_set_scroll_dir(page_sub_wifi_connect, LV_DIR_VER);
    lv_obj_t* scr_wifi_connect_btn = lv_obj_create(page_sub_wifi_connect);
    lv_obj_set_style_bg_color(scr_wifi_connect_btn, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);
    temp_page = create_switch(page_sub_wifi,  _(STRING_WIFI_INFO), false, NULL);
    lv_menu_set_load_page_event(menu, temp_page, page_sub_wifi_connect);

    CREATE_ROLLER_PAGE(page_sub_wifi, page_sub_wifi_frequency, &wifi_frequency, _(STRING_WIFI_FREQUENCY));

    // bluetooth page
    page_sub_bluetooth = lv_menu_page_create(menu, "");
    lv_obj_set_scroll_dir(page_sub_bluetooth, LV_DIR_VER);
    lv_obj_t* label = lv_label_create(page_sub_bluetooth);
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_width(label, 630);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(label, _(STRING_TX_DISCONNECT_TIP));
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    temp_page = create_text(page_main, _(STRING_BT_DEVICE));
    lv_menu_set_load_page_event(menu, temp_page, page_sub_bluetooth);

    // pages
    CREATE_ROLLER_PAGE(page_main, page_sub_auto_dormant, &auto_dormant,  _(STRING_Auto_Dormant));
    CREATE_ROLLER_PAGE(page_main, page_sub_auto_poweroff, &auto_poweroff,  _(STRING_AUTO_OFF));
    CREATE_ROLLER_PAGE(page_main, page_sub_language, &language, _(STRING_LANGUAGE));
    CREATE_ROLLER_PAGE(page_main, page_sub_video_format, &video_format, _(STRING_VIDEO_STANDARD));
    CREATE_ROLLER_PAGE(page_main, page_sub_frequency, &frequency,  _(STRING_FREQ));
    CREATE_ROLLER_PAGE(page_main, page_sub_voice_volume, &voice_volume, _(STRING_MIC_VOLUME));
    CREATE_ROLLER_PAGE(page_main, page_sub_subscreen_play, &subscreen_play, _(STRING_SUBDISPLAY));

    // switch pages
    create_switch(page_main, _(STRING_LED), false, switch_leds_toggle);
    create_switch(page_main, _(STRING_DATE_STAMP), false, switch_date_stamp_toggle);
    create_switch(page_main, _(STRING_BRAND_STAMP), false, switch_stamp_toggle);
    create_switch(page_main, _(STRING_BOOT_SOUND), false, switch_power_tone_toggle);
    create_switch(page_main, _(STRING_KEY_TONE), false, switch_key_tone_toggle);
    create_switch(page_main, _(STRING_CAP_TONE), false, switch_clap_tone_toggle);
    create_switch(page_main, _(STRING_GRID), false, switch_grid_toggle);
    create_switch(page_main, _(STRING_QUICK_START_SWITCH), false, switch_quick_start_toggle);

    // date time pages
    CREATE_SIMPLE_PAGE(page_main, page_sub_date_time,  _(STRING_DATE_TIME));
    CREATE_SIMPLE_PAGE(page_sub_date_time, page_sub_date_time_date,_(STRING_DATE_FORMAT));
    CREATE_SIMPLE_PAGE(page_sub_date_time, page_sub_date_time_time, _(STRING_TIME));
    CREATE_ROLLER_PAGE(page_sub_date_time, page_sub_date_time_date_format, &date_format, "Date Format");

    // voice page
    page_sub_voice_control = lv_menu_page_create(menu, "");
    lv_obj_set_scroll_dir(page_sub_voice_control, LV_DIR_VER);
    temp_page = create_text(page_main, _(STRING_VOICE_CTR));
    lv_menu_set_load_page_event(menu, temp_page, page_sub_voice_control);

    create_switch(page_sub_voice_control, _(STRING_VOICE_CTR), false, switch_voice_control_toggle);

    page_sub_voice_command = lv_menu_page_create(menu, "");
    lv_obj_set_scroll_dir(page_sub_voice_command, LV_DIR_VER);
    label = lv_label_create(page_sub_voice_command);
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_width(label, 630);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(label, "command             command \ncommand             command \ncommand             command \ncommand             command \n");
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    temp_page = create_text(page_sub_voice_control,  _(STRING_VOICE_INFO));
    lv_menu_set_load_page_event(menu, temp_page, page_sub_voice_command);

    // other pages
    CREATE_SIMPLE_PAGE(page_main, page_sub_format_sd, _(STRING_FORMAT_SD));
    CREATE_SIMPLE_PAGE(page_main, page_sub_factory_reset, _(STRING_DEFAULT_SET));

    page_sub_information = lv_menu_page_create(menu, "");
    lv_obj_set_scroll_dir(page_sub_information, LV_DIR_VER);
    label = lv_label_create(page_sub_information);
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(label, "blablablabla\nblablabla");
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 0, 0);
    temp_page = create_text(page_main, _(STRING_INFO));
    lv_menu_set_load_page_event(menu, temp_page, page_sub_information);

    lv_obj_add_event_cb(menu, set_heading_cb, LV_EVENT_VALUE_CHANGED, lv_menu_get_cur_main_page(menu));

    //headers
    lv_obj_t* back_btn = lv_menu_get_main_header_back_button(menu);

    lv_obj_t* back_icon = lv_obj_get_child(back_btn, 0);
    lv_image_set_src(back_icon, &Pattern_Return);
    lv_obj_align(lv_menu_get_main_header(menu), LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_add_event_cb(back_btn, settings_back_cb, LV_EVENT_CLICKED, lv_menu_get_cur_main_page(menu));

    lv_obj_t* heading_cont = lv_obj_create(lv_menu_get_main_header(menu));
    lv_obj_set_size(heading_cont, lv_pct(65), 60);
    lv_obj_set_style_bg_color(heading_cont, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);
    lv_obj_set_style_border_width(heading_cont, 0, LV_PART_MAIN);
    lv_obj_remove_flag(heading_cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_flag(lv_menu_get_main_header(menu), LV_OBJ_FLAG_SCROLLABLE);

    heading = lv_label_create(heading_cont);
    lv_obj_add_style(heading, &style_font_default_36, LV_PART_MAIN);
    lv_obj_center(heading);

    lv_obj_t* exit_btn = lv_menu_get_main_header(menu);
    lv_obj_t* exit_icon = lv_image_create(exit_btn);
    lv_image_set_src(exit_icon, &system_set_exit);
    lv_obj_align(exit_icon, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_add_flag(exit_icon, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(exit_icon, settings_quit_cb, LV_EVENT_CLICKED, NULL);

    lv_menu_set_mode_root_back_button(menu, LV_MENU_ROOT_BACK_BUTTON_ENABLED);
}

// Bluetooth界面
void create_scr_menu_bluetooth(void) {
    scr_menu_bluetooth = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_menu_bluetooth, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);

    lv_obj_t* exit_icon = create_exit_icon(scr_menu_bluetooth);
    lv_obj_remove_event_cb(exit_icon, open_scr_home_cb);
    lv_obj_add_event_cb(exit_icon, open_scr_menu_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t* search_icon = create_search_icon(scr_menu_bluetooth);
    lv_obj_add_event_cb(search_icon, bluetooth_search_clicked_cb, LV_EVENT_CLICKED, NULL);

    create_label_top_center(scr_menu_bluetooth, _(STRING_BT_DEVICE));
}

void switch_language(uint8_t new_lang) {
//    if (new_lang > 8) return;



 //   if (heading && cur_page) {
 //       set_heading_cb(NULL);
 //   }

    // 刷新所有 roller
refresh_all_rollers();
}

void refresh_all_rollers(void) {
    char options[300];

    // 刷新 auto_poweroff
    if (auto_poweroff.roller) {
        options[0] = '\0';
        for (int i = 0; i < auto_poweroff.states_count; i++) {
            strcat(options, lv_lang_string[auto_poweroff.ids[i]][current_lang]);
            if (i < auto_poweroff.states_count - 1) strcat(options, "\n");
        }
        lv_roller_set_options(auto_poweroff.roller, options, LV_ROLLER_MODE_NORMAL);
        int selected = lv_roller_get_selected(auto_poweroff.roller);
        if (auto_poweroff.state_label) {
            lv_label_set_text(auto_poweroff.state_label,
                lv_lang_string[auto_poweroff.ids[selected]][current_lang]);
        }
    }

    // 刷新 auto_dormant
    if (auto_dormant.roller) {
        options[0] = '\0';
        for (int i = 0; i < auto_dormant.states_count; i++) {
            strcat(options, lv_lang_string[auto_dormant.ids[i]][current_lang]);
            if (i < auto_dormant.states_count - 1) strcat(options, "\n");
        }
        lv_roller_set_options(auto_dormant.roller, options, LV_ROLLER_MODE_NORMAL);
        int selected = lv_roller_get_selected(auto_dormant.roller);
        if (auto_dormant.state_label) {
            lv_label_set_text(auto_dormant.state_label,
                lv_lang_string[auto_dormant.ids[selected]][current_lang]);
        }
    }

    // 刷新 language
    if (language.roller) {
        options[0] = '\0';
        for (int i = 0; i < language.states_count; i++) {
            strcat(options, lv_lang_string[language.ids[i]][current_lang]);
            if (i < language.states_count - 1) strcat(options, "\n");
        }
        lv_roller_set_options(language.roller, options, LV_ROLLER_MODE_NORMAL);
        int selected = lv_roller_get_selected(language.roller);
        if (language.state_label) {
            lv_label_set_text(language.state_label,
                lv_lang_string[language.ids[selected]][current_lang]);
        }
    }

    // 刷新 video_format
    if (video_format.roller) {
        options[0] = '\0';
        for (int i = 0; i < video_format.states_count; i++) {
            strcat(options, lv_lang_string[video_format.ids[i]][current_lang]);
            if (i < video_format.states_count - 1) strcat(options, "\n");
        }
        lv_roller_set_options(video_format.roller, options, LV_ROLLER_MODE_NORMAL);
        int selected = lv_roller_get_selected(video_format.roller);
        if (video_format.state_label) {
            lv_label_set_text(video_format.state_label,
                lv_lang_string[video_format.ids[selected]][current_lang]);
        }
    }

    // 刷新 frequency
    if (frequency.roller) {
        options[0] = '\0';
        for (int i = 0; i < frequency.states_count; i++) {
            strcat(options, lv_lang_string[frequency.ids[i]][current_lang]);
            if (i < frequency.states_count - 1) strcat(options, "\n");
        }
        lv_roller_set_options(frequency.roller, options, LV_ROLLER_MODE_NORMAL);
        int selected = lv_roller_get_selected(frequency.roller);
        if (frequency.state_label) {
            lv_label_set_text(frequency.state_label,
                lv_lang_string[frequency.ids[selected]][current_lang]);
        }
    }

    // 刷新 voice_volume
    if (voice_volume.roller) {
        options[0] = '\0';
        for (int i = 0; i < voice_volume.states_count; i++) {
            strcat(options, lv_lang_string[voice_volume.ids[i]][current_lang]);
            if (i < voice_volume.states_count - 1) strcat(options, "\n");
        }
        lv_roller_set_options(voice_volume.roller, options, LV_ROLLER_MODE_NORMAL);
        int selected = lv_roller_get_selected(voice_volume.roller);
        if (voice_volume.state_label) {
            lv_label_set_text(voice_volume.state_label,
                lv_lang_string[voice_volume.ids[selected]][current_lang]);
        }
    }

    // 刷新 subscreen_play
    if (subscreen_play.roller) {
        options[0] = '\0';
        for (int i = 0; i < subscreen_play.states_count; i++) {
            strcat(options, lv_lang_string[subscreen_play.ids[i]][current_lang]);
            if (i < subscreen_play.states_count - 1) strcat(options, "\n");
        }
        lv_roller_set_options(subscreen_play.roller, options, LV_ROLLER_MODE_NORMAL);
        int selected = lv_roller_get_selected(subscreen_play.roller);
        if (subscreen_play.state_label) {
            lv_label_set_text(subscreen_play.state_label,
                lv_lang_string[subscreen_play.ids[selected]][current_lang]);
        }
    }

    // 刷新 wifi_frequency
    if (wifi_frequency.roller) {
        options[0] = '\0';
        for (int i = 0; i < wifi_frequency.states_count; i++) {
            strcat(options, lv_lang_string[wifi_frequency.ids[i]][current_lang]);
            if (i < wifi_frequency.states_count - 1) strcat(options, "\n");
        }
        lv_roller_set_options(wifi_frequency.roller, options, LV_ROLLER_MODE_NORMAL);
        int selected = lv_roller_get_selected(wifi_frequency.roller);
        if (wifi_frequency.state_label) {
            lv_label_set_text(wifi_frequency.state_label,
                lv_lang_string[wifi_frequency.ids[selected]][current_lang]);
        }
    }

    // 刷新 date_format
    if (date_format.roller) {
        options[0] = '\0';
        for (int i = 0; i < date_format.states_count; i++) {
            strcat(options, lv_lang_string[date_format.ids[i]][current_lang]);
            if (i < date_format.states_count - 1) strcat(options, "\n");
        }
        lv_roller_set_options(date_format.roller, options, LV_ROLLER_MODE_NORMAL);
        int selected = lv_roller_get_selected(date_format.roller);
        if (date_format.state_label) {
            lv_label_set_text(date_format.state_label,
                lv_lang_string[date_format.ids[selected]][current_lang]);
        }
    }
}
