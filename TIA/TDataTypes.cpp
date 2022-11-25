#pragma once

#include "TDataTypes.h"
#include <iostream>


namespace TDataTypes
{

    // Split on character delimiter.
    void splitString(std::string s, char del, std::vector<std::string>& out) {
        std::stringstream stream(s);
        std::string word;
        while (getline(stream, word, del)) {
            out.push_back(word);
        }
        if (s.back() == del)
            out.push_back("");
    }

    // Split on character delimiter.
    void splitString(std::string s, char del, std::vector<int>& out) {
        if (s.empty())
            return;

        std::stringstream stream(s);
        std::string word;
        while (getline(stream, word, del)) {
            out.push_back(std::stoi(word));
        }
    }

    // Join vector to std::string on character delimiter.
    std::string joinVector(std::vector<std::string>& in, char del) {
        std::string out;
        for (auto it : in) {
            out += it + del;
        }
        out.pop_back();
        return out;
    };

    // Join vector to std::string on character delimiter.
    std::string joinVector(std::vector<int>& in, char del) {
        std::string out;
        for (auto it : in) {
            out  += std::to_string(it) + del;
        }
        out.pop_back();
        return out;
    };

    std::string prettifyToString(int val) {
        std::string stringVal = std::to_string(val);
        std::string ret(stringVal.size() + int(stringVal.size() / 3), ' ');

        int count = 0;
        int retItr = ret.size() - 1;
        int i = stringVal.size() - 1;
        while(i >= 0) {

            if (count < 3) {
                ret[retItr] = stringVal[i];
                count++;
                i--;
            }

            else{
                ret[retItr] = ',';
                count = 0;
            }

             retItr--;
        }
        return ret;
    }

    TPixelCoordinate::TPixelCoordinate()
    {
        // Default constructor
        i = 0;
        j = 0;
    };

    TPixelCoordinate::TPixelCoordinate(int i, int j) 
    {
        this->i = i;
        this->j = j;
    };
    TPixelCoordinate::TPixelCoordinate(std::pair<int, int> p) 
    {
        this->i = p.first;
        this->j = p.second;
    };



};

namespace Hash {
    cv::Mat PhashImage(const cv::Mat& image) {

        cv::Mat resized;
        cv::resize(image, resized, cv::Size(32, 32));

        auto phasher = cv::img_hash::PHash::create();
        cv::Mat hash;
        phasher->compute(resized, hash);

        auto res = phasher->compare(hash, hash);
        return hash;

    }


    cv::Mat templateMatch(const cv::Mat& a, const cv::Mat& b) {
        cv::Mat resized_a;
        cv::resize(a, resized_a, cv::Size(32, 32));

        cv::Mat resized_b;
        cv::resize(b, resized_b, cv::Size(32, 32));

        cv::Mat res;
        cv::matchTemplate(resized_a, resized_b,res, cv::TM_SQDIFF_NORMED);
        return res;

    };


} 