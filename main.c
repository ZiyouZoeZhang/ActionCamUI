#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "lvgl/lvgl.h"
#include "demos/my_demo_2/my_demo_2.h"
#include "demos/my_demo_2/camera_modes.h"

static const wchar_t * title = L"this is a demo"; //must be wide - long


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
    lv_init();

    //lv_port_disp_init();

    //initialize screens
    lv_display_t * display = lv_windows_create_display(title, 800, 480, 100, FALSE, FALSE); //title, horizontal, vertical, zoom level
    //lv_display_t * another_display = lv_windows_create_display(title2, 400, 900, 100, FALSE, FALSE);

    //allow inputs
    lv_windows_acquire_pointer_indev(display); //allows pointer input.
    lv_windows_acquire_keypad_indev(display); //allows keyboard input

    printf("initialized\n"); //same as LV_LOG_USER

    //run demo
    //my_demo_create();
     my_demo_2_create();

    while(1) {
        /* Periodically call the lv_task handler.
         * It could be done in a timer interrupt or an OS task too.*/
        lv_task_handler();
        usleep(5000);       /*Just to let the system breath*/
    }
    return 0;
}
