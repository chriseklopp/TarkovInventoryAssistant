#pragma once
#include "TItemTypes.h"
#include "TDataTypes.h"
#include "VPTree.h"
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


    /*
    * Given a TItem, return the a ptr to the best matching item from the catalog.
    */
    std::shared_ptr<TItemTypes::TItem> getBestMatch(TItemTypes::TItem& inTtem);


    // Search catalog for item by name. !! This will return nullptr if it fails !!
    // O(n) lookup. Should not be regularly used. TODO: Improve using Binary Search in futre.
    std::shared_ptr<TItemTypes::TItem> getItem(std::string& name);


    /* This function will compile the raw information from Data/catalog and build the neccessary tree structures
     * This information will be saved to Data/CompiledCatalog
     * This should ONLY be used when a compiled catalog isnt present or when the raw catalog has been updated.
     */
    bool compileCatalogFromRaw();


    // This function compares an item to the catalog and populates a reference to the item that best matches it.
    bool getBestMatch(const TItemTypes::TItem& in, TItemTypes::TItem& out);

private:

    // Load a compiled catalog from Data/CompiledCatalog. Grabs the first one it sees.
    bool loadCatalog();


    // Search the referenced VPTree for best matches to the item given. n specifices number to ret.
    void searchVPTree(TItemTypes::TItem& inItem, TDataTypes::VPTree& a, int n=8);

    // TODO: We are sorting but are still using linear search.Utilize bianry search.
    // Sort the item vector by name. Helps facilitate by name lookups of items. 
    void sortItems();

    // Map of L-W dimensions to respective VPTrees.
    std::unordered_map<std::string, TDataTypes::VPTree> m_dimensionalTrees;
    std::vector<std::shared_ptr<TItemTypes::TItem>> m_items;
    int m_moduleCount;

};