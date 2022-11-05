#pragma once

#include <opencv2/opencv.hpp>
#include "TImageReader.h"
#include "TDataCatalog.h"

//#include "TScreenCapture.h" // TODO: figure out how to stop this breaking wxWidgets..

/* Acts as an interface between backend and gui.
* All backend objects will be owned here, and interfacing to and from them will be defined here.
*/
class TCore {
public:
    TCore();

public:

    /*
    * Getters that allow gui panels to access information from our core.
    * Each returns a reference to the underlying information.
    */

    //Returns a reference to our detection results.
    const std::vector<TItemSupport::DetectionResult>& getDetectionResults() { return m_detectionResults; };

    // Returns reference to our loaded images.
    const std::vector<std::unique_ptr<cv::Mat>>& getLoadedImages() { return m_loadedImages; };

    // Return reference to our item catalog.
    const std::vector<std::unique_ptr<TItemTypes::TItem>>& getCatalogItemList() { return m_dataCatalog.getItemList(); };



    // Delete all detections.
    void clearDetections();

    // Delete a detection.
    void deleteDetection(TItemSupport::DetectionResult& det);

    // Adds this image to our collection and parse any items on it into detection results.
    void addAndParseImage(std::unique_ptr<cv::Mat> image);

    // Clear all loaded images.
    void clearImages();

    // Save detections to csv file.
    void saveDetectionsToCSV(std::string fname);


private:

    // Detect content and compare to catalog to make detection results.
    void detectImageContent(cv::Mat* image);


    // Contains currently loaded images.
    std::vector<std::unique_ptr<cv::Mat>> m_loadedImages;

                                         
    // Contains results of TImageReader's detections. 
    // !! Clearing this vector will cause the loss of detected items. !!
    std::vector<TItemSupport::DetectionResult> m_detectionResults; // Almost everything in this is a pointer.



    TDataCatalog m_dataCatalog;
    TImageReader m_imageReader;
    



};

