#ifndef IMAGE_STORAGE_H_INCLUDED
#define IMAGE_STORAGE_H_INCLUDED

#include "lvgl.h"
#include "camera_modes.h"



typedef struct {
    //const void *img_src;
    //const char *name;
    camera_mode_t mode;
} image_info_t;


static image_info_t storage_images[] = {
    {
        //.img_src = &img_photo,
        //.name = "Photo",
        .mode = CAM_MODE_PHOTO,
    },
    {
        //.img_src = &img_photo,
        //.name = "Photo",
        .mode = CAM_MODE_VIDEO,
    },
    {
        //.img_src = &img_photo,
        //.name = "Photo",
        .mode = CAM_MODE_TIMELAPSE,
    },
    {
        //.img_src = &img_photo,
        //.name = "Photo",
        .mode = CAM_MODE_LONG_EXPOSURE,
    },
    {
        //.img_src = &img_photo,
        //.name = "Photo",
        .mode = CAM_MODE_NIGHT,
    },
    {
        //.img_src = &img_photo,
        //.name = "Photo",
        .mode = CAM_MODE_UNDERWATER,
    },
    {
        //.img_src = &img_photo,
        //.name = "Photo",
        .mode = CAM_MODE_SLOW_MOTION,
    }
};


static inline int get_storage_image_count(void) {
    return sizeof(storage_images) / sizeof(storage_images[0]);
}

#endif // IMAGE_STORAGE_H_INCLUDED
