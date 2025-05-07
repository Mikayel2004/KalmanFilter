#ifndef MOUSETRACKER_KALMAN_FILTER_H
#define MOUSETRACKER_KALMAN_FILTER_H

#include <opencv2/opencv.hpp>

class MouseKalman {
public:
    MouseKalman();

    void update(float x, float y);
    cv::Point predict();

private:
    cv::KalmanFilter kf;
    cv::Mat state;
    cv::Mat measurement;
};

#endif //MOUSETRACKER_KALMAN_FILTER_H
