#pragma once
#include <filesystem>
/*
Contains global variables that may be of use in other modules.
*/

namespace TGlobal {


    // Finds home path
    std::filesystem::path resolveHomePath();

    // Path to TarkovInventoryAssistant folder.
    extern const std::filesystem::path HOME_PATH;

    // Path to Data folder
    extern const std::filesystem::path DATA_PATH;

    // Path to catalogs folder
    extern const std::filesystem::path CATALOG_PATH;

    // Path to compiled catalogs folder.
    extern const std::filesystem::path COMPCATS_PATH;
}