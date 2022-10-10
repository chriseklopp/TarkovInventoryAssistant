#include "TItemTypes.h"



namespace TItemTypes {

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

}