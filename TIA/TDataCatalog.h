#pragma once
#include "TItemTypes.h"
#include "TDataTypes.h"
#include <unordered_map>
#include <map>
/*
Singleton
This class will manage the catalog of items and their properties
Will initialize from a file containing the necessary information.

Items are hashed using perceptual hash on their image and placed into VP-Trees.
*/

class TDataCatalog {



public:

    TDataCatalog() : 
        m_dimensionalTrees(std::unordered_map<std::string, TDataTypes::VPTree>()),
        m_items(std::vector<std::shared_ptr<TItemTypes::TItem>>()),
        m_moduleCount(0)
                    
    {};

    void init();


    /* This function will compile the raw information from Data/catalog and build the neccessary tree structures
     * This information will be saved to Data/CompiledCatalog
     * This should ONLY be used when a compiled catalog isnt present or when the raw catalog has been updated.
     */
    bool compileCatalogFromRaw();


    /* This function compares an item to the catalog and populates a reference to the item that best matches it.
    * 
    */
    bool getBestMatch(const TItemTypes::TItem& in, TItemTypes::TItem& out);

private:

    // Load a compiled catalog from Data/CompiledCatalog. Grabs the first one it sees.
    bool loadCatalog();

    // Map of L-W dimensions to repsective VPTrees.
    std::unordered_map<std::string, TDataTypes::VPTree> m_dimensionalTrees;
    std::vector<std::shared_ptr<TItemTypes::TItem>> m_items;
    int m_moduleCount;

};