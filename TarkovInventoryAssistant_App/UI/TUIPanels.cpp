#include <UI/TUIPanels.h>



namespace TUI {

    SettingsDialog::SettingsDialog(TAppInterface* core, wxWindow* parent,
        wxWindowID id,
        const wxString& titl,
        const wxPoint& pos,
        const wxSize& size,
        long style)

        : m_coreptr(core), wxDialog(parent,
            id,
            titl,
            pos,
            size,
            style) {

        wxBoxSizer* bSizer1;
        bSizer1 = new wxBoxSizer(wxVERTICAL);

        m_headerline = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
        bSizer1->Add(m_headerline, 0, wxEXPAND | wxALL, 5);

        m_dataDirText = new wxStaticText(this, wxID_ANY, wxT("Data Directory"), wxDefaultPosition, wxDefaultSize, 0);
        m_dataDirText->Wrap(-1);
        bSizer1->Add(m_dataDirText, 0, 0, 5);

        m_dataDirSelect = new wxDirPickerCtrl(this, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE);
        m_dataDirSelect->SetInitialDirectory(m_coreptr->getConfigPtr()->getROOT_DIR().string());
        m_dataDirSelect->SetPath(m_coreptr->getConfigPtr()->getDATA_DIR().string());
        bSizer1->Add(m_dataDirSelect, 0, wxEXPAND | wxALL, 5);

        m_compiledCatalogText = new wxStaticText(this, wxID_ANY, wxT("Compiled Catalog"), wxDefaultPosition, wxDefaultSize, 0);
        m_compiledCatalogText->Wrap(-1);
        bSizer1->Add(m_compiledCatalogText, 0, wxALL, 5);

        m_catalogSelect = new wxDirPickerCtrl(this, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE);
        m_catalogSelect->SetPath(m_coreptr->getConfigPtr()->getACTIVE_CATALOG().string());
        m_catalogSelect->SetInitialDirectory(m_coreptr->getConfigPtr()->getCATALOGS_DIR().string());
        bSizer1->Add(m_catalogSelect, 0, wxEXPAND | wxALL, 5);


        bSizer1->Add(0, 0, 1, wxEXPAND, 5);

        wxBoxSizer* bSizer4;
        bSizer4 = new wxBoxSizer(wxHORIZONTAL);


        bSizer4->Add(0, 0, 1, wxEXPAND, 5);

        m_cancelButton = new wxButton(this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
        bSizer4->Add(m_cancelButton, 0, wxALL, 5);

        m_saveButton = new wxButton(this, wxID_OK, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0);
        bSizer4->Add(m_saveButton, 0, wxALL, 5);


        bSizer1->Add(bSizer4, 1, wxEXPAND, 5);


        this->SetSizer(bSizer1);
        this->Layout();


        Bind(wxEVT_BUTTON, &SettingsDialog::OnSave, this, wxID_OK);
        //Bind(wxEVT_BUTTON, &SettingsDialog::OnCancel, this, wxID_CANCEL);
    }


    void SettingsDialog::OnSave(wxCommandEvent& evt) {
        // Set our selections through the core.
        m_coreptr->setDATA_DIR(m_dataDirSelect->GetTextCtrlValue().ToStdString());
        m_coreptr->setACTIVECATALOG(m_catalogSelect->GetTextCtrlValue().ToStdString());
        m_coreptr->saveConfig();
        Close();
    };
    void SettingsDialog::OnCancel(wxCommandEvent& evt) {

    };

    CompileCatalogDialog::CompileCatalogDialog(TAppInterface* core, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
        : m_coreptr(core), wxDialog(parent,
            id,
            title,
            pos,
            size,
            style) {

        this->SetSizeHints(wxDefaultSize, wxDefaultSize);

        wxBoxSizer* sizer;
        sizer = new wxBoxSizer(wxVERTICAL);


        m_headerLine = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
        sizer->Add(m_headerLine, 0, wxEXPAND | wxALL, 5);


        m_rawCatalogPathText = new wxStaticText(this, wxID_ANY, wxT("Raw Catalog Path"), wxDefaultPosition, wxDefaultSize, 0);
        m_rawCatalogPathText->Wrap(-1);
        sizer->Add(m_rawCatalogPathText, 0, wxUP | wxLEFT, 5);


        m_rawCatalogPathSelect = new wxDirPickerCtrl(this, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE);
        m_rawCatalogPathSelect->SetInitialDirectory(m_coreptr->getConfigPtr()->getRAW_CATALOGS_DIR().string());
        sizer->Add(m_rawCatalogPathSelect, 1, wxEXPAND | wxALL, 5);


        m_optionLine = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
        sizer->Add(m_optionLine, 0, wxEXPAND | wxALL, 5);


        m_optionSizer = new wxStaticBoxSizer(wxHORIZONTAL, this);

        wxBoxSizer* nameSizer;
        nameSizer = new wxBoxSizer(wxVERTICAL);


        m_catalogNameText = new wxStaticText(m_optionSizer->GetStaticBox(), wxID_ANY, wxT("Catalog Name"), wxDefaultPosition, wxDefaultSize, 0);
        m_catalogNameText->Wrap(-1);
        nameSizer->Add(m_catalogNameText, 1, wxUP | wxLEFT, 5);



        m_catalogNameSelect = new wxTextCtrl(m_optionSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
        nameSizer->Add(m_catalogNameSelect, 1, wxEXPAND | wxALL, 5);

        m_optionSizer->Add(nameSizer, 2, wxEXPAND, 5);

        wxBoxSizer* m_checkboxOptionSizer;
        m_checkboxOptionSizer = new wxBoxSizer(wxVERTICAL);


        m_checkboxOptionSizer->Add(0, 0, 1, wxEXPAND, 5);

        m_toggleGenerateRotations = new wxCheckBox(m_optionSizer->GetStaticBox(), wxID_ANY, wxT("Generate Rotations"), wxDefaultPosition, wxDefaultSize, 0);
        m_checkboxOptionSizer->Add(m_toggleGenerateRotations, 0, wxALL, 5);


        m_optionSizer->Add(m_checkboxOptionSizer, 1, wxEXPAND, 5);

        m_optionSizer->GetStaticBox()->Disable();


        sizer->Add(m_optionSizer, 1, wxEXPAND, 5);

        m_footerLine = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
        sizer->Add(m_footerLine, 0, wxEXPAND | wxALL, 5);

        wxBoxSizer* footerSizer;
        footerSizer = new wxBoxSizer(wxHORIZONTAL);


        footerSizer->Add(0, 0, 1, wxEXPAND, 5);

        m_cancelButton = new wxButton(this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
        footerSizer->Add(m_cancelButton, 0, wxLEFT | wxRIGHT, 5);

        m_compileButton = new wxButton(this, wxID_OK, wxT("Compile"), wxDefaultPosition, wxDefaultSize, 0);
        footerSizer->Add(m_compileButton, 0, wxLEFT | wxRIGHT, 5);
        m_compileButton->Disable();

        sizer->Add(footerSizer, 1, wxEXPAND);

        Bind(wxEVT_BUTTON, &CompileCatalogDialog::OnCompile, this, wxID_OK);
        m_rawCatalogPathSelect->Bind(wxEVT_DIRPICKER_CHANGED, &CompileCatalogDialog::OnRawPathSelected, this, wxID_ANY);
        m_toggleGenerateRotations->Bind(wxEVT_CHECKBOX, &CompileCatalogDialog::OnToggleRotations, this);

        this->SetSizer(sizer);
        this->Layout();

        this->Centre(wxBOTH);
    }

    void CompileCatalogDialog::OnCompile(wxCommandEvent& evt) {

        // Send our request to the core.
        std::filesystem::path rawpath = m_rawCatalogPathSelect->GetTextCtrlValue().ToStdString();
        std::string name = m_catalogNameSelect->GetValue().ToStdString();
        bool makeRotations = m_toggleGenerateRotations->GetValue();

        // Check if path exists and warn it will be overrwritten.
        std::filesystem::path candidatePath = m_coreptr->getConfigPtr()->getCATALOGS_DIR() / name;
        if (std::filesystem::exists(candidatePath)) {
            // Open pop up that existing catalog will be overritten.
            wxMessageDialog* pathExistsDialog = new wxMessageDialog(NULL,
                wxT("Warning: " + candidatePath.string() + " already exists.\nWould you like to overrite it? "), wxT("Warning"), wxYES_NO | wxNO_DEFAULT | wxICON_EXCLAMATION);
            int res = pathExistsDialog->ShowModal();
            if (res == wxID_NO)
                return;
        }

        m_coreptr->compileRawCatalog(rawpath, name, makeRotations);

        Close();
    }

    void CompileCatalogDialog::OnCancel(wxCommandEvent& evt) {

    }


    void CompileCatalogDialog::OnRawPathSelected(wxFileDirPickerEvent& evt) {
        m_rawCatalogPathSelect->Unbind(wxEVT_DIRPICKER_CHANGED, &CompileCatalogDialog::OnRawPathSelected, this, wxID_ANY);
        m_compileButton->Enable();
        m_optionSizer->GetStaticBox()->Enable();
        std::filesystem::path p = m_rawCatalogPathSelect->GetTextCtrlValue().ToStdString();
        m_catalogNameSelect->SetValue("ItemCatalog_" + p.filename().string());
    }

    void CompileCatalogDialog::OnToggleRotations(wxCommandEvent& evt) {
        wxString currentName = m_catalogNameSelect->GetValue();
        if (evt.IsChecked()) {
            currentName += "_wRotations";
            m_catalogNameSelect->SetValue(currentName);
        }
        else {
            size_t idx = currentName.find("_wRotations");
            if (idx != wxNOT_FOUND) {
                currentName.erase(idx);
                m_catalogNameSelect->SetValue(currentName);
            }
        }

    }


}