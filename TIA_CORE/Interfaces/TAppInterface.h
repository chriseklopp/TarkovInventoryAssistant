#pragma once
/*
Interface used as a medium between the Core library and the TarkovInventoryAssistant App
*/
#include <Core/TCore.h>

class TAppInterface : public TEvent::TSubject {

public:

    TAppInterface();

    /*
* Getters that allow gui panels to access information from our core.
* Each returns a reference to the underlying information.
*/

//Returns a pointer to detection results for given ID. Returns nullptr if invalid id.
    const std::vector<TItemSupport::DetectionResult>* getDetectionResults(imageID imageID);

    // Returns pointer to image associated with the given id.
    // ** If no image is associated with the ID this will return null ptr **
    const cv::Mat* getImage(imageID id);

    // Returns reference to our loaded images.
    const std::vector<imageID> getLoadedImageIDs();

    // Return reference to our item catalog.
    const std::vector<TDataTypes::dcID>& getCatalogItemList();

    // Return pointer to the catalog item associated with ID.
    const TItemTypes::TItem* getCatalogItem(const TDataTypes::dcID catID);


    // Return vector of activated image IDs.
    const std::vector<imageID> getActivatedIDs();

    // Returns a pointer to the config. This is safe because the config is read only.
    TConfig::TConfig* getConfigPtr();


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

    void setDATA_DIR(std::string dir);

    // ** Notifies observers with TEventEnum::CatalogChanged **
    void setACTIVECATALOG(std::string dir);

    void setRAW_CATALOGS_DIR(std::string dir);

    void setCATALOGS_DIR(std::string dir);

    // Loads the config specified.
    // If empty, will attempt to load catalog specified by the config.
    bool loadCatalog(std::filesystem::path dir = "");

    // Create a compiled catalog from a raw catalog.
    void compileRawCatalog(std::filesystem::path& path, std::string name, bool makeRotations);

    // Save config settings to file.
    void saveConfig();



private:

    imageID createNewImageID();

    // Detect content and compare to catalog to make detection results.
    void detectImageContent(imageID id);

    imageID m_idCounter;

    // Signifies which images should be treated as "active".
    std::set<imageID> m_activeImages;

    std::map<imageID, std::unique_ptr<cv::Mat>> m_idImageMap;


    // Contains results of TImageReader's detections. 
    // !! Clearing this vector will cause the loss of detected items. !!
    std::map<imageID, std::vector<TItemSupport::DetectionResult>> m_detectionResults;



    TCore m_core;
};