#pragma once
#include "TConfig.h"
#include <iostream>


namespace TConfig {

    class TConfig {

    public:
        TConfig() {
        // Load config file or make new one if missing.
        };

        const std::string getDATA_DIR();

        const std::string getACTIVE_CATALOG();

        const std::string getRAW_CATALOGS_DIR();

        const std::string getCATALOGS_DIR();

    private:

        void loadConfig();
        void makeNewConfig();



        // Path to /Data dir
        std::string DATA_PATH;

        // Path to active catalog.
        std::string ACTIVE_CATALOG_PATH;

        // Path to raw catalogs dir
        std::string RAW_CATALOGS_PATH;

        // Path to compiled catalogs dir.
        std::string CATALOGS_PATH_PATH;

        friend class TConfigEditor;

    };


    /* Friend class that allows accessing the setters on TConfig
    * We are going to pass a reference to the Config to many different places in the code so they can read
    * from the config but we dont want them changing any data- only the TCore should have those permissions
    * This will be able to call the setters on TConfig
    */
    class TConfigEditor{

    public:
        TConfigEditor(TConfig* config): m_config(config) {};


        void setDATA_DIR(std::string dir);

        void setACTIVECATALOG(std::string dir);

        void setRAW_CATALOGS_DIR(std::string dir);

        void setCATALOGS_DIR(std::string dir);

    private:
        TConfig* m_config;

    };


}