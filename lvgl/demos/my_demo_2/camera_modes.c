#include "camera_modes.h"
#include "lvgl_ex_language_string.h"

const camera_mode_info_t mode_table[] = {
    {CAM_MODE_RAW,            "Raw",     &mode_rawphoto_normal, &filelist_rawphoto},
    {CAM_MODE_LONG_EXPOSURE,  "Long Exposure",  &mode_longexposurephoto_normal, &filelist_longexposurephoto},
    {CAM_MODE_CONTINUOUS,     "Continuous",    &mode_burstphoto_normal, &filelist_burstphoto},
    {CAM_MODE_TIMER,          "Timer",    &mode_delayphoto_normal, &filelist_delayphoto},
    {CAM_MODE_TIMELAPSE,      "Timelapse",    &mode_lapsephoto_normal, &filelist_lapsephoto},
    {CAM_MODE_PHOTO,          "Photo",    &mode_normphoto_normal, &filelist_normphoto},

    {CAM_MODE_VIDEO,          "Record",    &mode_normrec_normal, &filelist_normrec},
    {CAM_MODE_TIMELAPSE_VIDEO,          "Timelapse Video",    &mode_lapserec_normal, &filelist_lapserec},

    {CAM_MODE_SLOW_MOTION,    "Slow Motion", &mode_slowrec_normal, &filelist_slowrec},
    {CAM_MODE_SHORT_VIDEO,    "Short Video",  &mode_shortrec_normal, &filelist_shortrec},

    {CAM_MODE_UNDERWATER,     "Underwater",    &mode_underwaterrec_normal, &filelist_underwaterrec},
    {CAM_MODE_CAR_LOOP,       "Car loop",    &mode_looprec_normal, &filelist_looprec},
    {CAM_MODE_VIDEO_PHOTO,    "Video photo", &mode_recsnap_normal, &filelist_recsnap},
    {CAM_MODE_NIGHT,          "Night",    &mode_nightrec_normal, &filelist_nightrec}
};

int mode_names[] = {
    STRING_PHOTO_RAW,
    STRING_PHOTO_LONGEX,
    STRING_MULTI_BURST,
    STRING_PHOTO_DELAY,
    STRING_MULTI_LAPSE,
    STRING_PHOTO_NORMAL,
    STRING_RECORD_NORMAL,
    STRING_RECORD_LAPSE,
    STRING_RECORD_SLOW,
    STRING_RECORD_SHORT,
    STRING_RECORD_UNDERWATER,
    STRING_RECORD_CAR,
    STRING_RECORD_SNAP,
    STRING_RECORD_NIGHT
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
