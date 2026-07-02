#ifndef SCR_ZOOM_H_INCLUDED
#define SCR_ZOOM_H_INCLUDED


extern lv_obj_t *zoom;

void open_scr_zoom_cb();
extern const char* cam_zoom_table[];

int get_selected_zoom_from_roller(void);


#endif // SCR_ZOOM_H_INCLUDED
