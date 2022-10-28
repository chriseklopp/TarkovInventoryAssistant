#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/img_hash.hpp>
#include <opencv2/imgproc.hpp>



namespace TDataTypes
{
    // Split on character delimiter.
    void splitString(std::string s, char del, std::vector<std::string>& out);
    // Split on character delimiter.
    void splitString(std::string s, char del, std::vector<int>& out);

    // Join vector to std::string on character delimiter.
    std::string joinVector(std::vector<std::string>& in, char del);

    // Join vector to std::string on character delimiter.
    std::string joinVector(std::vector<int>& in, char del);

    /*
    * TPixelCoordinate
    *This class is intended to hold pixel coordinates for CV operations.
    * Uses matrix conventions of i,j to denote row and column of pixels.
    *   j ------------------------------>
    * i
    * |
    * |
    * |
    * |
    * V


    */
    class TPixelCoordinate {
    public:
        TPixelCoordinate();
        TPixelCoordinate(int i, int j);
        TPixelCoordinate(std::pair<int, int> p);

        TPixelCoordinate operator+ (const TPixelCoordinate &other) {
            TPixelCoordinate pc;
            pc.i = this->i + other.i;
            pc.j = this->j + other.j;
            return pc;
        }

    private:
        int i;
        int j;

    };

    using TPC = TPixelCoordinate;

    // Denotes what type of storage a TContainerItem contains.
    enum gridType {
        ARRAY = 0, // A contiguous block/grid of item slots.
        DYNAMIC = 1 // Could have disconnected slots of varying sizes and positions.
    };

  
}

namespace Hash {

    // Allows using std::pairs as keys for std::unordered_map
    struct pair_hash
    {
        template <class T1, class T2>
        std::size_t operator() (const std::pair<T1, T2>& pair) const {
            return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
        }
    };


    cv::Mat hashImage(cv::Mat& image);
}