#include <DataSupport/VPTree.h>
#include <opencv2/img_hash.hpp>
namespace TDataTypes {

    static cv::Ptr<cv::img_hash::PHash> phasher = cv::img_hash::PHash::create();

    double TDistance(TItemTypes::TItem* const& p1, TItemTypes::TItem* const& p2)
    {
        return phasher->compare(p1->m_imageHash, p2->m_imageHash);
    }
}