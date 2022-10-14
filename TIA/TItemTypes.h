#pragma once

#include <iostream>
#include <vector>
#include "TDataTypes.h"
#include <opencv2/opencv.hpp>
namespace TItemTypes {




    class TItem {
    public:

        TItem() :
            m_name("None"),
            m_image(cv::Mat()),
            m_dim(std::make_pair(0, 0)),
            m_isRotated(false),
            isPlaceHolder(true)
        {};

        // Construct a fully-fledged TItem. This should be the most common way to construct a TItem.
        TItem(std::string& name, cv::Mat& image, std::pair<int, int>& dim, bool rotated, bool placeholder = false):
            m_name(name),
            m_image(image),
            m_dim(dim),
            m_isRotated(rotated),
            isPlaceHolder(placeholder)
        {};

        // Construct a NEW placeholder item from an image and populate its dims using cellsize.
        // Will have isPlaceHolder flag = true to signify it is not fully populated.
        // A placeholder is intended to be populated by another fully formed TItem (or derived) object.
        static std::shared_ptr<TItem> makePlaceHolder(cv::Mat& image, int cellSize);


        // Populates the empty fields of a placeholder TItem with the values from a fully-fledged TItem.
        // The placeholder will be considered fully-fledged after this process.
        // !! There is not check that the fully-fledged item ACTUALLY is so !!
        static void makeQualified(TItem& to, TItem& from);



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
            isPlaceHolder(true) {};
    };

    class TContainerItem : public TItem {


    public:
        // Insert item(s) into the container.
        void insert(TItem item);
        void insert(std::vector<TItem> items);


        std::vector<TItem>& getContents();
        void printContents();


    private:

        TDataTypes::gridType m_storageType; // Will be used when we try to accurately reflect the contents of images.
        std::vector<TItem> m_contents; // Contains all items placed in the container.

        int m_spaceFilled;
        int m_capacity;

    };


    bool compareByName(std::shared_ptr<TItem> a, std::shared_ptr<TItem> b);


}