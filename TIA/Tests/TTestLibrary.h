#pragma once
#include <DataSupport/TDataTypes.h>


namespace Test {
        // Display cv mat, wait => waitkey (0 = until button press)
        void displayImage(const cv::Mat& im, int wait=0);

        // Display slice of an image.
        void displaySlice(const cv::Mat& im, const std::pair<cv::Point,cv::Point> loc);
        void displaySlice(const cv::Mat& im, const cv::Point& lower, const cv::Point& upper);
}