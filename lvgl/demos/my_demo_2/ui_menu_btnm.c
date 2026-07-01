#include <string.h>
#include <stdio.h>
#include "lvgl.h"
#include "ui_roller.h"
#include "camera_modes.h"
#include "my_demo_2.h"
#include "ui_menu_btnm.h"
#include "pic_converted_transparent/image_declares.h"

static lv_obj_t * pop_up_btn;
static lv_obj_t * pop_up_btn_label;

lv_obj_t *cont = NULL;
lv_timer_t * timer = NULL;

/**access**/
void create_menu_btnm(lv_obj_t * parent);

/**create**/
static void create_menu_grid();
static void create_pop_up_btn(lv_obj_t * parent);

/**CB**/
static void menu_btn_toggle_state_cb(lv_event_t *e);
static void hide_btn_cb(lv_event_t *e);
static void hide_btn_timer_cb(lv_timer_t * t);

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
    if (menu_buttons[index].state == BTN_STATE_ON) {
        menu_buttons[index].state = BTN_STATE_OFF;
    } else {
        menu_buttons[index].state = BTN_STATE_ON;
    }

   /**call action**/
    menu_buttons[index].action();

    /**draw**/
    create_menu_grid();
}


static void create_menu_grid() {
    for (int i = 0; i < 8; i++) {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, i % 4, 1, LV_GRID_ALIGN_STRETCH, i / 4, 1);
        lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
        lv_obj_set_style_bg_color(btn, BG_COLOR_DARK_GREY, LV_PART_MAIN);

        lv_obj_t *img = lv_image_create(btn);
        if (menu_buttons[i].state == BTN_STATE_ON){
            lv_image_set_src(img, menu_buttons[i].icon_src_on);
        } else {
            lv_image_set_src(img, menu_buttons[i].icon_src_off);
        }

        lv_obj_align(img, LV_ALIGN_TOP_MID, 0, -10);
        lv_obj_set_style_bg_color(img, BG_COLOR_DARK_GREY, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(img, LV_OPA_COVER, LV_PART_MAIN);

        lv_obj_set_user_data(btn, (void*)(intptr_t)i);

        lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
        lv_obj_add_event_cb(btn, menu_btn_toggle_state_cb, LV_EVENT_CLICKED, NULL);
    }
}

void create_menu_btnm(lv_obj_t * parent){
    timer = lv_timer_create(hide_btn_timer_cb, 2000, NULL);
    lv_timer_pause(timer);

    /**create grid layout**/
    cont = lv_obj_create(parent);
    lv_obj_set_size(cont, 780, 365);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(cont, BG_COLOR_DARK_GREY, LV_PART_MAIN);
    lv_obj_set_style_border_width(cont, 0, LV_PART_MAIN);

    static const int width = 147;
    static lv_coord_t col_dsc[] = {width, width, width, width, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {width, width, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);
    lv_obj_set_style_pad_all(cont, 35, LV_PART_MAIN);
    lv_obj_set_style_pad_row(cont, 10, LV_PART_MAIN); //行间距
    lv_obj_set_style_pad_column(cont, 40, LV_PART_MAIN); //列间距

    lv_obj_remove_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    /**create ui components**/
     create_menu_grid();
     create_pop_up_btn(parent);
}


// Callback function for click event
static void hide_btn_cb(lv_event_t *e) {
    lv_obj_add_flag(pop_up_btn, LV_OBJ_FLAG_HIDDEN);
}

static void hide_btn_timer_cb(lv_timer_t * t){
    printf("Called\n");
    lv_obj_add_flag(pop_up_btn, LV_OBJ_FLAG_HIDDEN);
    lv_timer_pause(timer);
}

static void create_pop_up_btn(lv_obj_t * parent){
    pop_up_btn = lv_button_create(parent);
    lv_obj_set_style_bg_color(pop_up_btn, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);
    lv_obj_set_size(pop_up_btn, lv_pct(100), lv_pct(20));
    lv_obj_set_style_radius(pop_up_btn, 40, LV_PART_MAIN );

    pop_up_btn_label = lv_label_create(pop_up_btn);
    lv_obj_center(pop_up_btn_label);

    /**hide btn**/
    lv_obj_add_flag(pop_up_btn, LV_OBJ_FLAG_HIDDEN);

    /**cb hide on click**/
    lv_obj_add_event_cb(pop_up_btn, hide_btn_cb, LV_EVENT_CLICKED, NULL);
}

//actions
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


//TODO

static void wifi_action(void) {
    printf("WiFi toggle\n");
    // quick_wifi_enable/disable logic
}

static void settings_action(void) {
    printf("Open settings\n");
    // quick_system_set logic
}

void poweroff_action(void) {
    printf("Power off\n");
    // quick_poweroff logic
}

static void bluetooth_action(void) {
    printf("Bluetooth toggle\n");
    // quick_bt_enable/disable logic
}

static void lock_action(void) {
    printf("Lock/Unlock toggle\n");
    // quick_lock_enable/disable logic
}
