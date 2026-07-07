#ifndef SCR_HOME_H_INCLUDED
#define SCR_HOME_H_INCLUDED

typedef struct {
    lv_obj_t *screen;
    lv_obj_t *cam_mode_icon;
    lv_obj_t *resolution_icon;
    lv_obj_t *zoom_icon;
    lv_obj_t *battery_icon;
    lv_obj_t *sd_icon;
    lv_obj_t *spot_metering_icon;
} home_ui_t;

extern home_ui_t g_home;

#endif // SCR_HOME_H_INCLUDED
