#ifndef SCR_RESOLUTION_H_INCLUDED
#define SCR_RESOLUTION_H_INCLUDED

extern lv_obj_t *scr_resolution;
extern const char* cam_resolution_table[];

void open_scr_resolution_cb();
void create_scr_resolution();

int get_selected_resolution_from_roller(void);

#endif // SCR_RESOLUTION_H_INCLUDED
