#include "TDataCatalog.h"


void TDataCatalog::init() {};


std::shared_ptr<TItemTypes::TItem> TDataCatalog::getBestMatch(TItemTypes::TItem& inTtem) {
    return std::shared_ptr<TItemTypes::TItem>(nullptr);
};


std::shared_ptr<const TItemTypes::TItem> TDataCatalog::getItem(std::string& name) {

    // Search by item name.
    auto res = std::find_if(m_items.begin(), m_items.end(),
        [&](std::shared_ptr<const TItemTypes::TItem> e) { return e->m_name == name; });
    if (res != m_items.end()){
        return *res;
    }
    return nullptr;
};


bool TDataCatalog::compileCatalogFromRaw(std::string rawpath) {

    // Compiled Catalog will contain a CSV and folder of all images.
    bool success;
    if (rawpath.empty())
        success = loadRawCatalog();
    else
        success = loadRawCatalog(std::filesystem::path(rawpath));
    if (!success)
        return false;

    // Create Directory for new compiled catalog.
    time_t t = std::time(nullptr);
    std::filesystem::path catPath = TGlobal::COMPCATS_PATH / ("ItemCatalog_" + std::to_string(t)) / "images";

    std::filesystem::create_directories(catPath);

    return false;
};

std::string convertToTItemString(std::string& instring) {

    // The defined ordering of read and write to compiled catalog string shall be:
    // Name, path, dims, outerDims, rotation, hash
    // The file format shall be CSV with - delimiting between array values.
    // EX: Dog,Dog.img,4-5,2-2,0,23-43-677-234-893
    std::string s;
    std::vector<std::string> fields;
    TDataTypes::splitString(instring, ',', fields);



    return s;
};

std::shared_ptr<TItemTypes::TItem> makeTItemFromString(const std::string& instring) {
    // The defined ordering of read and write to compiled catalog string shall be:
    // Name, path, dims, containerDims, rotation, hash
    // The file format shall be CSV with - delimiting between array values.
    // EX: Dog,Dog.img,4-5,2-2,0,23-43-677-234-893

    std::vector<std::string> fields;
    TDataTypes::splitString(instring, ',', fields);

    // Populate necessary parameters.
    std::string name;
    std::filesystem::path imagePath;
    std::pair<int, int> dims;
    std::pair<int, int> contDims;
    bool rotation;
    cv::Mat hashVals;

    try {
        std::vector<int> temp;
        // Name
        name = fields.at(0);

        // Image
        imagePath = std::filesystem::path(fields.at(1));

        // Dims
        TDataTypes::splitString(fields.at(2), '-', temp);
        dims.first = temp.at(0);
        dims.second = temp.at(1);
        temp.clear();

        //Container Dims
        TDataTypes::splitString(fields.at(3), '-', temp);
        contDims.first = temp.at(0);
        contDims.second = temp.at(1);

        //Rotation
        if (fields.at(4) == "1")
            rotation = true;
        else if (fields.at(4) == "0")
            rotation = false;
        else
            throw std::invalid_argument("Invalid rotation");

        //Hash
        TDataTypes::splitString(fields.at(5), '-', temp);
        cv::Mat mat(temp); // TODO: is this the correct format, or does it need transposing.

    }
    catch (std::invalid_argument) {
        std::cout << "makeTItemFromString: Invalid argument found in string " <<
            instring << " Item will not be created \n";
    }

    // This is a container item.
    if(contDims.first != 0 && contDims.second != 0)
        return std::make_shared<TItemTypes::TItem>(TItemTypes::TContainerItem(
                                                    name,
                                                    imagePath,
                                                    dims,
                                                    contDims,
                                                    rotation,
                                                    hashVals));

    // This is a regular TItem.
    return std::make_shared<TItemTypes::TItem>(TItemTypes::TItem(
                                                name,
                                                imagePath,
                                                dims,
                                                rotation,
                                                hashVals));
};


bool TDataCatalog::getBestMatch(const TItemTypes::TItem& in, TItemTypes::TItem& out) {
    return false;
};


bool TDataCatalog::loadCatalog() {
    return false;
};

void TDataCatalog::searchVPTree(TItemTypes::TItem& inItem, TDataTypes::TVpTree& tree) {
    
};

void TDataCatalog::sortItems() {
    std::sort(m_items.begin(), m_items.end(), TItemTypes::compareByName);
};

bool TDataCatalog::loadRawCatalog() {

    // No path specifed. Find most recently created catalog.
    std::filesystem::path newestCat; 
    std::filesystem::file_time_type currtime;

    std::filesystem::directory_iterator itr = std::filesystem::directory_iterator(TGlobal::CATALOG_PATH);

    if (itr != std::filesystem::end(itr)) {
        newestCat = *itr;
        currtime = itr->last_write_time();
    }
    else {
        std::cout << "LoadRawCatalog: No catalogs found at: " << TGlobal::CATALOG_PATH.string() << std::endl;
        return false;
    }

    for (itr; itr != std::filesystem::end(itr); ++itr){
        if (currtime < itr->last_write_time()) {
            currtime = itr->last_write_time();
            newestCat = *itr;
        }
    }

    return loadRawCatalog(newestCat);
};

bool TDataCatalog::loadRawCatalog(std::filesystem::path& catalog) {

    std::vector<std::filesystem::path> modulePaths;

    // Find modules in the catalog.
    for (auto itr = std::filesystem::directory_iterator(catalog); itr != std::filesystem::end(itr); ++itr) {
        modulePaths.push_back(*itr);
    }

    if (!modulePaths.size()) {
        std::cout << "LoadRawCatalog: No data modules found at: " << catalog.string() << std::endl;
        return false;
    }

    //m_rawcat = 

    return true;
};

