#include "TDataCatalog.h"



void TDataCatalog::init() {};


std::shared_ptr<TItemTypes::TItem> TDataCatalog::getBestMatch(TItemTypes::TItem& inTtem) {
    return std::shared_ptr<TItemTypes::TItem>(nullptr);
};


std::shared_ptr<TItemTypes::TItem> TDataCatalog::getItem(std::string& name) {

    // Search by item name.
    auto res = std::find_if(m_items.begin(), m_items.end(),
        [&](std::shared_ptr<TItemTypes::TItem> e) { return e->m_name == name; });
    if (res != m_items.end()){
        return *res;
    }
    return nullptr;
};


bool TDataCatalog::compileCatalogFromRaw() {
    return false;
};


bool TDataCatalog::getBestMatch(const TItemTypes::TItem& in, TItemTypes::TItem& out) {
    return false;
};


bool TDataCatalog::loadCatalog() {
    return false;
};

void TDataCatalog::searchVPTree(TItemTypes::TItem& inItem, TDataTypes::VPTree& a, int n) {
    
};

void TDataCatalog::sortItems() {
    std::sort(m_items.begin(), m_items.end(), TItemTypes::compareByName);
};
