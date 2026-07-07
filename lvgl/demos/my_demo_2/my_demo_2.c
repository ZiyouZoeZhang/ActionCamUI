#include "my_demo_2.h"

/**VARIABLES FOR INPUT**/
int battery_level = 30;
bool battery_charging = true;
int sd_status = 1; //0 = none, 1 = available, -1 = error
float sd_storage = 26.3;
int cur_cam_mode = 0;
int cur_cam_resolution = 0;
int cur_cam_zoom = 0;
bool wifi_active = false;

//global
lv_obj_t *scr_home = NULL;
lv_obj_t *scr_poweroff = NULL;

lv_style_t style_font_default_36;
lv_style_t style_font_default_30;
lv_style_t style_font_default_24;

lv_style_t style_swipe_icon;
lv_style_t style_scrollbar;

/**static SCREENS**/
static void swipe_scr_main_cb(lv_event_t *e);

/**TEST**/
 void test_error_check(); //UNUSED

/**TEST**/
void  test_error_check(){
    printf("\nERROR\n");
}

/**CB functions**/


/**Access of Screens**/
void open_scr_poweroff_cb(){
    lv_screen_load(scr_poweroff);
}

/**SCREENS**/
void create_scr_poweroff(){
    scr_poweroff = lv_obj_create(NULL);
    lv_obj_t * img_xtu_poweroff = lv_image_create(scr_poweroff);
    lv_image_set_src(img_xtu_poweroff, &xtu_poweroff);

    lv_obj_remove_flag(scr_poweroff, LV_OBJ_FLAG_SCROLLABLE);
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
}

static void initialize_screens(){
    create_scr_home();
    create_scr_poweroff();
}

void my_demo_2_create() {
    initializ_styles();
    initialize_screens();

    /**driver code**/
    open_scr_home_cb();

    ///TEST
    //open_scr_menu_cb();

}
