#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>


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
    std::string joinVector(const std::vector<std::string>& in, char del);

    // Join vector to std::string on character delimiter.
    std::string joinVector(const std::vector<int>& in, char del);

    // Converts integer to string and adds commas where necessary.
    std::string prettifyToString(int val);


    // Denotes what type of storage a TContainerItem contains.
    enum gridType {
        ARRAY = 0, // A contiguous block/grid of item slots.
        DYNAMIC = 1 // Could have disconnected slots of varying sizes and positions.
    };


    enum class currencyType {
        UNKNOWN,
        RUBLE,
        DOLLAR,
        EURO,
    };

    currencyType getCurrencyType(const std::string& currency);

    constexpr int convertToRubles(int count, currencyType curr) {
        // These values from https://escapefromtarkov.fandom.com/wiki/Currency
        switch (curr) {
            case currencyType::RUBLE:
                return count;

            case currencyType::DOLLAR:
                return 116*count;

            case currencyType::EURO:
                return 127 * count;

            default:
                return 0;
        }
    }


    enum class trader {
        Unknown,
        Prapor,
        Therapist,
        Fence,
        Skier,
        Peacekeeper,
        Mechanic,
        Ragman,
        Jaeger
    };

    trader toTraderEnum(const std::string& traderString);


    // Multiplier at which a trader will buy back an item.
    constexpr float traderBuybackMultiplier(trader buyer) {
        // These values from https://escapefromtarkov.fandom.com/wiki/Trading
        switch (buyer) {
            case trader::Prapor:
                return 0.50f;

            case trader::Therapist:
                return 0.63f;

            case trader::Fence:
                return 0.4f;

            case trader::Skier:
                return 0.49f;

            case trader::Peacekeeper:
                return 0.45f;

            case trader::Mechanic:
                return 0.56f;

            case trader::Ragman:
                return 0.62f;

            case trader::Jaeger:
                return 0.60f;

            default:
                return -1.0f;
        }
    }



    class TCurrency {

    public:
        TCurrency();
        TCurrency(const std::string& currString);
        ~TCurrency() {};

        const int getValue() const;
        const std::string getCurrencyString(bool prettify=false) const;
        const std::string& getUnit() const;
        const bool isPrependedUnit() const;

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
  
    // Calculates whether selling an item to the trader or on the flea is better.
    bool isFleaOptimal(TCurrency fleaCurr, TCurrency traderCurr, trader trad);

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