#pragma once
#include "TItemTypes.h"
#include "TDataTypes.h"
#include "VPTree.h"
#include <unordered_map>
#include <map>
#include <filesystem>
#include "TGlobal.h"
#include <iostream>
/*
Singleton
This class will manage the catalog of items and their properties
Will initialize from a file containing the necessary information.

Items are hashed using perceptual hash on their image and placed into VP-Trees.
*/

class TDataCatalog {



public:

    TDataCatalog() : 
        m_dimensionalTrees(std::unordered_map<std::string, TDataTypes::TVpTree>()),
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
    std::shared_ptr<const TItemTypes::TItem> getItem(std::string& name);


    /* This function will compile the raw information from Data/catalog and build the neccessary tree structures
     * This information will be saved to Data/CompiledCatalog
     * This should ONLY be used when a compiled catalog isnt present or when the raw catalog has been updated.
     */
    bool compileCatalogFromRaw(std::string rawpath ="");


    // This function compares an item to the catalog and populates a reference to the item that best matches it.
    bool getBestMatch(const TItemTypes::TItem& in, TItemTypes::TItem& out);

    // Todo: Move to private.
    // Load raw catalog from Data/catalog. Raw catalog data used to make compiled catalog.
    // Only use when using a new catalog.
    bool loadRawCatalog();

    bool loadRawCatalog(std::filesystem::path& catalog);


private:


    // Load a compiled catalog from Data/CompiledCatalog. Grabs the first one it sees.
    bool loadCatalog();


    // Search the referenced VPTree for best matches to the item given. n specifices number to ret.
    void searchVPTree(TItemTypes::TItem& inItem, TDataTypes::TVpTree& tree) ;

    // TODO: We are sorting but are still using linear search.Utilize bianry search.
    // Sort the item vector by name. Helps facilitate by name lookups of items. 
    void sortItems();


    // Construct a TItem from a string input. This is to be used when creating items from a compiled catalog.
    // !! This may be deprecated in the future. !!
    std::shared_ptr<TItemTypes::TItem> makeTItemFromString(const std::string& instring);

    // Map of L-W dimensions to respective VPTrees.
    std::unordered_map<std::string, TDataTypes::TVpTree> m_dimensionalTrees;

    //Contains shared_ptrs to all items in the catalog. Shares ownership of items with the VpTrees.
    std::vector<std::shared_ptr<TItemTypes::TItem>> m_items;
    int m_moduleCount;

    std::vector<std::string> m_rawcat; // Those should be empty when not needed.

};