#ifndef UI_ROLLER_H_INCLUDED
#define UI_ROLLER_H_INCLUDED

#include "lvgl.h"

/**create roller**/
void create_cam_mode_roller(lv_obj_t *parent, int cur_mode);

/**get modes**/
int get_selected_mode_from_roller(void);

#endif // UI_ROLLER_H_INCLUDED
