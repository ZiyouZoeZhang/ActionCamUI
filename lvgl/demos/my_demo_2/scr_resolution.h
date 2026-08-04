#ifndef SCR_RESOLUTION_H_INCLUDED
#define SCR_RESOLUTION_H_INCLUDED

extern lv_obj_t *scr_resolution;
extern lv_obj_t *scr_resolution_pro;
extern const int cam_resolution_table[];

void open_scr_resolution_cb();
void create_scr_resolution();
void create_scr_resolution_pro();

int get_current_gyro_eis();

extern bool pal_ntsc;

int get_selected_resolution_from_roller(void);

#endif // SCR_RESOLUTION_H_INCLUDED
