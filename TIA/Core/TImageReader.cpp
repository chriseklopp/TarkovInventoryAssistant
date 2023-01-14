#include <Core/TImageReader.h>

bool TImageReader::parseImage(const cv::Mat& image,
    std::vector<std::unique_ptr<TItemTypes::TItem>>& retItems, std::vector<std::pair<cv::Point, cv::Point>>& retLocs){


    m_cellsize = lrint(image.rows / 17.14); // This cell size ratio SEEMS to hold for atleast 1080 and 1440 res.

    // Determine if this is an image containing containers or a stash (TODO: check for gamerun mode)
    std::vector<std::pair<cv::Point, cv::Point>> containerLocations;
    int numContainers = detectOpenContainers(image, containerLocations);

    // Detect containers, detect if a stash is present.
    // If containers are within stash, ignore stash.
    std::pair<cv::Point, cv::Point> stashLoc;
    bool stashDetected = detectStash(image, stashLoc);
    if (stashDetected) {
        // Check if any containers obfuscate the stash. If so, we toss out the stash detection.
        auto inRange = [&](int value, int lower, int upper) {
            return lower <= value && value <= upper;
        };
        for (auto& cont : containerLocations) {
            // Test if any 4 corners of the container rect are inside the stash.
            if ((inRange(stashLoc.first.x, cont.first.x, stashLoc.second.x) && inRange(stashLoc.first.y, cont.first.y, stashLoc.second.y)) ||
                (inRange(stashLoc.first.x, cont.second.x, stashLoc.second.x) && inRange(stashLoc.first.y, cont.first.y, stashLoc.second.y)) ||
                (inRange(stashLoc.first.x, cont.first.x, stashLoc.second.x) && inRange(stashLoc.first.y, cont.second.y, stashLoc.second.y)) ||
                (inRange(stashLoc.first.x, cont.second.x, stashLoc.second.x) && inRange(stashLoc.first.y, cont.second.y, stashLoc.second.y)) )
            {
                stashDetected = false;
                break;
            }
        }
    }


    bool success = false;
    if (numContainers > 0) {
        resolveContainerImage(image, containerLocations, retItems, retLocs);
        success =  true;
    }

    if (stashDetected)
        resolveStashImage(image, stashLoc, retItems, retLocs);

    return success;
} 



bool TImageReader::parseFromPath(const std::string path,
    std::vector<std::unique_ptr<TItemTypes::TItem>>& retItems, std::vector<std::pair<cv::Point, cv::Point>>& retLocs){

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
    cv::findContours(dilated, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);


    for (auto& cont : contours){

        double area = cv::contourArea(cont, true);
        if (std::abs(area) > 15000) {
            double peri = cv::arcLength(cont, true);

            std::vector<cv::Point> approx;
            cv::approxPolyDP(cont, approx, .2 * peri, true);

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
                                         const std::vector<std::pair<cv::Point, cv::Point>>& locs,
                                         std::vector<std::unique_ptr<TItemTypes::TItem>>& retItems,
                                         std::vector<std::pair<cv::Point, cv::Point>>& retLocs) {


    for (auto& loc : locs) {
        const cv::Point& containerUL = loc.first;
        const cv::Point& containerBR = loc.second;
        cv::Mat containerSlice = image(cv::Range(containerUL.y, containerBR.y), cv::Range(containerUL.x, containerBR.x + 1));
        cv::Mat imgHSV;
        cv::cvtColor(containerSlice, imgHSV, cv::COLOR_BGR2HSV);

        // Our "Magic" threshold values to detect item outlines.
        cv::Scalar lower(94, 7, 50);
        cv::Scalar upper(105, 36, 115);

        cv::Mat mask;
        cv::inRange(imgHSV, lower, upper, mask);

        std::vector<std::vector<cv::Point> > contours;
        cv::findContours(mask, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

        for (auto& cont : contours) {

            double area = cv::contourArea(cont, true);
            if (area > 900) {
                double peri = cv::arcLength(cont, true);

                std::vector<cv::Point> approx;
                cv::approxPolyDP(cont, approx, .2 * peri, true);

                if (approx.size() < 2)
                    continue;
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


                cv::Mat itemImage = containerSlice(cv::Range(lowerP.y, upperP.y), cv::Range(lowerP.x, upperP.x + 1));
                if (itemImage.empty() ||
                    isEmptySpace(imgHSV(cv::Range(lowerP.y, upperP.y), cv::Range(lowerP.x, upperP.x + 1))))
                    continue;

                // We create a placeholder TItem with our image and its dimensions.
                retItems.push_back(TItemTypes::TItem::makePlaceHolder(itemImage, m_cellsize));
                retLocs.push_back(std::make_pair(lowerP+ containerUL, upperP+ containerUL)); // Shift to parent image coordinates.
            }

        }
    }

    


}


void TImageReader::resolveStashImage(const cv::Mat& image,
    std::pair<cv::Point, cv::Point> loc,
    std::vector<std::unique_ptr<TItemTypes::TItem>>& retItems, std::vector<std::pair<cv::Point, cv::Point>>& retLocs) {};



bool TImageReader::detectStash(const cv::Mat& image, std::pair<cv::Point, cv::Point>& loc) {
    return false;
};

bool TImageReader::isEmptySpace(const cv::Mat& hsvImg) {

    /*
    * Empty spaces tend to have:
    * H = 0
    * S = 0
    * V = [7,10]
    */
    cv::Scalar avgHSV = cv::mean(hsvImg);
    std::cout << avgHSV << std::endl;
    if (avgHSV.channels < 3 || (avgHSV[0] < 10 && avgHSV[1] < 10 && avgHSV[2] < 30))
        return true;

    return false;
}