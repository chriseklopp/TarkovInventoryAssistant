#include "TImageReader.h"


bool TImageReader::parseImage(const cv::Mat& image,
    std::vector<std::unique_ptr<TItemTypes::TItem>>& retItems, std::vector<cv::Point>& retLocs){


    m_cellsize = lrint(image.rows / 17.14); // This cell size ratio SEEMS to hold for atleast 1080 and 1440 res.

    // Determine if this is an image containing containers or a stash (TODO: check for gamerun mode)
    std::vector<std::pair<cv::Point, cv::Point>> containerLocations;
    int numDet = detectOpenContainers(image, containerLocations);

    if (numDet > 0) {
        resolveContainerImage(image, containerLocations, retItems, retLocs);
        return true;
    }
    else {
        std::pair<cv::Point, cv::Point> stashLoc;
        bool stashDetected = detectStash(image, stashLoc);
    }
    return true;
} 



bool TImageReader::parseFromPath(const std::string path,
    std::vector<std::unique_ptr<TItemTypes::TItem>>& retItems, std::vector<cv::Point>& retLocs){

    cv::Mat image = cv::imread(path);
    
    return parseImage(image, retItems, retLocs);
}

int TImageReader::detectOpenContainers(const cv::Mat& image,
                                       std::vector <std::pair<cv::Point, cv::Point>>& out){
    int ret = 0;
    cv::Mat imgHSV;
    cv::cvtColor(image, imgHSV, cv::COLOR_BGR2HSV);

    // Our "Magic" threshold values to containers.
    cv::Scalar lower(0, 0, 1);
    cv::Scalar upper(179, 255, 255);

    cv::Mat mask;
    cv::inRange(imgHSV, lower, upper, mask);

    cv::Mat invertMask;
    cv::bitwise_not(mask, invertMask);

    cv::Mat eroded;
    // TODO: MAKE KERNEL (its currently default cv::Mat() ) 
    cv::erode(invertMask, eroded, cv::Mat(), cv::Point(-1,-1), 1);

    cv::Mat dilated;
    cv::dilate(eroded, dilated, cv::Mat(), cv::Point(-1, -1), 1);


    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(dilated, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);


    for (auto cont = contours.begin(); cont != contours.end(); ++cont) {

        double area = cv::contourArea(*cont, true);
        if (area > 15000) {
            double peri = cv::arcLength(*cont, true);

            std::vector<cv::Point> approx;
            cv::approxPolyDP(*cont, approx, .2 * peri, true);

            cv::Point lowerP = approx.at(0);
            cv::Point upperP = approx.at(1);
            
            // Occasionally the points in the first item of approx are not the lowest points.
            if (lowerP.x > upperP.x) {
                int temp = upperP.x;
                upperP.x = lowerP.x;
                lowerP.x = temp; 
            }

            if (lowerP.y > upperP.y) {
                int temp = upperP.y;
                upperP.y = lowerP.y;
                lowerP.y = temp;
            }
            ret++;
            out.push_back(std::make_pair(approx.at(0), approx.at(1)));
        }



    }

    return ret;
}

void TImageReader::resolveContainerImage(const cv::Mat& image,
                                         std::vector<std::pair<cv::Point, cv::Point>> locs,
                                         std::vector<std::unique_ptr<TItemTypes::TItem>>& retItems,
                                         std::vector<cv::Point>& retLocs) {

    cv::Mat imgHSV;
    cv::cvtColor(image, imgHSV, cv::COLOR_BGR2HSV);

    // Our "Magic" threshold values to detect item outlines.
    cv::Scalar lower(94, 7, 50);
    cv::Scalar upper(105, 36, 115);

    cv::Mat mask;
    cv::inRange(imgHSV, lower, upper, mask);


    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(mask, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

    for (auto cont = contours.begin(); cont != contours.end(); ++cont) {

        double area = cv::contourArea(*cont, true);
        if (area > 900) {
            double peri = cv::arcLength(*cont, true);

            std::vector<cv::Point> approx;
            cv::approxPolyDP(*cont, approx, .2 * peri, true);

            cv::Point lowerP = approx.at(0);
            cv::Point upperP = approx.at(1);

            // Occasionally the points in the first item of approx are not the lowest points.
            if (lowerP.x > upperP.x) {
                int temp = upperP.x;
                upperP.x = lowerP.x;
                lowerP.x = temp;
            }

            if (lowerP.y > upperP.y) {
                int temp = upperP.y;
                upperP.y = lowerP.y;
                lowerP.y = temp;
            }


            cv::Mat itemImage = image(cv::Range(lowerP.y, upperP.y), cv::Range(lowerP.x, upperP.x));
            //cv::imshow("a", itemImage);
            //cv::waitKey(0);
            // We create a placeholder TItem with our image and its dimensions.
            retItems.push_back(TItemTypes::TItem::makePlaceHolder(itemImage, m_cellsize));
            retLocs.push_back(lowerP);
        }

    }

}


void TImageReader::resolveStashImage(const cv::Mat& image,
    std::pair<cv::Point, cv::Point> loc,
    std::vector<std::unique_ptr<TItemTypes::TItem>>& retItems, std::vector<cv::Point>& retLocs) {};

bool TImageReader::detectStash(const cv::Mat& image, std::pair<cv::Point, cv::Point>& loc) {
    return false;
};