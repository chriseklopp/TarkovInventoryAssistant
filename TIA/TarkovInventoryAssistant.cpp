// TarkovInventoryAssistant.cpp : Defines the entry point for the application.
//

#include "TarkovInventoryAssistant.h"
#include "TScreenCapture.h"
#include "TImageReader.h"
int main()
{
	std::cout << "Hello CMake." << std::endl;

    TImageReader reader = TImageReader();

    std::vector < std::shared_ptr < TItemTypes::TItem>> out;
    std::string impath = "C:/pyworkspace/tarkovinventoryproject/Data/screenshots/testcontainerbody221008_111257.png";
    reader.parseFromPath(impath, out);
    return 0;
}
