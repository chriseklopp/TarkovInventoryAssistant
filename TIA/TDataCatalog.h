#pragma once
#include "TItemTypes.h"

#include "TDataTypes.h"
#include "VPTree.h"
#include <unordered_map>
#include <map>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <numeric>
#include "TConfig.h"
/*
Singleton
This class will manage the catalog of items and their properties
Will initialize from a file containing the necessary information.

Items are hashed using perceptual hash on their image and placed into VP-Trees.
*/

class TDataCatalog {



public:

    TDataCatalog(TConfig::TConfig* config) : 
        m_configptr(config),
        m_dimensionalTrees(std::unordered_map<std::pair<int,int>, TDataTypes::TVpTree, Hash::pair_hash>()),
        m_items(std::vector<std::unique_ptr<TItemTypes::TItem>>())
                    
    {};

    // Search catalog for item by name. !! This will return nullptr if it fails !!

    // TODO: We are sorting but are still using linear search.Utilize bianry search.
    TItemTypes::TItem* TDataCatalog::getItem(std::string& name);

    // Returns const reference to the underlying item list.
    const std::vector<std::unique_ptr<TItemTypes::TItem>>& getItemList() { return m_items; };


    /* This function will compile the raw information from Data/catalog
     * This information will be saved to Data/CompiledCatalog
     * This should ONLY be used when a compiled catalog isnt present or when the raw catalog has been updated.
     */
    bool compileCatalogFromRaw(std::filesystem::path rawpath ="", bool makeRotatedItems=false);


    // This function compares an item to the catalog and populates a reference to the item that best matches it.
    TItemTypes::TItem* getBestMatch(TItemTypes::TItem& in);
    
    // This function compares an item to the catalog and finds the N nearest items.
    // !! The NEAREST matches are not necessarily the BEST matches.
    void getNNearestMatches(TItemTypes::TItem& in, std::vector<TItemTypes::TItem*>& out, std::vector<double>& distances, int N=5);


    // Load a compiled catalog from Data/CompiledCatalog.
    bool loadCatalog();
    bool loadCatalog(const std::filesystem::path& catalog);




    // Clears all catalog data from this object.
    void clearCatalog();

private:

    void makeVPTrees();
    // Search the referenced VPTree for best matches to the item given. n specifices number to ret.
    void searchVPTree(TItemTypes::TItem& inItem, TDataTypes::TVpTree& tree) ;


    // Sort the item vector by name. Helps facilitate by name lookups of items. 
    void sortItems();

    // Construct a TItem from a string input. This is to be used when creating items from a compiled catalog.
    // !! This may be deprecated in the future. !!
    std::unique_ptr<TItemTypes::TItem> makeTItemFromCompiledString(const std::string& instring);

    // Methods for handling reading of raw catalog files.
    void writeFileToCompiledCatalog(const std::filesystem::path& file, std::ofstream& out, bool makeRotations);

    void addItemToDimMap(TItemTypes::TItem* item);


    // Load raw catalog from Data/catalog. Raw catalog data used to make compiled catalog.
    // Only use when using a new catalog.
    bool loadRawCatalog(std::vector<std::filesystem::path>& outMods);

    bool loadRawCatalog(std::filesystem::path& catalog, std::vector<std::filesystem::path>& outMods);



    // Map of L-W dimensions to respective VPTrees.
    // Uses unordered map because performance is paramount here.
    std::unordered_map<std::pair<int, int>, TDataTypes::TVpTree, Hash::pair_hash> m_dimensionalTrees;

    //Contains shared_ptrs to all items in the catalog. This is the prime owner (and should be sole tbh) of catalog items.
    std::vector<std::unique_ptr<TItemTypes::TItem>> m_items;



    // Map of dimensions to vectors of items. This structure will be used to generate the VP trees
    // There will be a different tree for each unique dimension.
    std::map<std::pair<int, int>, std::vector<TItemTypes::TItem*>> m_dimItemsMap;

    std::filesystem::path m_catalogPath;


    TConfig::TConfig* m_configptr;
};