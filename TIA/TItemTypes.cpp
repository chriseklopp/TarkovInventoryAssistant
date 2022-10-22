#include "TItemTypes.h"



namespace TItemTypes {

    std::shared_ptr<TItem> TItem::makePlaceHolder(cv::Mat& image, int cellSize) {
        int width = lrint(image.cols / cellSize);
        int height = lrint(image.rows / cellSize);
        return  std::make_shared<TItem>(TItem(image, std::make_pair(width, height)));
    };

    void TItem::makeQualified(TItem& to, TItem& from) {
        to.m_name = from.m_name;
        to.m_isRotated = from.m_isRotated;
        to.isPlaceHolder = false;
        // TODO: Needs to cast "to" to derived class if "from" is a derived class type.
    };


    bool TContainerItem::insert(TItem item, cv::Point location)
    {
        m_spaceFilled += item.m_dim.first * item.m_dim.second;
        m_contents.push_back(item);
        return true;
    };

    bool TContainerItem::insert(std::vector<std::pair<TItem, cv::Point>> items)
    {
        for (std::pair<TItem, cv::Point> p : items) {
            // TODO: this is so so so inefficient. Also needs to use shared ptr.
            TItem it = p.first;
            cv::Point loc = p.second;
            m_spaceFilled += it.m_dim.first * it.m_dim.second;
            m_contents.push_back(it);
        }
        return true;
    };



    // Return contents of this Container
    std::vector<TItem>& TContainerItem::getContents()
    {
        return m_contents;
    };

    // Print names of all contents in this container.
    void TContainerItem::printContents()
    {
        for (TItem item : m_contents) {
            std::cout << item.m_name << std::endl;
        }
    };

    bool TItemTypes::compareByName(std::shared_ptr<TItem> a, std::shared_ptr<TItem> b)
    {
        return a->m_name < b->m_name;
    }

}