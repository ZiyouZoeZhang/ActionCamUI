#include <string.h>
#include <stdio.h>
#include "lvgl.h"
#include "camera_modes.h"
#include "my_demo_2.h"
#include "camera_menu.h"
#include "pic_converted_transparent/image_declares.h"

static lv_obj_t * pop_up_btn;
static lv_obj_t * pop_up_btn_label;
static lv_obj_t *cont = NULL;
static lv_timer_t * timer = NULL;
static lv_obj_t * scr_menu = NULL;

/**access**/
static void create_menu_btnm(lv_obj_t *parent);
void swipe_scr_menu_cb(lv_event_t *e);

/**create**/
static void create_menu_grid();
static void create_pop_up_btn(lv_obj_t * parent);
///use create scr setting
///(maybe use create scr bluetooth, or create scr bluetooth)

/**CB**/
static void menu_btn_toggle_state_cb(lv_event_t *e);
static void hide_btn_cb(lv_event_t *e);
static void hide_btn_timer_cb(lv_timer_t * t);
//void open_scr_menu_cb();
//void open_scr_poweroff_cb(lv_event_t *e);

/**actions**/
static void wifi_action(void);
static void rotation_action(void);
static void lock_action(void);
static void settings_action(void);
static void voice_action(void);
static void bluetooth_action(void);
static void poweroff_action(void);
static void grid_action(void);

menu_btn_info_t menu_buttons[] = {
    {CAM_MENU_WIFI,          "WiFi",        &quick_wifi_enable, &quick_wifi_disable, BTN_STATE_OFF, wifi_action},
    {CAM_MENU_AUTO_ROTATION, "Auto Rotate", &quick_autorotate_enable, &quick_autorotate_disable, BTN_STATE_OFF, rotation_action},
    {CAM_MENU_LOCK,          "Lock",        &quick_lock_enable, &quick_lock_disable, BTN_STATE_OFF, lock_action},
    {CAM_MENU_SETTINGS,      "Settings",    &quick_system_set, &quick_system_set, BTN_STATE_OFF, settings_action},
    {CAM_MENU_VOICE_REC,     "Voice Rec",   &quick_voice_enable, &quick_voice_disable, BTN_STATE_OFF, voice_action},
    {CAM_MENU_BLUETOOTH,     "Bluetooth",   &quick_bt_enable, &quick_bt_disable, BTN_STATE_OFF, bluetooth_action},
    {CAM_MENU_TURN_OFF,      "Power Off",   &quick_poweroff, &quick_poweroff, BTN_STATE_OFF, poweroff_action},
    {CAM_MENU_GRID_VIEW,     "Grid View",   &quick_grid_enable, &quick_grid_disable, BTN_STATE_OFF, grid_action}
};

static void menu_btn_toggle_state_cb(lv_event_t *e){
    lv_obj_t *btn_obj = lv_event_get_target(e);
    int index = (int)(intptr_t)lv_obj_get_user_data(btn_obj);

    /**toggle state**/
    if (index == CAM_MENU_WIFI){
        if (wifi_active) menu_buttons[index].state = BTN_STATE_ON;
        else menu_buttons[index].state = BTN_STATE_OFF;
    } else if (menu_buttons[index].state == BTN_STATE_ON) {
        menu_buttons[index].state = BTN_STATE_OFF;
    } else {
        menu_buttons[index].state = BTN_STATE_ON;
    }

   /**call action**/
    menu_buttons[index].action();

    /**draw**/
    create_menu_grid();
}

void open_scr_menu_cb(){
    lv_screen_load(scr_menu);
}

void swipe_scr_menu_cb(lv_event_t *e){
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
    switch(dir) {
        case LV_DIR_TOP:
            open_scr_home_cb();
            break;
        default:
            break;
    }
}

void create_scr_menu(){
    /**background**/
    scr_menu = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_menu, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);

    create_menu_btnm(scr_menu);
    create_battery_icon(scr_menu, battery_charging, battery_level);
    create_exit_icon(scr_menu);

    /**CB**/
    lv_obj_add_event_cb(scr_menu, swipe_scr_menu_cb, LV_EVENT_RELEASED, NULL);

}

static void create_menu_grid() {
    for (int i = 0; i < 8; i++) {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, i % 4, 1, LV_GRID_ALIGN_STRETCH, i / 4, 1);
        lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
        lv_obj_set_style_bg_color(btn, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);

        lv_obj_t *img = lv_image_create(btn);
        if (menu_buttons[i].state == BTN_STATE_ON){
            lv_image_set_src(img, menu_buttons[i].icon_src_on);
        } else {
            lv_image_set_src(img, menu_buttons[i].icon_src_off);
        }

        lv_obj_align(img, LV_ALIGN_TOP_MID, 0, -10);
        lv_obj_set_style_bg_color(img, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(img, LV_OPA_COVER, LV_PART_MAIN);

        lv_obj_set_user_data(btn, (void*)(intptr_t)i);

        lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
        lv_obj_add_event_cb(btn, menu_btn_toggle_state_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_add_flag(btn, LV_OBJ_FLAG_ADV_HITTEST);
        //lv_obj_add_event_cb(btn, menu_btn_toggle_state_cb, LV_EVENT_PRESSED | LV_EVENT_RELEASED, NULL);
    }
}

static void create_menu_btnm(lv_obj_t * parent){
    timer = lv_timer_create(hide_btn_timer_cb, 2000, NULL);
    lv_timer_pause(timer);

    /**create grid layout**/
    cont = lv_obj_create(parent);
    lv_obj_set_size(cont, 780, 365);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, -25);
    lv_obj_set_style_bg_color(cont, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);
    lv_obj_set_style_border_width(cont, 0, LV_PART_MAIN);
    lv_obj_remove_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_event_cb(cont, swipe_scr_menu_cb, LV_EVENT_RELEASED, NULL);

    static const int width = 147;
    static lv_coord_t col_dsc[] = {width, width, width, width, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {width, width, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);
    lv_obj_set_style_pad_all(cont, 35, LV_PART_MAIN);
    lv_obj_set_style_pad_row(cont, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_column(cont, 40, LV_PART_MAIN);

    lv_obj_t * swipe_icon = lv_obj_create(parent);
    lv_obj_add_style(swipe_icon, &style_swipe_icon, LV_PART_MAIN);
    lv_obj_remove_flag(swipe_icon, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(swipe_icon, LV_ALIGN_BOTTOM_MID, 0, -15);

    /**create ui components**/
     create_menu_grid();
     create_pop_up_btn(parent);
}

// Callback function for click event
static void hide_btn_cb(lv_event_t *e) {
    lv_obj_add_flag(pop_up_btn, LV_OBJ_FLAG_HIDDEN);
}

static void hide_btn_timer_cb(lv_timer_t * t){
    lv_obj_add_flag(pop_up_btn, LV_OBJ_FLAG_HIDDEN);
    lv_timer_pause(timer);
}

static void create_pop_up_btn(lv_obj_t * parent){
    pop_up_btn = lv_button_create(parent);
    lv_obj_set_style_bg_color(pop_up_btn, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);
    lv_obj_set_size(pop_up_btn, lv_pct(100), lv_pct(17));
    lv_obj_set_style_radius(pop_up_btn, 20, LV_PART_MAIN );

    pop_up_btn_label = lv_label_create(pop_up_btn);
    lv_obj_add_style(pop_up_btn_label, &style_font_default_36, LV_PART_MAIN);

    /**hide btn**/
    lv_obj_add_flag(pop_up_btn, LV_OBJ_FLAG_HIDDEN);

    /**cb hide on click**/
    lv_obj_add_event_cb(pop_up_btn, hide_btn_cb, LV_EVENT_CLICKED, NULL);
}

/**actions**/

static void rotation_action(void) {
    if (menu_buttons[CAM_MENU_AUTO_ROTATION].state == BTN_STATE_ON){
        lv_label_set_text(pop_up_btn_label, "AUTO ROTATION: ON");
    } else {
         lv_label_set_text(pop_up_btn_label, "AUTO ROTATION: OFF");
    }
    lv_obj_remove_flag(pop_up_btn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(pop_up_btn);
    lv_timer_reset(timer);
    lv_timer_resume(timer);
}

static void voice_action(void) {
    if (menu_buttons[CAM_MENU_VOICE_REC].state == BTN_STATE_ON){
        lv_label_set_text(pop_up_btn_label, "VOICE RECOGNITION: ON");
    } else {
        lv_label_set_text(pop_up_btn_label, "VOICE RECOGNITION: OFF");
    }
    lv_obj_remove_flag(pop_up_btn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(pop_up_btn);
    lv_timer_reset(timer);
    lv_timer_resume(timer);
}

static void grid_action(void) {

    if (menu_buttons[CAM_MENU_GRID_VIEW].state == BTN_STATE_ON){
        lv_label_set_text(pop_up_btn_label,"GRID VIEW: ON");
    } else {
        lv_label_set_text(pop_up_btn_label, "GRID VIEW: OFF");
    }
    lv_obj_remove_flag(pop_up_btn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(pop_up_btn);
    lv_timer_reset(timer);
    lv_timer_resume(timer);
}

static void wifi_action(void) {
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr, BG_COLOR_DARK_BLUE_GREY,LV_PART_MAIN);

    lv_obj_t * qr = lv_image_create(scr);
    lv_image_set_src(qr, &xtugo_qrcode);
    lv_obj_align(qr, LV_ALIGN_TOP_LEFT, lv_pct(10), lv_pct(10));
    lv_obj_set_size(qr, 260, 260);

    lv_obj_t * btn  = lv_button_create(scr);
    lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);
    lv_obj_set_size(btn, lv_pct(100), lv_pct(25));
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_event_cb(btn, open_scr_menu_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * txt = lv_label_create(scr);
    lv_label_set_text(txt, "TQWERTYUIOHJ \nasdfghjkzxcvbnm \nqwertyhbncxfg");
    lv_obj_add_style(txt, &style_font_default_36, LV_PART_MAIN);
    lv_obj_align(txt, LV_ALIGN_CENTER, lv_pct(18), lv_pct(-15));

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "CANCLE");
    lv_obj_add_style(label, &style_font_default_36, LV_PART_MAIN);

    lv_screen_load(scr);
}

static void poweroff_action(void) {
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr, BG_COLOR_DARK_BLUE_GREY,LV_PART_MAIN);

    /**btn back**/
    lv_obj_t * btn_back  = lv_button_create(scr);
    lv_obj_set_style_bg_color(btn_back, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN);
    lv_obj_set_size(btn_back, lv_pct(35), lv_pct(20));
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_MID, lv_pct(-20), lv_pct(-25));
    lv_obj_add_event_cb(btn_back, open_scr_menu_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * label_back = lv_label_create(btn_back);
    lv_label_set_text(label_back, "CANCLE");
    lv_obj_add_style(label_back, &style_font_default_36, LV_PART_MAIN);

    /**btn continue**/
    lv_obj_t * btn_continue  = lv_button_create(scr);
    lv_obj_set_style_bg_color(btn_continue, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);
    lv_obj_set_size(btn_continue, lv_pct(35), lv_pct(20));
    lv_obj_align(btn_continue, LV_ALIGN_BOTTOM_MID, lv_pct(20), lv_pct(-25));
    lv_obj_add_event_cb(btn_continue, open_scr_poweroff_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * label_continue = lv_label_create(btn_continue);
    lv_label_set_text(label_continue, "CONTINUE");
    lv_obj_add_style(label_continue, &style_font_default_36, LV_PART_MAIN);

    lv_obj_t * txt = lv_label_create(scr);
    lv_label_set_text(txt, "POWEROFF");
    lv_obj_add_style(txt, &style_font_default_36, LV_PART_MAIN);
    lv_obj_align(txt, LV_ALIGN_CENTER, 0, lv_pct(-15));

    /**OTHER UI COMPONENTS**/
    create_exit_icon(scr);
    create_battery_icon(scr, battery_charging, battery_level);

    lv_screen_load(scr);
}


static void bluetooth_action(void) {
    printf("Bluetooth toggle\n");
    // quick_bt_enable/disable logic
}

static void settings_action(void) {
    printf("Settings\n");
    // quick_poweroff logic
}

///start lock action
static lv_obj_t *lock_left = NULL;
static lv_obj_t *lock_right = NULL;
static lv_obj_t *lock_slide = NULL;
static lv_obj_t *slider = NULL;

static void slider_pressing_cb(lv_event_t *e);
static void create_lock_default();
static void slider_released_cb(lv_event_t * e);

static void slider_pressing_cb(lv_event_t *e) {
    lv_obj_set_style_opa(slider, LV_OPA_100, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(slider, LV_OPA_0, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(slider, LV_OPA_0, LV_PART_KNOB);
    lv_obj_set_style_bg_opa(slider, LV_OPA_0, LV_PART_INDICATOR);
    lv_image_set_src(lock_right, &Sliding_unlock);
    lv_obj_add_flag(lock_slide, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(lock_left, LV_OBJ_FLAG_HIDDEN);
}

static void create_lock_default() {
    lv_slider_set_value(slider, 12, LV_ANIM_OFF);
    lv_image_set_src(lock_left, &Sliding);
    lv_image_set_src(lock_right, &Lock_Normal);
    lv_image_set_src(lock_slide, &Sliding_unlock_BG);
    lv_obj_remove_flag(lock_slide, LV_OBJ_FLAG_HIDDEN);
    lv_obj_remove_flag(lock_left, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_opa(slider, LV_OPA_0, LV_PART_MAIN);
}

static void slider_released_cb(lv_event_t * e){
    if (lv_slider_get_value(slider) >= 95) {
        open_scr_home_cb();
    } else {
        create_lock_default();
    }
}

static void lock_action(void) {
    //draw default view
    lv_obj_t *scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);

    lock_left = lv_image_create(scr);
    lv_obj_align(lock_left, LV_ALIGN_CENTER, lv_pct(-30), 0);

    lock_right = lv_image_create(scr);
    lv_obj_align(lock_right, LV_ALIGN_CENTER, lv_pct(30), 0);

    lock_slide = lv_image_create(scr);
    lv_obj_center(lock_slide);

    //draw slider
    slider = lv_slider_create(scr);
    lv_obj_set_size(slider, 535, 120);
    lv_obj_center(slider);
    lv_obj_align(slider, LV_ALIGN_CENTER, -30, 0);
    lv_slider_set_range(slider, 00, 100);
    lv_slider_set_value(slider, 12, LV_ANIM_OFF);

    lv_obj_set_style_bg_image_src(slider, &Sliding_touch, LV_PART_KNOB);
    lv_obj_set_style_bg_image_opa(slider, LV_OPA_COVER, LV_PART_KNOB);

    lv_obj_add_flag(slider, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_add_event_cb(slider, slider_pressing_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(slider, slider_pressing_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(slider, slider_released_cb, LV_EVENT_RELEASED, NULL);

    create_lock_default();
    lv_screen_load(scr);
}

///end lock action
