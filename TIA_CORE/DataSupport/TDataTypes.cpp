#include <DataSupport/TDataTypes.h>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/img_hash.hpp>

namespace TDataTypes
{

    // Split on character delimiter.
    void splitString(std::string s, char del, std::vector<std::string>& out) {
        if (s.empty())
            return;
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
        int numCommas = round((stringVal.size() - 1) / 3);
        if (!numCommas)
            return stringVal;
        std::string ret(stringVal.size() + numCommas, ' ');

        int count = 0;
        size_t retItr = ret.size() - 1;
        size_t i = stringVal.size() - 1;
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

    TCurrency::TCurrency() :
        m_value(0),
        m_unit(),
        m_isPrependedUnit(false)
        {};


    TCurrency::TCurrency(const std::string& currString) {
        if (currString.empty()) {
            m_unit = "";
            m_value = 0;
            m_isPrependedUnit = false;
            return;
        }

        int lastDigitIdx = -1;
        for (size_t i = currString.size() - 1; i > 0; i--) {
            if (std::isdigit(static_cast<unsigned char>(currString[i]))) {
                lastDigitIdx = i;
                break;
            }
        }

        // Implies this has a prepended unit.
        if (lastDigitIdx == currString.size() - 1) {
            m_isPrependedUnit = true;
            m_unit = currString.substr(0,1);
            std::string val = currString.substr(1);
            val.erase(std::remove(val.begin(), val.end(), ','), val.end());
            m_value = std::stoi(val);
        }
        else {
            // Appended unit. (these are utf8 symbols that take up multiple characters)
            m_isPrependedUnit = false;
            m_unit = currString.substr(lastDigitIdx + 1);
            std::string val = currString.substr(0, lastDigitIdx + 1);
            val.erase(std::remove(val.begin(), val.end(), ','), val.end());
            m_value = std::stoi(val);
        }

    }

    const int TCurrency::getValue() const {
        return m_value;
    }

    const std::string TCurrency::getCurrencyString(bool prettify) const {
        if (m_value) {
            if (m_unit == "$") {
                std::cout << "wed";
            }
            if(!prettify)
                return m_isPrependedUnit ? m_unit + std::to_string(m_value) : std::to_string(m_value) + m_unit;
            else
                return m_isPrependedUnit ? m_unit + TDataTypes::prettifyToString(m_value) : TDataTypes::prettifyToString(m_value) + m_unit;
        }
        else
            return "";
    }

    const std::string& TCurrency::getUnit() const {
        return m_unit;
    }

    const bool TCurrency::isPrependedUnit() const {
        return m_isPrependedUnit;
    }

    std::string TCurrency::getValueMultipledBy(int num, bool prettify) const {
        if (!prettify)
            return m_isPrependedUnit ? m_unit + std::to_string(m_value * num) : std::to_string(m_value * num) + m_unit;

        return m_isPrependedUnit ? m_unit + TDataTypes::prettifyToString(m_value * num) : TDataTypes::prettifyToString(m_value * num) + m_unit;

    }

    TCurrency& TCurrency::operator+=(const TCurrency& rhs) {
        if (m_unit != rhs.m_unit)
            throw  std::invalid_argument("LHS Unit: " + m_unit + " != " + "RHS Unit: " + rhs.m_unit);
        m_value += rhs.m_value;
        return *this;
    }

    TCurrency& TCurrency::operator-=(const TCurrency& rhs) {
        if (m_unit != rhs.m_unit)
            throw  std::invalid_argument("LHS Unit: " + m_unit + " != " + "RHS Unit: " + rhs.m_unit);
        m_value -= rhs.m_value;
        return *this;
    }
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