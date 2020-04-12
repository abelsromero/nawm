#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xinerama.h>

#define SIZE_PIXELS 0

Display *display = NULL;
XineramaScreenInfo *screensInfo = NULL;
int screensCount;

XineramaScreenInfo *inintScreensInfo();
void printScreensInfo();

int main(int argc, char **argv)
{

    display = XOpenDisplay(NULL);
    screensInfo = inintScreensInfo();

    printScreensInfo();

    XCloseDisplay(display);

    return 0;
}

// returns array
XineramaScreenInfo *inintScreensInfo()
{
    XineramaScreenInfo *sc = XineramaQueryScreens(display, &screensCount);
    if (sc == NULL)
    {
        fprintf(stderr, "Xinerama not found");
        exit(1);
    }
    return sc;
}

void printScreensInfo()
{
    printf("Found screens: %d\n", screensCount);
    for (int i = 0; i < screensCount; i++)
    {
        printf("\t%d: %dx%d pixels\n", screensInfo[i].screen_number, screensInfo[i].width, screensInfo[i].height);
    }
}