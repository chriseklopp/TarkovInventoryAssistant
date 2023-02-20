#include <Tests/TTestLibrary.h>

namespace Test {

    void displayImage(const cv::Mat& im, int wait) {
        cv::imshow("Image", im);
        cv::waitKey(0);
    }

    void displaySlice(const cv::Mat& im, const std::pair<cv::Point, cv::Point> loc) {
        displaySlice(im, loc.first, loc.second);
    }

    void displaySlice(const cv::Mat& im, const cv::Point& lower, const cv::Point& upper) {
        displayImage(im(cv::Range(lower.y, upper.y), cv::Range(lower.x, upper.x + 1)));
    }
}