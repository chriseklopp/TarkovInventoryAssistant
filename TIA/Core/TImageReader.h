#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>
#include <DataSupport/TItemTypes.h>
#include <Core/TConfig.h>


#include <Tests/TTestLibrary.h> // TODO: REMOVE BEFORE MERGE!!!!!!!
/*
* 
Processes the screenshot(s) into detected TItems.
Contains logic for processing a  screenshots into TItems.
Will determine if screenshot is of stash or of a container object.
*/
class TImageReader {
public:
    TImageReader(TConfig::TConfig* config) : 
        m_configptr(config),
        m_cellsize(63),
        needsInit(true)
    {}

    // Parses a cv::Mat into a vector of items detected.
    bool parseImage(const cv::Mat& image,
        std::vector<std::unique_ptr<TItemTypes::TItem>>& retItems, std::vector<std::pair<cv::Point, cv::Point>>& retLocs);

    // Works like above fucntion, except accepts a path instead of an already made cv::Mat. 
    bool parseFromPath(const std::string path, std::vector<std::unique_ptr<TItemTypes::TItem>>& retItems, std::vector<std::pair<cv::Point, cv::Point>>& retLocs);

private:

    //bool parseItems(std::vector<std::shared_ptr<TItemTypes::TItem>>& retItems);

    // Detect if there are any open containers in the image. Returns number of containers found.
    // and 
    int detectOpenContainers(const cv::Mat& image, std::vector<std::pair<cv::Point, cv::Point>>& out );
    
    bool detectStash(const cv::Mat& image, std::pair<cv::Point, cv::Point>& loc); // Finds coordinates of the stash in an image.


     // Accept container coordinates. Returns all items in the container.
    void resolveContainerImage(const cv::Mat& image,
        const std::vector<std::pair<cv::Point, cv::Point>>& locs,
        std::vector<std::unique_ptr<TItemTypes::TItem>>& retItems, std::vector<std::pair<cv::Point, cv::Point>>& retLocs);

    void resolveStashImage(const cv::Mat& image,
        std::pair<cv::Point, cv::Point> loc,
        std::vector<std::unique_ptr<TItemTypes::TItem>>& retItems, std::vector<std::pair<cv::Point, cv::Point>>& retLocs);


    void resolveContainerHeader(); //Find the header in a container image.

    // Determine if a detected "item" image is actually empty space!
    // Input hsv image ONLY!
    bool isEmptySpace(const cv::Mat& hsvImg);

    int m_cellsize;
    // std::vector<TEMP> m_containerList;
    bool needsInit;


    TConfig::TConfig* m_configptr;
};