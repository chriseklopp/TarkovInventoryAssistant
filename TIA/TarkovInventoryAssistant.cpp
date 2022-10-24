// TarkovInventoryAssistant.cpp : Defines the entry point for the application.
//

#include "TarkovInventoryAssistant.h"

int main()
{
	std::cout << "Hello CMake." << std::endl;
    auto x = TGlobal::HOME_PATH;
    auto y = TGlobal::CATALOG_PATH;
    auto z = TGlobal::COMPCATS_PATH;
    TImageReader reader = TImageReader();
    TDataCatalog cat = TDataCatalog();
    //cat.compileCatalogFromRaw();
    cat.loadCatalog();
    std::vector <std::shared_ptr<TItemTypes::TItem>> out;
    std::string impath = "C:/pyworkspace/tarkovinventoryproject/Data/screenshots/testcontainerbody221008_111257.png";
    reader.parseFromPath(impath, out);
    return 0;
}
