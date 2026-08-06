#include "my_demo_2.h"

/**VARIABLES FOR INPUT**/
int battery_level = 30;
bool battery_charging = true;
int sd_status = 1; //0 = none, 1 = available, -1 = error
float sd_storage = 26.3;
int cur_cam_mode = 0;
int cur_cam_resolution = STRING_1080P;
int cur_cam_zoom = 0;
bool wifi_active = false;
bool bluetooth_active = false;
bool grid_active = false;
//bool voice_control_active = false;
uint8_t current_lang = 0;
bool show_zoom = true;

//global
lv_style_t style_font_default_36;
lv_style_t style_font_default_30;
lv_style_t style_font_default_24;
lv_style_t style_swipe_icon;
lv_style_t style_scrollbar;
lv_style_t style_cont_transparent;


/**TEST**/
 void test_error_check(); //UNUSED

/**TEST**/
void  test_error_check(){
    printf("\nERROR\n");
}


/**initialize**/

static void initializ_styles(){
    lv_style_init(&style_font_default_36);
    lv_style_set_text_font(&style_font_default_36, &font_36);
    lv_style_set_text_color(&style_font_default_36, lv_color_white());
    lv_style_set_align(&style_font_default_36, LV_ALIGN_CENTER);
    lv_style_set_text_opa(&style_font_default_36, LV_OPA_100);

    lv_style_init(&style_font_default_30);
    lv_style_set_text_font(&style_font_default_30, &font_30);
    lv_style_set_text_color(&style_font_default_30, lv_color_white());
    lv_style_set_align(&style_font_default_30, LV_ALIGN_CENTER);
    lv_style_set_text_opa(&style_font_default_30, LV_OPA_100);

    lv_style_init(&style_font_default_24);
    lv_style_set_text_font(&style_font_default_24, &font_24);
    lv_style_set_text_color(&style_font_default_24, lv_color_white());
    lv_style_set_align(&style_font_default_24, LV_ALIGN_CENTER);
    lv_style_set_text_opa(&style_font_default_24, LV_OPA_100);

    lv_style_init(&style_swipe_icon);
    lv_style_set_size(&style_swipe_icon, 100, 10);
    lv_style_set_align(&style_swipe_icon, LV_ALIGN_BOTTOM_MID);
    lv_style_set_bg_color(&style_swipe_icon, BG_COLOR_DARK_GREY);
    lv_style_set_border_width(&style_swipe_icon, 0);

    lv_style_init(&style_scrollbar);
    lv_style_set_bg_color(&style_scrollbar, lv_color_white());
    lv_style_set_width(&style_scrollbar, 8);
    lv_style_set_radius(&style_scrollbar, 0);


    lv_style_init(&style_cont_transparent);
   lv_style_set_bg_opa(&style_cont_transparent, LV_OPA_0);
//   lv_style_set_opa(&style_cont_transparent, LV_OPA_0);
 //   lv_obj_set_style_bg_opa(cont, LV_OPA_0, LV_PART_MAIN);
    lv_style_set_align(&style_cont_transparent, LV_ALIGN_CENTER);
    lv_style_set_border_width(&style_cont_transparent, 0);
//    lv_style_set_bg_color(&style_cont_transparent, BG_COLOR_DARK_GREY);
}

static void initialize_screens(){
    create_scr_poweroff();
    create_scr_zoom();
    create_scr_menu();
    create_scr_pic_library();
    create_scr_resolution();
    create_scr_media_settings();
    create_scr_media_selection();
    create_scr_mode_selection(0);
    create_scr_resolution_pro();

    create_scr_menu_settings();
    //create_scr_menu_bluetooth();

    create_scr_home();
}

void my_demo_2_create() {
    initializ_styles();
    initialize_ui();
    initialize_screens();


    /**driver code**/
    open_scr_home_cb();
}
