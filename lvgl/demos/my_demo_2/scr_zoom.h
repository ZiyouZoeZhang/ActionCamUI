#ifndef SCR_ZOOM_H_INCLUDED
#define SCR_ZOOM_H_INCLUDED


extern lv_obj_t * scr_zoom;
//extern const char* cam_zoom_table[];

void open_scr_zoom_cb();
void create_scr_zoom();

int get_selected_zoom_from_roller(void);

#endif // SCR_ZOOM_H_INCLUDED
