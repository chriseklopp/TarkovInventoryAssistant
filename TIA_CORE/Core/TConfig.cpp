#include <Core/TConfig.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <DataSupport/TDataTypes.h>


namespace TConfig {


    TConfig::TConfig() : m_isValid(false){

        ROOT_DIR = resolveRootPath();
        std::filesystem::path iniPath = "Settings.ini";

        // check if config exists 
        if (!std::filesystem::exists(iniPath)) {
            writeConfig(iniPath);
        }

        loadConfig(iniPath);
    }

    const std::filesystem::path& TConfig::getDATA_DIR() {
        return DATA_PATH;
    };

    const std::filesystem::path& TConfig::getACTIVE_CATALOG() {
        return ACTIVE_CATALOG_PATH;
    };

    const std::filesystem::path& TConfig::getRAW_CATALOGS_DIR() {
        return RAW_CATALOGS_PATH;
    };

    const std::filesystem::path& TConfig::getCATALOGS_DIR() {
        return CATALOGS_PATH;
    };

    const std::filesystem::path& TConfig::getROOT_DIR() {
        return ROOT_DIR;
    }

    void TConfig::loadConfig(std::filesystem::path path) {
        if (!std::filesystem::exists(path)) {
            return;
        }


        std::ifstream in;
        in.open(path);

        if (!in.is_open())
            return;

        std::string line;
        std::vector<std::string> split;

        std::map<std::string, std::string> handleValueMap;

        std::getline(in, line);

        while (std::getline(in, line)) {
            if (line.empty())
                continue;
            split.clear();
            
            TDataTypes::splitString(line, '=', split);
            if(split.size() >= 2)
                handleValueMap.insert({ split.at(0), split.at(1) });
        }

        if (handleValueMap.find("Data") != handleValueMap.end()) {
            DATA_PATH = handleValueMap.at("Data");

            std::filesystem::path p = handleValueMap.at("Data");
            if (p.is_absolute()) {
                DATA_PATH = p;
            }
            else {
                DATA_PATH = ROOT_DIR / "Data";
            }

            // A lot depends on this path existing. We need to create it if it doesnt.
            if (!std::filesystem::exists(DATA_PATH)) {
                std::cout << "Existing Data directory not found!!";
                makeDataDirectories(ROOT_DIR);
                DATA_PATH = ROOT_DIR / "Data";
            }


        }
        if (handleValueMap.find("Catalog") != handleValueMap.end()) {
            std::filesystem::path p = handleValueMap.at("Catalog");
            if (p.is_absolute()) {
                ACTIVE_CATALOG_PATH = p;
            }
            else {
                ACTIVE_CATALOG_PATH = DATA_PATH / "CompiledCatalogs" / p;
            }

            if(!std::filesystem::exists(ACTIVE_CATALOG_PATH))
                ACTIVE_CATALOG_PATH = "";
            
        }

        CATALOGS_PATH = DATA_PATH / "CompiledCatalogs";
        RAW_CATALOGS_PATH = DATA_PATH / "RawCatalogs";

        m_iniPath = path;
        in.close();
        m_isValid = true;
    };

    void TConfig::saveConfig() {
        writeConfig(m_iniPath);
    };

    void TConfig::makeDataDirectories(std::filesystem::path& rootPath) {
        std::filesystem::path dataDir = rootPath / "Data";
        std::filesystem::path compCatDir = dataDir / "CompiledCatalogs";
        std::filesystem::path rawCatDir = dataDir / "RawCatalogs";
        std::filesystem::create_directory(dataDir);
        std::filesystem::create_directory(compCatDir);
        std::filesystem::create_directory(rawCatDir);

    };


    std::filesystem::path TConfig::writeConfig(std::filesystem::path& path) {

        std::ofstream file;
        file.open(path, std::ios::out);

        if (!file.is_open())
            std::filesystem::path();

        // Write config file
        file << "[Settings]\n";

        file << "Data=" + DATA_PATH.string()+ "\n";

        file << "Catalog=" + ACTIVE_CATALOG_PATH.string() + "\n";

        return path;
    };

    std::filesystem::path TConfig::resolveRootPath() {
        std::filesystem::path cwd = std::filesystem::current_path();
        if (cwd.filename() == "build") {
            return cwd = cwd.parent_path();
        }
        else {
            // TODO: Alternate path handling.
            return cwd;
        }
    }

    void TConfigManager::setDATA_DIR(std::filesystem::path dir) {
        m_config.DATA_PATH = dir;
    }

    void TConfigManager::setACTIVECATALOG(std::filesystem::path dir) {
        m_config.ACTIVE_CATALOG_PATH = dir;
    };

    void TConfigManager::setRAW_CATALOGS_DIR(std::filesystem::path dir) {
        m_config.RAW_CATALOGS_PATH = dir;
    };

    void TConfigManager::setCATALOGS_DIR(std::filesystem::path dir) {
        m_config.CATALOGS_PATH = dir;
    };

}