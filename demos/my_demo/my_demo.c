#include "my_demo.h"

/* screen*/
static lv_obj_t * screen_a = NULL;
static lv_obj_t * screen_b = NULL;
static lv_obj_t * screen_test = NULL;

/*series of switch on screen a */
static lv_obj_t * a_swts[5];    //switch
static lv_obj_t * a_txt[5];      // label next to switch
static lv_obj_t * a_text_box;
static lv_obj_t * a_keyboard;

static lv_style_t a_btn_hover;

LV_FONT_DECLARE(Yahei);
LV_FONT_DECLARE(user_defined);


void tests(){
    lv_obj_t * label = lv_label_create(screen_test);
    lv_label_set_text(label, " 啊啊啊啊啊おはようございます TEST!");

    lv_obj_set_style_bg_color(screen_test, lv_palette_main(LV_PALETTE_AMBER), LV_PART_MAIN);

    lv_obj_set_style_text_font(label, &user_defined, 0);
    lv_obj_center(label);

    lv_obj_t * img_2 = lv_image_create(screen_test);
    lv_image_set_src(img_2, &Sliding_touch);
    //lv_image_set_scale(img_2, 500);
    lv_image_set_align(img_2, LV_ALIGN_OUT_RIGHT_MID);

   //lv_obj_set_style_bg_color(img_2, lv_palette_main(LV_PALETTE_RED), 0); //change bg color
   //lv_obj_set_style_bg_opa(img_2, LV_OPA_COVER, 0);

}


static void create_test_screen(){
    screen_test = lv_obj_create(NULL);
    tests();
}

/**load styles**/
static void load_styles(){
    //enlarge button when hover
    lv_style_init(&a_btn_hover);
    lv_style_set_border_width(&a_btn_hover, 5);
    lv_style_set_border_color(&a_btn_hover, lv_palette_lighten(LV_PALETTE_LIGHT_BLUE, 3));
    lv_style_set_transform_width(&a_btn_hover, 10);
    lv_style_set_transform_height(&a_btn_hover, 8);
}

/**set switch state**/
static void a_switch_set_state(int opt){
    for (int i = 0; i<5; i++){
        switch(opt) {
            case 0:      //set all to off
                lv_obj_remove_state(a_swts[i], LV_STATE_CHECKED);
                lv_label_set_text(a_txt[i], "OFF");
            case 1:      //set all to on
                lv_obj_add_state(a_swts[i], LV_STATE_CHECKED);
                lv_label_set_text(a_txt[i], "ON");
            case 2:      //state = !state
                if (lv_obj_has_state(a_swts[i], LV_STATE_CHECKED)) {
                    lv_obj_remove_state(a_swts[i], LV_STATE_CHECKED);
                    lv_label_set_text(a_txt[i], "OFF");
                } else {
                    lv_obj_add_state(a_swts[i], LV_STATE_CHECKED);
                    lv_label_set_text(a_txt[i], "ON");
            }
        }
    }
}

static void a_text_box_set_color(bool focus){
    if (focus) lv_obj_set_style_bg_color(a_text_box,lv_color_make(255,255,255) ,0);
    else lv_obj_set_style_bg_color(a_text_box,lv_palette_lighten(LV_PALETTE_GREY,4),0);
}

/**event functions **/
static void b_arc_cb (lv_event_t * e){
    lv_obj_t * b_arc = lv_event_get_target(e);
    lv_obj_t * b_arc_label = lv_event_get_user_data(e);

    char buf[16];
    lv_snprintf(buf, sizeof(buf), "%d", lv_arc_get_value(b_arc));
    lv_label_set_text(b_arc_label, buf);
}

static void a_btn_cb(lv_event_t * e) {
    lv_obj_t * btn = lv_event_get_target(e);
    uint32_t code = lv_event_get_code(e);
    lv_obj_t * btn_label = lv_event_get_user_data(e);

    //printf(lv_label_get_text(btn_label));

    if (code == LV_EVENT_PRESSED) {             // pressed -> visual effect
        lv_obj_set_style_bg_color(btn, lv_palette_darken(LV_PALETTE_BLUE, 2), 0);
    } else if (code == LV_EVENT_CLICKED) {      //clicked
        printf("Button clicked\n");
        if (!strcmp(lv_label_get_text(btn_label), "NEXT") ) {
            lv_screen_load_anim(screen_b, LV_SCR_LOAD_ANIM_FADE_ON , 500, 0, FALSE);
        } else if (!strcmp(lv_label_get_text(btn_label), "reverse")) {
            a_switch_set_state(2);
        }

    } else if (code == LV_EVENT_RELEASED) {     //release -> return normal color
        lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_BLUE), 0);
    }
}

static void b_swt_clicked_cb(lv_event_t * e){
    printf("B CLICKED\n");
    lv_screen_load(screen_a);
}

static void a_swt_clicked_cb(lv_event_t * e){
    printf("A switch toggled");

    lv_obj_t * swt = lv_event_get_target(e);
    lv_obj_t * label = lv_event_get_user_data(e);
    bool is_on = lv_obj_has_state(swt, LV_STATE_CHECKED); //important line
    if (is_on) {
            printf(" to on\n");
            lv_label_set_text(label, "ON");
    }
    else{
        printf(" to off\n");
        lv_label_set_text(label, "OFF");
    }
}

static void a_text_box_focus_cb(lv_event_t * e){
    uint32_t code = lv_event_get_code(e);

    if (code == LV_EVENT_FOCUSED){
        printf("called & in focus\n");
        a_text_box_set_color(true);
    } else if (code == LV_EVENT_DEFOCUSED) {
        printf("called & defocused\n");
        a_text_box_set_color(false);
    } else {
        printf("called & keyboard pressed\n");
        a_text_box_set_color(true);
    }
}

static void screen_click_cb(lv_event_t * e){    //clicked on screen
    printf("called & clicked on screen\n");
     a_text_box_set_color(false);
}

/**functions**/

void a_container_1(){
    /**switch container**/
    lv_obj_t * cont1 = lv_obj_create(screen_a);
    lv_obj_set_pos(cont1, lv_pct(10), lv_pct(10));
    lv_obj_set_size(cont1, lv_pct(25), 300);

    /*series of switch on screen a */
    int border = 10;
    for (int i = 0; i<5; i++){
        //switch
        a_swts[i] = lv_switch_create(cont1);
        if (i==0) lv_obj_align_to(a_swts[0], cont1, LV_ALIGN_TOP_MID, -25, border);
        else  lv_obj_align_to(a_swts[i], a_swts[i-1], LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

        //label
        a_txt[i] = lv_label_create(cont1);
        lv_label_set_text(a_txt[i], "OFF");
        lv_obj_align_to(a_txt[i], a_swts[i], LV_ALIGN_OUT_RIGHT_MID, 5, 0);
        //lv_obj_set_pos(a_txt[i], 70, 5+i*50);

        //switch toggled
        lv_obj_add_event_cb(a_swts[i], a_swt_clicked_cb, LV_EVENT_VALUE_CHANGED, a_txt[i]);
    }

    //button
    lv_obj_t * a_btn_rev = lv_btn_create(cont1);
    lv_obj_align(a_btn_rev, LV_ALIGN_CENTER, 0, 110);

    //text on button
    lv_obj_t * a_label_rev = lv_label_create(a_btn_rev);
    lv_label_set_text(a_label_rev, "reverse");
    //lv_obj_set_style_text_font(a_label_rev, LV_FONT_MONTSERRAT_12, 0);
    lv_obj_center(a_label_rev);


    /**actions**/
    /*callback  -> upon uder actionss -> object, call function, event, data*/
    lv_obj_add_event_cb(a_btn_rev, a_btn_cb, LV_EVENT_ALL, a_label_rev);

    //button hover
    lv_obj_add_style(a_btn_rev, &a_btn_hover, LV_STATE_HOVERED);

    //button clicked
    //lv_obj_add_event_cb(a_btn, a_btn_clicked, LV_EVENT_CLICKED, NULL);
}

void a_container_2(){
     /**container**/
    lv_obj_t * cont = lv_obj_create(screen_a);
    lv_obj_set_pos(cont, lv_pct(40), lv_pct(10));
    lv_obj_set_size(cont, lv_pct(50), 300);

    /** keyboard & textbox **/
    //text area
    a_text_box = lv_textarea_create(cont);
    lv_obj_align(a_text_box, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_size(a_text_box, lv_pct(80), lv_pct(45));
    lv_textarea_set_cursor_click_pos(a_text_box, true);
    lv_obj_set_style_bg_color(a_text_box, lv_palette_lighten(LV_PALETTE_GREY, 5), 0);

    //keyboard
    a_keyboard = lv_keyboard_create(cont);
    lv_keyboard_set_textarea(a_keyboard, a_text_box);

    lv_obj_set_style_bg_color(a_keyboard, lv_palette_main(LV_PALETTE_BLUE), LV_PART_ITEMS | LV_STATE_PRESSED);
    lv_obj_set_style_text_color(a_keyboard, lv_color_hex(0xFFFFFF), LV_PART_ITEMS | LV_STATE_PRESSED);

    //cb
    lv_obj_add_event_cb(a_keyboard,  a_text_box_focus_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(a_text_box,  a_text_box_focus_cb, LV_EVENT_FOCUSED, NULL);
    lv_obj_add_event_cb(a_text_box,  a_text_box_focus_cb, LV_EVENT_DEFOCUSED, NULL);
    lv_obj_add_event_cb(screen_a, screen_click_cb, LV_EVENT_CLICKED, NULL);
}

void create_screen_a(){
    //create screen
    screen_a = lv_obj_create(NULL);
    a_container_1();
    a_container_2();
}


void b_container_1(){
    lv_obj_t * cont = lv_obj_create(screen_b);
    lv_obj_align(cont, LV_ALIGN_DEFAULT, lv_pct(10), lv_pct(10));
    lv_obj_set_size(cont, lv_pct(55), lv_pct(80));

    /**table**/
    //lv_obj_t * table = lv_table_create(cont);
    //lv_table_set_cell_value(table, 8, 3, "A table ");

}

void b_container_2(){
    lv_obj_t * cont2 = lv_obj_create(screen_b);
    lv_obj_align(cont2, LV_ALIGN_DEFAULT, lv_pct(70 ), lv_pct(10));
    lv_obj_set_size(cont2, lv_pct(20), lv_pct(80));

    /**switch**/
    lv_obj_t * b_swt = lv_switch_create(cont2);
    lv_obj_set_pos(b_swt, 30, 50);


    /** arc **/
    //arc
    lv_obj_t * b_arc = lv_arc_create(cont2);
    lv_obj_align(b_arc, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_size(b_arc,80, 80);
    //label
    lv_obj_t * b_arc_label = lv_label_create(cont2);
    lv_label_set_text(b_arc_label, "value");
    lv_obj_align_to(b_arc_label, b_arc, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);


    /**callback functions**/
    lv_obj_add_event_cb(b_swt, b_swt_clicked_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(b_arc, b_arc_cb, LV_EVENT_VALUE_CHANGED, b_arc_label);

}


void create_screen_b(){
    //create screen
    screen_b = lv_obj_create(NULL);

    b_container_1();
    b_container_2();

    tests();
}


///main
void my_demo_create(void) {

    printf("LVGL Version: %d.%d.%d\n", LVGL_VERSION_MAJOR, LVGL_VERSION_MINOR, LVGL_VERSION_PATCH);

    /**preload**/
    load_styles();
        create_test_screen();
    create_screen_a();
    create_screen_b();


    /**outsite containers**/
    /*button next*/
    lv_obj_t * a_btn_next = lv_btn_create (screen_a);
    lv_obj_set_size(a_btn_next, 70, 40);
    //lv_obj_align_to(a_btn_next, screen_a, LV_ALIGN_BOTTOM_MID, 0, -35);
    lv_obj_align(a_btn_next, LV_ALIGN_BOTTOM_MID, 0, -35);

    //text
    lv_obj_t * a_label_next = lv_label_create(a_btn_next);
    lv_label_set_text(a_label_next, "NEXT");
    lv_obj_center(a_label_next);

    //action
    lv_obj_add_event_cb(a_btn_next, a_btn_cb, LV_EVENT_ALL, a_label_next);
    lv_obj_add_style(a_btn_next, &a_btn_hover, LV_STATE_HOVERED);

    /**code**/
    lv_screen_load(screen_test);
}
