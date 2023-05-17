#pragma once

#ifdef _WIN32
#  define EXPORT __declspec( dllexport )
#else
#  define EXPORT
#endif



/*
* This project statically links to the TIA_CORE library and compiles into a DLL
* This makes it easy to export to TarkovInventoryAssistant_Server without making TarkovInventoryAssistant_App angry, which 
* statically links to the core library.
*
Interface used as a medium between the Core library and the TarkovInventoryAssistant server.
This will provide interfaces in a way that are friendly for interop with the C# code running the server.
*/
#include <Core/TCore.h>


namespace WebInterface {

    // Current Size = 103
    struct DetectionResultMarshal {

        DetectionResultMarshal();


        DetectionResultMarshal(const TCore& core, const TItemSupport::DetectionResult& det);

        char name[60];
        char fleaUnit[4];
        int fleaPrice;
        int fleaPricePerSlot;
        char traderUnit[4];
        int traderPrice;
        char trader[12];
        bool sellOnFlea;
        uint8_t dimWidth;
        uint8_t dimHeight;
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

        int detectImageContent(unsigned char* image, int width, int height, int channels, DetectionResultMarshal*& out);

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




extern "C" EXPORT void getDATA_DIR_INTEROP(WebInterface::TWebInterface * iface, char* str, int size) {
    if (iface)
        iface->getDATA_DIR(str, size);
    return;
}

extern "C" EXPORT void getACTIVE_CATALOG_INTEROP(WebInterface::TWebInterface * iface, char* str, int size) {
    if (iface)
        iface->getACTIVE_CATALOG(str, size);
    return;

}

extern "C" EXPORT void getRAW_CATALOGS_DIR_INTEROP(WebInterface::TWebInterface * iface, char* str, int size) {
    if (iface)
        iface->getRAW_CATALOGS_DIR(str, size);
    return;
}

extern "C" EXPORT void getCATALOGS_DIR_INTEROP(WebInterface::TWebInterface * iface, char* str, int size) {
    if (iface)
        iface->getCATALOGS_DIR(str, size);
    return;

}

extern "C" EXPORT void getROOT_DIR_INTEROP(WebInterface::TWebInterface* iface, char* str, int size) {
    if (iface)
        iface->getROOT_DIR(str, size);
    return;
}

extern "C" EXPORT void setDATA_DIR_INTEROP(WebInterface::TWebInterface* iface, char* dir) {
    if (iface)
        iface->setDATA_DIR(dir);
    return;
}


extern "C" EXPORT bool compileRawCatalog_INTEROP(WebInterface::TWebInterface* iface, const char* path, const char* name, bool makeRotations) {
    bool ret = false;
    if (iface)
        ret = iface->compileRawCatalog(path,name,makeRotations);
    return ret;
}


extern "C" EXPORT bool setACTIVECATALOG_INTEROP(WebInterface::TWebInterface* iface, char* dir) {
    bool ret = false;
    if (iface)
        ret = iface->setACTIVECATALOG(dir);
    return ret;
}

extern "C" EXPORT void setRAW_CATALOGS_DIR_INTEROP(WebInterface::TWebInterface* iface, char* dir) {
    if (iface)
        iface->setRAW_CATALOGS_DIR(dir);
    return;
}

extern "C" EXPORT void setCATALOGS_DIR_INTEROP(WebInterface::TWebInterface* iface, char* dir) {
    if (iface)
        iface->setCATALOGS_DIR(dir);
    return;
}

extern "C" EXPORT void saveConfig_INTEROP(WebInterface::TWebInterface* iface) {
    if (iface)
        iface->saveConfig();
    return;
}

extern "C" EXPORT WebInterface::TWebInterface* CreateCoreInterface_INTEROP()
{
    return new WebInterface::TWebInterface();
}

extern "C" EXPORT void DeleteCoreInterface_INTEROP(WebInterface::TWebInterface* iface)
{
    if (iface)
        delete iface;
}



extern "C" EXPORT int detectImageContent_INTEROP(WebInterface::TWebInterface* iface, unsigned char* image, int width, int height, int channels, WebInterface::DetectionResultMarshal*& out) {
    if (iface)
        return iface->detectImageContent(image, width, height, channels, out);
    return 0;
}

// Deletes a result aray from a ptr.
extern "C" EXPORT void disposeDetectionResults_INTEROP(WebInterface::DetectionResultMarshal* in) {
    if (in) {
        delete[] in;
    }
    return;
}