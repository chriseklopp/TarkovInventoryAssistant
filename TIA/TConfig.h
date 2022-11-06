#pragma once
#include "TConfig.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include "TDataTypes.h"



/* Container for environment paths and directories necessary for program execution
* This attempts to populate on init using Settings.ini
* 
*/
namespace TConfig {

    class TConfig {

    public:
        TConfig();

        const std::filesystem::path getDATA_DIR();

        const std::filesystem::path getACTIVE_CATALOG();

        const std::filesystem::path getRAW_CATALOGS_DIR();

        const std::filesystem::path getCATALOGS_DIR();

        bool isValid() { return m_isValid; };

    private:

        // Load Settings.ini config file.
        void loadConfig(std::filesystem::path path);

        // create new config file and return its path
        std::filesystem::path makeNewConfig(std::filesystem::path& path);

        //Creates the data directory and the required structures within at the given path.
        void makeDataDirectories(std::filesystem::path& rootPath);

        std::filesystem::path resolveRootPath();

        // Path to /Data dir
        std::filesystem::path DATA_PATH;

        // Path to active catalog.
        std::filesystem::path ACTIVE_CATALOG_PATH;

        // Path to raw catalogs dir
        std::filesystem::path RAW_CATALOGS_PATH;

        // Path to compiled catalogs dir.
        std::filesystem::path CATALOGS_PATH;

        // Root directory of the program.
        std::filesystem::path ROOT_DIR;

        friend class TConfigEditor;
        bool m_isValid;
    };


    /* Friend class that allows accessing the setters on TConfig
    * We are going to pass a reference to the Config to many different places in the code so they can read
    * from the config but we dont want them changing any data- only the TCore should have those permissions
    * This will be able to call the setters on TConfig
    */
    class TConfigEditor{

    public:
        TConfigEditor(TConfig* config): m_config(config) {};

        TConfigEditor() {};
        void setDATA_DIR(std::filesystem::path dir);

        void setACTIVECATALOG(std::filesystem::path dir);

        void setRAW_CATALOGS_DIR(std::filesystem::path dir);

        void setCATALOGS_DIR(std::filesystem::path dir);

    private:
        TConfig* m_config;

    };


}