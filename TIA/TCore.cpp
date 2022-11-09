#include "TCore.h"



TCore::TCore():
    m_configManager(),
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

void TCore::addAndParseImage(std::unique_ptr<cv::Mat> image) {

    detectImageContent(image.get());
    m_loadedImages.push_back(std::move(image));

};
void TCore::clearImages() {
    m_loadedImages.clear();
};

void TCore::clearDetections() {
    m_detectionResults.clear();
};


void TCore::deleteDetection(TItemSupport::DetectionResult& det) {

    // Checks if the address of the arg passed is equal to one in the vector.
   m_detectionResults.erase(std::find_if(m_detectionResults.begin(), m_detectionResults.end(),
       [&](TItemSupport::DetectionResult& r) {return &r == &det; }));
};



void TCore::detectImageContent(cv::Mat* image) {
    if (!image)
        return;
    std::vector<std::unique_ptr<TItemTypes::TItem>> res;
    std::vector<cv::Point> resLocs;
    m_imageReader.parseImage(*image, res, resLocs);

    // I dont know if I fully like this.. but they are guaranteed to be the same size so it works.
    for (int i = 0; i < res.size(); i++)
    {
        TItemTypes::TItem* catalogMatch = m_dataCatalog.getBestMatch(*(res[i].get()));
        if (!catalogMatch) // TODO: Do something smarter here...
            return;

        m_detectionResults.push_back(TItemSupport::DetectionResult(catalogMatch, std::move(res[i]),
            image, resLocs[i], false));
    }
}


void TCore::setDATA_DIR(std::string dir) { m_configManager.setDATA_DIR(dir); };

void TCore::setACTIVECATALOG(std::string dir) {
    m_configManager.setACTIVECATALOG(dir);
    m_dataCatalog.loadCatalog(m_config->getACTIVE_CATALOG());
};

void TCore::setRAW_CATALOGS_DIR(std::string dir){ m_configManager.setRAW_CATALOGS_DIR(dir); };

void TCore::setCATALOGS_DIR(std::string dir) { m_configManager.setCATALOGS_DIR(dir); };


void TCore::saveConfig() { m_configManager.saveConfig(); };