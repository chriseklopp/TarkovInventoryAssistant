#include <DataSupport/VPTree.h>
#include <opencv2/img_hash.hpp>
namespace TDataTypes {


    double TDistance(TItemTypes::TItem* const& p1, TItemTypes::TItem* const& p2)
    {
        auto phasher = cv::img_hash::PHash::create();
        return phasher->compare(p1->m_imageHash, p2->m_imageHash);

    }
}