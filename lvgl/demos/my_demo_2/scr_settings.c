#include "my_demo_2.h"
#include "lvgl_ex_language_string.h"

#define EMPTY_STRING ""
#define TEXT_WRAP_WIDTH 630
#define ROLLER_OPTIONS_BUF_SIZE 512

/* 修复1: 标准 C 数组大小宏 */
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

/* ============================================================================
 * 外部符号声明（编译器不知道这些定义在其他文件）
 * ============================================================================ */

extern uint8_t current_lang;
extern bool grid_active;

/* 外部样式 */
extern lv_style_t style_font_default_36;
extern lv_style_t style_font_default_30;
extern lv_style_t style_scrollbar;
extern lv_style_t style_cont_transparent;

/* 外部图片资源（LVGL 图片描述符是 const） */
extern const lv_img_dsc_t set_have_sub_menu;
extern const lv_img_dsc_t Pattern_Return;
extern const lv_img_dsc_t system_set_exit;

/* 外部辅助函数（注意原代码拼写是 cancle） */
extern lv_obj_t *create_btn_cancle(lv_obj_t *parent);
extern lv_obj_t *create_btn_confirm(lv_obj_t *parent);

/* 外部函数签名是 void(void)，但 LVGL 事件需要 void(lv_event_t*) */
static void open_scr_poweroff_cb_wrapper(lv_event_t *e)
{
    (void)e;
    open_scr_poweroff_cb();
}

static void open_scr_menu_cb_wrapper(lv_event_t *e)
{
    (void)e;
    open_scr_menu_cb();
}

/* ============================================================================
 * 枚举
 * ============================================================================ */

typedef enum {
    ROLLER_ID_DATE_FORMAT = 0,
    ROLLER_ID_WIFI_FREQUENCY,
    ROLLER_ID_FREQUENCY,
    ROLLER_ID_VOICE_VOLUME,
    ROLLER_ID_SUBSCREEN_PLAY,
    ROLLER_ID_AUTO_DORMANT,
    ROLLER_ID_AUTO_POWEROFF,
    ROLLER_ID_VIDEO_FORMAT,
    ROLLER_ID_LANGUAGE,
    ROLLER_ID_COUNT
} roller_id_t;

typedef enum {
    SWITCH_ID_LEDS = 0,
    SWITCH_ID_DATE_STAMP,
    SWITCH_ID_BRAND_STAMP,
    SWITCH_ID_POWER_TONE,
    SWITCH_ID_KEY_TONE,
    SWITCH_ID_CLAP_TONE,
    SWITCH_ID_GRID,
    SWITCH_ID_QUICK_START,
    SWITCH_ID_VOICE_CONTROL,
    SWITCH_ID_COUNT
} switch_id_t;

typedef enum {
    PAGE_ID_MAIN = 0,
    PAGE_ID_WIFI,
    PAGE_ID_WIFI_FREQUENCY,
    PAGE_ID_WIFI_CONNECT,
    PAGE_ID_BLUETOOTH,
    PAGE_ID_AUTO_DORMANT,
    PAGE_ID_AUTO_POWEROFF,
    PAGE_ID_LANGUAGE,
    PAGE_ID_VIDEO_FORMAT,
    PAGE_ID_FREQUENCY,
    PAGE_ID_VOICE_VOLUME,
    PAGE_ID_SUBSCREEN_PLAY,
    PAGE_ID_DATE_TIME,
    PAGE_ID_DATE_TIME_DATE,
    PAGE_ID_DATE_TIME_TIME,
    PAGE_ID_DATE_TIME_DATE_FORMAT,
    PAGE_ID_VOICE_CONTROL,
    PAGE_ID_VOICE_COMMAND,
    PAGE_ID_FORMAT_SD,
    PAGE_ID_FACTORY_RESET,
    PAGE_ID_INFORMATION,
    PAGE_ID_COUNT
} page_id_t;

/* ============================================================================
 * 结构体
 * ============================================================================ */

typedef struct {
    int id;
    int name_str_id;
    lv_obj_t *page_obj;
    lv_obj_t *menu_cont;
    bool has_sub_menu;
} page_item_t;

typedef struct {
    int id;
    int name_str_id;
    const int *states;
    int states_count;
    lv_obj_t *roller_obj;
    lv_obj_t *state_label;
    void (*on_change)(int selected);
} roller_item_t;

typedef struct {
    int id;
    int name_str_id;
    lv_obj_t *switch_obj;
    lv_obj_t *cont_obj;
    bool state;
    void (*toggle_cb)(bool state);
} switch_item_t;

typedef struct {
    lv_obj_t *menu;
    lv_obj_t *current_page;
    lv_obj_t *heading;
    lv_obj_t *scr;

    page_item_t   pages[PAGE_ID_COUNT];
    roller_item_t rollers[ROLLER_ID_COUNT];
    switch_item_t switches[SWITCH_ID_COUNT];
} menu_manager_t;

/* ============================================================================
 * 数据表
 * ============================================================================ */

static const int list_wifi_frequency[] = {
    STRING_LOW, STRING_MEDIUM, STRING_HIGH
};
static const int list_auto_poweroff[] = {
    STRING_OFF, STRING_1MIN, STRING_3MIN, STRING_5MIN
};
static const int list_auto_dormant[] = {
    STRING_OFF, STRING_10S, STRING_15S, STRING_30S, STRING_60S
};
static const int list_language[] = {
    STRING_ENGLISH, STRING_ja, STRING_CHINESE, STRING_zh_HK,
    STRING_fr, STRING_es, STRING_de, STRING_it
};
static const int list_video_format[] = {
    STRING_PAL, STRING_NTSC
};
static const int list_frequency[] = {
    STRING_50HZ, STRING_60HZ
};
static const int list_voice_volume[] = {
    STRING_DEFAULT, STRING_HIGH
};
static const int list_subscreen_play[] = {
    STRING_DEFAULT, STRING_FULLDISPLAY
};
static const int list_date_format[] = {
    STRING_YYYY_MM_DD, STRING_MM_DD_YYYY, STRING_DD_MM_YYYY
};

/* 页面配置表 */
static const struct {
    page_id_t id;
    int       name_str_id;
} page_configs[PAGE_ID_COUNT] = {
    {PAGE_ID_MAIN,                  STRING_SET_INFO},
    {PAGE_ID_WIFI,                  STRING_WIFI},
    {PAGE_ID_WIFI_FREQUENCY,        STRING_WIFI_FREQUENCY},
    {PAGE_ID_WIFI_CONNECT,          STRING_WIFI_INFO},
    {PAGE_ID_BLUETOOTH,             STRING_BT_DEVICE},
    {PAGE_ID_AUTO_DORMANT,          STRING_Auto_Dormant},
    {PAGE_ID_AUTO_POWEROFF,         STRING_AUTO_OFF},
    {PAGE_ID_LANGUAGE,              STRING_LANGUAGE},
    {PAGE_ID_VIDEO_FORMAT,          STRING_VIDEO_STANDARD},
    {PAGE_ID_FREQUENCY,             STRING_FREQ},
    {PAGE_ID_VOICE_VOLUME,          STRING_MIC_VOLUME},
    {PAGE_ID_SUBSCREEN_PLAY,        STRING_SUBDISPLAY},
    {PAGE_ID_DATE_TIME,             STRING_DATE_TIME},
    {PAGE_ID_DATE_TIME_DATE,        STRING_DATE_FORMAT},
    {PAGE_ID_DATE_TIME_TIME,        STRING_TIME},
    {PAGE_ID_DATE_TIME_DATE_FORMAT, STRING_DATE_FORMAT},
    {PAGE_ID_VOICE_CONTROL,         STRING_VOICE_CTR},
    {PAGE_ID_VOICE_COMMAND,         STRING_VOICE_INFO},
    {PAGE_ID_FORMAT_SD,             STRING_FORMAT_SD},
    {PAGE_ID_FACTORY_RESET,         STRING_DEFAULT_SET},
    {PAGE_ID_INFORMATION,           STRING_INFO},
};

/* ============================================================================
 * 修复2: 回调函数前向声明（配置表在函数定义之前引用它们）
 * ============================================================================ */

static void roller_language_cb(int selected);
static void video_format_cb(int selected);
static void set_heading_cb(lv_event_t *e);

static void switch_leds_toggle(bool state);
static void switch_date_stamp_toggle(bool state);
static void switch_brand_stamp_toggle(bool state);  /* 修复5: 统一名称 */
static void switch_power_tone_toggle(bool state);
static void switch_key_tone_toggle(bool state);
static void switch_clap_tone_toggle(bool state);
static void switch_grid_toggle(bool state);
static void switch_quick_start_toggle(bool state);
static void switch_voice_control_toggle(bool state);

/* roller 配置表：修复1 LV_ARRAYSIZE → ARRAY_SIZE */
static const struct {
    roller_id_t id;
    int         name_str_id;
    const int  *states;
    int         states_count;
    page_id_t   entry_page_id;
    void      (*on_change)(int);
} roller_configs[ROLLER_ID_COUNT] = {
    {ROLLER_ID_DATE_FORMAT,     STRING_DATE_FORMAT,    list_date_format,    ARRAY_SIZE(list_date_format),    PAGE_ID_DATE_TIME, NULL},
    {ROLLER_ID_WIFI_FREQUENCY,  STRING_WIFI_FREQUENCY, list_wifi_frequency, ARRAY_SIZE(list_wifi_frequency), PAGE_ID_WIFI,      NULL},
    {ROLLER_ID_FREQUENCY,       STRING_FREQ,           list_frequency,      ARRAY_SIZE(list_frequency),      PAGE_ID_MAIN,      NULL},
    {ROLLER_ID_VOICE_VOLUME,    STRING_MIC_VOLUME,     list_voice_volume,   ARRAY_SIZE(list_voice_volume),   PAGE_ID_MAIN,      NULL},
    {ROLLER_ID_SUBSCREEN_PLAY,  STRING_SUBDISPLAY,     list_subscreen_play, ARRAY_SIZE(list_subscreen_play), PAGE_ID_MAIN,      NULL},
    {ROLLER_ID_AUTO_DORMANT,    STRING_Auto_Dormant,   list_auto_dormant,   ARRAY_SIZE(list_auto_dormant),   PAGE_ID_MAIN,      NULL},
    {ROLLER_ID_AUTO_POWEROFF,   STRING_AUTO_OFF,       list_auto_poweroff,  ARRAY_SIZE(list_auto_poweroff),  PAGE_ID_MAIN,      NULL},
    {ROLLER_ID_VIDEO_FORMAT,    STRING_VIDEO_STANDARD, list_video_format,   ARRAY_SIZE(list_video_format),   PAGE_ID_MAIN,      video_format_cb},
    {ROLLER_ID_LANGUAGE,        STRING_LANGUAGE,       list_language,       ARRAY_SIZE(list_language),       PAGE_ID_MAIN,      roller_language_cb},
};

/* roller 到子页面的映射 */
static const page_id_t roller_page_map[ROLLER_ID_COUNT] = {
    [ROLLER_ID_DATE_FORMAT]     = PAGE_ID_DATE_TIME_DATE_FORMAT,
    [ROLLER_ID_WIFI_FREQUENCY]  = PAGE_ID_WIFI_FREQUENCY,
    [ROLLER_ID_FREQUENCY]       = PAGE_ID_FREQUENCY,
    [ROLLER_ID_VOICE_VOLUME]    = PAGE_ID_VOICE_VOLUME,
    [ROLLER_ID_SUBSCREEN_PLAY]  = PAGE_ID_SUBSCREEN_PLAY,
    [ROLLER_ID_AUTO_DORMANT]    = PAGE_ID_AUTO_DORMANT,
    [ROLLER_ID_AUTO_POWEROFF]   = PAGE_ID_AUTO_POWEROFF,
    [ROLLER_ID_VIDEO_FORMAT]    = PAGE_ID_VIDEO_FORMAT,
    [ROLLER_ID_LANGUAGE]        = PAGE_ID_LANGUAGE,
};

/* switch 配置表 */
static const struct {
    switch_id_t id;
    int         name_str_id;
    bool        default_state;
    page_id_t   parent_page_id;
    void      (*toggle_cb)(bool);
} switch_configs[SWITCH_ID_COUNT] = {
    {SWITCH_ID_LEDS,          STRING_LED,                false, PAGE_ID_MAIN,          switch_leds_toggle},
    {SWITCH_ID_DATE_STAMP,    STRING_DATE_STAMP,         false, PAGE_ID_MAIN,          switch_date_stamp_toggle},
    {SWITCH_ID_BRAND_STAMP,   STRING_BRAND_STAMP,        false, PAGE_ID_MAIN,          switch_brand_stamp_toggle},
    {SWITCH_ID_POWER_TONE,    STRING_BOOT_SOUND,         false, PAGE_ID_MAIN,          switch_power_tone_toggle},
    {SWITCH_ID_KEY_TONE,      STRING_KEY_TONE,           false, PAGE_ID_MAIN,          switch_key_tone_toggle},
    {SWITCH_ID_CLAP_TONE,     STRING_CAP_TONE,           false, PAGE_ID_MAIN,          switch_clap_tone_toggle},
    {SWITCH_ID_GRID,          STRING_GRID,               false, PAGE_ID_MAIN,          switch_grid_toggle},
    {SWITCH_ID_QUICK_START,   STRING_QUICK_START_SWITCH, false, PAGE_ID_MAIN,          switch_quick_start_toggle},
    {SWITCH_ID_VOICE_CONTROL, STRING_VOICE_CTR,          false, PAGE_ID_VOICE_CONTROL, switch_voice_control_toggle},
};

/* ============================================================================
 * 静态实例 + 外部符号
 * ============================================================================ */

static menu_manager_t g_mgr = {0};
lv_style_t style_cont;

/* 蓝牙页面内部文本控件（用于语言刷新） */
static lv_obj_t *s_bt_tip_label       = NULL;
static lv_obj_t *s_bt_search_label    = NULL;
static lv_obj_t *s_bt_list_tip_label  = NULL;

extern void open_scr_home_cb(void);
extern void open_scr_menu_cb(void);
extern void open_scr_poweroff_cb(void);

/* ============================================================================
 * 工具函数
 * ============================================================================ */

static const char *get_lang_text(int str_id)
{
    return lv_lang_string[str_id][current_lang];
}

static const char *safe_lang_text(int str_id)
{
    if (str_id < 0) return "???";
    return get_lang_text(str_id);
}

static void build_roller_options(const int *ids, int count, char *buf, size_t buf_size)
{
    buf[0] = '\0';
    size_t offset = 0;
    for (int i = 0; i < count; i++) {
        const char *txt = safe_lang_text(ids[i]);
        size_t len = strlen(txt);
        if (offset + len + 2 >= buf_size) break;
        if (i > 0) buf[offset++] = '\n';
        memcpy(buf + offset, txt, len);
        offset += len;
    }
    buf[offset] = '\0';
}

static void refresh_roller(roller_item_t *r)
{
    if (!r || !r->roller_obj) return;

    char buf[ROLLER_OPTIONS_BUF_SIZE];
    build_roller_options(r->states, r->states_count, buf, sizeof(buf));

    int sel = lv_roller_get_selected(r->roller_obj);
    lv_roller_set_options(r->roller_obj, buf, LV_ROLLER_MODE_NORMAL);
    if (sel < r->states_count) {
        lv_roller_set_selected(r->roller_obj, sel, LV_ANIM_OFF);
    }

    if (r->state_label) {
        int selected = lv_roller_get_selected(r->roller_obj);
        lv_label_set_text(r->state_label, safe_lang_text(r->states[selected]));
    }
}

static void refresh_bluetooth_page(void)
{
    if (s_bt_tip_label) {
        lv_label_set_text(s_bt_tip_label, _(STRING_TX_DISCONNECT_TIP));
    }
}

static void refresh_all_rollers(void)
{
    for (int i = 0; i < ROLLER_ID_COUNT; i++) {
        refresh_roller(&g_mgr.rollers[i]);
    }
}

static void refresh_all_switches(void)
{
    for (int i = 0; i < SWITCH_ID_COUNT; i++) {
        switch_item_t *s = &g_mgr.switches[i];
        if (s->cont_obj) {
            lv_obj_t *label = lv_obj_get_child(s->cont_obj, 0);
            if (label) lv_label_set_text(label, safe_lang_text(s->name_str_id));
        }
    }
}

static void refresh_all_page_entries(void)
{
    for (int i = 0; i < PAGE_ID_COUNT; i++) {
        page_item_t *p = &g_mgr.pages[i];
        if (p->menu_cont) {
            lv_obj_t *label = lv_obj_get_child(p->menu_cont, 0);
            if (label) lv_label_set_text(label, safe_lang_text(p->name_str_id));
        }
    }
}

//cb

static void switch_cont_click_cb(lv_event_t *e)
{
    switch_item_t *item = lv_event_get_user_data(e);
    if (!item || !item->switch_obj) return;

    bool cur = lv_obj_has_state(item->switch_obj, LV_STATE_CHECKED);
    if (cur) lv_obj_remove_state(item->switch_obj, LV_STATE_CHECKED);
    else     lv_obj_add_state(item->switch_obj, LV_STATE_CHECKED);

    if (item->toggle_cb) item->toggle_cb(!cur);
}

static void switch_value_changed_cb(lv_event_t *e)
{
    lv_obj_t *sw = lv_event_get_target(e);
    switch_item_t *item = lv_event_get_user_data(e);
    if (!item || !item->toggle_cb) return;
    bool state = lv_obj_has_state(sw, LV_STATE_CHECKED);
    item->state = state;
    item->toggle_cb(state);
}

static void roller_value_changed_cb(lv_event_t *e)
{
    lv_obj_t *roller = lv_event_get_target(e);
    roller_item_t *r = lv_event_get_user_data(e);
    if (!r) return;
    int sel = lv_roller_get_selected(roller);
    if (r->state_label) lv_label_set_text(r->state_label, safe_lang_text(r->states[sel]));
    if (r->on_change) r->on_change(sel);
}

static void roller_language_cb(int selected)
{

    if (selected >= MY_LANG_KOREAN) {
        current_lang = selected + 1;
    } else {
        current_lang = selected;
    }

    refresh_all_rollers();
    refresh_all_switches();
    refresh_all_page_entries();
    refresh_bluetooth_page();
    set_heading_cb(NULL);
}


static void video_format_cb(int selected){
    if (selected == 0) pal_ntsc = true;
    else pal_ntsc = false;
}


static void set_heading_cb(lv_event_t *e)
{
    (void)e;
    g_mgr.current_page = lv_menu_get_cur_main_page(g_mgr.menu);
    page_id_t pid = PAGE_ID_COUNT;
    for (int i = 0; i < PAGE_ID_COUNT; i++) {
        if (g_mgr.pages[i].page_obj == g_mgr.current_page) {
            pid = i;
            break;
        }
    }
    if (pid < PAGE_ID_COUNT) {
        lv_label_set_text(g_mgr.heading, safe_lang_text(g_mgr.pages[pid].name_str_id));
    } else {
        lv_label_set_text(g_mgr.heading, EMPTY_STRING);
    }
}

static void settings_back_cb(lv_event_t *e)
{
    if (lv_menu_back_button_is_root(g_mgr.menu, lv_event_get_target(e))) {
        open_scr_menu_cb();
    }
}

static void settings_quit_cb(lv_event_t *e)
{
    (void)e;
    open_scr_home_cb();
}

/* ============================================================================
 * Switch toggle 实现（修复5: 统一函数名）
 * ============================================================================ */

static void switch_leds_toggle(bool state) {
    printf("LEDs: %s\n", state ? "ON" : "OFF");
}

static void switch_date_stamp_toggle(bool state) {
    printf("Date Stamp: %s\n", state ? "ON" : "OFF");
}

static void switch_brand_stamp_toggle(bool state) {  /* 原名 switch_stamp_toggle */
    printf("Brand Stamp: %s\n", state ? "ON" : "OFF");
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
    printf("Quick-Start: %s\n", state ? "ON" : "OFF");
}

static void switch_voice_control_toggle(bool state) {
    printf("Voice Control: %s\n", state ? "ON" : "OFF");
}

static void bluetooth_search_clicked_cb(lv_event_t *e)
{
    (void)e;
    printf("searching bluetooth\n");
}

/* ============================================================================
 * 创建辅助函数
 * ============================================================================ */

static void load_styles(void)
{
    lv_style_init(&style_cont);
    lv_style_set_bg_color(&style_cont, BG_COLOR_DARK_GREY);
    lv_style_set_border_width(&style_cont, 0);
}

static lv_obj_t *create_basics(lv_obj_t *parent, const char *txt)
{
    lv_obj_t *overall_cont = lv_menu_cont_create(parent);
    lv_obj_remove_flag(overall_cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(overall_cont, 810, 80);

    lv_obj_t *cont = lv_obj_create(overall_cont);
    lv_obj_remove_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(cont, &style_cont, LV_PART_MAIN);
    lv_obj_set_size(cont, 810, 77);

    lv_obj_t *label = lv_label_create(cont);
    lv_obj_add_style(label, &style_font_default_36, LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 30, 0);
    lv_label_set_text(label, txt);

    return cont;
}

static lv_obj_t *create_text_entry(lv_obj_t *parent, const char *txt)
{
    lv_obj_t *cont = create_basics(parent, txt);
    lv_obj_t *img = lv_image_create(cont);
    lv_image_set_src(img, &set_have_sub_menu);
    lv_obj_align(img, LV_ALIGN_RIGHT_MID, -30, 0);
    return cont;
}

static lv_obj_t *create_switch_impl(lv_obj_t *parent, switch_item_t *item)
{
    const char *txt = safe_lang_text(item->name_str_id);
    lv_obj_t *cont = create_basics(parent, txt);

    lv_obj_t *sw = lv_switch_create(cont);
    lv_obj_set_size(sw, 100, 50);
    if (item->state) lv_obj_add_state(sw, LV_STATE_CHECKED);
    lv_obj_align(sw, LV_ALIGN_RIGHT_MID, -40, 0);

    item->switch_obj = sw;
    item->cont_obj = cont;

    lv_obj_add_flag(cont, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(sw, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_add_event_cb(cont, switch_cont_click_cb, LV_EVENT_CLICKED, item);
    if (item->toggle_cb) {
        lv_obj_add_event_cb(sw, switch_value_changed_cb, LV_EVENT_VALUE_CHANGED, item);
    }
    return cont;
}

static lv_obj_t *create_roller_entry(lv_obj_t *parent, roller_item_t *r, page_item_t *target_page)
{
    const char *txt = safe_lang_text(r->name_str_id);
    lv_obj_t *cont = create_basics(parent, txt);

    r->state_label = lv_label_create(cont);
    lv_obj_add_style(r->state_label, &style_font_default_36, LV_PART_MAIN);
    lv_obj_align(r->state_label, LV_ALIGN_RIGHT_MID, -80, 0);

    if (r->states_count > 0) {
        lv_label_set_text(r->state_label, safe_lang_text(r->states[0]));
    }

    lv_obj_t *img = lv_image_create(cont);
    lv_image_set_src(img, &set_have_sub_menu);
    lv_obj_align(img, LV_ALIGN_RIGHT_MID, -30, 0);

    if (target_page) {
        target_page->menu_cont = cont;
    }

    return cont;
}

static void create_roller_page(lv_obj_t *parent, roller_item_t *r)
{
    char buf[ROLLER_OPTIONS_BUF_SIZE];
    build_roller_options(r->states, r->states_count, buf, sizeof(buf));

    r->roller_obj = lv_roller_create(parent);
    lv_obj_set_size(r->roller_obj, lv_pct(100), 250);
    lv_obj_align(r->roller_obj, LV_ALIGN_CENTER, 0, 0);
    lv_roller_set_options(r->roller_obj, buf, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_selected(r->roller_obj, 0, LV_ANIM_OFF);

    lv_obj_set_style_bg_opa(r->roller_obj, LV_OPA_0, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(r->roller_obj, LV_OPA_0, LV_PART_SELECTED);
    lv_obj_set_style_border_width(r->roller_obj, 0, LV_PART_MAIN);
    lv_obj_add_style(r->roller_obj, &style_font_default_36, LV_PART_MAIN);
    lv_obj_set_style_text_color(r->roller_obj, lv_palette_main(LV_PALETTE_BLUE), LV_PART_SELECTED);
    lv_obj_set_style_text_line_space(r->roller_obj, 30, LV_PART_MAIN);

    lv_obj_add_event_cb(r->roller_obj, roller_value_changed_cb, LV_EVENT_VALUE_CHANGED, r);
}

static lv_obj_t *create_page_basic(lv_obj_t *parent, const char *title, const char *text)
{
    lv_obj_t *cont = lv_obj_create(parent);
    lv_obj_set_size(cont, lv_pct(100), lv_pct(100));
    lv_obj_add_style(cont, &style_cont_transparent, LV_PART_MAIN);

    lv_obj_t *label = lv_label_create(cont);
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_width(label, 550);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(label, title);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, lv_pct(3));
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    label = lv_label_create(cont);
    lv_obj_add_style(label, &style_font_default_30, LV_PART_MAIN);
    lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN);
    lv_obj_set_width(label, 550);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(label, text);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, lv_pct(18));
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    return cont;
}

static void back_cb(lv_event_t *e)
{
    (void)e;
    lv_obj_send_event(lv_menu_get_main_header_back_button(g_mgr.menu), LV_EVENT_CLICKED, NULL);
}

static void create_page_format_sd(void)
{
    page_item_t *p = &g_mgr.pages[PAGE_ID_FORMAT_SD];
    p->page_obj = lv_menu_page_create(g_mgr.menu, "");
    lv_obj_set_scroll_dir(p->page_obj, LV_DIR_VER);

    lv_obj_t *cont = create_page_basic(p->page_obj,
                                       _(STRING_FORMAT_SD_AT_ONCE),
                                       _(STRING_FORMAT_SD_PROMPT));

    lv_obj_t *btn_cancel = create_btn_cancle(cont);
    lv_obj_add_event_cb(btn_cancel, back_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btn_confirm = create_btn_confirm(cont);
    lv_obj_add_event_cb(btn_confirm, open_scr_poweroff_cb_wrapper, LV_EVENT_CLICKED, NULL);

    p->menu_cont = create_text_entry(g_mgr.pages[PAGE_ID_MAIN].page_obj, _(STRING_FORMAT_SD));
    lv_menu_set_load_page_event(g_mgr.menu, p->menu_cont, p->page_obj);
}

static void create_page_factory_reset(void)
{
    page_item_t *p = &g_mgr.pages[PAGE_ID_FACTORY_RESET];
    p->page_obj = lv_menu_page_create(g_mgr.menu, "");
    lv_obj_set_scroll_dir(p->page_obj, LV_DIR_VER);

    lv_obj_t *cont = create_page_basic(p->page_obj,
                                       _(STRING_RESET_FACTORY_AT_ONCE),
                                       _(STRING_RESET_FACTORY_PROMPT));

    lv_obj_t *btn_cancel = create_btn_cancle(cont);
    lv_obj_t *btn_confirm = create_btn_confirm(cont);

    lv_obj_add_event_cb(btn_cancel, back_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_confirm, open_scr_poweroff_cb_wrapper, LV_EVENT_CLICKED, NULL);

    p->menu_cont = create_text_entry(g_mgr.pages[PAGE_ID_MAIN].page_obj, _(STRING_DEFAULT_SET));
    lv_menu_set_load_page_event(g_mgr.menu, p->menu_cont, p->page_obj);
}

/* ============================================================================
 * 主创建函数
 * ============================================================================ */

void create_scr_menu_settings(void)
{
    g_mgr.scr = lv_obj_create(NULL);

    g_mgr.menu = lv_menu_create(g_mgr.scr);
    lv_obj_add_style(g_mgr.menu, &style_font_default_36, LV_PART_MAIN);
    lv_obj_set_style_bg_color(g_mgr.menu, BG_COLOR_VERY_DARK_GREY, LV_PART_MAIN);
    lv_obj_set_size(g_mgr.menu, lv_pct(100), lv_pct(100));
    lv_obj_center(g_mgr.menu);

    load_styles();

    for (int i = 0; i < PAGE_ID_COUNT; i++) {
        g_mgr.pages[i].id = page_configs[i].id;
        g_mgr.pages[i].name_str_id = page_configs[i].name_str_id;
    }

    page_item_t *main_p = &g_mgr.pages[PAGE_ID_MAIN];
    main_p->page_obj = lv_menu_page_create(g_mgr.menu, "");
    lv_obj_add_style(main_p->page_obj, &style_scrollbar, LV_PART_SCROLLBAR);
    lv_obj_set_scroll_dir(main_p->page_obj, LV_DIR_VER);

    for (int i = PAGE_ID_MAIN + 1; i < PAGE_ID_COUNT; i++) {
        if (!g_mgr.pages[i].page_obj) {
            g_mgr.pages[i].page_obj = lv_menu_page_create(g_mgr.menu, "");
            lv_obj_set_scroll_dir(g_mgr.pages[i].page_obj, LV_DIR_VER);
        }
    }

    for (int i = 0; i < ROLLER_ID_COUNT; i++) {
        roller_item_t *r = &g_mgr.rollers[i];
        r->id = roller_configs[i].id;
        r->name_str_id = roller_configs[i].name_str_id;
        r->states = roller_configs[i].states;
        r->states_count = roller_configs[i].states_count;
        r->on_change = roller_configs[i].on_change;

        page_id_t entry_page_id = roller_configs[i].entry_page_id;
        page_id_t roller_page_id = roller_page_map[r->id];

        /* 创建 roller 选择页面 */
        page_item_t *rp = &g_mgr.pages[roller_page_id];
        create_roller_page(rp->page_obj, r);

        /* 在指定的父页面创建入口，并绑定到子页面的 menu_cont */
        page_item_t *entry_p = &g_mgr.pages[entry_page_id];
        lv_obj_t *entry = create_roller_entry(entry_p->page_obj, r, rp);  /* ← 传入 rp */
        lv_menu_set_load_page_event(g_mgr.menu, entry, rp->page_obj);
    }

    for (int i = 0; i < SWITCH_ID_COUNT; i++) {
        switch_item_t *s = &g_mgr.switches[i];
        s->id = switch_configs[i].id;
        s->name_str_id = switch_configs[i].name_str_id;
        s->state = switch_configs[i].default_state;
        s->toggle_cb = switch_configs[i].toggle_cb;

        page_item_t *parent_p = &g_mgr.pages[switch_configs[i].parent_page_id];
        lv_obj_t *entry = create_switch_impl(parent_p->page_obj, s);
        (void)entry;
    }

    /* WiFi 体系 */
    {
        page_item_t *wifi_p = &g_mgr.pages[PAGE_ID_WIFI];
        wifi_p->menu_cont = create_text_entry(main_p->page_obj, safe_lang_text(wifi_p->name_str_id));
        lv_menu_set_load_page_event(g_mgr.menu, wifi_p->menu_cont, wifi_p->page_obj);

        page_item_t *wifi_conn = &g_mgr.pages[PAGE_ID_WIFI_CONNECT];
        lv_obj_t *wifi_conn_cont = lv_obj_create(wifi_conn->page_obj);
        lv_obj_set_size(wifi_conn_cont, lv_pct(100), lv_pct(100));
        lv_obj_set_style_bg_color(wifi_conn_cont, BG_COLOR_VERY_DARK_GREY, LV_PART_MAIN);
        lv_obj_remove_flag(wifi_conn_cont, LV_OBJ_FLAG_SCROLLABLE);

        lv_obj_t *wifi_info = lv_label_create(wifi_conn_cont);
        lv_obj_set_style_text_color(wifi_info, lv_color_white(), LV_PART_MAIN);
        lv_obj_set_width(wifi_info, TEXT_WRAP_WIDTH);
        lv_label_set_long_mode(wifi_info, LV_LABEL_LONG_WRAP);
        lv_label_set_text(wifi_info, safe_lang_text(STRING_WIFI_INFO));
        lv_obj_set_style_text_align(wifi_info, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
        lv_obj_center(wifi_info);

        lv_obj_t *wifi_conn_entry = create_text_entry(wifi_p->page_obj, safe_lang_text(wifi_conn->name_str_id));
        lv_menu_set_load_page_event(g_mgr.menu, wifi_conn_entry, wifi_conn->page_obj);
    }
    /* 蓝牙页面（标准 menu page） */
    {
        page_item_t *bt_p = &g_mgr.pages[PAGE_ID_BLUETOOTH];
        bt_p->menu_cont = create_text_entry(main_p->page_obj, safe_lang_text(bt_p->name_str_id));
        lv_menu_set_load_page_event(g_mgr.menu, bt_p->menu_cont, bt_p->page_obj);

        /* 断开提示 */
        s_bt_tip_label = lv_label_create(bt_p->page_obj);
        lv_obj_set_style_text_color(s_bt_tip_label, lv_color_white(), LV_PART_MAIN);
        lv_obj_set_width(s_bt_tip_label, TEXT_WRAP_WIDTH);
        lv_label_set_long_mode(s_bt_tip_label, LV_LABEL_LONG_WRAP);
        lv_label_set_text(s_bt_tip_label, _(STRING_TX_DISCONNECT_TIP));
        lv_obj_set_style_text_align(s_bt_tip_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
        lv_obj_set_style_pad_top(s_bt_tip_label, 30, LV_PART_MAIN);
        lv_obj_set_style_pad_bottom(s_bt_tip_label, 20, LV_PART_MAIN);

    }

    /* Date Time 体系 */
    {
        page_item_t *dt_p = &g_mgr.pages[PAGE_ID_DATE_TIME];
        dt_p->menu_cont = create_text_entry(main_p->page_obj, safe_lang_text(dt_p->name_str_id));
        lv_menu_set_load_page_event(g_mgr.menu, dt_p->menu_cont, dt_p->page_obj);

        page_item_t *dt_date = &g_mgr.pages[PAGE_ID_DATE_TIME_DATE];
        lv_obj_t *entry_date = create_text_entry(dt_p->page_obj, safe_lang_text(dt_date->name_str_id));
        lv_menu_set_load_page_event(g_mgr.menu, entry_date, dt_date->page_obj);

        page_item_t *dt_time = &g_mgr.pages[PAGE_ID_DATE_TIME_TIME];
        lv_obj_t *entry_time = create_text_entry(dt_p->page_obj, safe_lang_text(dt_time->name_str_id));
        lv_menu_set_load_page_event(g_mgr.menu, entry_time, dt_time->page_obj);
    }

    /* Voice 体系 */
    {
        page_item_t *voice_p = &g_mgr.pages[PAGE_ID_VOICE_CONTROL];
        voice_p->menu_cont = create_text_entry(main_p->page_obj, safe_lang_text(voice_p->name_str_id));
        lv_menu_set_load_page_event(g_mgr.menu, voice_p->menu_cont, voice_p->page_obj);

        page_item_t *vcmd = &g_mgr.pages[PAGE_ID_VOICE_COMMAND];
        lv_obj_t *vcmd_label = lv_label_create(vcmd->page_obj);
        lv_obj_set_style_text_color(vcmd_label, lv_color_white(), LV_PART_MAIN);
        lv_obj_set_width(vcmd_label, TEXT_WRAP_WIDTH);
        lv_label_set_long_mode(vcmd_label, LV_LABEL_LONG_WRAP);
        lv_label_set_text(vcmd_label, "command             command \ncommand             command \ncommand             command \ncommand             command \n");
        lv_obj_set_style_text_align(vcmd_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
        lv_obj_center(vcmd_label);

        lv_obj_t *vcmd_entry = create_text_entry(voice_p->page_obj, safe_lang_text(vcmd->name_str_id));
        lv_menu_set_load_page_event(g_mgr.menu, vcmd_entry, vcmd->page_obj);
    }

    create_page_format_sd();
    create_page_factory_reset();

    {
        page_item_t *info_p = &g_mgr.pages[PAGE_ID_INFORMATION];
        info_p->menu_cont = create_text_entry(main_p->page_obj, safe_lang_text(info_p->name_str_id));
        lv_menu_set_load_page_event(g_mgr.menu, info_p->menu_cont, info_p->page_obj);

        lv_obj_t *info_label = lv_label_create(info_p->page_obj);
        lv_obj_set_style_text_color(info_label, lv_color_white(), LV_PART_MAIN);
        lv_label_set_long_mode(info_label, LV_LABEL_LONG_WRAP);
        lv_label_set_text(info_label, "blablablabla\nblablabla");
        lv_obj_align(info_label, LV_ALIGN_TOP_LEFT, 30, 30);
    }

    lv_obj_add_event_cb(g_mgr.menu, set_heading_cb, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t *back_btn = lv_menu_get_main_header_back_button(g_mgr.menu);
    lv_obj_t *back_icon = lv_obj_get_child(back_btn, 0);
    lv_image_set_src(back_icon, &Pattern_Return);
    lv_obj_align(lv_menu_get_main_header(g_mgr.menu), LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_add_event_cb(back_btn, settings_back_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *heading_cont = lv_obj_create(lv_menu_get_main_header(g_mgr.menu));
    lv_obj_set_size(heading_cont, lv_pct(65), 60);
    lv_obj_set_style_bg_color(heading_cont, BG_COLOR_VERY_DARK_GREY, LV_PART_MAIN);
    lv_obj_set_style_border_width(heading_cont, 0, LV_PART_MAIN);
    lv_obj_remove_flag(heading_cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_flag(lv_menu_get_main_header(g_mgr.menu), LV_OBJ_FLAG_SCROLLABLE);

    g_mgr.heading = lv_label_create(heading_cont);
    lv_obj_add_style(g_mgr.heading, &style_font_default_36, LV_PART_MAIN);
    lv_obj_center(g_mgr.heading);

    lv_obj_t *exit_icon = lv_image_create(lv_menu_get_main_header(g_mgr.menu));
    lv_image_set_src(exit_icon, &system_set_exit);
    lv_obj_align(exit_icon, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_add_flag(exit_icon, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(exit_icon, settings_quit_cb, LV_EVENT_CLICKED, NULL);

    lv_menu_set_mode_root_back_button(g_mgr.menu, LV_MENU_ROOT_BACK_BUTTON_ENABLED);
}

/* ============================================================================
 * 设置菜单动作
 * ============================================================================ */

void settings_action(void)
{
    lv_menu_clear_history(g_mgr.menu);
    lv_menu_set_page(g_mgr.menu, g_mgr.pages[PAGE_ID_MAIN].page_obj);

    switch_item_t *grid = &g_mgr.switches[SWITCH_ID_GRID];
    if (grid->switch_obj) {
        if (grid_active) lv_obj_add_state(grid->switch_obj, LV_STATE_CHECKED);
        else             lv_obj_remove_state(grid->switch_obj, LV_STATE_CHECKED);
    }

    roller_item_t *lang = &g_mgr.rollers[ROLLER_ID_LANGUAGE];
    if (lang->roller_obj) {
        int sel = (current_lang >= MY_LANG_KOREAN) ? (current_lang - 1) : current_lang;
        lv_roller_set_selected(lang->roller_obj, sel, LV_ANIM_OFF);
        if (lang->state_label) {
            int idx = lv_roller_get_selected(lang->roller_obj);
            lv_label_set_text(lang->state_label, safe_lang_text(lang->states[idx]));
        }
    }

    roller_item_t *video_format = &g_mgr.rollers[ROLLER_ID_VIDEO_FORMAT];
    if (video_format->roller_obj) {
        if (pal_ntsc) lv_roller_set_selected(video_format->roller_obj, 0, LV_ANIM_OFF);
        else  lv_roller_set_selected(video_format->roller_obj, 1, LV_ANIM_OFF);
        if (video_format->state_label) {
            int idx = lv_roller_get_selected(video_format->roller_obj);
            lv_label_set_text(video_format->state_label, safe_lang_text(video_format->states[idx]));
        }
    }

    lv_screen_load(g_mgr.scr);
}

/* ============================================================================
 * 独立蓝牙管理界面
 * ============================================================================ */


void bluetooth_action(void)
{
    if (!g_mgr.scr) create_scr_menu_settings();

    lv_screen_load(g_mgr.scr);
    lv_menu_clear_history(g_mgr.menu);
    lv_menu_set_page(g_mgr.menu, g_mgr.pages[PAGE_ID_BLUETOOTH].page_obj);
}
