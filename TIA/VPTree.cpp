#include "VPTree.h"

namespace TDataTypes {

    double TDataTypes::TDistance(const TItemTypes::TItem& p1, const TItemTypes::TItem& p2)
    {
        auto phasher = cv::img_hash::PHash::create();
        return phasher->compare(p1.m_imageHash, p2.m_imageHash);

    }
    double TDataTypes::TDistance(const std::shared_ptr<TItemTypes::TItem>& p1, const std::shared_ptr<TItemTypes::TItem>& p2)
    {
        auto phasher = cv::img_hash::PHash::create();
        return phasher->compare(p1->m_imageHash, p2->m_imageHash);

    }

}