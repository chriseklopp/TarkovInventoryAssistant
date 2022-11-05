#include "TConfig.h"



namespace TConfig {

    const std::string TConfig::getDATA_DIR() {
        return DATA_PATH;
    };

    const std::string TConfig::getACTIVE_CATALOG() {
        return ACTIVE_CATALOG_PATH;
    };

    const std::string TConfig::getRAW_CATALOGS_DIR() {
        return RAW_CATALOGS_PATH;
    };

    const std::string TConfig::getCATALOGS_DIR() {
        return CATALOGS_PATH_PATH;
    };

    void TConfigEditor::setDATA_DIR(std::string dir) {
        m_config->DATA_PATH = dir;
    }

    void TConfigEditor::setACTIVECATALOG(std::string dir) {
        m_config->ACTIVE_CATALOG_PATH = dir;
    };

    void TConfigEditor::setRAW_CATALOGS_DIR(std::string dir) {
        m_config->RAW_CATALOGS_PATH = dir;
    };

    void TConfigEditor::setCATALOGS_DIR(std::string dir) {
        m_config->CATALOGS_PATH_PATH = dir;
    };

}