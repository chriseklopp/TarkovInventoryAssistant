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
    };


    void TContainerItem::insert(TItem item)
    {
        m_spaceFilled += item.m_dim.first * item.m_dim.second;
        m_contents.push_back(item);
    };

    void TContainerItem::insert(std::vector<TItem> items)
    {
        for (TItem item : items) {
            m_spaceFilled += item.m_dim.first * item.m_dim.second;
            m_contents.push_back(item);
        }
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