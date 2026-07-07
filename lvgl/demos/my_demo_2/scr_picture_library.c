#include "my_demo_2.h"
#define PICTURE_H 183
#define PICTURE_W 244

static lv_obj_t * scr_pic_library = NULL;
static bool select_mode = false;
static int selected_pic_number = 0;

static lv_obj_t * icon_top_right = NULL;

static void image_toggle_state_cb(lv_event_t * e);
static void image_clicked_cb(lv_event_t * e);

static void top_right_icon_toggled_cb(){
    select_mode = !select_mode;
    if (select_mode){ //now starting to select pictuures
        for(int i = 0; i<get_storage_image_count(); i++){
            storage_images[i].selected = false;
            lv_obj_remove_flag(storage_images[i].select_icon, LV_OBJ_FLAG_HIDDEN);
        }
        selected_pic_number = 0;
    } else { //
        if (selected_pic_number > 0) {
            //delete these picture
            printf("DELETING PICS...");
        } else {
            for(int i = 0; i<get_storage_image_count(); i++){
                lv_obj_add_flag(storage_images[i].select_icon, LV_OBJ_FLAG_HIDDEN);
            }
        }
    }
}

static void create_scr_pic_library(){
    /**preset**/
    select_mode = false;
    selected_pic_number = 0;

    /**background**/
    scr_pic_library = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_pic_library,BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);

    create_exit_icon(scr_pic_library);
    icon_top_right = create_pic_select_icon(scr_pic_library);
    lv_obj_add_event_cb(icon_top_right, top_right_icon_toggled_cb, LV_EVENT_CLICKED, NULL);

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

   //individual pictures
   for (int i = 0; i <  get_storage_image_count() ; i++) {
       create_image_btn(cont_pics, i);
   }
}


void create_image_btn(lv_obj_t * parent, int index){
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
    lv_obj_add_flag(image_obj->select_icon, LV_OBJ_FLAG_HIDDEN);

    if (storage_images[index].selected) {
        lv_image_set_src(image_obj->select_icon,&filelist_selected);
    } else {
        lv_image_set_src(image_obj->select_icon,&filelist_selecting);
    }

    //mode icon
    image_obj->mode_icon = lv_image_create(image_obj->btn);
    lv_image_set_src(image_obj->mode_icon, mode_table[storage_images[index].mode].filelist_icon_src);
    lv_obj_align(image_obj->mode_icon, LV_ALIGN_BOTTOM_LEFT, 0, 0);

    return;
}


static void image_clicked_cb(lv_event_t * e) {
    int index = (int)(intptr_t)lv_event_get_user_data(e);

    if (!select_mode) {
        printf("ENTER PIC LARGE");
    }

    if (select_mode) {
        storage_images[index].selected = !storage_images[index].selected;

        //update select icon
        lv_obj_t *btn = lv_event_get_target(e);
        lv_obj_t *select_icon = lv_obj_get_child(btn, 0);
        if (storage_images[index].selected) {
            lv_image_set_src(select_icon, &filelist_selected);
        } else {
            lv_image_set_src(select_icon, &filelist_selecting);
        }

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

void open_scr_pic_lib_cb(){
    create_scr_pic_library();
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
