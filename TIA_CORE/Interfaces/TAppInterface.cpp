#include <Interfaces/TAppInterface.h>


TAppInterface::TAppInterface() :
    m_idCounter(1),
    m_core()
{
}


const std::vector<TItemSupport::DetectionResult>* TAppInterface::getDetectionResults(imageID id) {
    if (m_detectionResults.find(id) != m_detectionResults.end())
        return &m_detectionResults.at(id);
    return nullptr;
}

const cv::Mat* TAppInterface::getImage(imageID id) {
    if (m_idImageMap.find(id) != m_idImageMap.end())
        return m_idImageMap.at(id).get();

    return nullptr;
}

const std::vector<imageID> TAppInterface::getLoadedImageIDs() {
    std::vector<imageID> idVect;
    for (auto& itr : m_idImageMap) {
        idVect.push_back(itr.first);
    }
    return idVect;
}
const std::vector<TDataTypes::dcID>& TAppInterface::getCatalogItemList() {
    return m_core.getCatalogItemList();
}


const TItemTypes::TItem* TAppInterface::getCatalogItem(const TDataTypes::dcID catID) {
    return m_core.getCatalogItem(catID);
}



const std::vector<imageID> TAppInterface::getActivatedIDs() {
    std::vector<imageID> ret;
    for (int id : m_activeImages) {
        ret.push_back(id);
    }
    return ret;
}


TConfig::TConfig* TAppInterface::getConfigPtr() {
    return m_core.getConfigPtr();
}



int TAppInterface::loadImage(std::filesystem::path imPath) {
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



bool TAppInterface::loadImagesInDir(std::filesystem::path imDir) {
    if (!imDir.is_absolute() || !std::filesystem::exists(imDir))
        return false;

    std::filesystem::directory_iterator itr = std::filesystem::directory_iterator(imDir);
    for (itr; itr != std::filesystem::end(itr); ++itr) {
        loadImage(*itr);
    }
    return true;
}

int TAppInterface::addImage(std::unique_ptr<cv::Mat> image) {
    imageID id = createNewImageID();
    m_idImageMap.insert({ id,std::move(image) });
    detectImageContent(id);

    notifyTObservers(TEvent::TEvent(TEvent::TEventEnum::ImageAdded, std::to_string(id)));

    return id;
}


void TAppInterface::deleteImage(imageID id) {

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

void TAppInterface::activateImage(imageID id) {
    if (m_activeImages.find(id) != m_activeImages.end())
        return;
    m_activeImages.insert(id);
    notifyTObservers(TEvent::TEvent(TEvent::TEventEnum::ImageActivated, std::to_string(id)));
}

void TAppInterface::deactivateImage(imageID id) {
    if (m_activeImages.find(id) == m_activeImages.end())
        return;

    notifyTObservers(TEvent::TEvent(TEvent::TEventEnum::ImageDeactivated, std::to_string(id)));
    m_activeImages.erase(id);

}

void TAppInterface::deactivateAllImages() {
    m_activeImages.clear();
    notifyTObservers(TEvent::TEvent(TEvent::TEventEnum::AllImagesDeactivated, ""));
}

void TAppInterface::clearImages() {
    m_idImageMap.clear();
    m_detectionResults.clear();

    notifyTObservers(TEvent::TEvent(TEvent::TEventEnum::ImagesCleared, ""));
};

void TAppInterface::setDATA_DIR(std::string dir) {
    m_core.setDATA_DIR(std::move(dir));
}

void TAppInterface::setACTIVECATALOG(std::string dir) {
    if(loadCatalog(dir))
        m_core.setACTIVECATALOG(std::move(dir));
}

void TAppInterface::setRAW_CATALOGS_DIR(std::string dir) {}

void TAppInterface::setCATALOGS_DIR(std::string dir) {}

bool TAppInterface::loadCatalog(std::filesystem::path dir) {
    if (dir.empty())
        dir = m_core.getConfigPtr()->getACTIVE_CATALOG();
    bool success = m_core.loadCatalog(dir);
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

void TAppInterface::compileRawCatalog(std::filesystem::path& path, std::string name, bool makeRotations) {
    bool res = m_core.compileRawCatalog(path, name, makeRotations);
    std::string message;
    if (res) {
        message = "Successfully created Compiled Catalog: " + m_core.getConfigPtr()->getCATALOGS_DIR().string() + "/" + name;
    }
    else {
        message = "Failed to create Compiled Catalog: " + path.string() + " Raw Catalog may be invalid.";
    }

    notifyTObservers(TEvent::TEvent(TEvent::TEventEnum::ConsoleMessage, message));
}


void TAppInterface::saveConfig() {
    m_core.saveConfig();
}

// Detect content and compare to catalog to make detection results.
void TAppInterface::detectImageContent(imageID id) {
    const cv::Mat* image = getImage(id);
    if (!image)
        return;

    m_detectionResults.insert({ id, std::move(m_core.detectImageContent(*image, id)) });
}


imageID TAppInterface::createNewImageID() {
    imageID ret = m_idCounter;
    m_idCounter++;
    return ret;

}