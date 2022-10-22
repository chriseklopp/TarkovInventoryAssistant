#include "TGlobal.h"
namespace TGlobal {

    // Finds home path
    std::filesystem::path resolveHomePath() {
        std::filesystem::path cwd = std::filesystem::current_path();
        if (cwd.filename() == "build") {
            return cwd += "\\..";
        }
        else {
            // TODO: Alternate path handling.
            return cwd;
        }
    }

    inline const std::filesystem::path HOME_PATH = resolveHomePath();
    inline const std::filesystem::path DATA_PATH = HOME_PATH / "Data";
    inline const std::filesystem::path CATALOG_PATH = DATA_PATH / "Catalogs";
    inline const std::filesystem::path COMPCATS_PATH = DATA_PATH / "CompiledCatalogs";

}