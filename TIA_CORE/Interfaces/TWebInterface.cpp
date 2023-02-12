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



    extern "C" _declspec(dllexport) int getUsefulNumber(WebInterface::TWebInterface* iface) {
        if (!iface)
            return 0;
        return iface->getUsefulNumber();
    }


    extern "C" _declspec(dllexport) WebInterface::TWebInterface * CreateCore()
    {
        return new WebInterface::TWebInterface();
    }

    extern "C" _declspec(dllexport) WebInterface::DetectionResultMarshal detectImageContent(WebInterface::TWebInterface* iface, int image) {
        if (iface)
            return iface->detectImageContent(image);
    }






}