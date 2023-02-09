#include <UI/DetectionValidatorWidget.h>
#include <wx/spinctrl.h>
#include <fstream>


namespace TUI {

    // Catalog display column ordering
    const std::map<std::string, int> DetectionValidatorWidget::m_columnIndexMap = {
    {"Image",  0},
    {"Name", 1},
    {"Loc", 2}
    };

    DetectionValidatorWidget::DetectionValidatorWidget(TCore* corePtr,
        wxWindow* parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos,
        const wxSize& size,
        long style) :

        m_corePtr(corePtr),
        wxDialog(parent, id, title, pos, size, style)
    {


        this->SetSizeHints(wxDefaultSize, wxDefaultSize);
        // ---------------- Input -------------------------------------
        wxBoxSizer* mainsizer;
        mainsizer = new wxBoxSizer(wxVERTICAL);

        m_referenceFileText = new wxStaticText(this, wxID_ANY, wxT("Reference File"), wxDefaultPosition, wxDefaultSize, 0);
        m_referenceFileText->Wrap(-1);
        mainsizer->Add(m_referenceFileText, 0, wxALL, 5);

        m_referenceFileSelector = new wxFilePickerCtrl(this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE);
        m_referenceFileSelector->SetInitialDirectory(m_corePtr->getConfigPtr()->getDATA_DIR().string());
        mainsizer->Add(m_referenceFileSelector, 0, wxALL, 5);

        m_imageIDText = new wxStaticText(this, wxID_ANY, wxT("Comparison Image ID"), wxDefaultPosition, wxDefaultSize, 0);
        m_imageIDText->Wrap(-1);
        mainsizer->Add(m_imageIDText, 0, wxALL, 5);

        m_imageIDSelector = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 9999, 0);
        mainsizer->Add(m_imageIDSelector, 0, wxALL, 5);

        m_saveButton = new wxButton(this, wxID_ANY, wxT("Save New Reference"), wxDefaultPosition, wxDefaultSize, 0);
        m_saveButton->Bind(wxEVT_BUTTON, &DetectionValidatorWidget::OnSave, this);
        mainsizer->Add(m_saveButton, 0, wxALL, 5);


        m_compareButton = new wxButton(this, wxID_ANY, wxT("Compare"), wxDefaultPosition, wxDefaultSize, 0);
        m_compareButton->Bind(wxEVT_BUTTON, &DetectionValidatorWidget::OnCompare, this);
        mainsizer->Add(m_compareButton, 0, wxALL | wxEXPAND, 5);

        m_inputOutputLine = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
        mainsizer->Add(m_inputOutputLine, 0, wxEXPAND | wxALL, 5);


        // ----------------Output -------------------------------------
        wxBoxSizer* outputInfosizer;
        outputInfosizer = new wxBoxSizer(wxVERTICAL);

        wxBoxSizer* accuracySizer;
        accuracySizer = new wxBoxSizer(wxHORIZONTAL);

        m_accuracyText = new wxStaticText(this, wxID_ANY, wxT("Accuracy"), wxDefaultPosition, wxDefaultSize, 0);
        m_accuracyText->Wrap(-1);
        accuracySizer->Add(m_accuracyText, 0, wxALL, 5);


        accuracySizer->Add(0, 0, 1, wxEXPAND, 5);

        m_accuracyOutput = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
        accuracySizer->Add(m_accuracyOutput, 0, wxALL, 5);


        outputInfosizer->Add(accuracySizer, 1, wxEXPAND, 5);

        wxBoxSizer* differenceCountSizer;
        differenceCountSizer = new wxBoxSizer(wxHORIZONTAL);

        m_numDiffText = new wxStaticText(this, wxID_ANY, wxT("Number of Differences"), wxDefaultPosition, wxDefaultSize, 0);
        m_numDiffText->Wrap(-1);
        differenceCountSizer->Add(m_numDiffText, 0, wxALL, 5);


        differenceCountSizer->Add(0, 0, 1, wxEXPAND, 5);

        m_numberDifferencesOutput = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
        differenceCountSizer->Add(m_numberDifferencesOutput, 0, wxALL, 5);


        outputInfosizer->Add(differenceCountSizer, 1, wxEXPAND, 5);

        wxBoxSizer* comparisonDetCount;
        comparisonDetCount = new wxBoxSizer(wxHORIZONTAL);

        m_compDetCountText = new wxStaticText(this, wxID_ANY, wxT("Comparison Detection Count"), wxDefaultPosition, wxDefaultSize, 0);
        m_compDetCountText->Wrap(-1);
        comparisonDetCount->Add(m_compDetCountText, 0, wxALL, 5);


        comparisonDetCount->Add(0, 0, 1, wxEXPAND, 5);

        m_comparisonDetectionCount = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
        comparisonDetCount->Add(m_comparisonDetectionCount, 0, wxALL, 5);


        outputInfosizer->Add(comparisonDetCount, 1, wxEXPAND, 5);

        wxBoxSizer* referenceDetCount;
        referenceDetCount = new wxBoxSizer(wxHORIZONTAL);

        m_refDetCountText = new wxStaticText(this, wxID_ANY, wxT("Reference Detection Count"), wxDefaultPosition, wxDefaultSize, 0);
        m_refDetCountText->Wrap(-1);
        referenceDetCount->Add(m_refDetCountText, 0, wxALL, 5);


        referenceDetCount->Add(0, 0, 1, wxEXPAND, 5);

        m_referenceDetectionCount = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
        referenceDetCount->Add(m_referenceDetectionCount, 0, wxALL, 5);


        outputInfosizer->Add(referenceDetCount, 1, wxEXPAND, 5);

        mainsizer->Add(outputInfosizer, 0, wxEXPAND | wxALL, 2);

        m_interactiveDisplayLine = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
        mainsizer->Add(m_interactiveDisplayLine, 0, wxEXPAND | wxALL, 5);

        m_discrepancyGridText = new wxStaticText(this, wxID_ANY, wxT("Discrepancies"), wxDefaultPosition, wxDefaultSize, 0);
        m_discrepancyGridText->Wrap(-1);
        mainsizer->Add(m_discrepancyGridText, 0);

        m_discrepancyGrid = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);

        // Grid

        m_discrepancyGrid->CreateGrid(0, m_columnIndexMap.size());
        m_discrepancyGrid->HideRowLabels();

        for (auto& c : m_columnIndexMap) {
            m_discrepancyGrid->SetColLabelValue(c.second, c.first);
        }

        m_discrepancyGrid->SetColLabelSize(20);
        m_discrepancyGrid->SetDefaultRowSize(m_imageMaxRows);
        m_discrepancyGrid->SetColSize(m_columnIndexMap.at("Image"), m_imageMaxCols);
        m_discrepancyGrid->SetColSize(m_columnIndexMap.at("Name"), 240);
        m_discrepancyGrid->EnableEditing(false);

        m_discrepancyGrid->SetDefaultCellBackgroundColour(wxColor(20, 20, 20));
        m_discrepancyGrid->SetDefaultCellTextColour(wxColor(255, 255, 255));
        m_discrepancyGrid->SetDefaultCellFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        m_discrepancyGrid->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
        m_discrepancyGrid->SetGridLineColour(wxColour(73, 81, 84));

        mainsizer->Add(m_discrepancyGrid, 1, wxALL| wxEXPAND, 5);


        this->SetSizer(mainsizer);
        this->Layout();

        this->Centre(wxVERTICAL);

        this->SetMinSize(this->GetSize());
        wxSize maxSize = this->GetSize();
        maxSize.SetHeight(maxSize.GetHeight() * 4);
        this->SetMaxSize(maxSize);

    }

    DetectionValidatorWidget::~DetectionValidatorWidget() {
    }

    void DetectionValidatorWidget::OnCompare(wxCommandEvent& evt) {
        clearDiscrepancyGrid();
        std::filesystem::path referenceInfoPath = m_referenceFileSelector->GetTextCtrlValue().ToStdString();

        imageID id = std::stoi(m_imageIDSelector->GetTextValue().ToStdString());

        const std::vector<TItemSupport::DetectionResult>* detections = m_corePtr->getDetectionResults(id);
        std::unordered_set<ReferenceInfo, ReferenceInfo_hash> refSet = loadReferenceInfo(referenceInfoPath);

        const cv::Mat* imagePtr = m_corePtr->getImage(id);

        // Check for bad things
        if (!std::filesystem::exists(referenceInfoPath) ||
            !detections || detections->empty() || !refSet.size() || !imagePtr) {
            wxMessageDialog* errorDialog = new wxMessageDialog(NULL,
                wxT("An Error Occurred. Reference path may be invalid or Image ID is invalid or has 0 associated detections."),
                wxT("Warning"), wxICON_EXCLAMATION);
            errorDialog->ShowModal();
            return;
        }

        m_comparisonImage = *imagePtr;

        m_comparisonDetectionCount->SetValue(wxString::Format(wxT("%i"), int(detections->size())));
        m_referenceDetectionCount->SetValue(wxString::Format(wxT("%i"), int(refSet.size())));

        int originalRefSetSize = refSet.size(); // for use in accuracy calculation, since we remove elements while comparing.

        int numMatches(0);
        ReferenceInfo detRef;
        for (auto& det : *detections) {
            const TItemTypes::TItem* catItem = m_corePtr->getCatalogItem(det.catalogItem);
            if (!catItem)
                continue;
            detRef = ReferenceInfo(catItem->getName(), det.imageLoc);
            auto itr = refSet.find(detRef);

            if (itr == refSet.end()) {
                // This location is NOT in our reference set.
                addToDiscrepancyGrid(detRef);
                continue;
            }

            // This location IS in our reference set.
            if (itr->name != detRef.name)
                addToDiscrepancyGrid(detRef); // If the names are different it is a discrepancy.
            else
                numMatches++;

            refSet.erase(itr); // remove from reference set

        }

        // Add any remaining items to incorrectDetectionsGrid
        for (auto& ref : refSet) {
            addToDiscrepancyGrid(ref);
        }

        m_numberDifferencesOutput->SetValue(wxString::Format(wxT("%i"), m_discrepancyGrid->GetNumberRows()));

        //accuracy =  1- (# incorrect / (||A|| + ||B|| - ||A intersect B||) )
        float accuracy = 100* (1 - ((float)m_discrepancyGrid->GetNumberRows() / (float)(detections->size() + originalRefSetSize - numMatches)));
        m_accuracyOutput->SetValue((std::to_string(accuracy) + "%"));
    }

    void DetectionValidatorWidget::OnSave(wxCommandEvent& evt) {
        TUI::SaveFileDialog saveDialog = TUI::SaveFileDialog(".csv",this, wxID_ANY, "Save New Reference");
        if (saveDialog.ShowModal() == wxID_CANCEL)
            return;

        std::filesystem::path path = saveDialog.getFilePath();

        if (!std::filesystem::exists(path.parent_path()) || path.extension().string() != ".csv") {
            wxMessageDialog* badPathDialog = new wxMessageDialog(NULL,
                wxT("Warning: " + path.parent_path().string() + " Path does not exist"), wxT("Warning"));
            badPathDialog->ShowModal();
            return;
        }
        saveDetectionInfo(std::stoi(m_imageIDSelector->GetTextValue().ToStdString()), path);
    }

    void DetectionValidatorWidget::saveDetectionInfo(imageID id, const std::filesystem::path& path) {

        // Format is Name,x1-y1-x2-y2
        const std::vector<TItemSupport::DetectionResult>* detections = m_corePtr->getDetectionResults(id);

        if (detections) {
            std::ofstream file;
            file.open(path, std::ios::out);
            if (!file.is_open())
                return;
            file << "[Reference Info]\n";

            std::string out;
            for (auto& det : *detections) {

                out = m_corePtr->getCatalogItem(det.catalogItem)->getName();

                out += ("," + std::to_string(det.imageLoc.first.x) + "-"
                    + std::to_string(det.imageLoc.first.y) + "-"
                    + std::to_string(det.imageLoc.second.x) + "-"
                    + std::to_string(det.imageLoc.second.y) + "\n");
                file << out;
            }
            // clean up
            file.close();

        }


    }

    // This is one hell of a mouthfull.. 
    std::unordered_set<DetectionValidatorWidget::ReferenceInfo, DetectionValidatorWidget::ReferenceInfo_hash> DetectionValidatorWidget::loadReferenceInfo(const std::filesystem::path& path) {

        // Format is Name,x1-y1-x2-y2
        auto retSet = std::unordered_set<DetectionValidatorWidget::ReferenceInfo, DetectionValidatorWidget::ReferenceInfo_hash>();
        std::ifstream in;
        std::string line;
        std::vector<std::string> fields;
        std::vector<std::string> locStrings;
        ReferenceInfo refInfo;
        in.open(path);
        if (!in.is_open())
            return retSet;

        std::getline(in, line);
        if (line != "[Reference Info]")
            return retSet;

        while (std::getline(in, line)) {
            fields.clear();
            locStrings.clear();
            try {
                TDataTypes::splitString(line, ',', fields);
                refInfo.name = fields.at(0);
                TDataTypes::splitString(fields.at(1), '-', locStrings);
                if (locStrings.size() < 4)
                    continue;

                refInfo.location = std::make_pair(cv::Point(std::stoi(locStrings[0]), std::stoi(locStrings[1])),
                    cv::Point(std::stoi(locStrings[2]), std::stoi(locStrings[3])));

                retSet.insert(refInfo);
            }
            catch (std::invalid_argument) {}
            catch (std::out_of_range) {}

        }

        return retSet;
    }

    void DetectionValidatorWidget::addToDiscrepancyGrid(const ReferenceInfo& refInfo) {
        int row = m_discrepancyGrid->GetNumberRows();
        m_discrepancyGrid->AppendRows(1);
        m_discrepancyGrid->SetCellValue(row, m_columnIndexMap.at("Name"), refInfo.name);

        cv::Mat fmtImage = formatImage(m_comparisonImage(cv::Range(refInfo.location.first.y, refInfo.location.second.y),
            cv::Range(refInfo.location.first.x, refInfo.location.second.x)), m_imageMaxRows, m_imageMaxCols);

        m_discrepancyGrid->SetCellRenderer(row, m_columnIndexMap.at("Image"),
            new ImageGridCellRenderer(wxImage(fmtImage.cols, fmtImage.rows, fmtImage.data, true)));

        m_discrepancyGrid->SetCellValue(row, m_columnIndexMap.at("Loc"), locToString(refInfo.location));

    }

    void DetectionValidatorWidget::clearDiscrepancyGrid() {
        if (m_discrepancyGrid->GetNumberRows())
            m_discrepancyGrid->DeleteRows(0, m_discrepancyGrid->GetNumberRows());
    }

    std::string DetectionValidatorWidget::locToString(const std::pair<cv::Point, cv::Point>& loc) {
        return "(" + std::to_string(loc.first.x) + "," + std::to_string(loc.first.y) + ")\n" +
            "(" + std::to_string(loc.second.x) + "," + std::to_string(loc.second.y) + ")";
    }


    DetectionValidatorWidget::ReferenceInfo::ReferenceInfo(const std::string& name, const std::pair<cv::Point, cv::Point>& loc)
        : name(name), location(loc) { }

}