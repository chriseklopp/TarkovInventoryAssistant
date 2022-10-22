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

    cv::Mat hashImage(cv::Mat& image);
}