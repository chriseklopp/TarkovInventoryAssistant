#include <DataSupport/TItemTypes.h>



namespace TItemTypes {




    // Probably dont use this..
    TItem::TItem() :
        m_name("None"),
        m_image(cv::Mat()),
        m_dim(std::make_pair(0, 0)),
        m_isRotated(false),
        isPlaceHolder(true),
        m_imageHash(Hash::PhashImage(cv::Mat(32, 32, CV_8UC3)))
    {};

    // Construct a fully-fledged TItem. This should be the most common way to construct a TItem.
    TItem::TItem(const std::string& name, const cv::Mat& image, const std::pair<int, int>& dim, bool rotated, bool placeholder) :
        m_name(name),
        m_image(image),
        m_dim(dim),
        m_isRotated(rotated),
        isPlaceHolder(placeholder),
        m_imageHash(Hash::PhashImage(image))
    {};

    // Construct a fully-fledged TItem, using an image from a path.
    TItem::TItem(const std::string& name, const std::filesystem::path& imagePath, const std::pair<int, int>& dim, bool rotated, bool placeholder) :
        m_name(name),
        m_image(cv::imread(imagePath.string())),
        m_dim(dim),
        m_isRotated(rotated),
        isPlaceHolder(placeholder),
        m_imageHash(Hash::PhashImage(m_image))
    {};

    // Construct a fully-fledged TItem, using an image from a path and a premade hash. Mostly used for loading catalog items.
    TItem::TItem(const std::string& name, const std::filesystem::path& imagePath, const std::pair<int, int>& dim, const TItemSupport::PriceInfo& priceInfo, bool rotated, const cv::Mat& hash, bool placeholder) :
        m_name(name),
        m_image(cv::imread(imagePath.string())),
        m_dim(dim),
        m_priceInfo(priceInfo),
        m_isRotated(rotated),
        isPlaceHolder(placeholder),
        m_imageHash(hash)
    {};




    std::unique_ptr<TItem> TItem::makePlaceHolder(const cv::Mat& image, double cellSize) {
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


    const TDataTypes::TCurrency& TItem::getPrice() const {
        return m_priceInfo.price;
    }

    //Returns TDataTypes flea price per slot for item
    const TDataTypes::TCurrency& TItem::getPricePerSlot() const {
        return m_priceInfo.pricePerSlot;
    }

    // Get best trader sell price
    const TDataTypes::TCurrency& TItem::getTraderSellPrice() const {
        return m_priceInfo.traderPrice;
    }

    // Get name of best trader to sell to.
    const std::string& TItem::getTrader() const {
        return m_priceInfo.trader;
    }

    const bool TItem::isFleaOptimal() const {
        return m_priceInfo.isFleaOptimal;
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

    bool compareByName(TItem a, TItem b)
    {
        return a.m_name < b.m_name;
    }

}


namespace TItemSupport {


    

}