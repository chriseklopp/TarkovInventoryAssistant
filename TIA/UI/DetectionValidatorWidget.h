#include <UI/UISupport.h>
#include <wx/filepicker.h>
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
            std::string name;
            std::pair<cv::Point, cv::Point> location;
        };

        void saveDetectionInfo(imageID id);

        std::unordered_set<ReferenceInfo> loadReferenceInfo(const std::filesystem::path& path);




        TCore* m_corePtr;

        wxStaticText* m_referenceFileText;
        wxFilePickerCtrl* m_referenceFileSelector;
        wxStaticText* m_imageIDText;
        wxSpinCtrl* m_imageIDSelector;
        wxToggleButton* m_compareButton;
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
        wxGrid* m_incorrectDetections;

    };



}

