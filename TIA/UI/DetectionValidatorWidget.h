#include <UI/UISupport.h>
#include <wx/statline.h>
#include <wx/tglbtn.h>
#include <unordered_set>

namespace TUI {

    /*
    Tool used to test and improve item detection algorithms.
    Saves detection results and allows comparison against them later.
    It is up to the user to compare against the correct Reference Info file otherwise you will obviously get garbage results.
    */

    class DetectionValidatorWidget : public wxDialog
    {
    public:

        DetectionValidatorWidget(TCore* corePtr,
            wxWindow* parent,
            wxWindowID id = wxID_ANY,
            const wxString& title = wxT("Detection Validator"),
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxSize(499, 481),
            long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);

        ~DetectionValidatorWidget();

        void OnCompare(wxCommandEvent& evt);
        void OnSave(wxCommandEvent& evt);


    private:

        // Contains info read from a reference info file
        struct ReferenceInfo {
            ReferenceInfo() = default;
            ReferenceInfo(const std::string& name, const std::pair<cv::Point, cv::Point>& loc);
            std::string name;
            std::pair<cv::Point, cv::Point> location;


        };

        // Margin of error within which two locations will be considered the same.
        // My current magic number is 2 pixels difference on each point.
        inline static bool locationWithinMOE(const std::pair<cv::Point, cv::Point>& loc1, const std::pair<cv::Point, cv::Point>& loc2) {
            constexpr int moe = 2;
            return (std::abs(loc1.first.x - loc2.first.x) < moe) &&
                (std::abs(loc1.second.x - loc2.second.x) < moe) &&
                (std::abs(loc1.first.y - loc2.first.y) < moe) &&
                (std::abs(loc1.second.y - loc2.second.y) < moe);

        }

        // TODO: Make location within some margin of error.
        friend bool operator==(const ReferenceInfo& lhs, const ReferenceInfo& rhs)
        {
            return lhs.name == rhs.name && locationWithinMOE(lhs.location, rhs.location);
            //return lhs.name == rhs.name && lhs.location == rhs.location;
        }

 


        struct ReferenceInfo_hash {

            std::size_t operator() (const ReferenceInfo& rinfo) const {
                return std::hash<std::string>()(rinfo.name) ^
                    std::hash<int>()(rinfo.location.first.x + rinfo.location.first.y) ^
                    std::hash<int>()(rinfo.location.second.x + rinfo.location.second.y);
            }



            template <class T1, class T2>
            std::size_t operator() (const std::pair<T1, T2>& pair) const {
                return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
            }

        };


        void saveDetectionInfo(imageID id, const std::filesystem::path& path);


        [[nodiscard]] std::unordered_set<ReferenceInfo, ReferenceInfo_hash>  loadReferenceInfo(const std::filesystem::path& path);

        void addToDiscrepancyGrid(const ReferenceInfo& refInfo);

        void clearDiscrepancyGrid();

        static const std::map<std::string, int> m_columnIndexMap;

        static const int m_imageMaxRows = 75;

        static const int m_imageMaxCols = 128;

        TCore* m_corePtr;
        cv::Mat m_comparisonImage;
        wxStaticText* m_referenceFileText;
        wxFilePickerCtrl* m_referenceFileSelector;
        wxStaticText* m_imageIDText;
        wxSpinCtrl* m_imageIDSelector;
        wxButton* m_compareButton;
        wxButton* m_saveButton;
        wxStaticLine* m_inputOutputLine;
        wxStaticText* m_accuracyText;
        wxTextCtrl* m_accuracyOutput;
        wxStaticText* m_numDiffText;
        wxTextCtrl* m_numberDifferencesOutput;
        wxStaticText* m_compDetCountText;
        wxTextCtrl* m_comparisonDetectionCount;
        wxStaticText* m_refDetCountText;
        wxTextCtrl* m_referenceDetectionCount;
        wxStaticLine* m_interactiveDisplayLine;
        wxStaticText* m_discrepancyGridText;
        wxGrid* m_discrepancyGrid;

    };






}

