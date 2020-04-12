#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xinerama.h>

#define SIZE_PIXELS 0

Display *display = NULL;
XineramaScreenInfo *screensInfo = NULL;
int screensCount;

void openScreens();
void closeScreens();

XineramaScreenInfo *inintScreensInfo();
void printScreensInfo();

int main(int argc, char **argv)
{

    if (argc == 2)
    {
        for (int i = 1; i < argc; i++)
        {
            if (strcmp(argv[i], "-info") == 0)
            {
                openScreens();
                printScreensInfo();
            }
        }
    }
    else
    {
        fprintf(stderr, "Invalid number of arguments\n");
        fprintf(stderr, "Usage: %s [-info]\n", argv[0]);
        exit(1);
    }

    closeScreens();

    return 0;
}

void openScreens()
{
    display = XOpenDisplay(NULL);
    screensInfo = inintScreensInfo();
}

void closeScreens()
{
    if (display != NULL)
        XCloseDisplay(display);
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