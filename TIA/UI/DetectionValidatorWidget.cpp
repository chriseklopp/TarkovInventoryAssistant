#include <UI/DetectionValidatorWidget.h>
#include <wx/spinctrl.h>


namespace TUI {

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
        mainsizer->Add(m_referenceFileSelector, 0, wxALL, 5);

        m_imageIDText = new wxStaticText(this, wxID_ANY, wxT("Comparison Image ID"), wxDefaultPosition, wxDefaultSize, 0);
        m_imageIDText->Wrap(-1);
        mainsizer->Add(m_imageIDText, 0, wxALL, 5);

        m_imageIDSelector = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 9999, 0);
        mainsizer->Add(m_imageIDSelector, 0, wxALL, 5);

        m_compareButton = new wxToggleButton(this, wxID_ANY, wxT("Compare"), wxDefaultPosition, wxDefaultSize, 0);
        m_compareButton->SetValue(true);
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

        m_incorrectDetections = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);

        // Grid
        m_incorrectDetections->CreateGrid(5, 5);
        m_incorrectDetections->EnableEditing(true);
        m_incorrectDetections->EnableGridLines(true);
        m_incorrectDetections->EnableDragGridSize(false);
        m_incorrectDetections->SetMargins(0, 0);

        // Columns
        m_incorrectDetections->EnableDragColMove(false);
        m_incorrectDetections->EnableDragColSize(true);
        m_incorrectDetections->SetColLabelAlignment(wxALIGN_CENTER, wxALIGN_CENTER);

        // Rows
        m_incorrectDetections->EnableDragRowSize(true);
        m_incorrectDetections->SetRowLabelAlignment(wxALIGN_CENTER, wxALIGN_CENTER);

        // Label Appearance

        // Cell Defaults
        m_incorrectDetections->SetDefaultCellAlignment(wxALIGN_LEFT, wxALIGN_TOP);
        mainsizer->Add(m_incorrectDetections, 0, wxALL, 5);


        this->SetSizer(mainsizer);
        this->Layout();

        this->Centre(wxVERTICAL);
    }

    DetectionValidatorWidget::~DetectionValidatorWidget()
    {
    }
}