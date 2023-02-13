#include <Interfaces/TWebInterface.h>



namespace WebInterface {

    DetectionResultMarshal::DetectionResultMarshal(const TCore& core, const TItemSupport::DetectionResult& det) :
        name(),
        fleaUnit(),
        fleaPrice(0),
        fleaPricePerSlot(0),
        traderUnit(),
        traderPrice(0),
        trader(),
        dimHeight(0),
        dimWidth(0),
        x1(0),
        y1(0),
        x2(0),
        y2(0)

    {
        const TItemTypes::TItem* catItem = core.getCatalogItem(det.catalogItem);
        if (!catItem) // now this would be bad!!
            return;

        strcpy_s(name, sizeof(name) / sizeof(char), catItem->getName().c_str());

        strcpy_s(fleaUnit, sizeof(fleaUnit) / sizeof(char), catItem->getPrice().getUnit().c_str());

        fleaPrice = catItem->getPrice().getValue();

        fleaPricePerSlot = catItem->getPricePerSlot().getValue();

        strcpy_s(traderUnit, sizeof(traderUnit) / sizeof(char), catItem->getTraderSellPrice().getUnit().c_str());

        traderPrice = catItem->getTraderSellPrice().getValue();

        strcpy_s(trader, sizeof(trader) / sizeof(char), catItem->getTrader().c_str());

        dimHeight = catItem->getDim().first;

        dimWidth = catItem->getDim().second;

        x1 = det.imageLoc.first.x;
        y1 = det.imageLoc.first.y;

        x2 = det.imageLoc.second.x;
        y2 = det.imageLoc.second.y;
    }


    DetectionResultMarshal TWebInterface::detectImageContent(int image) {


        TItemTypes::TItem testy = TItemTypes::TItem();

        TItemTypes::TItem* itm = new TItemTypes::TItem();
        std::unique_ptr<TItemTypes::TItem> itmptr(itm);
        TItemSupport::DetectionResult testResult = TItemSupport::DetectionResult(69,
            std::move(itmptr),
            420,
            std::make_pair(cv::Point(0, 0),
                cv::Point(0, 0)));

        return DetectionResultMarshal(m_core, testResult);
    }


    void TWebInterface::getDATA_DIR(char* out, int size) const {
        strcpy_s(out, size, m_core.getConfigPtr()->getDATA_DIR().string().c_str());
    }

    void TWebInterface::getACTIVE_CATALOG(char* out, int size) const {
        strcpy_s(out, size, m_core.getConfigPtr()->getACTIVE_CATALOG().string().c_str());
    }


    void TWebInterface::getRAW_CATALOGS_DIR(char* out, int size) const {
        strcpy_s(out, size, m_core.getConfigPtr()->getRAW_CATALOGS_DIR().string().c_str());
    }

    void TWebInterface::getCATALOGS_DIR(char* out, int size) const {
        strcpy_s(out, size, m_core.getConfigPtr()->getCATALOGS_DIR().string().c_str());
    }

    void TWebInterface::getROOT_DIR(char* out, int size) const{
        strcpy_s(out, size, m_core.getConfigPtr()->getROOT_DIR().string().c_str());
    }

    void TWebInterface::setDATA_DIR(const char* dir){
        m_core.setDATA_DIR(std::string(dir));
    }

    bool TWebInterface::setACTIVECATALOG(const char* dir){
        if (m_core.loadCatalog(dir)) {
            m_core.setACTIVECATALOG(dir);
            return true;
        }
        return false;
    }

    void TWebInterface::setRAW_CATALOGS_DIR(const char* dir){
        m_core.setRAW_CATALOGS_DIR(std::string(dir));
    }

    void TWebInterface::setCATALOGS_DIR(const char* dir){
        m_core.setCATALOGS_DIR(std::string(dir));
    }


    void TWebInterface::saveConfig(){
        m_core.saveConfig();
    }

    bool TWebInterface::loadCatalog(const char* dir){
        return m_core.loadCatalog(dir);
    }

    bool TWebInterface::compileRawCatalog(const char* path, const char* name, bool makeRotations){
        return m_core.compileRawCatalog(path, name, makeRotations);
    }

}


extern "C" _declspec(dllexport) void getDATA_DIR_INTEROP(WebInterface::TWebInterface* iface, char* str, int size) {
    if (iface)
        iface->getDATA_DIR(str, size);
    return;
}

extern "C" _declspec(dllexport) void getACTIVE_CATALOG_INTEROP(WebInterface::TWebInterface* iface, char* str, int size) {
    if (iface)
        iface->getACTIVE_CATALOG(str, size);
    return;

}

extern "C" _declspec(dllexport) void getRAW_CATALOGS_DIR_INTEROP(WebInterface::TWebInterface* iface, char* str, int size) {
    if (iface)
        iface->getRAW_CATALOGS_DIR(str, size);
    return;
}

extern "C" _declspec(dllexport) void getCATALOGS_DIR_INTEROP(WebInterface::TWebInterface* iface, char* str, int size) {
    if (iface)
        iface->getCATALOGS_DIR(str, size);
    return;

}

extern "C" _declspec(dllexport) void getROOT_DIR_INTEROP(WebInterface::TWebInterface* iface, char* str, int size) {
    if (iface)
        iface->getROOT_DIR(str, size);
    return;
}

extern "C" _declspec(dllexport) void setDATA_DIR_INTEROP(WebInterface::TWebInterface* iface, char* dir) {
    if (iface)
        iface->setDATA_DIR(dir);
    return;
}

extern "C" _declspec(dllexport) bool setACTIVECATALOG_INTEROP(WebInterface::TWebInterface* iface, char* dir) {
    bool ret = false;
    if (iface)
        ret = iface->setACTIVECATALOG(dir);
    return ret;
}

extern "C" _declspec(dllexport) void setRAW_CATALOGS_DIR_INTEROP(WebInterface::TWebInterface* iface, char* dir) {
    if (iface)
        iface->setRAW_CATALOGS_DIR(dir);
    return;
}

extern "C" _declspec(dllexport) void setCATALOGS_DIR_INTEROP(WebInterface::TWebInterface* iface, char* dir) {
    if (iface)
        iface->setCATALOGS_DIR(dir);
    return;
}

extern "C" _declspec(dllexport) void saveConfig_INTEROP(WebInterface::TWebInterface* iface) {
    if (iface)
        iface->saveConfig();
    return;
}

extern "C" _declspec(dllexport) WebInterface::TWebInterface* CreateCoreInterface_INTEROP()
{
    return new WebInterface::TWebInterface();
}

extern "C" _declspec(dllexport) WebInterface::DetectionResultMarshal detectImageContent_INTEROP(WebInterface::TWebInterface* iface, int image) {
    if (iface)
        return iface->detectImageContent(image);
    //return WebInterface::DetectionResultMarshal::DetectionResultMarshal();
    // TODO: add return statement to fix undefined behavior
}

