#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>
#include "TItemTypes.h"
#include "TDataTypes.h"
/*
* 
Processes the screenshot(s) into detected TItems.
Contains logic for processing a  screenshots into TItems.
Will determine if screenshot is of stash or of a container object.
Read items into a dynamic placeholder TContainerItem.
The contents of this container can then be copied to its rightful container object (be it the Stash or a container)
later. (and probably in the TStashManager module)

*/
class TImageReader {
public:
    TImageReader() : 
        m_cellsize(63),
        needsInit(true)
    {}

    // Parses a cv::Mat into a vector of items detected.
    bool parseImage(const cv::Mat& image,
        std::vector<std::shared_ptr<TItemTypes::TItem>>& retItems);

    // Works like above fucntion, except accepts a path instead of an already made cv::Mat. 
    bool parseFromPath(const std::string path, std::vector<std::shared_ptr<TItemTypes::TItem>>& retItems);

private:

    //bool parseItems(std::vector<std::shared_ptr<TItemTypes::TItem>>& retItems);

    // Detect if there are any open containers in the image. Returns number of containers found.
    // and 
    int detectOpenContainers(const cv::Mat& image, std::vector <std::pair<cv::Point, cv::Point>>& out );
    
    bool detectStash(const cv::Mat& image, std::pair<cv::Point, cv::Point>& loc); // Finds coordinates of the stash in an image.


     // Accept container coordinates. Returns all items in the container.
    void resolveContainerImage(const cv::Mat& image,
        std::vector<std::pair<cv::Point, cv::Point>> locs,
        std::vector<std::shared_ptr<TItemTypes::TItem>>& retItems);

    void resolveStashImage(const cv::Mat& image,
        std::pair<cv::Point, cv::Point> loc,
        std::vector<std::shared_ptr<TItemTypes::TItem>>& retItems);


    void resolveContainerHeader(); //Find the header in a container image.



    int m_cellsize;
    // std::vector<TEMP> m_containerList;
    bool needsInit;
};