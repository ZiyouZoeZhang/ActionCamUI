#include "camera_modes.h"
#include "scr_media_settings.h"
#include "lvgl_ex_language_string.h"

/*
static const media_set_t raw_non_pro_features[] = {
    CAM_MEDIA_NUMBER,
    CAM_MEDIA_SELF_TIMER,
    CAM_MEDIA_LDC
};

static const media_set_t long_exp_non_pro_features[] = {
    CAM_MEDIA_NUMBER,
    CAM_MEDIA_SELF_TIMER,
    CAM_MEDIA_LDC,
    CAM_MEDIA_EXPOSURE
};

static const media_set_t continuous_non_pro_features[] = {
    CAM_MEDIA_NUMBER,
    CAM_MEDIA_SELF_TIMER,
    CAM_MEDIA_LDC
};

static const media_set_t timer_non_pro_features[] = {
    CAM_MEDIA_NUMBER,
    CAM_MEDIA_SELF_TIMER,
    CAM_MEDIA_LDC
};

static const media_set_t timelapse_non_pro_features[] = {
    CAM_MEDIA_NUMBER,
    CAM_MEDIA_LAPSE,
    CAM_MEDIA_SELF_TIMER,
    CAM_MEDIA_LDC
};

static const media_set_t photo_non_pro_features[] = {
    CAM_MEDIA_NUMBER,
    CAM_MEDIA_LAPSE,
    CAM_MEDIA_SELF_TIMER,
    CAM_MEDIA_LDC
};

// ============ 3. 定义各模式的专业功能列表 ============
static const media_set_t raw_pro_features[] = {
    CAM_MEDIA_METER_MODE,
    CAM_MEDIA_EXPOSURE,
    CAM_MEDIA_SHUTTER,
    CAM_MEDIA_ISO
};

static const media_set_t long_exp_pro_features[] = {
    CAM_MEDIA_METER_MODE,
    CAM_MEDIA_SHUTTER
};

static const media_set_t continuous_pro_features[] = {
    CAM_MEDIA_METER_MODE,
    CAM_MEDIA_EXPOSURE,
    CAM_MEDIA_ISO,
    CAM_MEDIA_AWB,
    CAM_MEDIA_SCENE_MODE,
    CAM_MEDIA_SHARPNESS,
    CAM_MEDIA_FILTER
};

static const media_set_t timer_pro_features[] = {
    CAM_MEDIA_METER_MODE,
    CAM_MEDIA_EXPOSURE,
    CAM_MEDIA_ISO,
    CAM_MEDIA_AWB,
    CAM_MEDIA_SCENE_MODE,
    CAM_MEDIA_SHARPNESS,
    CAM_MEDIA_FILTER
};

static const media_set_t timelapse_pro_features[] = {
    CAM_MEDIA_METER_MODE,
    CAM_MEDIA_EXPOSURE,
    CAM_MEDIA_ISO,
    CAM_MEDIA_AWB,
    CAM_MEDIA_SCENE_MODE,
    CAM_MEDIA_SHARPNESS,
    CAM_MEDIA_FILTER
};

static const media_set_t photo_pro_features[] = {
    CAM_MEDIA_METER_MODE,
    CAM_MEDIA_EXPOSURE,
    CAM_MEDIA_SHUTTER,
    CAM_MEDIA_ISO,
    CAM_MEDIA_AWB,
    CAM_MEDIA_SCENE_MODE,
    CAM_MEDIA_SHARPNESS,
    CAM_MEDIA_FILTER
};
*/


// ============ 4. 辅助宏 ============

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

// 0 = not supported, 1 = normal supproted, 2 = pro supported
const uint8_t supported_setting[CAM_MODE_COUNT][CAM_MEDIA_COUNT] = {
    // CAM_MODE_RAW
    {
        1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0
    },
    // CAM_MODE_LONG_EXPOSURE
    {
        1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    },
    // CAM_MODE_CONTINUOUS
    {
        1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 2, 2, 0, 2, 2, 2, 2, 0, 2, 0, 0, 0, 0
    },
    // CAM_MODE_TIMER
    {
        1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 2, 2, 0, 2, 2, 2, 2, 0, 2, 0, 0, 0, 0
    },
    // CAM_MODE_TIMELAPSE (照片)
    {
        1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 2, 2, 0, 2, 2, 2, 2, 0, 2, 0, 0, 0, 0
    },
    // CAM_MODE_PHOTO
    {
        1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 2, 0, 0, 0, 0
    },
    // CAM_MODE_VIDEO
    {
        0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 2, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
    },
    // CAM_MODE_TIMELAPSE_VIDEO
    {
        0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0
    },
    // CAM_MODE_SLOW_MOTION
    {
        0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 2
    },
    // CAM_MODE_SHORT_VIDEO
    {
        0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 2
    },
    // CAM_MODE_UNDERWATER
    {
        0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 0, 2, 2, 0, 0, 0, 0, 2
    },
    // CAM_MODE_CAR_LOOP
    {
        0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
    },
    // CAM_MODE_VIDEO_PHOTO
    {
        0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 2, 2, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 2
    },
    // CAM_MODE_NIGHT
    {
        0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
    },
};

// ============ 3. 辅助宏和函数 ============


// 获取支持级别
static inline uint8_t get_support(camera_mode_t mode, media_set_t setting) {
    if (mode >= CAM_MODE_COUNT || setting >= CAM_MEDIA_COUNT) return 0;
    return supported_setting[mode][setting];
}

// 获取当前模式支持的所有设置列表
int get_supported_list(camera_mode_t mode, bool pro_mode, media_set_t *out_list) {
    int count = 0;
    uint8_t min_level = pro_mode ? 2 : 1;

    for (int i = 0; i < CAM_MEDIA_COUNT; i++) {
        if (supported_setting[mode][i] >= min_level) {
            out_list[count++] = (media_set_t)i;
        }
    }
    return count;
}

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
