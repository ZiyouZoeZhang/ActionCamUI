#include "my_demo_2.h"
#define PICTURE_H 183
#define PICTURE_W 244
static lv_obj_t * scr_pic_library = NULL;
static lv_obj_t * scr_pic_large = NULL;
static bool select_mode = false;
static int selected_pic_number = 0;
static lv_obj_t * icon_top_right = NULL;
static lv_obj_t * cont_pics = NULL;

static int current_pic_index = 0;
static int total_pic_count = 0;

static bool individual_image_opened = true;

static void image_clicked_cb(lv_event_t * e);
static void delete_images_cb();
static void create_image_btn(lv_obj_t * parent);
static void open_scr_delete_images();
static void swipe_scr_img_large_cb(lv_event_t *e);
static void delete_image_large(int index);

static void reset_image_select_icon(int index, bool hide){
    if (hide){
        lv_obj_add_flag(storage_images[index].select_icon, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_remove_flag(storage_images[index].select_icon, LV_OBJ_FLAG_HIDDEN);
    }
    if (storage_images[index].selected) {
        lv_image_set_src(storage_images[index].select_icon,&filelist_selected);
    } else {
        lv_image_set_src(storage_images[index].select_icon,&filelist_selecting);
    }
}

static void top_right_icon_toggled_cb(){
    select_mode = !select_mode;
    if (select_mode){ //now starting to select pictures
        lv_image_set_src(icon_top_right, &filelist_multiselect);
        for(int i = 0; i<get_storage_image_count(); i++){
            storage_images[i].selected = false;
            reset_image_select_icon(i, false);
        }
        selected_pic_number = 0;
        return;
    }

    if (!select_mode) { //delete mode
        if (selected_pic_number > 0) { //delete these picture
            open_scr_delete_images();
            return;
        }
        for (int i = 0; i<get_storage_image_count(); i++){ //switch select to off
            reset_image_select_icon(i, true);
        }
    }
    return;
}

static lv_obj_t * scr_delete;

static void cancle_clicked_cb(){
    if (individual_image_opened){
        lv_screen_load(scr_pic_large);
    } else {
        open_scr_pic_lib_cb();
    }
}

static void confirm_clicked_cb(){
    delete_images_cb();
    cancle_clicked_cb();
}

void create_scr_delete(){
    scr_delete = lv_obj_create(NULL);
     lv_obj_set_style_bg_color(scr_delete,BG_COLOR_VERY_DARK_GREY, LV_PART_MAIN);

    lv_obj_t *label = lv_label_create(scr_delete);
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    lv_obj_add_style(label, &style_font_default_36, LV_PART_MAIN);
    lv_obj_set_width(label, 550);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(label, _(STRING_DIALOG_Delete_ASK));
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, lv_pct(30));
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    lv_obj_t *btn_cancel = create_btn_cancle(scr_delete);
    lv_obj_add_event_cb(btn_cancel, cancle_clicked_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btn_confirm = create_btn_confirm(scr_delete);
    lv_obj_add_event_cb(btn_confirm, confirm_clicked_cb, LV_EVENT_CLICKED, NULL);
}


static void open_scr_delete_images(){
    lv_screen_load(scr_delete);
}

static void delete_images_cb(){
    //printf("deleting\n");
    if (individual_image_opened){
            delete_image_large(current_pic_index);
    } else {
         for (int index = 0; index <  get_storage_image_count() ; index++) {
                if (storage_images[index].selected) {
                        ///delete image from storage_images
                }
                storage_images[index].selected = false;
                reset_image_select_icon(index, true);
         }
         lv_image_set_src(icon_top_right, &filelist_multiselect);
    }
}


void create_scr_pic_library(){
    /**preset**/
    select_mode = false;
    selected_pic_number = 0;

    /**background**/
    scr_pic_library = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_pic_library,BG_COLOR_VERY_DARK_GREY, LV_PART_MAIN);

    create_exit_icon(scr_pic_library);
    icon_top_right = create_pic_select_icon(scr_pic_library);
    lv_obj_add_event_cb(icon_top_right, top_right_icon_toggled_cb, LV_EVENT_CLICKED, NULL);

    /**container **/
    cont_pics = lv_obj_create(scr_pic_library);
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

   //individual pictures
   create_image_btn(cont_pics);
}

static void create_image_btn(lv_obj_t * parent){
        for (int index = 0; index <  get_storage_image_count() ; index++) {
            image_info_t *image_obj = &storage_images[index];

            //image
            image_obj->btn = lv_btn_create(parent);
            lv_obj_set_style_bg_color(image_obj->btn, BG_COLOR_DARK_GREY, LV_PART_MAIN);
            lv_obj_set_style_shadow_width(image_obj->btn, 0, LV_PART_MAIN);
            lv_obj_set_grid_cell(image_obj->btn, LV_GRID_ALIGN_STRETCH, index % 3, 1, LV_GRID_ALIGN_STRETCH, index / 3, 1);
            lv_obj_add_event_cb(image_obj->btn, image_clicked_cb, LV_EVENT_CLICKED,  (void*)(intptr_t)index);

            //select icon
            image_obj->select_icon = lv_image_create(image_obj->btn);
            lv_obj_align(image_obj->select_icon, LV_ALIGN_TOP_LEFT, -3, -5);
            lv_obj_add_flag(image_obj->select_icon, LV_OBJ_FLAG_CLICKABLE);
            reset_image_select_icon(index, true);

            //mode icon
            image_obj->mode_icon = lv_image_create(image_obj->btn);
            lv_image_set_src(image_obj->mode_icon, mode_table[storage_images[index].mode].filelist_icon_src);
            lv_obj_align(image_obj->mode_icon, LV_ALIGN_BOTTOM_LEFT, 0, 0);

        }
        return;
}

static void image_clicked_cb(lv_event_t * e) {
    int index = (int)(intptr_t)lv_event_get_user_data(e);
    if (!select_mode) {
        open_pic_large_cb(index);
    }

    if (select_mode) {
        storage_images[index].selected = !storage_images[index].selected;
        reset_image_select_icon(index, false);
        //update icon top right & number of selected pics
        if (storage_images[index].selected){
            ++selected_pic_number;
            if (selected_pic_number > 0) lv_image_set_src(icon_top_right, &playback_filemanager);
        } else {
            --selected_pic_number;
            if (selected_pic_number == 0) lv_image_set_src(icon_top_right, &filelist_multiselect);
        }
    }
}

lv_obj_t * exit_icon_pic_large = NULL;
lv_obj_t * cam_icon_pic_large = NULL;
lv_obj_t * label_index_pic_large = NULL;
lv_obj_t * delete_icon_pic_large = NULL;

void create_scr_pic_large(){
    scr_pic_large = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_pic_large,BG_COLOR_VERY_DARK_GREY, LV_PART_MAIN);
    lv_obj_add_event_cb(scr_pic_large, swipe_scr_img_large_cb, LV_EVENT_RELEASED, NULL);

    exit_icon_pic_large = create_exit_icon(scr_pic_large);
    lv_obj_remove_event_cb(exit_icon_pic_large, open_scr_home_cb);
    lv_obj_add_event_cb(exit_icon_pic_large, open_scr_pic_lib_cb, LV_EVENT_CLICKED, NULL);

    //  右上角delete
    delete_icon_pic_large = create_pic_select_icon(scr_pic_large);
     lv_image_set_src(delete_icon_pic_large, &playback_filemanager);

  //   open_scr_delete_images
   lv_obj_add_event_cb(delete_icon_pic_large, open_scr_delete_images, LV_EVENT_CLICKED, NULL);

    // 左下image
    cam_icon_pic_large = lv_image_create(scr_pic_large);
    lv_obj_align(cam_icon_pic_large, LV_ALIGN_BOTTOM_LEFT, lv_pct(3),  lv_pct(-5));

    // 中上index
    label_index_pic_large = lv_label_create(scr_pic_large);
    lv_obj_add_style(label_index_pic_large, &style_font_default_36, LV_PART_MAIN);
    lv_obj_align(label_index_pic_large, LV_ALIGN_TOP_MID, 0, lv_pct(5));

}


static void swipe_scr_img_large_cb(lv_event_t *e) {
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());

    int new_index = current_pic_index;

    switch(dir) {
        case LV_DIR_TOP:
            new_index++;
            break;
        case LV_DIR_BOTTOM:
            new_index--;
        default:
            break;
    }

    if (new_index >= 0 && new_index < total_pic_count) {
        open_pic_large_cb(new_index);
    }
}

static void delete_image_large(int index) {

    int new_index = index;

    if (index+1 < total_pic_count){
        open_pic_large_cb(index+1);
        return;
    } else if (index-1 >=0) {
        open_pic_large_cb(index-1);
        return;
    } else {
        open_scr_pic_lib_cb();
        return;
    }
}

void open_pic_large_cb(int index) {
    individual_image_opened = true;
    total_pic_count = get_storage_image_count();
    current_pic_index = index;

    lv_image_set_src(cam_icon_pic_large, mode_table[storage_images[index].mode].filelist_icon_src);

    char buf[32];
    snprintf(buf, sizeof(buf), "%d/%d", index + 1, total_pic_count);
    lv_label_set_text(label_index_pic_large, buf);

    lv_screen_load(scr_pic_large);
}

void open_scr_pic_lib_cb(){
    individual_image_opened = false;
    lv_screen_load(scr_pic_library);
}

image_info_t storage_images[] = {
    { .selected = false, .mode = CAM_MODE_PHOTO, .btn = NULL, .select_icon = NULL, .mode_icon = NULL },
    { .selected = false, .mode = CAM_MODE_VIDEO, .btn = NULL, .select_icon = NULL, .mode_icon = NULL },
    { .selected = false, .mode = CAM_MODE_TIMELAPSE, .btn = NULL, .select_icon = NULL, .mode_icon = NULL },
    { .selected = false, .mode = CAM_MODE_LONG_EXPOSURE, .btn = NULL, .select_icon = NULL, .mode_icon = NULL },
    { .selected = false, .mode = CAM_MODE_NIGHT, .btn = NULL, .select_icon = NULL, .mode_icon = NULL },
    { .selected = false, .mode = CAM_MODE_UNDERWATER, .btn = NULL, .select_icon = NULL, .mode_icon = NULL },
    { .selected = false, .mode = CAM_MODE_SLOW_MOTION, .btn = NULL, .select_icon = NULL, .mode_icon = NULL }
};

int get_storage_image_count(void) {
    return sizeof(storage_images) / sizeof(storage_images[0]);
}
