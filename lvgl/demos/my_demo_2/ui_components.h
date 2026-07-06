#ifndef UI_COMPONENTS_H_INCLUDED
#define UI_COMPONENTS_H_INCLUDED

#include "lvgl.h"
#include "camera_modes.h"

/**create icons**/
void create_sd_icon(lv_obj_t *parent, int status, float storage);
void create_battery_icon(lv_obj_t *parent, bool charging, int level);
void create_pic_lib_icon(lv_obj_t *parent);
void create_cam_mode_icon(lv_obj_t * parent, int mode);
void create_exit_icon(lv_obj_t * parent);
void create_media_set_icon(lv_obj_t * parent);
void create_buttons(lv_obj_t * parent);
void create_resolution_icon(lv_obj_t * parent, int cur_res);
void create_zoom_icon(lv_obj_t * parent, int cur_zoom);
void create_grid(lv_obj_t * parent);
void create_spot_metering(lv_obj_t * parent);
void create_pic_select_icon(lv_obj_t * parent);
//void create_pro_scroll(lv_obj_t * parent);

/**updates**/
void update_battery_icon(bool charging, int level);
void update_sd_icon(int status, float storage);
void update_cam_mode_icon(int mode);
void update_spot_metering_cb(lv_event_t * e);

/**CB**/
void open_scr_cam_modes_cb(lv_event_t * e);
void open_scr_cam_modes_by_mode(int mode);
void open_scr_home_cb();
void open_scr_pic_lib_cb();

#endif // UI_COMPONENTS_H_INCLUDED
