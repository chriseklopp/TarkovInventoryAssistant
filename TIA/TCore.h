#pragma once

#include <opencv2/opencv.hpp>
#include "TImageReader.h"
#include "TDataCatalog.h"
#include "TConfig.h"
#include "TEvents.h"
//#include "TScreenCapture.h" // TODO: figure out how to stop this breaking wxWidgets..

/* Acts as an interface between backend and gui.
* All backend objects will be owned here, and interfacing to and from them will be defined here.
*/


typedef int imageID;


class TCore : public TEvent::TSubject {
public:
    TCore();

public:

    /*
    * Getters that allow gui panels to access information from our core.
    * Each returns a reference to the underlying information.
    */

    //Returns a reference to detection results for given ID.
    const std::vector<TItemSupport::DetectionResult>& getDetectionResults(imageID imageID);

    // Returns a vector of all detection results associated with a given image.
    //const std::vector<TItemSupport::DetectionResult*> getDetectionResults(std::unique_ptr<cv::Mat>& im);
    //const std::vector<TItemSupport::DetectionResult*> getDetectionResults(const cv::Mat* im);

    // Returns pointer to image associated with the given id.
    // ** If no image is associated with the ID this will return null ptr **
    cv::Mat* const getImage(imageID id);

    // Returns reference to our loaded images.
    const std::vector<imageID> getLoadedImageIDs();

    // Return reference to our item catalog.
    const std::vector<std::unique_ptr<TItemTypes::TItem>>& getCatalogItemList() { return m_dataCatalog.getItemList(); };

    // Returns a pointer to the config. This is safe because the config is read only.
    TConfig::TConfig* getConfigPtr() { return m_config; };


    // Delete a detection.
    void deleteDetection(TItemSupport::DetectionResult& det);

    // Adds this image to our collection and parse any items on it into detection results.
    // returns the unique imageID associated with the image.
    // ** Notifies observers with TEventEnum::ImageAdded **
    int addImage(std::unique_ptr<cv::Mat> image);

    // Deletes image with the given id.
    // ** Notifies observers with TEventEnum::ImageDeleted **
    void deleteImage(imageID id);

    // Clear all loaded images.
    // ** Notifies observers with TEventEnum::ImageDeleted **
    void clearImages();

    // Save detections to csv file.
    void saveDetectionsToCSV(std::string fname);

    void setDATA_DIR(std::string dir);

    // ** Notifies observers with TEventEnum::CatalogChanged **
    void setACTIVECATALOG(std::string dir);

    void setRAW_CATALOGS_DIR(std::string dir);

    void setCATALOGS_DIR(std::string dir);

    // Loads the config specified by the config.
    void loadCatalog();


    // Save config settings to file.
    void saveConfig();


    // Implement TSubject overrides

    //virtual void registerTObserver(TEvent::TObserver* obs) override {};

    //virtual void removeTObserver(TEvent::TObserver* obs) override {};

    //virtual void notifyObservers(TEvent::TEventEnum e) override {};

private:


    // Detect content and compare to catalog to make detection results.
    void detectImageContent(imageID id);

    imageID createNewImageID();

    imageID m_idCounter;

    // Config handlers.
    TConfig::TConfigManager m_configManager;
    TConfig::TConfig* m_config;

    // Contains currently loaded images.
    //std::vector<std::unique_ptr<cv::Mat>> m_loadedImages;


    std::map<imageID, std::unique_ptr<cv::Mat>> m_idImageMap;

                                         
    // Contains results of TImageReader's detections. 
    // !! Clearing this vector will cause the loss of detected items. !!
    //std::vector<TItemSupport::DetectionResult> m_detectionResults; // Almost everything in this is a pointer.
    std::map<imageID, std::vector<TItemSupport::DetectionResult>> m_detectionResults;

    TDataCatalog m_dataCatalog;
    TImageReader m_imageReader;


};

