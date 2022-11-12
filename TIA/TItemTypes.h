#pragma once
#include <iostream>
#include <vector>
#include "TDataTypes.h"
#include <opencv2/opencv.hpp>
#include <filesystem>
namespace TItemTypes {




    class TItem {
    public:

        // Probably dont use this..
        TItem() :
            m_name("None"),
            m_image(cv::Mat()),
            m_dim(std::make_pair(0, 0)),
            m_isRotated(false),
            isPlaceHolder(true),
            m_imageHash(Hash::PhashImage(cv::Mat()))
        {};

        // Construct a fully-fledged TItem. This should be the most common way to construct a TItem.
        TItem(std::string& name, cv::Mat& image, std::pair<int, int>& dim, bool rotated, bool placeholder = false):
            m_name(name),
            m_image(image),
            m_dim(dim),
            m_isRotated(rotated),
            isPlaceHolder(placeholder),
            m_imageHash(Hash::PhashImage(image))
        {};

        // Construct a fully-fledged TItem, using an image from a path.
        TItem(std::string& name, std::filesystem::path& imagePath, std::pair<int, int>& dim, bool rotated, bool placeholder = false) :
            m_name(name),
            m_image(cv::imread(imagePath.string())),
            m_dim(dim),
            m_isRotated(rotated),
            isPlaceHolder(placeholder),
            m_imageHash(Hash::PhashImage(m_image))
        {};

        // Construct a fully-fledged TItem, using an image from a path and a premade hash.
        TItem(std::string& name, std::filesystem::path& imagePath, std::pair<int, int>& dim, bool rotated, cv::Mat& hash, bool placeholder = false) :
            m_name(name),
            m_image(cv::imread(imagePath.string())),
            m_dim(dim),
            m_isRotated(rotated),
            isPlaceHolder(placeholder),
            m_imageHash(hash)
        {};


        virtual ~TItem() {};

        // Construct a NEW placeholder item from an image and populate its dims using cellsize.
        // Will have isPlaceHolder flag = true to signify it is not fully populated.
        // A placeholder is intended to be populated by another fully formed TItem (or derived) object.
        static std::unique_ptr<TItem> makePlaceHolder(cv::Mat& image, double cellSize);

        // TItem does not hold items. But we need this functionality for derived TContainerItem
        virtual bool insert(TItem item, cv::Point location) { return false; };

        virtual bool insert(std::vector<std::pair<TItem, cv::Point>> items) { return false; };

        // Populates the empty fields of a placeholder TItem with the values from a fully-fledged TItem.
        // The placeholder will be considered fully-fledged after this process.
        // !! There is no check that the fully-fledged item ACTUALLY is so !!
        static void makeQualified(TItem& to, TItem& from);


        const cv::Mat getImage() const { return m_image; };

        const std::string getName() const { return m_name; };

        const std::pair<int, int> getDim() const { return m_dim; };
        const std::string getDimAsString(char dlm = 'x') const { return std::to_string(m_dim.first) + dlm + std::to_string(m_dim.second); };

        cv::Mat m_imageHash; // TODO: change this to proper type.
        std::string m_name;
        std::pair<int, int> m_dim; // (Width, Height)

        bool operator==(TItem const& other) const { return m_name == other.m_name; }

    protected:

        cv::Mat m_image;
        bool m_isRotated;
        bool isPlaceHolder;

    private:



        TItem(cv::Mat& image, std::pair<int,int> dim ): 
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
        TContainerItem(std::string& name,
            cv::Mat& image,
            std::pair<int, int>& dim,
            std::pair<int,int>& containerDim,
            bool rotated,
            bool placeholder = false) : 

            TItem(name,image, dim, rotated),
            m_spaceFilled(0),
            m_capacity(containerDim.first * containerDim.second),
            m_containerDim(containerDim)
        {};

        TContainerItem(std::string& name,
            std::filesystem::path& imagePath,
            std::pair<int, int>& dim,
            std::pair<int, int>& containerDim,
            bool rotated,
            cv::Mat& hash,
            bool placeholder = false) :

            TItem(name, imagePath, dim, rotated, hash),
            m_spaceFilled(0),
            m_capacity(containerDim.first* containerDim.second),
            m_containerDim(containerDim)
        {};


        // Insert item(s) into the container. Returns true on success.
        bool insert(TItem item, cv::Point location) override;
        bool insert(std::vector<std::pair<TItem, cv::Point>> items) override;


        std::vector<TItem>& getContents();
        void printContents();


    private:

        TDataTypes::gridType m_storageType; // Will be used when we try to accurately reflect the contents of images.
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

        DetectionResult() {};

        DetectionResult(TItemTypes::TItem* catItem,
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




        TItemTypes::TItem* catalogItem;
        std::unique_ptr<TItemTypes::TItem> inputItem;

        int parentImageID; // ImageID from where the item was detected.
        std::pair<cv::Point, cv::Point> imageLoc; // Point locations in the parent image where item was detected. (top left corner, bottom right corner).

        bool detectionError = false; // Mostly for testing and debug uses, signify that this detection was wrong.
    };


}