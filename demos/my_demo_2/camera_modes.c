#include "camera_modes.h"
#include "lvgl.h"
#include <string.h>
#include <stdio.h>
#include "pic_converted_transparent/image_declares.h"

static const camera_mode_info_t mode_table[] = {
    {CAM_MODE_RAW,            "Raw",     &mode_rawphoto_normal},
    {CAM_MODE_LONG_EXPOSURE,  "Long Exposure",  &mode_longexposurephoto_normal},
    {CAM_MODE_CONTINUOUS,     "Continuous",    &mode_burstphoto_normal},
    {CAM_MODE_TIMER,          "Timer",    &mode_delayphoto_normal},
    {CAM_MODE_TIMELAPSE,      "Timelapse",    &mode_lapsephoto_normal},
    {CAM_MODE_PHOTO,          "Photo",    &mode_normphoto_normal},

    {CAM_MODE_VIDEO,          "Record",    &mode_normrec_normal},
    {CAM_MODE_TIMELAPSE_VIDEO,          "Timelapse Video",    &mode_lapserec_normal},

    {CAM_MODE_SLOW_MOTION,    "Slow Motion", &mode_slowrec_normal},
    {CAM_MODE_SHORT_VIDEO,    "Short Video",  &mode_shortrec_normal},

    {CAM_MODE_UNDERWATER,     "Underwater",    &mode_underwaterrec_normal},
    {CAM_MODE_CAR_LOOP,       "Car loop",    &mode_looprec_normal},
    {CAM_MODE_VIDEO_PHOTO,    "Video photo", &mode_recsnap_normal},
    {CAM_MODE_NIGHT,          "Night",    &mode_nightrec_normal}
};

// GET All mode and return table
const camera_mode_info_t* get_all_modes(void) {
    return mode_table;
}

// mode count
int get_mode_count(void) {
    return CAM_MODE_COUNT;
}

// return mode name
const char* get_mode_name(camera_mode_t mode) {
    if (mode >= 0 && mode < CAM_MODE_COUNT) {
        return mode_table[mode].name;
    }
    return "mode out of bounds";
}

// get image scr
const void* get_mode_icon(camera_mode_t mode) {
    if (mode >= 0 && mode < CAM_MODE_COUNT) {
        return mode_table[mode].icon_src;
    }
    return NULL;
}
