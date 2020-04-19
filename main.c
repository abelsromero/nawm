#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <xdo.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xinerama.h>
#include <X11/extensions/Xrandr.h>
#include <log.h>

#define SIZE_PIXELS 0

Display *display = NULL;
XineramaScreenInfo *screensInfo = NULL;
int screensCount;

void panic(char *message);
void invalidArguments();

void openScreens();
void closeScreens();

XineramaScreenInfo *initScreensInfo();
void printScreensInfo();
bool isTargetWindow(unsigned int x, unsigned int y, unsigned int targetScreen);

Window getCurrentWindow(Display *);
int getActiveWindow(Display *, Window *);
void getWindowProperties(Window w, int *x_ret, int *y_ret, int *width, int *height);
unsigned char *getWindowPropertyByAtom(Display *display, Window window, Atom atom, long *nitems, Atom *type, int *size);

int main(int argc, char **argv)
{
    openScreens();
    log_set_fp(fopen("nawm.log", "a"));

    bool completedRun = False;

    // Window win;
    // xdo_t *xdo;
    // xdo = xdo_new(NULL);
    // xdo_get_active_window(xdo, &win);

    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            if (strcmp(argv[i], "--info") == 0)
            {
                openScreens();
                Screen *scr = XScreenOfDisplay(display, 0);
                log_info("dimensions:    %dx%d pixels", XDisplayWidth(display, 0), XDisplayHeight(display, 0));
                printScreensInfo();
                completedRun = true;
            }
            if (strcmp(argv[i], "--move") == 0)
            {
                if (argc > i + 1)
                {
                    openScreens();

                    Window window;
                    int ret;
                    ret = getActiveWindow(display, &window);
                    if (ret == 0)
                        panic("Unexpected xlib error obtaining focus window");

                    unsigned int x, y;
                    unsigned int win_width, win_height;
                    getWindowProperties(window, &x, &y, &win_width, &win_height);

                    int targetScreen = atoi(argv[i + 1]);
                    int originScreen = targetScreen == 0 ? 1 : 0;
                    if (isTargetWindow(x, y, targetScreen))
                    {
                        log_info("Ignoring window move: target window is current window");
                    }
                    else
                    {
                        int targetX, targetY;
                        // - moving right/left reports +4 in X after moving
                        // - moving left adds +56 in Y after moving
                        // targetX = (targetScreen == 1 ? x + screensInfo[0].width : x - screensInfo[0].width) - 4;
                        if (originScreen == 1)
                            x -= screensInfo[0].width;
                        targetX = ((x / (float)screensInfo[originScreen].width) * screensInfo[targetScreen].width) - 4;
                        if (originScreen == 0)
                            targetX += screensInfo[0].width;

                        if (targetX < 0)
                            targetX = 0;
                        targetY = ((y / (float)screensInfo[originScreen].height) * screensInfo[targetScreen].height) - 56;
                        if (targetY < 0)
                            targetY = 0;

                        // FIXME there's
                        int targetWidth, targetHeight;
                        targetWidth = (win_width / (float)screensInfo[originScreen].width) * screensInfo[targetScreen].width;
                        targetHeight = (win_height / (float)screensInfo[originScreen].height) * screensInfo[targetScreen].height;

                        log_info("Moving to\t\t(%d, %d) \t%d,%d", targetX, targetY, targetWidth, targetHeight);
                        ret = XMoveResizeWindow(display, window, targetX, targetY, targetWidth, targetHeight);
                        if (ret == 0)
                            panic("Unexpected xlib error moving window");
                    }
                    i++;
                    completedRun = true;
                }
                else
                {
                    invalidArguments(argv[0]);
                }
            }
        }
    }

    if (!completedRun)
    {
        invalidArguments(argv[0]);
    }

    closeScreens();

    return 0;
}

void panic(char *message)
{
    log_error(message);
    exit(1);
}

void invalidArguments(char *appName)
{
    log_info("Invalid number of arguments");
    log_info("Usage: %s [-info] [-move target_screen_index]", appName);
    exit(1);
}

void openScreens()
{
    display = XOpenDisplay(NULL);
    screensInfo = initScreensInfo();
}

void closeScreens()
{
    if (display != NULL)
        XCloseDisplay(display);
}

// returns array
XineramaScreenInfo *initScreensInfo()
{
    XineramaScreenInfo *sc = XineramaQueryScreens(display, &screensCount);
    if (sc == NULL)
    {
        panic("Xinerama not found");
    }
    return sc;
}

void printScreensInfo()
{
    log_info("Found screens: %d", screensCount);
    for (int i = 0; i < screensCount; i++)
    {
        log_info("\t%d: %dx%d", screensInfo[i].screen_number, screensInfo[i].width, screensInfo[i].height);
    }
}

bool isTargetWindow(unsigned int x, unsigned int y, unsigned int targetScreen)
{
    if (targetScreen == 0 && x < screensInfo[0].width)
        return true;

    if (targetScreen == 1 && x > screensInfo[0].width)
        return true;

    return false;
}

Window getCurrentWindow(Display *d)
{
    Window focused;
    int revert_to;
    XGetInputFocus(display, &focused, &revert_to);
    return focused;
}

int getActiveWindow(Display *display, Window *window_ret)
{
    Atom type;
    int size;
    long nitems;
    unsigned char *data;
    Atom request;
    Window root;

    request = XInternAtom(display, "_NET_ACTIVE_WINDOW", False);
    root = XDefaultRootWindow(display);
    data = getWindowPropertyByAtom(display, root, request, &nitems, &type, &size);

    if (nitems > 0)
    {
        *window_ret = *((Window *)data);
    }
    else
    {
        *window_ret = 0;
    }
    free(data);

    return (*window_ret != 0);
}

unsigned char *getWindowPropertyByAtom(Display *display, Window window, Atom atom, long *nitems, Atom *type, int *size)
{
    Atom actual_type;
    int actual_format;
    unsigned long _nitems;
    /*unsigned long nbytes;*/
    unsigned long bytes_after; /* unused */
    unsigned char *prop;
    int status;

    status = XGetWindowProperty(display, window, atom, 0, (~0L),
                                False, AnyPropertyType, &actual_type,
                                &actual_format, &_nitems, &bytes_after,
                                &prop);
    if (status == BadWindow)
    {
        fprintf(stderr, "window id # 0x%lx does not exists!", window);
        return NULL;
    }
    if (status != Success)
    {
        fprintf(stderr, "XGetWindowProperty failed!");
        return NULL;
    }

    if (nitems != NULL)
    {
        *nitems = _nitems;
    }

    if (type != NULL)
    {
        *type = actual_type;
    }

    if (size != NULL)
    {
        *size = actual_format;
    }
    return prop;
}

void getWindowProperties(Window w, int *x_ret, int *y_ret, int *width, int *height)
{
    XWindowAttributes attr;
    int ret = XGetWindowAttributes(display, w, &attr);
    if (ret == 0)
        panic("Unexpected xlib error obtaining window attributes");

    int translated_x, translated_y;
    Window unused_child;
    XTranslateCoordinates(display, w, attr.root, attr.x, attr.y, &translated_x, &translated_y, &unused_child);
    log_info("Window properties: x=%d,y=%d; w=%d,h=%d", translated_x, translated_y, attr.width, attr.height);
    *x_ret = translated_x;
    *y_ret = translated_y;
    *width = attr.width;
    *height = attr.height;
}