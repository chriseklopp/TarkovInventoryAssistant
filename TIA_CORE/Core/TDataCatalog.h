#pragma once
#include <DataSupport/TItemTypes.h>
#include <DataSupport/VPTree.h>

#include <unordered_map>
#include <map>
#include <Core/TConfig.h>
/*
Singleton
This class will manage the catalog of items and their properties
Will initialize from a file containing the necessary information.

Items are hashed using perceptual hash on their image and placed into VP-Trees.
*/


namespace TDataCatalog {

    using DimItemMap = std::map<std::pair<int, int>, std::vector<TItemTypes::TItem*>>;


    class TDataCatalog {


    public:

        TDataCatalog(TConfig::TConfig* config) :
            m_dimensionalTrees(),
            m_items(),
            m_reverseItemMap(),
            m_itemIDList(),
            m_idCounter(0),
            m_configptr(config),
            m_catalogPath()

        {};

        // Search catalog for item by name. !! This will return nullptr if it fails !!

        // TODO: We are sorting but are still using linear search.Utilize bianry search.
        const TItemTypes::TItem* getItem(std::string& name) const;

        const TItemTypes::TItem* getItem(TDataTypes::dcID id) const;



        // Returns const reference to the underlying item list.
        const std::vector<TDataTypes::dcID>& getItemIDList() const;


        /* This function will compile the raw information from Data/catalog
         * This information will be saved to Data/CompiledCatalog
         * This should ONLY be used when a compiled catalog isnt present or when the raw catalog has been updated.
         */
        bool compileCatalogFromRaw(const std::filesystem::path& rawpath, std::string name, bool makeRotatedItems = false) const;


        // This function compares an item to the catalog and returns the item's ID.
        TDataTypes::dcID getBestMatch(TItemTypes::TItem& in);

        // This function compares an item to the catalog and finds the N nearest items.
        // !! The NEAREST matches are not necessarily the BEST matches.
        void getNNearestMatches(TItemTypes::TItem& in, std::vector<TItemTypes::TItem*>& out, std::vector<double>& distances, int N = 5);


        // Load a compiled catalog from Data/CompiledCatalog.
        bool loadCatalog();
        bool loadCatalog(const std::filesystem::path& catalog);




        // Clears all catalog data from this object.
        void clearCatalog();

    private:

        void makeVPTrees(DimItemMap& dMap);
        // Search the referenced VPTree for best matches to the item given. n specifices number to ret.
        void searchVPTree(TItemTypes::TItem& inItem, TDataTypes::TVpTree& tree);



        // Construct a TItem from a string input. This is to be used when creating items from a compiled catalog.
        // !! This may be deprecated in the future. !!
        std::unique_ptr<TItemTypes::TItem> makeTItemFromCompiledString(const std::string& instring);

        // Methods for handling reading of raw catalog files.
        bool writeFileToCompiledCatalog(const std::filesystem::path& file, const std::filesystem::path& compiledImagesPath, std::ofstream& out, bool makeRotations) const;

        void addItemToDimMap(TItemTypes::TItem* item, DimItemMap& dMap);


        // NOTE: Currently autodetecting raw catalog is unused.
        bool loadRawCatalog(std::vector<std::filesystem::path>& outMods) const;

        // Load raw catalog from Data/catalog. Raw catalog data used to make compiled catalog.
        bool loadRawCatalog(const std::filesystem::path& catalog, std::vector<std::filesystem::path>& outMods) const;


        TDataTypes::dcID createNewDCID();


        // Map of L-W dimensions to respective VPTrees.
        // Uses unordered map because performance is paramount here.
        std::unordered_map<std::pair<int, int>, TDataTypes::TVpTree, Hash::pair_hash> m_dimensionalTrees;

        //Contains shared_ptrs to all items in the catalog. This is the sole owner of catalog items.
        std::unordered_map<TDataTypes::dcID, std::unique_ptr<TItemTypes::TItem>> m_items;

        /*
        Reverse TItem dcID map.This is not ideal but having this allows me to leave
        TVpTree unaltered through the refactor. In addition with this method
        TVpTree wont have to lookup through dcID EVERY comparison so it is more performant.
        This is ABSOLUTELY only for internal private use only.
        */
        std::unordered_map<TItemTypes::TItem* , TDataTypes::dcID> m_reverseItemMap;

        // Maps ItemID of a rotated item to the item ID of its non rotated variant.
        std::unordered_map<TDataTypes::dcID, TDataTypes::dcID> m_rotatedItemAliasMap;

        // List of catalog item IDs. 
        std::vector<TDataTypes::dcID> m_itemIDList;


        TDataTypes::dcID m_idCounter;

        TConfig::TConfig* m_configptr;

        std::filesystem::path m_catalogPath;

    };
}