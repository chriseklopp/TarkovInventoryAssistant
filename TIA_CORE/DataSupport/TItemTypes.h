#pragma once
#include <iostream>
#include <vector>
#include <DataSupport/TDataTypes.h>
#include <filesystem>


// Contains supporting objects for TItemTypes; using a separate namespace to preserve the
// original meaning of the TItemTypes space
namespace TItemSupport {
    

    struct PriceInfo {

        PriceInfo(): isFleaOptimal(false) {};

        PriceInfo(std::string price,
            std::string pricePerSlot,
            std::string traderPrice,
            std::string trader,
            bool fleaOptimal
        ) :

            price(price),
            pricePerSlot(pricePerSlot),
            traderPrice(traderPrice),
            trader(trader),
            isFleaOptimal(fleaOptimal)
        {}


        const TDataTypes::TCurrency price;
        const TDataTypes::TCurrency pricePerSlot;
        const TDataTypes::TCurrency traderPrice;
        const std::string trader;
        const bool isFleaOptimal; // True if selling on flea is better than selling to trader.

    };


}

namespace TItemTypes {




    class TItem {
    public:

        // Probably dont use this..
        TItem();

        // Construct a fully-fledged TItem. This should be the most common way to construct a TItem.
        TItem(const std::string& name, const cv::Mat& image, const std::pair<int, int>& dim, bool rotated, bool placeholder = false);

        // Construct a fully-fledged TItem, using an image from a path.
        TItem(const std::string& name, const std::filesystem::path& imagePath, const std::pair<int, int>& dim, bool rotated, bool placeholder = false);

        // Construct a fully-fledged TItem, using an image from a path and a premade hash. Mostly used for loading catalog items.
        TItem(const std::string& name, const std::filesystem::path& imagePath, const std::pair<int, int>& dim, const TItemSupport::PriceInfo& priceInfo, bool rotated, const cv::Mat& hash, bool placeholder = false);


        virtual ~TItem() {};

        // Construct a NEW placeholder item from an image and populate its dims using cellsize.
        // Will have isPlaceHolder flag = true to signify it is not fully populated.
        // A placeholder is intended to be populated by another fully formed TItem (or derived) object.
        static std::unique_ptr<TItem> makePlaceHolder(const cv::Mat& image, double cellSize);

        // TItem does not hold items. But we need this functionality for derived TContainerItem
        virtual bool insert(TItem item, cv::Point location) { return false; };

        virtual bool insert(std::vector<std::pair<TItem, cv::Point>> items) { return false; };

        // Populates the empty fields of a placeholder TItem with the values from a fully-fledged TItem.
        // The placeholder will be considered fully-fledged after this process.
        // !! There is no check that the fully-fledged item ACTUALLY is so !!
        static void makeQualified(TItem& to, TItem& from);


        const cv::Mat& getImage() const { return m_image; };

        const std::string& getName() const { return m_name; };

        const bool isRotated() const { return m_isRotated; }


        const std::pair<int, int>& getDim() const { return m_dim; };
        const std::string getDimAsString(char dlm = 'x') const { return std::to_string(m_dim.first) + dlm + std::to_string(m_dim.second); };

        // Price info
        // Returns average flea price for item
        const TDataTypes::TCurrency& getPrice() const;

        //Returns average flea price per slot for item
        const TDataTypes::TCurrency& getPricePerSlot() const;

        // Get best trader sell price
        const TDataTypes::TCurrency& getTraderSellPrice() const;

        // Get name of best trader to sell to.
        const std::string& getTrader() const;

        // True if selling on flea is better than selling to trader.
        const bool isFleaOptimal() const;


        cv::Mat m_imageHash;
        std::string m_name;
        std::pair<int, int> m_dim; // (Width, Height)

        bool operator==(TItem const& other) const { return m_name == other.m_name; }

    protected:

        cv::Mat m_image;
        bool m_isRotated;
        bool isPlaceHolder;
        TItemSupport::PriceInfo m_priceInfo;

    private:



        TItem(const cv::Mat& image, std::pair<int,int> dim ): 
            m_name("None"),
            m_image(image),
            m_dim(dim),
            m_isRotated(false),
            isPlaceHolder(true),
            m_imageHash(Hash::PhashImage(image)) {};
    };

    // TODO: We need a way to handle irregular shaped container objects. The solution could be some TIrregularContainerClass.
    class TContainerItem : public TItem {


    public:
            
        // Construct a fully-fledged TContainerItem. This should be the most common way to construct a TContainerItem.
        TContainerItem(const std::string& name,
            const cv::Mat& image,
            const std::pair<int, int>& dim,
            const std::pair<int,int>& containerDim,
            bool rotated,
            bool placeholder = false) : 

            TItem(name,image, dim, rotated),
            m_spaceFilled(0),
            m_capacity(containerDim.first * containerDim.second),
            m_containerDim(containerDim)
        {};

        TContainerItem(const std::string& name,
            const std::filesystem::path& imagePath,
            const std::pair<int, int>& dim,
            const std::pair<int, int>& containerDim,
            const TItemSupport::PriceInfo& priceInfo,
            bool rotated,
            cv::Mat& hash,
            bool placeholder = false) :

            TItem(name, imagePath, dim, priceInfo, rotated, hash),
            m_spaceFilled(0),
            m_capacity(containerDim.first* containerDim.second),
            m_containerDim(containerDim)
        {};

        ~TContainerItem() {};

        // Insert item(s) into the container. Returns true on success.
        bool insert(TItem item, cv::Point location) override;
        bool insert(std::vector<std::pair<TItem, cv::Point>> items) override;


        std::vector<TItem>& getContents();
        void printContents();


    private:

        std::vector<TItem> m_contents; // Contains all items placed in the container.

        std::pair<int, int> m_containerDim; // (Width, Height) of storage grid inside the container.


        int m_spaceFilled;
        int m_capacity;


    };


    bool compareByName(TItem a, TItem b);


}

// Contains supporting objects for TItemTypes; using a separate namespace to preserve the
// original meaning of the TItemTypes space
namespace TItemSupport {


  /* Packaging for the results of an item detection.
  * By keeping this info together we can more easily access the stuff we need in the GUI without passing
  * multiple objects and associations around.
  * Note: This struct OWNs its inputItem.
  */
    struct DetectionResult {

        //DetectionResult() {};

        DetectionResult(TDataTypes::dcID catItem,
                        std::unique_ptr<TItemTypes::TItem> inItem,
                        int parentImageID,
                        std::pair<cv::Point, cv::Point> itemLoc= std::make_pair(cv::Point(-1, -1), cv::Point(-1, -1)),
                        bool isDetectionError=false) :

            catalogItem(catItem),
            parentImageID(parentImageID),
            imageLoc(itemLoc),
            detectionError(isDetectionError)

        {
            inputItem = std::move(inItem);
        };


        TDataTypes::dcID catalogItem;
        std::unique_ptr<TItemTypes::TItem> inputItem;

        imageID parentImageID; // ImageID from where the item was detected.
        std::pair<cv::Point, cv::Point> imageLoc; // Point locations in the parent image where item was detected. (top left corner, bottom right corner).

        bool detectionError = false; // Mostly for testing and debug uses, signify that this detection was wrong.
    };




}