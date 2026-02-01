#include <X11/Xatom.h>
#include <X11/Xlib.h>

#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>

volatile sig_atomic_t gSignalStatus = 0;

void signalHandler(int sig) { gSignalStatus = sig; }

int main() {
    signal(SIGINT, signalHandler);
    Display* display = XOpenDisplay(nullptr);
    if (display == nullptr) {
        std::cout << "Error: no display connection!";
        return 1;
    }
    Window root = DefaultRootWindow(display);
    Atom property = XInternAtom(display, "_NET_ACTIVE_WINDOW", False);
    // return values
    Atom type_return;
    int format_return;
    unsigned long nitems_return;
    unsigned long bytes_left;
    unsigned char* data;

    bool running = true;
    auto last_tick = std::chrono::steady_clock::now();
    std::cout << "Tracker is running... (Press ctr+c to stop!) \n";
    while (running) {
        if (gSignalStatus != 0) {
            running = false;
        }
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double> delta = now - last_tick;
        std::cout << delta.count() << std::endl;

        last_tick = now;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }

    XGetWindowProperty(display, root, property, 0, 1, False, XA_WINDOW,
                       &type_return, &format_return, &nitems_return,
                       &bytes_left, &data);

    //(return_value == Success) && nitems_return
    Window win = ((Window*)data)[0];
    XFree(data);

    // Get window title
    Atom title_atom = XInternAtom(display, "_NET_WM_NAME", False);
    XGetWindowProperty(display, win, title_atom, 0, 1024, False,
                       AnyPropertyType, &type_return, &format_return,
                       &nitems_return, &bytes_left, &data);

    if (nitems_return > 0) {
        std::string title((char*)data);
        std::cout << "Window title: " << title << std::endl;
        XFree(data);
    }

    XCloseDisplay(display);
    std::cout << "END" << std::endl;

    return 0;
}