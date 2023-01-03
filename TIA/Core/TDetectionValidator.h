#pragma once
#include <DataSupport/TItemTypes.h>
#include <Core/TConfig.h>

namespace DV {

    // Contains info from a detection validation operation.
    struct DVResult {
        
    };


    class DetectionValidator {

        DetectionValidator(TConfig::TConfig* config) {};


        // Save detection info associated with an image.
        // This can be compared against later to test accuracy of a detection algorithm.
        // The accuracy of this info is only as good as the detection algorithm used when it was created.
        // Consider manually editing and validating this info to guarantee an accurate list for comparison.
        void saveDetectionInfo();

        void compareToDetectionInfoFile();

        // Load detectionInfo file
        void loadDetectionInfo();





    private:

        TConfig::TConfig* m_configptr;
    };







}
