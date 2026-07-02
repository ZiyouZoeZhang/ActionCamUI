#ifndef SCR_RESOLUTION_H_INCLUDED
#define SCR_RESOLUTION_H_INCLUDED

extern lv_obj_t *scr_resolution;

void open_scr_resolution_cb();
extern const char* cam_resolution_table[];

int get_selected_resolution_from_roller(void);

#endif // SCR_RESOLUTION_H_INCLUDED
