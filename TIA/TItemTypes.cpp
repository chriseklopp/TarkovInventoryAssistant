#include "TItemTypes.h"



namespace TItemTypes {




    // Probably dont use this..
    TItem::TItem() :
        m_name("None"),
        m_image(cv::Mat()),
        m_dim(std::make_pair(0, 0)),
        m_isRotated(false),
        isPlaceHolder(true),
        m_imageHash(Hash::PhashImage(cv::Mat()))
    {};

    // Construct a fully-fledged TItem. This should be the most common way to construct a TItem.
    TItem::TItem(std::string& name, cv::Mat& image, std::pair<int, int>& dim, bool rotated, bool placeholder) :
        m_name(name),
        m_image(image),
        m_dim(dim),
        m_isRotated(rotated),
        isPlaceHolder(placeholder),
        m_imageHash(Hash::PhashImage(image))
    {};

    // Construct a fully-fledged TItem, using an image from a path.
    TItem::TItem(std::string& name, std::filesystem::path& imagePath, std::pair<int, int>& dim, bool rotated, bool placeholder) :
        m_name(name),
        m_image(cv::imread(imagePath.string())),
        m_dim(dim),
        m_isRotated(rotated),
        isPlaceHolder(placeholder),
        m_imageHash(Hash::PhashImage(m_image))
    {};

    // Construct a fully-fledged TItem, using an image from a path and a premade hash. Mostly used for loading catalog items.
    TItem::TItem(std::string& name, std::filesystem::path& imagePath, std::pair<int, int>& dim, TItemSupport::PriceInfo& priceInfo, bool rotated, cv::Mat& hash, bool placeholder) :
        m_name(name),
        m_image(cv::imread(imagePath.string())),
        m_dim(dim),
        m_priceInfo(priceInfo),
        m_isRotated(rotated),
        isPlaceHolder(placeholder),
        m_imageHash(hash)
    {};




    std::unique_ptr<TItem> TItem::makePlaceHolder(cv::Mat& image, double cellSize) {
        int width = round(image.cols / cellSize);
        int height = round(image.rows / cellSize);
        return  std::make_unique<TItem>(TItem(image, std::make_pair(width, height)));
    };

    void TItem::makeQualified(TItem& to, TItem& from) {
        to.m_name = from.m_name;
        to.m_isRotated = from.m_isRotated;
        to.isPlaceHolder = false;
        // TODO: Needs to cast "to" to derived class if "from" is a derived class type.
    };


    const TItemSupport::TCurrency TItem::getPrice() const {
        return m_priceInfo.price;
    }

    //Returns average flea price per slot for item
    const TItemSupport::TCurrency TItem::getPricePerSlot() const {
        return m_priceInfo.pricePerSlot;
    }

    // Get best trader sell price
    const TItemSupport::TCurrency TItem::getTraderSellPrice() const {
        return m_priceInfo.traderPrice;
    }

    // Get name of best trader to sell to.
    const std::string TItem::getTrader() const {
        return m_priceInfo.trader;
    }




    bool TContainerItem::insert(TItem item, cv::Point location)
    {
        m_spaceFilled += item.m_dim.first * item.m_dim.second;
        m_contents.push_back(item);
        return true;
    };

    bool TContainerItem::insert(std::vector<std::pair<TItem, cv::Point>> items)
    {
        for (std::pair<TItem, cv::Point> p : items) {
            // TODO: this is so so so inefficient. Also needs to use shared ptr.
            TItem it = p.first;
            cv::Point loc = p.second;
            m_spaceFilled += it.m_dim.first * it.m_dim.second;
            m_contents.push_back(it);
        }
        return true;
    };



    // Return contents of this Container
    std::vector<TItem>& TContainerItem::getContents()
    {
        return m_contents;
    };

    // Print names of all contents in this container.
    void TContainerItem::printContents()
    {
        for (TItem item : m_contents) {
            std::cout << item.m_name << std::endl;
        }
    };

    bool TItemTypes::compareByName(TItem a, TItem b)
    {
        return a.m_name < b.m_name;
    }

}


namespace TItemSupport {


    TCurrency::TCurrency(const std::string& currString) {
        m_rawString = currString;
        if (currString.empty()) {
            m_unit = "";
            m_value = 0;
            m_isPrependedUnit = false;
            return;
        }

        int lastDigitIdx = -1;
        for(int i = currString.size()-1; i > 0; i--){
            if (std::isdigit(static_cast<unsigned char>(currString[i]))) {
                lastDigitIdx = i;
                break;
            }
        }

        // Implies this has a prepended unit.
        if (lastDigitIdx == currString.size()-1) {
            m_isPrependedUnit = true;
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

    int TCurrency::getValue() const {
        return m_value;
    }

    std::string TCurrency::getCurrencyString() const {
        if (m_value) {
            return m_rawString;
        }
        else
            return "";
    }

    std::string TCurrency::getUnit() const {
        return m_unit;
    }

    bool TCurrency::isPrependedUnit() const {
        return m_isPrependedUnit;
    }

    std::string TCurrency::multiplyBy(int num, bool prettify) const {
        if (num == 1 || !m_value) // more efficient for these edge cases
            return m_rawString;

        if(!prettify)
            return m_isPrependedUnit ? m_unit + std::to_string(m_value * num) : std::to_string(m_value * num) + m_unit;

        return m_isPrependedUnit ? m_unit + TDataTypes::prettifyToString(m_value * num) : TDataTypes::prettifyToString(m_value * num) + m_unit;

    }

}