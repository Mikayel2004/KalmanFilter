#include "kalman_filter.h"
#include <opencv2/core.hpp>
#include <opencv2/video/tracking.hpp>

MouseKalman::MouseKalman()
    : kf(4, 2, 0),
      state(4, 1, CV_32F),
      measurement(2, 1, CV_32F) {

    kf.transitionMatrix = (cv::Mat_<float>(4, 4) <<
        1, 0, 1, 0,
        0, 1, 0, 1,
        0, 0, 1, 0,
        0, 0, 0, 1);

    cv::setIdentity(kf.measurementMatrix);
    cv::setIdentity(kf.processNoiseCov, cv::Scalar(1e-4));
    cv::setIdentity(kf.measurementNoiseCov, cv::Scalar(1e-1));
    cv::setIdentity(kf.errorCovPost, cv::Scalar(1));

    cv::randu(kf.statePost, cv::Scalar::all(0), cv::Scalar::all(1));
}

void MouseKalman::update(float x, float y) {
    measurement.at<float>(0) = x;
    measurement.at<float>(1) = y;
    kf.correct(measurement);
}

cv::Point MouseKalman::predict() {
    cv::Mat prediction = kf.predict();
    return cv::Point(prediction.at<float>(0), prediction.at<float>(1));
}
