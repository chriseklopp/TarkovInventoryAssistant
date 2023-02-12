#pragma once

#include <opencv2/opencv.hpp>

#include <Core/TImageReader.h>
#include <Core/TDataCatalog.h>
#include <Core/TConfig.h>
#include <DataSupport/TEvents.h>
//#include "TScreenCapture.h" // TODO: figure out how to stop this breaking wxWidgets..

/* Acts as an interface between backend and gui.
* All backend objects will be owned here, and interfacing to and from them will be defined here.
*/




class TCore {
public:
    TCore();

public:

    // ID is for compatibility with the way these were handled before.  Intended to map to a parent image ID.
    // Leave empty if you dont care.
    std::vector<TItemSupport::DetectionResult> detectImageContent(cv::Mat image, int imID = -1);

    // Returns a pointer to the config. This is safe because the config is read only.
    TConfig::TConfig* getConfigPtr() { return m_config; };

    void setDATA_DIR(std::string dir);

    // ** Notifies observers with TEventEnum::CatalogChanged **
    void setACTIVECATALOG(std::string dir);

    void setRAW_CATALOGS_DIR(std::string dir);

    void setCATALOGS_DIR(std::string dir);
    // Save config settings to file.
    void saveConfig();


    // Loads the config specified.
    // If empty, will attempt to load catalog specified by the config.
    bool loadCatalog(std::filesystem::path dir = "");

    // Create a compiled catalog from a raw catalog.
    bool compileRawCatalog(std::filesystem::path& path, std::string name, bool makeRotations);

    // Return pointer to the catalog item associated with ID.
    const TItemTypes::TItem* getCatalogItem(const TDataTypes::dcID catID) const;

    // Return reference to our item catalog.
    const std::vector<TDataTypes::dcID>& getCatalogItemList() const { return m_dataCatalog.getItemIDList(); };


private:

    // Config handlers.
    TConfig::TConfigManager m_configManager;
    TConfig::TConfig* m_config;

    TDataCatalog::TDataCatalog m_dataCatalog;
    TImageReader m_imageReader;


};

