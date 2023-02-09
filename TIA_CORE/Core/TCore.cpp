#include <Core/TCore.h>



TCore::TCore():
    m_idCounter(0),
    m_configManager(),
    m_config(m_configManager.getConfigPtr()),
    m_activeImages(),
    m_idImageMap(),
    m_detectionResults(),
    m_dataCatalog(m_config),
    m_imageReader(m_config)
{
    if (!m_config->isValid()) {
        std::cout << "ERROR: Config file invalid";
        throw std::exception("Config invalid");
    }
    //TConfig::TConfigManager m_configManager(&m_config);
    //m_dataCatalog = TDataCatalog(&m_config);
    //m_imageReader = TImageReader(&m_config);

};




imageID TCore::addImage(std::unique_ptr<cv::Mat> image) {
    imageID id = createNewImageID();
    m_idImageMap.insert({ id,std::move(image) });
    detectImageContent(id);
    
    notifyTObservers(TEvent::TEvent(TEvent::TEventEnum::ImageAdded, std::to_string(id)));

    return id;
};

void TCore::deleteImage(imageID id) {

    // Remove from m_idImageMap
    if (m_idImageMap.find(id) != m_idImageMap.end())
        m_idImageMap.erase(id);

    // Notify observers first, they may need the images detection results to clean up.
    notifyTObservers(TEvent::TEvent(TEvent::TEventEnum::ImageDeleted, std::to_string(id)));


    // Remove from m_detectionResults
    if (m_detectionResults.find(id) != m_detectionResults.end())
        m_detectionResults.erase(id);

    // Remove from activated Images (but silently to not trigger sending two events)
    m_activeImages.erase(id);

}


int TCore::loadImage(std::filesystem::path imPath){
    if (!imPath.is_absolute() || !std::filesystem::exists(imPath))
        return -1;

    std::unique_ptr<cv::Mat> image = std::make_unique<cv::Mat>(cv::imread(imPath.string()));
    if (image->empty())
        return -1;
    bool x = image->data == 0;
    bool y = image->dims == 0;
    bool z = image->total() == 0;


    return addImage(std::move(image));
}

bool TCore::loadImagesInDir(std::filesystem::path imDir){
    if (!imDir.is_absolute() || !std::filesystem::exists(imDir))
        return false;

    std::filesystem::directory_iterator itr = std::filesystem::directory_iterator(imDir);
    for (itr; itr != std::filesystem::end(itr); ++itr) {
        loadImage(*itr);
    }
    return true;
}


void TCore::activateImage(imageID id){
    if (m_activeImages.find(id) != m_activeImages.end())
        return;
    m_activeImages.insert(id);
    notifyTObservers(TEvent::TEvent(TEvent::TEventEnum::ImageActivated, std::to_string(id)));
}

// Deactivate image.
void TCore::deactivateImage(imageID id) {
    if (m_activeImages.find(id) == m_activeImages.end())
        return;

    notifyTObservers(TEvent::TEvent(TEvent::TEventEnum::ImageDeactivated, std::to_string(id)));
    m_activeImages.erase(id);

}

void TCore::deactivateAllImages() {
    m_activeImages.clear();
    notifyTObservers(TEvent::TEvent(TEvent::TEventEnum::AllImagesDeactivated, ""));
}


void TCore::clearImages() {
    m_idImageMap.clear();
    m_detectionResults.clear();
    
    notifyTObservers(TEvent::TEvent(TEvent::TEventEnum::ImagesCleared, ""));
};

const std::vector<TItemSupport::DetectionResult>* TCore::getDetectionResults(imageID id) {
    if (m_detectionResults.find(id) != m_detectionResults.end())
        return &m_detectionResults.at(id);
    return nullptr;
}

void TCore::deleteDetection(TItemSupport::DetectionResult& det) {

   // // Checks if the address of the arg passed is equal to one in the vector.
   //m_detectionResults.erase(std::find_if(m_detectionResults.begin(), m_detectionResults.end(),
   //    [&](TItemSupport::DetectionResult& r) {return &r == &det; }));
};

imageID TCore::createNewImageID() {
    imageID ret = m_idCounter;
    m_idCounter++;
    return ret;

}

const cv::Mat*  TCore::getImage(imageID id) {
    if (m_idImageMap.find(id) != m_idImageMap.end())
        return m_idImageMap.at(id).get();

    return nullptr;
}

void TCore::detectImageContent(imageID id) {

    const cv::Mat*  image = getImage(id);
    if (!image)
        return;

    std::vector<std::unique_ptr<TItemTypes::TItem>> res;
    std::vector<std::pair<cv::Point,cv::Point>> resLocs;
    m_imageReader.parseImage(*image, res, resLocs);

    std::vector<TItemSupport::DetectionResult> detResults;

    // I dont know if I fully like this.. but they are guaranteed to be the same size so it works.
    for (int i = 0; i < res.size(); i++)
    {
        TDataTypes::dcID catalogMatch = m_dataCatalog.getBestMatch(*(res[i].get()));
        if (catalogMatch == -1) // TODO: Do something smarter here...
            continue;

        detResults.push_back(TItemSupport::DetectionResult(catalogMatch, std::move(res[i]),
            id, resLocs[i], false));
    }
    m_detectionResults.insert({ id,std::move(detResults) });

}

const std::vector<imageID> TCore::getLoadedImageIDs() {
    std::vector<imageID> idVect;
    for (auto& itr : m_idImageMap) {
        idVect.push_back(itr.first);
    }
    return idVect;
}

const TItemTypes::TItem* TCore::getCatalogItem(const TDataTypes::dcID catID) {
    return m_dataCatalog.getItem(catID);
}




void TCore::setDATA_DIR(std::string dir) { m_configManager.setDATA_DIR(dir); };

void TCore::setACTIVECATALOG(std::string dir) {
    if(loadCatalog(dir))
        m_configManager.setACTIVECATALOG(dir);
};

void TCore::setRAW_CATALOGS_DIR(std::string dir){ m_configManager.setRAW_CATALOGS_DIR(dir); };

void TCore::setCATALOGS_DIR(std::string dir) { m_configManager.setCATALOGS_DIR(dir); };

bool TCore::loadCatalog(std::filesystem::path dir) {
    if (dir.empty())
        dir = m_config->getACTIVE_CATALOG();
    bool success = m_dataCatalog.loadCatalog(dir);
    if (success) {
        // We need to remake all detections using the new catalog.
        m_detectionResults.clear();
        for (auto& itr : m_idImageMap) {
            detectImageContent(itr.first);
        }
    }
    notifyTObservers(TEvent::TEvent(TEvent::TEventEnum::CatalogChanged, dir.string(), success));
    return success;
}


void TCore::compileRawCatalog(std::filesystem::path& path, std::string name, bool makeRotations) {
    bool res = m_dataCatalog.compileCatalogFromRaw(path, name, makeRotations);
    std::string message;
    if (res) {
        message = "Successfully created Compiled Catalog: " + m_config->getCATALOGS_DIR().string() + "/" + name;
    }
    else {
        message = "Failed to create Compiled Catalog: " + path.string() + " Raw Catalog may be invalid.";
    }

    notifyTObservers(TEvent::TEvent(TEvent::TEventEnum::ConsoleMessage, message));
}


// Return vector of activated image IDs.
const std::vector<imageID> TCore::getActivatedIDs() {
    std::vector<imageID> ret;
    for (int id : m_activeImages) {
        ret.push_back(id);
    }
    return ret;
}


void TCore::saveConfig() { m_configManager.saveConfig(); };