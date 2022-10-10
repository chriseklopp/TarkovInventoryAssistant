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
            isPlaceHolder(false)
        {};

        // Construct a NEW placeholder item from an image and populate its dims using cellsize.
        // Will have isPlaceHolder flag = true to signify it is not fully populated.
        // A placeholder is intended to be populated by another fully formed TItem (or derived) object.
        static std::shared_ptr<TItem> makePlaceHolder(cv::Mat& image, int cellSize) {
            int width = lrint(image.cols / cellSize);
            int height = lrint(image.rows / cellSize);
            return  std::make_shared<TItem>(TItem(image, std::make_pair(width, height)));
        };

        std::string m_name;
        std::pair<int, int> m_dim; // (Width, Height)
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



}