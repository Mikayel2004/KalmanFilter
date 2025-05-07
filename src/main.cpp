#include <opencv2/opencv.hpp>
#include <X11/Xlib.h>
#include "kalman_filter.h"

cv::Point getGlobalMousePosition(Display* display) {
    Window root = DefaultRootWindow(display);
    Window retRoot, retChild;
    int rootX, rootY, winX, winY;
    unsigned int mask;

    XQueryPointer(display, root, &retRoot, &retChild,
                  &rootX, &rootY, &winX, &winY, &mask);

    return {rootX, rootY};
}

int main() {
    Display* display = XOpenDisplay(nullptr);
    if (!display) {
        std::cerr << "Failed to open X display\n";
        return 1;
    }

    MouseKalman kalman;
    bool initialized = false;

    std::cout << "Tracking mouse globally using Kalman filter. Press Ctrl+C to stop.\n";

    while (true) {
        cv::Point pos = getGlobalMousePosition(display);

        if (!initialized) {
            kalman.update(static_cast<float>(pos.x), static_cast<float>(pos.y));
            initialized = true;
        }

        kalman.update(static_cast<float>(pos.x), static_cast<float>(pos.y));
        cv::Point prediction = kalman.predict();

        std::cout << "Mouse: (" << pos.x << ", " << pos.y << ")"
                  << " | Prediction: (" << prediction.x << ", " << prediction.y << ")\n";

        cv::waitKey(30);  // Slight delay to reduce CPU usage
    }

    XCloseDisplay(display);
    return 0;
}
