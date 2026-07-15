#include "my_demo_2.h"

#define INIT_ROLLER(name, list_array) \
    static settings_roller_t name = { \
        .states = list_array, \
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



static const char* list_wifi_frequency[] = { "Red", "Green", "Blue", "Yellow", "Purple", "Orange" };
static const char* list_auto_poweroff[] = { "OFF", "1min", "3min", "5min" };
static const char* list_auto_dormant[] = { "OFF", "10Sec", "20Sec", "30Sec", "60Sec" };
static const char* list_language[] = { "English", "Simplified Chinese", "Traditional Chinese"};
static const char* list_video_format[] = { "PAL", "NTSC" };
static const char* list_frequency[] = { "50Hz", "60Hz" };
static const char* list_voice_volume[] = { "Default", "High" };
//static const char* list_voice_volume[] = {GET_LANG_STR(STRING_DEFAULT),  GET_LANG_STR(STRING_HIGH};
static const char* list_subscreen_play[] = { "Default", "Full Display" };
static const char* list_date_format[] = { "YYYY-MM-DD", "MM-DD-YYYY", "DD-MM-YYYY" };


typedef struct {
    lv_obj_t* page;
    const char* title;
    void (*action_cb)(void);
} page_map_t;

INIT_ROLLER(date_format, list_date_format);
INIT_ROLLER(wifi_frequency, list_wifi_frequency);
INIT_ROLLER(frequency, list_frequency);
INIT_ROLLER(voice_volume, list_voice_volume);
INIT_ROLLER(subscreen_play, list_subscreen_play);
INIT_ROLLER(auto_dormant, list_auto_dormant);
INIT_ROLLER(auto_poweroff, list_auto_poweroff);
INIT_ROLLER(video_format, list_video_format);
INIT_ROLLER(language, list_language);

static page_map_t* page_map = NULL;
static int page_map_size = 0;

static void init_page_map(void) {
    static page_map_t map[] = {
        {NULL, "Settings", NULL},
        {NULL, "WiFi", NULL},
        {NULL, "WiFi Frequency", NULL},
        {NULL, "WiFi Connect", wifi_action},
        {NULL, "Bluetooth Device", NULL},
        {NULL, "Auto Dormant", NULL},
        {NULL, "Auto Power Off", NULL},
        {NULL, "Language", NULL},
        {NULL, "Video Format", NULL},
        {NULL, "Frequency", NULL},
        {NULL, "Voice Volume", NULL},
        {NULL, "Subscreen Play", NULL},
        {NULL, "Date Time", NULL},
        {NULL, "Date", NULL},
        {NULL, "Time", NULL},
        {NULL, "Date Format", NULL},
        {NULL, "Voice Control", NULL},
        {NULL, "Command List", NULL},
        {NULL, "Format SD", NULL},
        {NULL, "Factory Reset", NULL},
        {NULL, "Information", NULL},
    };

    map[0].page = page_main;
    map[1].page = page_sub_wifi;
    map[2].page = page_sub_wifi_frequency;
    map[3].page = page_sub_wifi_connect;
    map[4].page = page_sub_bluetooth;
    map[5].page = page_sub_auto_dormant;
    map[6].page = page_sub_auto_poweroff;
    map[7].page = page_sub_language;
    map[8].page = page_sub_video_format;
    map[9].page = page_sub_frequency;
    map[10].page = page_sub_voice_volume;
    map[11].page = page_sub_subscreen_play;
    map[12].page = page_sub_date_time;
    map[13].page = page_sub_date_time_date;
    map[14].page = page_sub_date_time_time;
    map[15].page = page_sub_date_time_date_format;
    map[16].page = page_sub_voice_control;
    map[17].page = page_sub_voice_command;
    map[18].page = page_sub_format_sd;
    map[19].page = page_sub_factory_reset;
    map[20].page = page_sub_information;

    page_map = map;
    page_map_size = sizeof(map) / sizeof(map[0]);
}

static const char* get_current_settings_state(settings_roller_t* target) {
    if (!target || !target->roller || !target->states) return NULL;
    int selected = lv_roller_get_selected(target->roller);
    if (selected < 0 || selected >= target->states_count) return NULL;
    return target->states[selected];
}

/**CB**/
static void roller_value_changed_cb(lv_event_t* e) {
    lv_obj_t* roller = lv_event_get_target(e);
    settings_roller_t* target = (settings_roller_t*)lv_event_get_user_data(e);
    if (!target) return;

    int selected = lv_roller_get_selected(roller);
    if (selected < 0 || selected >= target->states_count) return;

    if (target->state_label) {
        lv_label_set_text(target->state_label, target->states[selected]);
    }
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


static void set_heading_cb(lv_event_t* e) {
    cur_page = lv_menu_get_cur_main_page(menu);
    if (page_map) {
        for (int i = 0; i < page_map_size; i++) {
            if (cur_page == page_map[i].page) {
                lv_label_set_text(heading, page_map[i].title);
                if (page_map[i].action_cb) {
                    page_map[i].action_cb();
                }
                return;
            }
        }
    }
    printf("page_else\n");
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

// Switch toggle callback implementations
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

/**creations**/
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

    if (strcmp(txt, "Grid") == 0) grid_switch = sw;

    return cont;
}

static lv_obj_t* create_scrollbar(lv_obj_t* parent, const char* txt, settings_roller_t* target) {
    lv_obj_t* page = create_basics(parent, txt);

    target->state_label = lv_label_create(page);
    lv_obj_add_style(target->state_label, &style_font_default_36, LV_PART_MAIN);
    lv_obj_align(target->state_label, LV_ALIGN_RIGHT_MID, -80, 0);
    lv_label_set_text(target->state_label, target->states[0]);

    lv_obj_t* img = lv_image_create(page);
    lv_image_set_src(img, &set_have_sub_menu);
    lv_obj_align(img, LV_ALIGN_RIGHT_MID, -30, 0);

    return page;
}

static lv_obj_t* create_scr_scrollbar(lv_obj_t* parent, settings_roller_t* target) {
    char options[300] = "";
    for (int i = 0; i < target->states_count; i++) {
        strcat(options, target->states[i]);
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

void aaaa(void){
    lv_coord_t current_y = lv_obj_get_scroll_y(lv_menu_get_cur_main_page(menu));
    printf("Current scroll Y: %d\n", current_y);
}

void settings_action(void) {
    lv_menu_clear_history(menu);
    lv_menu_set_page(menu, page_main);
    grid_active ?  lv_obj_add_state(grid_switch, LV_STATE_CHECKED) : lv_obj_remove_state(grid_switch, LV_STATE_CHECKED);
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

    // create menu
    menu = lv_menu_create(scr_menu_settings);
    lv_obj_add_style(menu, &style_font_default_36, LV_PART_MAIN);
    lv_obj_set_style_bg_color(menu, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);
    lv_obj_set_size(menu, lv_pct(100), lv_pct(100));
    lv_obj_center(menu);

    // load styles
    load_styles();

    // main page
    page_main = lv_menu_page_create(menu, "");
    lv_obj_add_style(page_main, &style_scrollbar, LV_PART_SCROLLBAR);
    lv_obj_set_scroll_dir(page_main, LV_DIR_VER);

    // wifi page
    CREATE_SIMPLE_PAGE(page_main, page_sub_wifi, "WiFi");

    page_sub_wifi_connect = lv_menu_page_create(menu, "");
    lv_obj_set_scroll_dir(page_sub_wifi_connect, LV_DIR_VER);
    lv_obj_t* scr_wifi_connect_btn = lv_obj_create(page_sub_wifi_connect);
    lv_obj_set_style_bg_color(scr_wifi_connect_btn, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);
    temp_page = create_switch(page_sub_wifi, "Wifi connect", false, NULL);
    lv_menu_set_load_page_event(menu, temp_page, page_sub_wifi_connect);

    CREATE_ROLLER_PAGE(page_sub_wifi, page_sub_wifi_frequency, &wifi_frequency, "Wifi frequency");

    // bluetooth page
    page_sub_bluetooth = lv_menu_page_create(menu, "");
    lv_obj_set_scroll_dir(page_sub_bluetooth, LV_DIR_VER);
    lv_obj_t* label = lv_label_create(page_sub_bluetooth);
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_width(label, 630);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(label, "\nnot connected to a device. Please ensure the Bluetooth microphone is turned on.");
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    temp_page = create_text(page_main, "Bluetooth Device");
    lv_menu_set_load_page_event(menu, temp_page, page_sub_bluetooth);

    //pages
    CREATE_ROLLER_PAGE(page_main, page_sub_auto_dormant, &auto_dormant, "Auto Dormant");
    CREATE_ROLLER_PAGE(page_main, page_sub_auto_poweroff, &auto_poweroff, "Auto Poweroff");
    CREATE_ROLLER_PAGE(page_main, page_sub_language, &language, "Language");
    CREATE_ROLLER_PAGE(page_main, page_sub_video_format, &video_format, "Video Format");
    CREATE_ROLLER_PAGE(page_main, page_sub_frequency, &frequency, "Frequency");
    CREATE_ROLLER_PAGE(page_main, page_sub_voice_volume, &voice_volume, "Voice Volume");
    CREATE_ROLLER_PAGE(page_main, page_sub_subscreen_play, &subscreen_play, "SubScreen Play");

    // switch pages

    create_switch(page_main, "LEDs", false, switch_leds_toggle);
    create_switch(page_main, "Date Stamp", false, switch_date_stamp_toggle);
    create_switch(page_main, "Stamp", false, switch_stamp_toggle);
    create_switch(page_main, "Power Tone", false, switch_power_tone_toggle);
    create_switch(page_main, "Key Tone", false, switch_key_tone_toggle);
    create_switch(page_main, "Clap Tone", false, switch_clap_tone_toggle);
    create_switch(page_main, "Grid", false, switch_grid_toggle);
    create_switch(page_main, "Quick-Start Switch", false, switch_quick_start_toggle);


    // date time pages
    CREATE_SIMPLE_PAGE(page_main, page_sub_date_time, "Date Time");
    CREATE_SIMPLE_PAGE(page_sub_date_time, page_sub_date_time_date, "Date");
    CREATE_SIMPLE_PAGE(page_sub_date_time, page_sub_date_time_time, "Time");
    CREATE_ROLLER_PAGE(page_sub_date_time, page_sub_date_time_date_format, &date_format, "Date Format");

    // voice page
    page_sub_voice_control = lv_menu_page_create(menu, "");
    lv_obj_set_scroll_dir(page_sub_voice_control, LV_DIR_VER);
    temp_page = create_text(page_main, "Voice Control");
    lv_menu_set_load_page_event(menu, temp_page, page_sub_voice_control);

    create_switch(page_sub_voice_control, "Voice Control", false, switch_voice_control_toggle);

    page_sub_voice_command = lv_menu_page_create(menu, "");
    lv_obj_set_scroll_dir(page_sub_voice_command, LV_DIR_VER);
    label = lv_label_create(page_sub_voice_command);
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_width(label, 630);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(label, "command             command \ncommand             command \ncommand             command \ncommand             command \n");
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    temp_page = create_text(page_sub_voice_control, "Command List");
    lv_menu_set_load_page_event(menu, temp_page, page_sub_voice_command);

    // ---- other pags ----
    CREATE_SIMPLE_PAGE(page_main, page_sub_format_sd, "Format SD");
    CREATE_SIMPLE_PAGE(page_main, page_sub_factory_reset, "Factory Reset");

    page_sub_information = lv_menu_page_create(menu, "");
    lv_obj_set_scroll_dir(page_sub_information, LV_DIR_VER);
    label = lv_label_create(page_sub_information);
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(label, "blablablabla\nblablabla");
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 0, 0);
    temp_page = create_text(page_main, "Information");
    lv_menu_set_load_page_event(menu, temp_page,  page_sub_information);

    init_page_map();

    lv_obj_add_event_cb(menu, set_heading_cb, LV_EVENT_VALUE_CHANGED, lv_menu_get_cur_main_page(menu));

    /**headers**/
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

/**BLUETOOT**/
void create_scr_menu_bluetooth(void) {
    scr_menu_bluetooth = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_menu_bluetooth, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);

    lv_obj_t* exit_icon = create_exit_icon(scr_menu_bluetooth);
    lv_obj_remove_event_cb(exit_icon, open_scr_home_cb);
    lv_obj_add_event_cb(exit_icon, open_scr_menu_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t* search_icon = create_search_icon(scr_menu_bluetooth);
    lv_obj_add_event_cb(search_icon, bluetooth_search_clicked_cb, LV_EVENT_CLICKED, NULL);

    create_label_top_center(scr_menu_bluetooth, "Bluetooth");
}

// functional
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

    printf("%s\n", result);
    return result;
}
