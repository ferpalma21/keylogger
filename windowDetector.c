#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <string.h>

#define MAX_TITLE_LENGTH 1024

// Function to get the window title
char* get_window_title(Display *display, Window window) {
    static char title[MAX_TITLE_LENGTH];
    Atom atom = XInternAtom(display, "WM_NAME", False);
    Atom actual_type;
    int actual_format;
    unsigned long nitems;
    unsigned long bytes_after;
    unsigned char *prop = NULL;

    if (XGetWindowProperty(display, window, atom, 0, MAX_TITLE_LENGTH / 4, False, AnyPropertyType,
                           &actual_type, &actual_format, &nitems, &bytes_after, &prop) == Success) {
        if (prop) {
            strncpy(title, (char*)prop, MAX_TITLE_LENGTH-1);
            title[MAX_TITLE_LENGTH-1] = '\0';
            XFree(prop);
        } else {
            strncpy(title, "Unknown", MAX_TITLE_LENGTH-1);
            title[MAX_TITLE_LENGTH-1] = '\0';
        }
    } else {
        strncpy(title, "Unknown", MAX_TITLE_LENGTH-1);
        title[MAX_TITLE_LENGTH-1] = '\0';
    }
    return title;
}

int main() {
    Display *display;
    Window root, focused_window;
    XEvent event;
    Atom atom;
    char *window_title;

    // Open connection to X server
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Unable to open X display\n");
        return EXIT_FAILURE;
    }

    root = DefaultRootWindow(display);

    // Get the atom for the "_NET_ACTIVE_WINDOW" property
    atom = XInternAtom(display, "_NET_ACTIVE_WINDOW", False);

    // Get the initial active window
    Window active_window;
    Atom actual_type;
    int actual_format;
    unsigned long nitems;
    unsigned long bytes_after;
    unsigned char *prop = NULL;

    if (XGetWindowProperty(display, root, atom, 0, sizeof(Window) / sizeof(long), False,
                           XA_WINDOW, &actual_type, &actual_format, &nitems, &bytes_after, &prop) == Success) {
        if (prop) {
            active_window = *((Window*)prop);
            XFree(prop);
            printf("Initial active window: %lu\n", (unsigned long)active_window);  // Debug print
        } else {
            active_window = None;
            printf("No initial active window detected.\n");  // Debug print
        }
    } else {
        active_window = None;
        printf("Failed to get initial active window.\n");  // Debug print
    }

    focused_window = active_window;

    while (1) {
        // Wait for an event
        XNextEvent(display, &event);

        // Debug print for event type
        printf("Event type: %d\n", event.type);

        // Check for changes in the active window
        if (event.type == PropertyNotify && event.xproperty.atom == atom) {
            if (XGetWindowProperty(display, root, atom, 0, sizeof(Window) / sizeof(long), False,
                                   XA_WINDOW, &actual_type, &actual_format, &nitems, &bytes_after, &prop) == Success) {
                if (prop) {
                    Window new_active_window = *((Window*)prop);
                    XFree(prop);
                    if (new_active_window != focused_window) {
                        focused_window = new_active_window;
                        // Get and print the new window title
                        window_title = get_window_title(display, focused_window);
                        printf("Active window: %s\n", window_title);
                    }
                }
            }
        }
    }

    // Close connection to X server
    XCloseDisplay(display);
    return EXIT_SUCCESS;
}
