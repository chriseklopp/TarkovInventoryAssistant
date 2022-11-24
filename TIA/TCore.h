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
    const std::vector<TItemSupport::DetectionResult>* getDetectionResults(imageID imageID);

    // Returns pointer to image associated with the given id.
    // ** If no image is associated with the ID this will return null ptr **
    cv::Mat* const getImage(imageID id);

    // Returns reference to our loaded images.
    const std::vector<imageID> getLoadedImageIDs();

    // Return reference to our item catalog.
    const std::vector<std::unique_ptr<TItemTypes::TItem>>& getCatalogItemList() { return m_dataCatalog.getItemList(); };

    // Return vector of activated image IDs.
    const std::vector<imageID> getActivatedIDs();

    // Returns a pointer to the config. This is safe because the config is read only.
    TConfig::TConfig* getConfigPtr() { return m_config; };


    // Delete a detection.
    void deleteDetection(TItemSupport::DetectionResult& det);

    // Attempt to load the image at the given *ASBOLUTE* path.
    // Returns the image's ID if successful.
    int loadImage(std::filesystem::path imPath);

    // Attempt to load all images in the given *ASBOLUTE* path.
    // Returns the image's ID if successful.
    bool loadImagesInDir(std::filesystem::path imDir);

    // Adds this image to our collection and parse any items on it into detection results.
    // returns the unique imageID associated with the image.
    // ** Notifies observers with TEventEnum::ImageAdded **
    int addImage(std::unique_ptr<cv::Mat> image);

    // Deletes image with the given id.
    // ** Notifies observers with TEventEnum::ImageDeleted **
    void deleteImage(imageID id);

    // Set image to active
    void activateImage(imageID id);

    // Deactivate image.
    void deactivateImage(imageID id);

    // Deactivates ALL active images.
    void deactivateAllImages();


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

    // Loads the config specified.
    // If empty, will attempt to load catalog specified by the config.
    void loadCatalog(std::string dir="");

    // Create a compiled catalog from a raw catalog.
    void compileRawCatalog(std::filesystem::path& path);

    // Save config settings to file.
    void saveConfig();

private:


    // Detect content and compare to catalog to make detection results.
    void detectImageContent(imageID id);

    imageID createNewImageID();

    imageID m_idCounter;

    // Config handlers.
    TConfig::TConfigManager m_configManager;
    TConfig::TConfig* m_config;

    // Signifies which images should be treated as "active".
    std::set<imageID> m_activeImages;


    std::map<imageID, std::unique_ptr<cv::Mat>> m_idImageMap;

                                         
    // Contains results of TImageReader's detections. 
    // !! Clearing this vector will cause the loss of detected items. !!
    //std::vector<TItemSupport::DetectionResult> m_detectionResults; // Almost everything in this is a pointer.
    std::map<imageID, std::vector<TItemSupport::DetectionResult>> m_detectionResults;

    TDataCatalog m_dataCatalog;
    TImageReader m_imageReader;


};

