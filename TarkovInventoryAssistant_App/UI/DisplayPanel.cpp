#include <UI/DisplayPanel.h>


namespace TUI {

    DisplayPanel::DisplayPanel(TCore* core, wxWindow* parent,
        wxWindowID 	id,
        const wxPoint& pos,
        const wxSize& size,
        long 	style,
        const wxString& name) : m_coreptr(core), wxPanel(parent,
            id,
            pos,
            size,
            style,
            name) {



        this->SetBackgroundColour(wxColor(59, 57, 57));

        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

        // Tool bar widgets.
        m_toolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL);
        wxString m_modeSelectChoices[] = { wxT("Screenshot mode"), wxT("Feed Mode (NYI)") };
        int m_modeSelectNChoices = sizeof(m_modeSelectChoices) / sizeof(wxString);
        m_modeSelect = new wxChoice(m_toolbar, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_modeSelectNChoices, m_modeSelectChoices, 0);
        m_modeSelect->SetSelection(0);
        m_toolbar->AddControl(m_modeSelect);

        m_staticLine = new wxStaticLine(m_toolbar, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
        m_toolbar->AddControl(m_staticLine);
        m_toggleDetections = new wxCheckBox(m_toolbar, wxID_ANY, wxT("Show detections"), wxDefaultPosition, wxDefaultSize, 0);
        m_toolbar->AddControl(m_toggleDetections);
        m_toolbar->Realize();

        m_toggleDetections->Bind(wxEVT_CHECKBOX, &DisplayPanel::OnToggleDetections, this);
        m_modeSelect->Bind(wxEVT_CHOICE, &DisplayPanel::OnModeSelect, this);

        sizer->Add(m_toolbar, 0, wxEXPAND, 5);


        // Main panel widgets.
        wxBoxSizer* contentSizer = new wxBoxSizer(wxHORIZONTAL);
        m_imageScrollList = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
        m_imageScrollList->HideRowLabels();
        m_imageScrollList->HideColLabels();
        m_imageScrollList->CreateGrid(0, 1, wxGrid::wxGridSelectionModes::wxGridSelectRows);
        m_imageScrollList->EnableEditing(false);
        m_imageScrollList->SetDefaultRowSize(m_scrollListMaxRows);
        m_imageScrollList->SetColSize(0, m_scrollListMaxCols);
        m_imageScrollList->DisableColResize(0);
        m_imageScrollList->DisableDragRowSize();
        m_imageScrollList->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
        m_imageScrollList->SetDefaultCellBackgroundColour(wxColor(20, 20, 20));
        m_imageScrollList->SetMargins(0, 2);

        m_imageScrollList->Bind(wxEVT_GRID_CELL_LEFT_CLICK, &DisplayPanel::OnImageSelect, this);
        m_imageScrollList->Bind(wxEVT_GRID_CELL_RIGHT_CLICK, &DisplayPanel::OnImageRightClick, this);


        // Image list header widgets
        wxBoxSizer* imageListSizer = new wxBoxSizer(wxVERTICAL);
        wxBoxSizer* imageListHeader = new wxBoxSizer(wxHORIZONTAL);

        m_addDirBtn = new wxButton(this, wxID_ANY, wxT("Add Dir"), wxDefaultPosition, wxDefaultSize, 0);
        imageListHeader->Add(m_addDirBtn, 1, wxEXPAND, 5);
        m_addDirBtn->Bind(wxEVT_BUTTON, &DisplayPanel::OnAddDirBtn, this);


        m_addImageBtn = new wxButton(this, wxID_ANY, wxT("Add Image"), wxDefaultPosition, wxDefaultSize, 0);
        imageListHeader->Add(m_addImageBtn, 1, wxEXPAND, 5);
        m_addImageBtn->Bind(wxEVT_BUTTON, &DisplayPanel::OnAddImageBtn, this);



        // m_imageDirSelect->SetInitialDirectory(m_coreptr->getConfigPtr()->getDATA_DIR().string());
         //m_imageDirSelect->SetPath(m_coreptr->getConfigPtr()->getDATA_DIR().string());


        imageListSizer->Add(imageListHeader, 0, wxUP | wxEXPAND, 2);
        imageListSizer->Add(m_imageScrollList, 1, wxEXPAND, 5);


        contentSizer->Add(imageListSizer, 0, wxEXPAND | wxALIGN_LEFT, 5);

        // Add main image display panel
        m_imagePanel = new ImagePanel(core, this);
        m_imagePanel->showDetections(m_toggleDetections->IsChecked());

        contentSizer->Add(m_imagePanel, 1, wxALL | wxEXPAND, 0);

        m_imageScrollListRCMenu = new TUI::DisplayPanel::RightClickMenu();
        wxMenuItem* removeImageItm = new wxMenuItem(m_imageScrollListRCMenu, wxID_REMOVE_IMAGE,
            wxString(wxT("Remove Image")), wxEmptyString, wxITEM_NORMAL);
        m_imageScrollListRCMenu->Append(removeImageItm);
        m_imageScrollListRCMenu->Bind(wxEVT_MENU, &DisplayPanel::OnRightClickMenuClicked, this);

        sizer->Add(contentSizer, 1, wxALL | wxEXPAND, 0);
        this->SetSizerAndFit(sizer);

    };

    void DisplayPanel::populateImageScrollList() {
        clearImageScrollList();

        std::vector<int> idVect = m_coreptr->getLoadedImageIDs();
        for (int id : idVect) {
            cv::Mat const* image = m_coreptr->getImage(id);

            m_imageScrollList->AppendRows(1);
            int row = m_imageScrollList->GetNumberRows() - 1;
            wxString cellVal = wxString::Format(wxT("%i"), id);
            m_imageScrollList->SetCellValue(row, 0, cellVal);
            cv::Mat fmtImage = formatImage(*image, m_scrollListMaxRows, m_scrollListMaxCols);
            m_imageScrollList->SetCellRenderer(row, 0,
                new ImageGridCellRenderer(wxImage(fmtImage.cols, fmtImage.rows, fmtImage.data, true)));
        }

    }

    void DisplayPanel::clearImageScrollList() {
        if (m_imageScrollList->GetNumberRows())
            m_imageScrollList->DeleteRows(0, m_imageScrollList->GetNumberRows());
    }

    void DisplayPanel::OnImageSelect(wxGridEvent& evt) {
        wxString imID = m_imageScrollList->GetCellValue(wxGridCellCoords(evt.GetRow(), evt.GetCol()));
        if (wxAtoi(imID) == m_selectedImageID)
            return;
        m_imagePanel->setActiveImage(wxAtoi(imID));

        m_coreptr->deactivateImage(m_selectedImageID);
        m_selectedImageID = wxAtoi(imID);
        m_imagePanel->paintNow();
        m_coreptr->activateImage(wxAtoi(imID));
        evt.Skip();
    }

    void DisplayPanel::OnImageRightClick(wxGridEvent& evt) {
        wxString imID = m_imageScrollList->GetCellValue(wxGridCellCoords(evt.GetRow(), evt.GetCol()));
        m_imageScrollListRCMenu->setImageId(wxAtoi(imID));
        PopupMenu(m_imageScrollListRCMenu);
    }

    void DisplayPanel::OnToggleDetections(wxCommandEvent& evt) {
        m_imagePanel->showDetections(evt.GetInt() == 1);
    }

    void DisplayPanel::OnModeSelect(wxCommandEvent& evt) {
        //NYI: Implement handlign of this when feed mode is actually implemented.
    }

    void DisplayPanel::OnRightClickMenuClicked(wxCommandEvent& evt) {
        switch (evt.GetId()) {
        case TUI::wxIDS::wxID_REMOVE_IMAGE:
            m_coreptr->deleteImage(m_imageScrollListRCMenu->getImageId());
        }
    }

    void DisplayPanel::OnAddDirBtn(wxCommandEvent& evt) {
        if (m_previousDirSelect.empty())
            m_previousDirSelect = m_coreptr->getConfigPtr()->getDATA_DIR();

        wxDirDialog dirSelect = new wxDirDialog(this, wxT("Select a folder"));
        dirSelect.SetPath(m_previousDirSelect.string());

        if (dirSelect.ShowModal() == wxID_CANCEL)
            return;

        std::filesystem::path selectedPath = std::filesystem::path(dirSelect.GetPath().ToStdString());
        m_previousDirSelect = selectedPath.parent_path();
        m_coreptr->loadImagesInDir(selectedPath);
    }

    void DisplayPanel::OnAddImageBtn(wxCommandEvent& evt) {
        if (m_previousImageSelect.empty())
            m_previousImageSelect = m_coreptr->getConfigPtr()->getDATA_DIR();

        wxFileDialog imageSelect = new wxFileDialog(this, wxT("Select an image"));
        imageSelect.SetWildcard("(*.bmp; *.png;*.jpg)");
        imageSelect.SetPath(m_previousImageSelect.string());

        if (imageSelect.ShowModal() == wxID_CANCEL)
            return;

        std::filesystem::path selectedPath = std::filesystem::path(imageSelect.GetPath().ToStdString());
        m_previousImageSelect = selectedPath;
        m_coreptr->loadImage(selectedPath);
    }

    void DisplayPanel::TEventReceived(TEvent::TEvent e) {
        switch (e.getType()) {

        case TEvent::TEventEnum::ImagesCleared:
        case TEvent::TEventEnum::ImageDeleted:
            if (std::stoi(e.getData()) == m_imagePanel->getActiveImage())
                m_imagePanel->clearDisplay();
        case TEvent::TEventEnum::ImageAdded:
            populateImageScrollList();
            break;
        }
    }

}