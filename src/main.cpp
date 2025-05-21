#include <opencv2/opencv.hpp>
#include <windows.h>
#include <iostream>
#include <vector>
#include "kalman_filter.h"

cv::Point getGlobalMousePosition() {
    POINT p;
    if (!GetCursorPos(&p)) {
        return {0, 0};
    }
    return {p.x, p.y};
}

int main() {
    int screenWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    if (screenWidth <= 0 || screenHeight <= 0) {
        std::cerr << "Failed to get screen dimensions\n";
        return 1;
    }

    cv::namedWindow("Mouse Tracking", cv::WINDOW_NORMAL);
    cv::resizeWindow("Mouse Tracking", screenWidth / 2, screenHeight / 2);

    cv::Mat trackingImage = cv::Mat::zeros(screenHeight, screenWidth, CV_8UC3);

    const int MAX_HISTORY = 20;
    std::vector<cv::Point> actualPositions;
    std::vector<cv::Point> predictedPositions;

    MouseKalman kalman;
    bool initialized = false;

    const int SQUARE_SIZE = 20;
    const cv::Scalar ACTUAL_COLOR(0, 255, 0);
    const cv::Scalar PREDICTED_COLOR(0, 0, 255);
    const cv::Scalar HISTORY_COLOR(100, 100, 100);

    std::cout << "Tracking mouse globally across the entire desktop (" << screenWidth << "x" << screenHeight << "). Press ESC to stop.\n";

    while (true) {
        cv::Point pos = getGlobalMousePosition();

        if (!initialized) {
            kalman.update(static_cast<float>(pos.x), static_cast<float>(pos.y));
            initialized = true;
        }

        kalman.update(static_cast<float>(pos.x), static_cast<float>(pos.y));
        cv::Point prediction = kalman.predict();

        actualPositions.push_back(pos);
        predictedPositions.push_back(prediction);

        if (actualPositions.size() > MAX_HISTORY) {
            actualPositions.erase(actualPositions.begin());
            predictedPositions.erase(predictedPositions.begin());
        }

        trackingImage = cv::Mat::zeros(screenHeight, screenWidth, CV_8UC3);

        for (size_t i = 0; i < actualPositions.size() - 1; i++) {
            int intensity = 50 + (i * 200 / actualPositions.size());
            cv::line(trackingImage, actualPositions[i], actualPositions[i + 1],
                     cv::Scalar(0, intensity, 0), 1);
            cv::line(trackingImage, predictedPositions[i], predictedPositions[i + 1],
                     cv::Scalar(0, 0, intensity), 1);
        }

        cv::rectangle(trackingImage,
                      cv::Point(pos.x - SQUARE_SIZE / 2, pos.y - SQUARE_SIZE / 2),
                      cv::Point(pos.x + SQUARE_SIZE / 2, pos.y + SQUARE_SIZE / 2),
                      ACTUAL_COLOR, 2);

        cv::rectangle(trackingImage,
                      cv::Point(prediction.x - SQUARE_SIZE / 2, prediction.y - SQUARE_SIZE / 2),
                      cv::Point(prediction.x + SQUARE_SIZE / 2, prediction.y + SQUARE_SIZE / 2),
                      PREDICTED_COLOR, 2);

        cv::line(trackingImage, cv::Point(pos.x - 5, pos.y), cv::Point(pos.x + 5, pos.y), ACTUAL_COLOR, 1);
        cv::line(trackingImage, cv::Point(pos.x, pos.y - 5), cv::Point(pos.x, pos.y + 5), ACTUAL_COLOR, 1);

        std::string posText = "Mouse: (" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ")";
        std::string predText = "Prediction: (" + std::to_string(prediction.x) + ", " + std::to_string(prediction.y) + ")";
        cv::putText(trackingImage, posText, cv::Point(20, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, ACTUAL_COLOR, 2);
        cv::putText(trackingImage, predText, cv::Point(20, 60), cv::FONT_HERSHEY_SIMPLEX, 0.7, PREDICTED_COLOR, 2);

        cv::imshow("Mouse Tracking", trackingImage);

        std::cout << "Mouse: (" << pos.x << ", " << pos.y << ")"
                  << " | Prediction: (" << prediction.x << ", " << prediction.y << ")\n";

        int key = cv::waitKey(10);
        if (key == 27) {
            break;
        }
    }

    cv::destroyAllWindows();
    return 0;
}