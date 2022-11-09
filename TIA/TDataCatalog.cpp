#include "TDataCatalog.h"


TItemTypes::TItem* TDataCatalog::getItem(std::string& name) {

    // Search by item name. Switch to std::lower_bound??
    auto res = std::find_if(m_items.begin(), m_items.end(),
        [&](std::unique_ptr<TItemTypes::TItem>& e) { return e->m_name == name; });
    if (res != m_items.end()) {
        return (*res).get();
    }
    return nullptr;
};


bool TDataCatalog::compileCatalogFromRaw(std::string rawpath, bool makeRotatedItems) {
    if (!m_configptr)
        return false;
    // Compiled Catalog will contain a CSV and folder of all images.
    bool success;
    std::vector<std::filesystem::path> modules;
    if (rawpath.empty())
        success = loadRawCatalog(modules);
    else
        success = loadRawCatalog(std::filesystem::path(rawpath), modules);
    if (!success)
        return false;

    // Create Directory for new compiled catalog.
    time_t t = std::time(nullptr);
    std::filesystem::path catPath = m_configptr->getCATALOGS_DIR() / ("ItemCatalog_" + std::to_string(t));
    std::filesystem::path imPath = catPath / "images";
    std::filesystem::create_directories(imPath);

    std::ofstream file;
    file.open(catPath / "catalog.csv", std::ios::out);

    if (!file.is_open())
        return false;

    for (auto modulePath : modules) {

        // Write item data
        std::filesystem::directory_iterator modItr = std::filesystem::directory_iterator(modulePath);
        for (auto modFile : modItr) {
            if (std::filesystem::is_directory(modFile))
                continue;
            writeFileToCompiledCatalog(modFile.path(), file, makeRotatedItems);
        }
        // Copy images to new image directory.
        std::filesystem::copy(modulePath / "images", imPath);
    }

    // Clean Up
    file.close();
    std::cout << "Successfully Generated Catalog: " << catPath << "\n";
    return true;
};


void TDataCatalog::writeFileToCompiledCatalog(const std::filesystem::path& file,
    std::ofstream& out,
    bool makeRotations) {

    // The defined ordering of read and write to compiled catalog string shall be:
    // Name, path, dims, containerDims, rotation, hash
    // The file format shall be CSV with - delimiting between array values.
    // EX: Dog,Dog.img,4-5,2-2,0,23-43-677-234-893
    std::ifstream in;
    in.open(file);

    if (!in.is_open())
        return;

    std::string line;
    std::vector<std::string> fields;

    // Skip header. (For now; maybe I'll utilize this in future to make it more robust).
    std::getline(in, line);

    while (std::getline(in, line)) {
        fields.clear();
        TDataTypes::splitString(line, ',', fields);

        std::string imagesPath((file.parent_path() / "images" / fields.at(1)).string());
        cv::Mat image(cv::imread(imagesPath));
        if (image.empty()) {
            std::cout << "Attempted to load invalid image: skipping it \n";
            continue;
        }

        // Make rotated items.
        if (makeRotations) {
            std::vector<std::string> rotFields = fields;
            rotFields.push_back("1");

            // Rotate image clockwise??? 90 degrees.
            cv::Mat rotImage;
            cv::transpose(image, rotImage);
            cv::flip(rotImage, rotImage, 1);

            // Construct hash, convert it to string and add to vector

            cv::Mat rotImHash(Hash::PhashImage(rotImage));
            std::vector<int> hashvec(rotImHash.begin<cv::uint8_t>(), rotImHash.end<cv::uint8_t>());
            fields.push_back(TDataTypes::joinVector(hashvec, '-'));
            out << TDataTypes::joinVector(rotFields, ',') << "\n";
        }

        // Rotation indicator (false)
        fields.push_back("0");

        // Construct hash, convert it to string and add to vector
        cv::Mat imHash(Hash::PhashImage(image));
        std::vector<int> hashvec(imHash.begin<cv::uint8_t>(), imHash.end<cv::uint8_t>());
        fields.push_back(TDataTypes::joinVector(hashvec, '-'));

        // Join and write to file.
        out << TDataTypes::joinVector(fields, ',') << "\n";

    }

    // Clean up
    in.close();

    return;
};

std::unique_ptr<TItemTypes::TItem> TDataCatalog::makeTItemFromCompiledString(const std::string& instring) {
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
        imagePath = m_catalogPath / "images" / fields.at(1);

        // Dims
        TDataTypes::splitString(fields.at(2), '-', temp);
        dims.first = temp.at(0);
        dims.second = temp.at(1);
        temp.clear();

        //Container Dims
        TDataTypes::splitString(fields.at(3), '-', temp);
        contDims.first = temp.at(0);
        contDims.second = temp.at(1);
        temp.clear();

        //Rotation
        if (fields.at(4) == "1")
            rotation = true;
        else if (fields.at(4) == "0")
            rotation = false;
        else
            throw std::invalid_argument("Invalid rotation");

        //Hash
        TDataTypes::splitString(fields.at(5), '-', temp);
        hashVals = cv::Mat(temp).t();
        hashVals.convertTo(hashVals, CV_8U);

    }
    catch (std::invalid_argument) {
        std::cout << "makeTItemFromString: Invalid argument found in string " <<
            instring << " Item will not be created \n";
        return std::unique_ptr<TItemTypes::TItem>(nullptr);
    }
    catch (std::out_of_range) {
        std::cout << "makeTItemFromString: Invalid argument found in string " <<
            instring << " Item will not be created \n";
        return std::unique_ptr<TItemTypes::TItem>(nullptr);
    }

    // This is a container item.
    if (contDims.first != 0 && contDims.second != 0)
        return std::make_unique<TItemTypes::TItem>(TItemTypes::TContainerItem(
            name,
            imagePath,
            dims,
            contDims,
            rotation,
            hashVals));

    // This is a regular TItem.
    return std::make_unique<TItemTypes::TItem>(TItemTypes::TItem(
        name,
        imagePath,
        dims,
        rotation,
        hashVals));
};


TItemTypes::TItem* TDataCatalog::getBestMatch(TItemTypes::TItem& in) {


    // Get 10 nearest results, then use template matching to more accurately determine the best choice.
    // TODO: This still needs to be improved. 
    // TODO: Keys are so oppressive they need to be in their own separate groups.
    std::vector<TItemTypes::TItem*> items;
    std::vector<double> dist;

    getNNearestMatches(in, items, dist, 10);

    if (!items.size())
        return nullptr;


    TItemTypes::TItem* bestIt;
    double best = 10;

    for (auto it : items) {
        auto res = Hash::templateMatch(in.getImage(), it->getImage());
        if (res.at<float>(cv::Point(0, 0)) < best) {
            best = res.at<float>(cv::Point(0, 0));
            bestIt = it;
        }
    }


    //DEBUG:
    /*cv::imshow("res", bestIt->getImage());
    cv::imshow("in", in.getImage());
    cv::waitKey(0);*/


    return bestIt;
};



void TDataCatalog::getNNearestMatches(TItemTypes::TItem& in,
    std::vector<TItemTypes::TItem*>& out,
    std::vector<double>& retDistances,
    int N) {

    if (m_dimItemsMap.find(in.m_dim) != m_dimItemsMap.end())
        m_dimensionalTrees.at(in.m_dim).search(&in, N, &out, &retDistances);
    return;
}

bool TDataCatalog::loadCatalog() {
    if (!m_configptr)
        return false;
    // No path specifed. Find most recently created catalog.
    std::filesystem::path newestCat;
    std::filesystem::file_time_type currtime;

    std::filesystem::directory_iterator itr = std::filesystem::directory_iterator(m_configptr->getCATALOGS_DIR());

    if (itr != std::filesystem::end(itr)) {
        newestCat = *itr;
        currtime = itr->last_write_time();
    }
    else {
        std::cout << "LoadRawCatalog: No catalogs found at: " << m_configptr->getCATALOGS_DIR().string() << std::endl;
        return false;
    }

    for (itr; itr != std::filesystem::end(itr); ++itr) {
        if (currtime < itr->last_write_time()) {
            currtime = itr->last_write_time();
            newestCat = *itr;
        }
    }

    return loadCatalog(newestCat);
};


bool TDataCatalog::loadCatalog(const std::filesystem::path& catalog) {
    std::filesystem::path dataFile = catalog / "catalog.csv";

    if (!std::filesystem::exists(dataFile))
        return false;

    std::ifstream in;
    in.open(dataFile);

    if (!in.is_open())
        return false;

    // We have confirmed that this catalog is atleast MOSTLY valid.
    // At this point we are probably good to clear the previous catalog.
    clearCatalog();
    m_catalogPath = catalog;

    std::string line;
    // Skip header. (For now; maybe I'll utilize this in future to make it more robust).
    std::getline(in, line);
    while (std::getline(in, line)) {
        std::unique_ptr<TItemTypes::TItem> item(makeTItemFromCompiledString(line));
        if (!item)
            continue;


        addItemToDimMap(item.get());
        m_items.push_back(std::move(item));

    }

    sortItems();
    // Finally make our VPTree structures.
    makeVPTrees();
    std::cout << "Loaded catalog of size: " << m_items.size() << "\n";
    return true;
};

void TDataCatalog::addItemToDimMap(TItemTypes::TItem* item) {
    if (m_dimItemsMap.find(item->m_dim) != m_dimItemsMap.end()) {
        m_dimItemsMap.at(item->m_dim).push_back(item);
    }
    else {
        m_dimItemsMap.insert({ item->m_dim,std::vector<TItemTypes::TItem*>() });
        m_dimItemsMap.at(item->m_dim).push_back(item);
    }
};

void TDataCatalog::makeVPTrees() {
    for (auto iter = m_dimItemsMap.begin(); iter != m_dimItemsMap.end(); ++iter) {
        m_dimensionalTrees.insert({ iter->first, TDataTypes::TVpTree() });
        m_dimensionalTrees.at(iter->first).create(iter->second);
    }
};

void TDataCatalog::clearCatalog() {
    m_items.clear();
    m_catalogPath.clear();
    m_dimensionalTrees.clear();
    m_dimItemsMap.clear();

};

void TDataCatalog::searchVPTree(TItemTypes::TItem& inItem, TDataTypes::TVpTree& tree) {

};

void TDataCatalog::sortItems() {
    std::sort(m_items.begin(), m_items.end(),
        [](std::unique_ptr<TItemTypes::TItem>& a,
            std::unique_ptr<TItemTypes::TItem>& b) {return TItemTypes::compareByName(*a, *b); });
};

bool TDataCatalog::loadRawCatalog(std::vector<std::filesystem::path>& outMods) {
    if (!m_configptr)
        return false;
    // No path specifed. Find most recently created catalog.
    std::filesystem::path newestCat;
    std::filesystem::file_time_type currtime;

    std::filesystem::directory_iterator itr = std::filesystem::directory_iterator(m_configptr->getRAW_CATALOGS_DIR());

    if (itr != std::filesystem::end(itr)) {
        newestCat = *itr;
        currtime = itr->last_write_time();
    }
    else {
        std::cout << "LoadRawCatalog: No catalogs found at: " << m_configptr->getRAW_CATALOGS_DIR().string() << std::endl;
        return false;
    }

    for (itr; itr != std::filesystem::end(itr); ++itr) {
        if (currtime < itr->last_write_time()) {
            currtime = itr->last_write_time();
            newestCat = *itr;
        }
    }

    return loadRawCatalog(newestCat, outMods);
};

bool TDataCatalog::loadRawCatalog(std::filesystem::path& catalog, std::vector<std::filesystem::path>& outMods) {


    // Find modules in the catalog.
    for (auto itr = std::filesystem::directory_iterator(catalog); itr != std::filesystem::end(itr); ++itr) {
        outMods.push_back(*itr);
    }

    if (!outMods.size()) {
        std::cout << "LoadRawCatalog: No data modules found at: " << catalog.string() << std::endl;
        return false;
    }

    return true;
};


