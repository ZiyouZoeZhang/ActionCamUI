#include "my_demo_2.h"
#include "ui_components.h"
#include "scr_resolution.h"

#define CAM_RES_COUNT 7
#define CAM_RES_PRO_COUNT 10
#define CAM_FRAME_RATES_PRO_COUNT 5
#define CAM_GYRO_EIS_COUNT 7

lv_obj_t *scr_resolution = NULL;
lv_obj_t *scr_resolution_pro = NULL;
lv_obj_t *scr_gyro_selection = NULL;

//SCR RES
static lv_obj_t * res_roller = NULL;
static lv_obj_t * res_heading;

//SCR RESPRO
lv_obj_t * video_format_state = NULL;
static lv_obj_t * video_format_label = NULL;
bool pal_ntsc = true;

lv_obj_t * gyro_eis_state = NULL;
static lv_obj_t * gyro_eis_label = NULL;

static lv_obj_t * res_pro_roller = NULL;
static lv_obj_t * res_pro_heading;

static lv_obj_t * frame_rate_pro_roller = NULL;
static lv_obj_t * frame_rate_pro_heading;

//GYROEIS SCREEN
static lv_obj_t * gyro_eis_roller = NULL;
static lv_obj_t * gyro_eis_title;
static lv_obj_t * gyro_eis_text;

static void gyro_eis_roller_toggled_cb();

typedef struct {
    int cam_mode;
    uint8_t eis_count;      // 支持的EIS数量
    const int *eis_list;    // 指向EIS选项列表
} cam_supported_res_frame_eis_t;

const int cam_resolution_table[] = {
    STRING_48M,
    STRING_20M,
    STRING_16M,
    STRING_12M,
    STRING_8M,
    STRING_5M,
    STRING_3M
};

static const int eis_7_list[] = {
    STRING_OFF,
    STRING_DIS_NORMAL,
    STRING_DIS_SUPER,
    STRING_DIS_GYROFLOW,
    STRING_TILT_CORRECTION,
    STRING_HORIZON_STABILIZATION,
    STRING_360_HORIZON_CORRECTION
};

static const int eis_4_list[] = {
    STRING_OFF,
    STRING_DIS_NORMAL,
    STRING_DIS_SUPER,
    STRING_DIS_GYROFLOW
};

static const int eis_2_list[] = {
    STRING_OFF,
    STRING_DIS_GYROFLOW
};

int cam_gyro_eis_text_table[] = {
    STRING_DIS_OFF_TIPS,
    STRING_DIS_NORMAL_TIPS,
    STRING_DIS_SUPER_TIPS,
    STRING_DIS_GYROFLOW_TIPS,
    STRING_TILT_CORRECTION_TIPS,
    STRING_HORIZON_STABILIZATION_TIPS,
    STRING_360_HORIZON_CORRECTION_TIPS
};

const int cam_resolution_pro_table[] = {
    STRING_4K,
    STRING_4KS,
    STRING_4KH,
    STRING_2_7K,
    STRING_2_7KH,
    STRING_1440P_4_3,
    STRING_1080P,
    STRING_1080PS,
    STRING_1080PH,
    STRING_720P
};


static const bool supported_frame_rate_video[CAM_RES_PRO_COUNT][CAM_FRAME_RATES_PRO_COUNT] = {
    //30,   50,    60,    120,    240
    {true, true, false, false, false},  // 4K
    {true, false, false, false, false}, // 4KS
    {true, false, false, false, false}, // 4KH
    {true, true, false, false, false},  // 2.7K
    {true, false, false, false, false}, // 2.7KH
    {true, false, true, false, false},  // 1440P
    {true, false, true, true, false},   // 1080P
    {true, false, false, false, false}, // 1080PS
    {true, false, false, false, false}, // 1080PH
    {true, false, true, true, true}     // 720P
};


static const bool supported_fram_rate_slow_mo[3][CAM_FRAME_RATES_PRO_COUNT] = {
    //30,   50,    60,    120,    240
    {false, false, true, false, false},  // 1440P
    {false, false, true, true, false},   // 1080P
    {false, false, true, true, true}     // 720P
};


int res_frame_map[2][CAM_RES_PRO_COUNT][CAM_FRAME_RATES_PRO_COUNT] = {
{
    //30,   50,    60,    120,    240
    {STRING_4K25, STRING_4K50, 0, 0},  // STRING_4K
    {STRING_4K25_S, 0, 0, 0},  // STRING_4KS
    {STRING_4K25_HDR, STRING_4K50_HDR, 0, 0},  // STRING_4KH
    {STRING_2_7K25, STRING_2_7K50, 0, 0},  // STRING_2_7K
    {STRING_2_7K25_HDR, 0, 0, 0},  // STRING_2_7KH
    {STRING_1440P25, STRING_1440P50, 0, 0},  // STRING_1440P_4_3
    {STRING_1080P25, STRING_1080P50, STRING_1080P100, 0},  // STRING_1080P
    {STRING_1080P25_S, STRING_1080P50_S, 0, 0},  // STRING_1080PS
    {STRING_1080P25_HDR, STRING_1080P50_HDR, 0, 0},  // STRING_1080PH
    {STRING_720P25, STRING_720P50, STRING_720P100, 0}  // STRING_720P
},
{
    // 25, 50, 100, 200
    {STRING_4K25, STRING_4K50, 0, 0},  // STRING_4K
    {STRING_4K25_S, 0, 0, 0},  // STRING_4KS
    {STRING_4K25_HDR, STRING_4K50_HDR, 0, 0},  // STRING_4KH
    {STRING_2_7K25, STRING_2_7K50, 0, 0},  // STRING_2_7K
    {STRING_2_7K25_HDR, 0, 0, 0},  // STRING_2_7KH
    {STRING_1440P25, STRING_1440P50, 0, 0},  // STRING_1440P_4_3
    {STRING_1080P25, STRING_1080P50, STRING_1080P100, 0},  // STRING_1080P
    {STRING_1080P25_S, STRING_1080P50_S, 0, 0},  // STRING_1080PS
    {STRING_1080P25_HDR, STRING_1080P50_HDR, 0, 0},  // STRING_1080PH
    {STRING_720P25, STRING_720P50, STRING_720P100, 0}  // STRING_720P
}
};

const int cam_frame_rates_pro_table[2][CAM_FRAME_RATES_PRO_COUNT] = {
    {30, 50, 60, 120, 240},   //ntsc
    {25, 50, 50, 100, 200}    //pal
};

static void update_all_rollers(){

    int cur_res = lv_roller_get_selected(res_pro_roller);
    //update ntsc roller
    if (cur_cam_mode == CAM_MODE_SLOW_MOTION){
        if (pal_ntsc){
            update_roller_options_active(frame_rate_pro_roller, cam_frame_rates_pro_table[0], supported_fram_rate_slow_mo[cur_res], 5, true);
        } else {
            update_roller_options_active(frame_rate_pro_roller, cam_frame_rates_pro_table[1], supported_frame_rate_video[cur_res], 5, true);
        }
    } else {

    }
}

int get_selected_resolution_from_roller(void){
    return res_frame_map[0][lv_roller_get_selected(res_pro_roller)][lv_roller_get_selected(frame_rate_pro_roller)];

    if (cur_cam_mode >= CAM_MODE_VIDEO) {
        if(res_pro_roller){
                return res_frame_map[0][lv_roller_get_selected(res_pro_roller)][lv_roller_get_selected(frame_rate_pro_roller)];
            }
    } else {
        if(res_roller){
           // return cam_resolution_table[0][lv_roller_get_selected(res_roller)];
        }
    }
    return 0;
}

static void switch_video_format_cb(){
    pal_ntsc = !pal_ntsc;
    open_scr_resolution_cb();
}

static void open_scr_gyro_selection_cb(){
    //update_roller_options(gyro_eis_roller, cam_gyro_eis_table, CAM_GYRO_EIS_COUNT, false);

    ///UPDATE ROLLER OPTION
    gyro_eis_roller_toggled_cb();
    lv_label_set_text(gyro_eis_title, _(STRING_DIS));

    lv_screen_load(scr_gyro_selection);
}

void open_scr_resolution_cb(){

    ///update all roller options
    ///use update roller options
    update_all_rollers();

    //update all labels
    lv_label_set_text(frame_rate_pro_heading, _(STRING_FPS));
    lv_label_set_text(res_pro_heading, _(STRING_RESOLUTION));
    lv_label_set_text(gyro_eis_label, _(STRING_DIS));
    lv_label_set_text(gyro_eis_state, _(cam_gyro_eis_text_table[lv_roller_get_selected(gyro_eis_roller)]));
     lv_label_set_text(video_format_label, _(STRING_VIDEO_STANDARD));
    (pal_ntsc) ? lv_label_set_text(video_format_state, _(STRING_PAL)) : lv_label_set_text(video_format_state, _(STRING_NTSC));


    lv_screen_load(scr_resolution_pro);


    /*
    if (cur_cam_mode >= CAM_MODE_VIDEO) {
        lv_label_set_text(frame_rate_pro_heading, _(STRING_FPS));
        lv_label_set_text(res_pro_heading, _(STRING_RESOLUTION));
        lv_screen_load(scr_resolution_pro);
    } else{
        lv_label_set_text(res_heading, _(STRING_RESOLUTION));
        lv_screen_load(scr_resolution);
    }
    */
}


static lv_obj_t * create_btn(int label_id, int state_id, int y){
        lv_obj_t * btn = lv_btn_create(scr_resolution_pro);
        lv_obj_add_style(btn, &style_media_default_btn, LV_PART_MAIN);
        lv_obj_align(btn, LV_ALIGN_RIGHT_MID, lv_pct(-2), y);
        lv_obj_set_size(btn, 220, 90);

//        lv_obj_add_event_cb(btn, open_scr_media_selection_cb, LV_EVENT_CLICKED, (void*)(intptr_t)i);

        lv_obj_t *name_label = lv_label_create(btn);
        lv_label_set_text(name_label, _(label_id));
        lv_obj_add_style(name_label, &style_font_default_30, LV_PART_MAIN);
        lv_obj_align(name_label, LV_ALIGN_CENTER, 0, -18);

        lv_obj_t *state_label = lv_label_create(btn);
        lv_label_set_text(state_label, _(state_id));

        lv_obj_add_style(state_label, &style_font_default_30, LV_PART_MAIN);
        lv_obj_align(state_label, LV_ALIGN_CENTER, 0, 20);
        lv_obj_set_style_text_opa(state_label, LV_OPA_100, LV_PART_MAIN);

        if (label_id == STRING_VIDEO_STANDARD){
            video_format_state = state_label;
            video_format_label = name_label;
            lv_obj_add_event_cb(btn, switch_video_format_cb, LV_EVENT_CLICKED, NULL);
        } else if (label_id == STRING_DIS){
            gyro_eis_state = state_label;
            gyro_eis_label = name_label;
            lv_obj_add_event_cb(btn, open_scr_gyro_selection_cb, LV_EVENT_CLICKED, NULL);
        }
        return btn;
}

static void gyro_eis_roller_toggled_cb(){
    lv_label_set_text(gyro_eis_text, _(cam_gyro_eis_text_table[lv_roller_get_selected(gyro_eis_roller)]));
}

static void create_gyro_selection_scr(){
    scr_gyro_selection = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_gyro_selection, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);

    lv_obj_t * exit_icon = create_exit_icon(scr_gyro_selection);
    lv_obj_remove_event_cb(exit_icon, open_scr_home_cb);
    lv_obj_add_event_cb(exit_icon, open_scr_resolution_cb, LV_EVENT_CLICKED, NULL);

    //gyro_eis_roller = create_roller(scr_gyro_selection, cam_gyro_eis_table, CAM_GYRO_EIS_COUNT, 0, 320, 250, &style_font_default_36, false);
    gyro_eis_roller = create_roller_align_right(scr_gyro_selection, cam_gyro_eis_text_table, CAM_GYRO_EIS_COUNT, 0, 380, 250, &style_font_default_30, false, false);
    lv_obj_align(gyro_eis_roller, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_style_text_align(gyro_eis_roller, LV_TEXT_ALIGN_RIGHT, LV_PART_ANY);

    gyro_eis_title = lv_label_create(scr_gyro_selection);
    lv_label_set_text(gyro_eis_title, _(STRING_DIS));
    lv_obj_add_style(gyro_eis_title, &style_font_default_36, LV_PART_MAIN);
    lv_obj_align(gyro_eis_title, LV_ALIGN_TOP_LEFT, 95, 20);

    gyro_eis_text = lv_label_create(scr_gyro_selection);
    lv_label_set_text(gyro_eis_text, _(cam_gyro_eis_text_table[lv_roller_get_selected(gyro_eis_roller)]));
    lv_obj_add_style(gyro_eis_text, &style_font_default_36, LV_PART_MAIN);
    lv_obj_align(gyro_eis_text, LV_ALIGN_TOP_LEFT, 40, 90);
    lv_label_set_long_mode(gyro_eis_text, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(gyro_eis_text, 300);
    lv_obj_set_style_text_line_space(gyro_eis_text, -7, LV_PART_MAIN);

    lv_obj_add_event_cb(gyro_eis_roller, gyro_eis_roller_toggled_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

void create_scr_resolution_pro(){
    scr_resolution_pro = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_resolution_pro, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);

    create_exit_icon(scr_resolution_pro);

    res_pro_heading = lv_label_create(scr_resolution_pro);
    lv_label_set_text(res_pro_heading, _(STRING_RESOLUTION));
    lv_obj_add_style(res_pro_heading, &style_font_default_36, LV_PART_MAIN);
    lv_obj_align(res_pro_heading, LV_ALIGN_CENTER, lv_pct(-30), -120);

    res_pro_roller = create_roller(scr_resolution_pro, cam_resolution_pro_table, CAM_RES_PRO_COUNT, 0, 320, 250, &style_font_default_36, false);
    lv_obj_align(res_pro_roller, LV_ALIGN_CENTER, lv_pct(-30), lv_pct(10));


    frame_rate_pro_heading = lv_label_create(scr_resolution_pro);
    lv_label_set_text(frame_rate_pro_heading, _(STRING_FPS));
    lv_obj_add_style(frame_rate_pro_heading, &style_font_default_36, LV_PART_MAIN);
    lv_obj_align(frame_rate_pro_heading, LV_ALIGN_CENTER, lv_pct(5), -120);

    frame_rate_pro_roller = create_roller(scr_resolution_pro, cam_frame_rates_pro_table[0], CAM_FRAME_RATES_PRO_COUNT, 0, 250, 250, &style_font_default_36, true);
    lv_obj_align(frame_rate_pro_roller, LV_ALIGN_CENTER, lv_pct(5), lv_pct(10));



    static lv_point_precise_t line_points[2] = {{315, 120}, {315, 380}};

    lv_obj_t * line;
    line = lv_line_create(scr_resolution_pro);
    lv_line_set_points(line, line_points, 2);
    lv_obj_set_style_line_color(line, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_line_width(line, 3, LV_PART_MAIN);


    create_btn(STRING_DIS, STRING_DIS_NORMAL, lv_pct(-5));
    create_btn(STRING_VIDEO_STANDARD, STRING_PAL, lv_pct(25));

     create_gyro_selection_scr();

}



void create_scr_resolution(){
    scr_resolution = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_resolution, BG_COLOR_DARK_BLUE_GREY, LV_PART_MAIN);

    create_exit_icon(scr_resolution);

    res_heading = lv_label_create(scr_resolution);
    lv_label_set_text(res_heading, "Resolution");
    lv_obj_add_style(res_heading, &style_font_default_36, LV_PART_MAIN);
    lv_obj_align(res_heading, LV_ALIGN_CENTER, 0, -120);

    res_roller = create_roller(scr_resolution, cam_resolution_table, CAM_RES_COUNT, 0, 250, 250, &style_font_default_36, false);

}
