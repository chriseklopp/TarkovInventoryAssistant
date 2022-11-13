#include "TCore.h"



TCore::TCore():
    m_configManager(),
    m_idCounter(0),
    m_config(m_configManager.getConfigPtr()),
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

    // Remove from m_detectionResults
    if (m_detectionResults.find(id) != m_detectionResults.end())
        m_detectionResults.erase(id);

    // Remove from activated Images (but silently to not trigger sending two events)
    m_activeImages.erase(id);
    notifyTObservers(TEvent::TEvent(TEvent::TEventEnum::ImageDeleted, std::to_string(id)));
}


int TCore::loadImage(std::filesystem::path imPath){
    if (!imPath.is_absolute() || !std::filesystem::exists(imPath))
        return -1;

    std::unique_ptr<cv::Mat> image = std::make_unique<cv::Mat>(cv::imread(imPath.string()));
    if (image->empty())
        std::cout << "wtf??";
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
    m_activeImages.erase(id);
    notifyTObservers(TEvent::TEvent(TEvent::TEventEnum::ImageDeactivated, std::to_string(id)));
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

cv::Mat* const TCore::getImage(imageID id) {
    if (m_idImageMap.find(id) != m_idImageMap.end())
        return m_idImageMap.at(id).get();

    return nullptr;
}

void TCore::detectImageContent(imageID id) {

    cv::Mat* const image = getImage(id);
    if (!image)
        return;

    std::vector<std::unique_ptr<TItemTypes::TItem>> res;
    std::vector<std::pair<cv::Point,cv::Point>> resLocs;
    m_imageReader.parseImage(*image, res, resLocs);

    std::vector<TItemSupport::DetectionResult> detResults;

    // I dont know if I fully like this.. but they are guaranteed to be the same size so it works.
    for (int i = 0; i < res.size(); i++)
    {
        TItemTypes::TItem* catalogMatch = m_dataCatalog.getBestMatch(*(res[i].get()));
        if (!catalogMatch) // TODO: Do something smarter here...
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


void TCore::setDATA_DIR(std::string dir) { m_configManager.setDATA_DIR(dir); };

void TCore::setACTIVECATALOG(std::string dir) {
    m_configManager.setACTIVECATALOG(dir);
    loadCatalog(dir);
};

void TCore::setRAW_CATALOGS_DIR(std::string dir){ m_configManager.setRAW_CATALOGS_DIR(dir); };

void TCore::setCATALOGS_DIR(std::string dir) { m_configManager.setCATALOGS_DIR(dir); };

void TCore::loadCatalog(std::string dir) {
    if (dir.empty())
        dir = m_config->getACTIVE_CATALOG().string();
    bool success = m_dataCatalog.loadCatalog(m_config->getACTIVE_CATALOG());
    if (success) {
        // We need to remake all detections using the new catalog.
        m_detectionResults.clear();
        for (auto& itr : m_idImageMap) {
            detectImageContent(itr.first);
        }
    }
    notifyTObservers(TEvent::TEvent(TEvent::TEventEnum::CatalogChanged, dir, success));
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