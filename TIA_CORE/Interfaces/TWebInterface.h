#pragma once
/*
Interface used as a medium between the Core library and the TarkovInventoryAssistant server.
This will provide interfaces in a way that are friendly for interop with the C# code running the server.
*/
#include <Core/TCore.h>

namespace WebInterface {

    // Current Size = 102
    struct DetectionResultMarshal {

        DetectionResultMarshal(const TCore& core, const TItemSupport::DetectionResult& det);

        char name[60];
        char fleaUnit[4];
        int fleaPrice;
        int fleaPricePerSlot;
        char traderUnit[4];
        int traderPrice;
        char trader[12];
        uint8_t dimHeight;
        uint8_t dimWidth;
        uint16_t x1;
        uint16_t y1;
        uint16_t x2;
        uint16_t y2;
    };




    class TWebInterface {

    public:
        TWebInterface() {
            // TEST
            m_core.loadCatalog("C:\\MyWorkspace\\TarkovInventoryAssistant\\Data\\CompiledCatalogs\\ItemCatalog_catalogJan16_2023_wRotations");
        }

        int getUsefulNumber() { return 69; }

        DetectionResultMarshal detectImageContent(int image);

    private:


        TCore m_core;
    };

};

