#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>



// Image id. Unique ID corresponding to a catalog item.
typedef int imageID;


namespace TDataTypes
{


    //Data catalog id. Unique ID corresponding to a catalog item.
    typedef int dcID;





    // Split on character delimiter.
    void splitString(std::string s, char del, std::vector<std::string>& out);
    // Split on character delimiter.
    void splitString(std::string s, char del, std::vector<int>& out);

    // Join vector to std::string on character delimiter.
    std::string joinVector(std::vector<std::string>& in, char del);

    // Join vector to std::string on character delimiter.
    std::string joinVector(std::vector<int>& in, char del);

    // Converts integer to string and adds commas where necessary.
    std::string prettifyToString(int val);

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


    class TCurrency {

    public:
        TCurrency() {};
        TCurrency(const std::string& currString);
        ~TCurrency() {};

        const int& getValue() const;
        const std::string getCurrencyString(bool prettify=false) const;
        const std::string& getUnit() const;
        const bool& isPrependedUnit() const;

        // Multiply currency value by the given int and return a string result.
        // Prettify will format the number to add commas where appropriate
        std::string getValueMultipledBy(int num, bool prettify = true) const;

        // Add the value of a TCurrency to this.
        //!!! This will throw std::invalid argument if they do not have the same units !!!
        TCurrency& operator+=(const TCurrency& rhs);

        // Subtract the value of a TCurrency to this.
        //!!! This will throw std::invalid argument if they do not have the same units !!!
        TCurrency& operator-=(const TCurrency& rhs);

    private:
        uint m_value;
        std::string m_unit;
        bool m_isPrependedUnit;
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


    cv::Mat PhashImage(const cv::Mat& image);

    cv::Mat templateMatch(const cv::Mat& a, const cv::Mat& b);
}