#include "my_demo_2.h"
#define PICTURE_H 183
#define PICTURE_W 244

static lv_obj_t * scr_pic_library = NULL;

static void create_scr_pic_library(){
    /**background**/
    scr_pic_library = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_pic_library,BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);
    create_exit_icon(scr_pic_library);
    create_pic_select_icon(scr_pic_library);

    /**container **/
    lv_obj_t * cont_pics = lv_obj_create(scr_pic_library);
    lv_obj_set_size(cont_pics, lv_pct(100), lv_pct(80));
    lv_obj_align(cont_pics, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_opa(cont_pics, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(cont_pics, 0, LV_PART_MAIN);

    lv_obj_add_style(cont_pics, &style_scrollbar, LV_PART_SCROLLBAR);

    //create row & columns
    static lv_coord_t col_dsc[] = {PICTURE_W, PICTURE_W, PICTURE_W, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {PICTURE_H, PICTURE_H, PICTURE_H, PICTURE_H, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(cont_pics, col_dsc, row_dsc);

    lv_obj_set_style_pad_top(cont_pics, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(cont_pics, 50, LV_PART_MAIN);
    lv_obj_set_style_pad_left(cont_pics, 10, LV_PART_MAIN);

   lv_obj_set_style_pad_column(cont_pics, 10, LV_PART_MAIN);
   lv_obj_set_style_pad_row(cont_pics, 10, LV_PART_MAIN);

   for (int i = 0; i <  get_storage_image_count() ; i++) {
        lv_obj_t *btn = lv_btn_create(cont_pics);
        lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, i % 3, 1, LV_GRID_ALIGN_STRETCH, i / 3, 1);
        lv_obj_set_style_bg_color(btn, BG_COLOR_DARK_GREY, LV_PART_MAIN);
        lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);

        lv_obj_t *mode_icon = lv_image_create(btn);
        lv_image_set_src(mode_icon, mode_table[storage_images[i].mode].filelist_icon_src);
        lv_obj_align(mode_icon, LV_ALIGN_BOTTOM_LEFT, 0, 0);
   }
}

void open_scr_pic_lib_cb(){
    create_scr_pic_library();
    lv_screen_load(scr_pic_library);
}

