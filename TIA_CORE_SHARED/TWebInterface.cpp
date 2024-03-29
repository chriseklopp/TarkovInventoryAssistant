#include <TWebInterface.h>


namespace WebInterface {

    DetectionResultMarshal::DetectionResultMarshal() :
        name(),
        fleaUnit(),
        fleaPrice(0),
        fleaPricePerSlot(0),
        traderUnit(),
        traderPrice(0),
        trader(),
        sellOnFlea(false),
        dimHeight(0),
        dimWidth(0),
        x1(0),
        y1(0),
        x2(0),
        y2(0)
    {};

    DetectionResultMarshal::DetectionResultMarshal(const TCore& core, const TItemSupport::DetectionResult& det) :
        name(),
        fleaUnit(),
        fleaPrice(0),
        fleaPricePerSlot(0),
        traderUnit(),
        traderPrice(0),
        trader(),
        sellOnFlea(false),
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

        strncpy(name, catItem->getName().c_str(), sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';


        strncpy(fleaUnit, catItem->getPrice().getUnit().c_str(), sizeof(fleaUnit) - 1);
        fleaUnit[sizeof(fleaUnit) - 1] = '\0';

        fleaPrice = catItem->getPrice().getValue();

        fleaPricePerSlot = catItem->getPricePerSlot().getValue();

        strncpy(traderUnit, catItem->getTraderSellPrice().getUnit().c_str(), sizeof(traderUnit)-1);
        traderUnit[sizeof(traderUnit) - 1] = '\0';

        traderPrice = catItem->getTraderSellPrice().getValue();

        //strcpy_s(trader, sizeof(trader), catItem->getTrader().c_str());
        strncpy(trader, catItem->getTrader().c_str(), sizeof(trader)-1);
        trader[sizeof(trader) - 1] = '\0';

        sellOnFlea = catItem->isFleaOptimal();

        dimWidth = catItem->getDim().first;

        dimHeight = catItem->getDim().second;

        x1 = det.imageLoc.first.x;
        y1 = det.imageLoc.first.y;

        x2 = det.imageLoc.second.x;
        y2 = det.imageLoc.second.y;
    }


    int TWebInterface::detectImageContent(unsigned char* image, int width, int height, int channels, DetectionResultMarshal*& out) {

        cv::Mat cvImage;
        switch (channels) {
            case 3: {
                cvImage = cv::Mat(height, width, CV_8UC3, image);
                break;
            }
            case 4: {
                cv::Mat temp = cv::Mat(height, width, CV_8UC4, image);
                cv::cvtColor(temp, cvImage, cv::COLOR_BGRA2BGR); // Discard alpha channel or code will break.
                break;
            }
            default: {
                std::cout << "WARNING: Invalid Number of channels: " + channels << "\n";
                return -1;
            }
        }


        std::vector<TItemSupport::DetectionResult> results = m_core.detectImageContent(cvImage);
        int size = results.size();

        DetectionResultMarshal* detResultArray = new DetectionResultMarshal[size];
        for (int i = 0; i < size; i++){
            detResultArray[i] = DetectionResultMarshal(m_core, results[i]);
        }

        out = detResultArray; //Set 'out' ptr to our new array for handling by the client code.
        return size;
    }


    void TWebInterface::getDATA_DIR(char* out, int size) const {
        strncpy(out, m_core.getConfigPtr()->getDATA_DIR().string().c_str(), size-1);
        out[size - 1] = '\0';

    }

    void TWebInterface::getACTIVE_CATALOG(char* out, int size) const {
        strncpy(out, m_core.getConfigPtr()->getACTIVE_CATALOG().string().c_str(), size-1);
        out[size - 1] = '\0';
    }


    void TWebInterface::getRAW_CATALOGS_DIR(char* out, int size) const {
        strncpy(out, m_core.getConfigPtr()->getRAW_CATALOGS_DIR().string().c_str(), size - 1);
        out[size - 1] = '\0';
    }

    void TWebInterface::getCATALOGS_DIR(char* out, int size) const {
        strncpy(out, m_core.getConfigPtr()->getCATALOGS_DIR().string().c_str(), size - 1);
        out[size - 1] = '\0';
    }

    void TWebInterface::getROOT_DIR(char* out, int size) const{
        strncpy(out, m_core.getConfigPtr()->getROOT_DIR().string().c_str(), size - 1);
        out[size - 1] = '\0';
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



