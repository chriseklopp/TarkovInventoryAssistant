#include <Core/TCore.h>



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
    loadCatalog(m_config->getACTIVE_CATALOG());
};

std::vector<TItemSupport::DetectionResult> TCore::detectImageContent(cv::Mat image, int imID) {

    std::vector<std::unique_ptr<TItemTypes::TItem>> res;
    std::vector<std::pair<cv::Point, cv::Point>> resLocs;
    m_imageReader.parseImage(image, res, resLocs);

    std::vector<TItemSupport::DetectionResult> detResults;
    for (int i = 0; i < res.size(); i++)
    {
        TDataTypes::dcID catalogMatch = m_dataCatalog.getBestMatch(*(res[i].get()));
        if (catalogMatch == -1) // TODO: Do something smarter here...
            continue;

        detResults.push_back(TItemSupport::DetectionResult(catalogMatch, std::move(res[i]),
            imID, resLocs[i], false));
    }
    return std::move(detResults);
}



const TItemTypes::TItem* TCore::getCatalogItem(const TDataTypes::dcID catID) const {
    return m_dataCatalog.getItem(catID);
}


void TCore::setDATA_DIR(std::string dir) { m_configManager.setDATA_DIR(dir); };

void TCore::setACTIVECATALOG(std::string dir) {
        m_configManager.setACTIVECATALOG(dir);
};

void TCore::setRAW_CATALOGS_DIR(std::string dir){ m_configManager.setRAW_CATALOGS_DIR(dir); };

void TCore::setCATALOGS_DIR(std::string dir) { m_configManager.setCATALOGS_DIR(dir); };

bool TCore::loadCatalog(std::filesystem::path dir) {
    return m_dataCatalog.loadCatalog(dir);
}


bool TCore::compileRawCatalog(const std::filesystem::path& path, std::string name, bool makeRotations) {
    return m_dataCatalog.compileCatalogFromRaw(path, name, makeRotations);
}


void TCore::saveConfig() { m_configManager.saveConfig(); };