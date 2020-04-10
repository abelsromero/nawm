#include <stdio.h>
// #include <xdo.h>
#include <X11/Xlib.h>

#define SIZE_PIXELS 0

/*
int print_data(xdo_t *xdo, Window window)
{
    unsigned char *window_name;
    int name_len = 0;
    int name_type = 0;
    unsigned int x, y = 0;
    unsigned int width, height = 0;
    Screen sc;
    Screen *screen = &sc;

    int ret = 0;

    ret = xdo_get_window_name(xdo, window, &window_name, &name_len, &name_type);
    ret = xdo_get_window_location(xdo, window, &x, &y, &screen);
    ret = xdo_get_window_size(xdo, window, &width, &height);

    if (ret)
    {
        fprintf(stderr, "error\n");
    }
    else
    {
        printf("%d - Window name: '%s' at (%d,%d), size (%d,%d)\n", ret, window_name, x, y, width, height);
    }
}
*/

int main()
{

    // xdo_t *xdo;
    Window window;
    unsigned int ret = 0;

    // xdo = xdo_new(NULL);
    // ret = xdo_get_active_window(xdo, &window);
    // print_data(xdo, window);

    // int xdo_set_window_size(const xdo_t *xdo, Window wid, int w, int h, int flags);

    //// Fails when is fullscreen
    // ret = xdo_move_window(xdo, window, 200, 400);
    //// also fails when fullscreen
    // XWindowChanges wc;
    // wc.x = 200;
    // wc.y = 400;
    // ret = XConfigureWindow(xdo->xdpy, window, CWX | CWY, &wc);
    //// also fails!!
    // ret = XMoveResizeWindow(xdo->xdpy, window, 200, 400, 700, 400);
    // ret = XResizeWindow(xdo->xdpy, window, 700, 600);
    // ret = XMoveWindow(xdo->xdpy, window, 200, 400);

    Display *display = XOpenDisplay(":0.0");
    Screen *screen;

    int screen_count = XScreenCount(display);
    printf("Screens count: %d\n", screen_count);
    // returns 1?
    for (int i = 0; i < screen_count; ++i)
    {
        screen = ScreenOfDisplay(display, i);
        printf("\tScreen %d: %dX%d\n", i + 1, screen->width, screen->height);
    }

    // close the display
    XCloseDisplay(display);

    // ret = xdo_set_window_size(xdo, window, 700, 400, SIZE_PIXELS);
    // print_data(xdo, window);

    return 0;
}