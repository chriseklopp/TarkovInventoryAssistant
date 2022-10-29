// TarkovInventoryAssistant.cpp : Defines the entry point for the application.
//

#include "TarkovInventoryAssistant.h"

int main()
{
	std::cout << "Hello CMake." << std::endl;

    TImageReader reader = TImageReader();
    TDataCatalog cat = TDataCatalog();
    //cat.compileCatalogFromRaw("C:\\MyWorkspace\\TarkovInventoryAssistant\\Data\\Catalogs\\catalog-Keyless");
    cat.loadCatalog();
    std::vector <std::unique_ptr<TItemTypes::TItem>> out;
    std::string impath = "C:\\pyworkspace\\tarkovinventoryproject\\Data\\screenshots\\testimage3.png";

    reader.parseFromPath(impath, out);


    for (auto&& it : out) {
        TItemTypes::TItem* res = cat.getBestMatch(*it);
        if (res) {
            //cv::imshow("Res", res->getImage());
            //cv::imshow("IN", it->getImage());
            //cv::waitKey(0);
        }
    }

    return 0;
}
