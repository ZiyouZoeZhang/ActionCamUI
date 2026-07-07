#ifndef IMAGE_STORAGE_H_INCLUDED
#define IMAGE_STORAGE_H_INCLUDED


#include "lvgl.h"
#include "camera_modes.h"

typedef struct {
    lv_obj_t *btn;
    lv_obj_t *select_icon;
    lv_obj_t *mode_icon;
    bool selected ;
    camera_mode_t mode;
} image_info_t;

extern image_info_t storage_images[];

int get_storage_image_count(void);
void create_image_btn(lv_obj_t * parent, int index);


#endif // IMAGE_STORAGE_H_INCLUDED
