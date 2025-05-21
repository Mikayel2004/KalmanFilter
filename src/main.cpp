#include <opencv2/opencv.hpp>
#include <X11/Xlib.h>
#include "kalman_filter.h"

cv::Point getGlobalMousePosition(Display* display) {
    Window root = DefaultRootWindow(display);
    Window retRoot, retChild;
    int rootX, rootY, winX, winY;
    unsigned int mask;
    XQueryPointer(display, root, &retRoot, &retChild, &rootX, &rootY, &winX, &winY, &mask);
    return {rootX, rootY};
}

int main() {
    Display* display = XOpenDisplay(nullptr);
    if (!display) return 1;

    Screen* screen = DefaultScreenOfDisplay(display);
    int screenWidth = WidthOfScreen(screen);
    int screenHeight = HeightOfScreen(screen);

    cv::namedWindow("Mouse Tracking", cv::WINDOW_NORMAL);
    cv::resizeWindow("Mouse Tracking", screenWidth/2, screenHeight/2);

    cv::Mat trackingImage = cv::Mat::zeros(screenHeight, screenWidth, CV_8UC3);
    std::vector<cv::Point> actualPositions;
    std::vector<cv::Point> predictedPositions;
    MouseKalman kalman;
    bool initialized = false;

    while (true) {
        cv::Point pos = getGlobalMousePosition(display);

        if (!initialized) {
            kalman.update(static_cast<float>(pos.x), static_cast<float>(pos.y));
            initialized = true;
        }

        kalman.update(static_cast<float>(pos.x), static_cast<float>(pos.y));
        cv::Point prediction = kalman.predict();

        actualPositions.push_back(pos);
        predictedPositions.push_back(prediction);
        if (actualPositions.size() > 20) {
            actualPositions.erase(actualPositions.begin());
            predictedPositions.erase(predictedPositions.begin());
        }

        trackingImage = cv::Mat::zeros(screenHeight, screenWidth, CV_8UC3);
        for (size_t i = 0; i < actualPositions.size() - 1; i++) {
            int intensity = 50 + (i * 200 / actualPositions.size());
            cv::line(trackingImage, actualPositions[i], actualPositions[i+1], cv::Scalar(0, intensity, 0), 1);
            cv::line(trackingImage, predictedPositions[i], predictedPositions[i+1], cv::Scalar(0, 0, intensity), 1);
        }

        cv::rectangle(trackingImage, cv::Point(pos.x - 10, pos.y - 10), cv::Point(pos.x + 10, pos.y + 10), cv::Scalar(0, 255, 0), 2);
        cv::rectangle(trackingImage, cv::Point(prediction.x - 10, prediction.y - 10), cv::Point(prediction.x + 10, prediction.y + 10), cv::Scalar(0, 0, 255), 2);

        std::cout << "Mouse: (" << pos.x << ", " << pos.y << ") | Prediction: (" << prediction.x << ", " << prediction.y << ")\n";

        cv::imshow("Mouse Tracking", trackingImage);
        if (cv::waitKey(10) == 27) break;
    }

    cv::destroyAllWindows();
    XCloseDisplay(display);
    return 0;
}