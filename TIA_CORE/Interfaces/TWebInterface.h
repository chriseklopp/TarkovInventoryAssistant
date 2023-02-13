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
            m_core.loadCatalog();
        }

        int getUsefulNumber() { return 69; }


        DetectionResultMarshal detectImageContent(int image);

        void getDATA_DIR(char* out, int size) const;

        void getACTIVE_CATALOG(char* out, int size) const;

        void getRAW_CATALOGS_DIR(char* out, int size) const;

        void getCATALOGS_DIR(char* out, int size) const;

        void getROOT_DIR(char* out, int size) const;

        void setDATA_DIR(const char* dir);

        // Returns true if catalog was successfully loaded
        bool setACTIVECATALOG(const char* dir);

        void setRAW_CATALOGS_DIR(const char* dir);

        void setCATALOGS_DIR(const char* dir);

        // Save config settings to file.
        void saveConfig();


        // Loads the catalog
        // If empty, will attempt to load catalog specified by the config.
        bool loadCatalog(const char* dir = "");

        // Create a compiled catalog from a raw catalog.
        bool compileRawCatalog(const char* path, const char* name, bool makeRotations);

        // Return pointer to the catalog item associated with ID.
        //const TItemTypes::TItem* getCatalogItem(const TDataTypes::dcID catID) const;

        // Return reference to our item catalog.
        //const std::vector<TDataTypes::dcID>& getCatalogItemList() const { return m_dataCatalog.getItemIDList(); };




    private:


        TCore m_core;
    };

};

