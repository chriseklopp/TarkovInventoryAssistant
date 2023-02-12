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


    const char* TWebInterface::getDATA_DIR() const {
        return m_core.getConfigPtr()->getDATA_DIR().string().c_str(); // pretty sure this will go out of scope and fucking die.
    }

    const char* TWebInterface::getACTIVE_CATALOG() const {
        return m_core.getConfigPtr()->getACTIVE_CATALOG().string().c_str(); // pretty sure this will go out of scope and fucking die.
    }


    const char* TWebInterface::getRAW_CATALOGS_DIR() const {
        return new char[12] {"Hello World"};
    }

    const char* TWebInterface::getCATALOGS_DIR() const {
        return new char[12] {"Hello World"};
    }

    const char* TWebInterface::getROOT_DIR() const{
        return new char[12] {"Hello World"};
    }

    void TWebInterface::setDATA_DIR(const char* dir){
        m_core.setDATA_DIR(std::string(dir));
    }

    void TWebInterface::setACTIVECATALOG(const char* dir){
        m_core.setACTIVECATALOG(std::string(dir));
    }

    void TWebInterface::setRAW_CATALOGS_DIR(const char* dir){
        m_core.setRAW_CATALOGS_DIR(std::string(dir));
    }

    void TWebInterface::setCATALOGS_DIR(const char* dir){
        m_core.setCATALOGS_DIR(std::string(dir));
    }

    // Save config settings to file.
    void TWebInterface::saveConfig(){
    }


    // Loads the config specified.
    // If empty, will attempt to load catalog specified by the config.
    bool TWebInterface::loadCatalog(const char* dir){
        return false;
    }

    // Create a compiled catalog from a raw catalog.
    bool TWebInterface::compileRawCatalog(const char* path, const char* name, bool makeRotations){
        return false;
    }






}


extern "C" _declspec(dllexport) const char* getDATA_DIR(WebInterface::TWebInterface* iface) {
    if (!iface)
        return 0;
    return iface->getDATA_DIR();
}

extern "C" _declspec(dllexport) const char* getACTIVE_CATALOG(WebInterface::TWebInterface* iface) {
    if (!iface)
        return 0;
    return iface->getACTIVE_CATALOG();
}

extern "C" _declspec(dllexport) const char* getRAW_CATALOGS_DIR(WebInterface::TWebInterface* iface) {
    if (!iface)
        return 0;
    return iface->getRAW_CATALOGS_DIR();
}

extern "C" _declspec(dllexport) const char* getCATALOGS_DIR(WebInterface::TWebInterface* iface) {
    if (!iface)
        return 0;
    return iface->getCATALOGS_DIR();
}

extern "C" _declspec(dllexport) const char* getROOT_DIR(WebInterface::TWebInterface* iface) {
    if (!iface)
        return 0;
    return iface->getROOT_DIR();
}

extern "C" _declspec(dllexport) void setDATA_DIR(WebInterface::TWebInterface* iface, char* dir) {
    if (!iface)
        return;
    return iface->setDATA_DIR(dir);
}

extern "C" _declspec(dllexport) void setACTIVECATALOG(WebInterface::TWebInterface* iface, char* dir) {
    if (!iface)
        return;
    return iface->setACTIVECATALOG(dir);
}



extern "C" _declspec(dllexport) void setRAW_CATALOGS_DIR(WebInterface::TWebInterface* iface, char* dir) {
    if (!iface)
        return;
    return iface->setRAW_CATALOGS_DIR(dir);
}

extern "C" _declspec(dllexport) void setCATALOGS_DIR(WebInterface::TWebInterface * iface, char* dir) {
    if (!iface)
        return;
    return iface->setCATALOGS_DIR(dir);
}


extern "C" _declspec(dllexport) WebInterface::TWebInterface* CreateCoreInterface()
{
    return new WebInterface::TWebInterface();
}

extern "C" _declspec(dllexport) WebInterface::DetectionResultMarshal detectImageContent(WebInterface::TWebInterface* iface, int image) {
    if (iface)
        return iface->detectImageContent(image);
}

