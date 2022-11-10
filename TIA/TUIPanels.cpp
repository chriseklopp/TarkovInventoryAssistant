#include "TUIPanels.h"


namespace TUI {
    OutputPanel::OutputPanel(TCore* core, wxWindow* parent,
        wxWindowID 	id,
        const wxPoint& pos,
        const wxSize& size,
        long 	style,
        const wxString& name) : m_coreptr(core), m_collapseSimilarItems(false), wxPanel(parent,
            id,
            pos,
            size,
            style,
            name) {




        this->SetBackgroundColour(wxColor(100, 200, 200));

        // Add output list
        m_outputList = new wxGrid(this, wxUSE_ANY);
        m_outputList->CreateGrid(0, m_columnIndexMap.size());
        m_outputList->HideRowLabels();
        for (auto& c : m_columnIndexMap) {
            m_outputList->SetColLabelValue(c.second, c.first);
        }

        m_outputList->SetColLabelSize(20);
        m_outputList->SetDefaultRowSize(m_imageMaxRows);
        m_outputList->SetColSize(m_columnIndexMap.at("CatalogImage"), m_imageMaxCols);
        m_outputList->SetColSize(m_columnIndexMap.at("SourceImage"), m_imageMaxCols);

        m_outputList->SetColSize(m_columnIndexMap.at("Name"), 240);
        m_outputList->EnableEditing(false);


        auto sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(m_outputList, 1, wxALL | wxEXPAND, 0);
        this->SetSizerAndFit(sizer);

    };

    void OutputPanel::populateOutputList() {
        // Populates outputList based on the newest info from the core.
        m_itemNameCountmap.clear();


        if (m_collapseSimilarItems) {
            // Populate collapsed list.
            populateCountMap();
            for (auto p : m_itemNameCountmap) {
                addItemToOutputList(p.first, p.second);
            }
        }
        else{
            // Populate uncollapsed list.
            for (int id : m_coreptr->getLoadedImageIDs()) {
                for (auto& det : m_coreptr->getDetectionResults(id)) {
                    addItemToOutputList(&det, 1);
                }
            }
        }

    };



    void OutputPanel::populateCountMap() {

        // TODO: Should be dependent on where we want all images or not.
        for (int id : m_coreptr->getLoadedImageIDs()) {

            for (auto& det : m_coreptr->getDetectionResults(id)) {

                if (m_itemNameCountmap.find(&det) != m_itemNameCountmap.end()) {
                    m_itemNameCountmap.at(&det)++;
                }
                else {
                    m_itemNameCountmap.insert({&det ,1});
                }
            }
        }

    };

    void OutputPanel::addItemToOutputList(const TItemSupport::DetectionResult* det, int count=1) {
        if (!det)
            return;
        m_outputList->AppendRows(1);
        std::string countString = std::to_string(count);
        int row = m_outputList->GetNumberRows()-1;
        const std::unique_ptr<TItemTypes::TItem>& item = det->inputItem;

        m_outputList->SetCellValue(row, m_columnIndexMap.at("Name"), det->catalogItem->getName());
        m_outputList->SetCellValue(row, m_columnIndexMap.at("Dim"), det->catalogItem->getDimAsString());
        m_outputList->SetCellValue(row, m_columnIndexMap.at("Count"), countString);

        // Add source image
        cv::Mat fmtSourceImage = formatItemImage(item.get(), m_imageMaxRows, m_imageMaxCols);
        m_outputList->SetCellRenderer(row, m_columnIndexMap.at("SourceImage"),
            new ImageGridCellRenderer(wxImage(fmtSourceImage.cols, fmtSourceImage.rows, fmtSourceImage.data, true)));

        // Add catalog image
        cv::Mat fmtCatImage = formatItemImage(det->catalogItem, m_imageMaxRows, m_imageMaxCols);
        m_outputList->SetCellRenderer(row, m_columnIndexMap.at("CatalogImage"),
            new ImageGridCellRenderer(wxImage(fmtCatImage.cols, fmtCatImage.rows, fmtCatImage.data, true)));

    };
    void OutputPanel::TEventReceived(TEvent::TEventEnum e) {
        switch (e) {

        case TEvent::TEventEnum::ImageDeleted:
        case TEvent::TEventEnum::ImageAdded:
            populateOutputList();
            break;
        }
    };




    const std::map<std::string, int> OutputPanel::m_columnIndexMap = {
       {"CatalogImage",  0},
       {"Count", 1},
       {"Name",   2},
       {"Dim", 3},
       {"SourceImage", 4}

    };

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



        this->SetBackgroundColour(wxColor(0, 0, 200));
        wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

        m_imageScrollList = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
        m_imageScrollList->HideRowLabels();
        m_imageScrollList->CreateGrid(0, 1);
        m_imageScrollList->EnableEditing(false);



        sizer->Add(m_imageScrollList, 1, wxALL | wxEXPAND, 0);

        m_imagePanel = new ImagePanel(core, this);

        sizer->Add(m_imagePanel, 1, wxALL | wxEXPAND, 0);
        this->SetSizerAndFit(sizer);

    };

    void DisplayPanel::populateImageScrollList() {
        clearImageScrollList();

        std::vector<int> idVect = m_coreptr->getLoadedImageIDs();
        for (int id : idVect) {
            cv::Mat const* image = m_coreptr->getImage(id);

            m_imageScrollList->AppendRows(1);
            int row = m_imageScrollList->GetNumberRows() - 1;
            m_imageScrollList->SetCellRenderer(row, 0,
                new ImageGridCellRenderer(wxImage(image->cols, image->rows, image->data, true)));
        }

    }

    void DisplayPanel::clearImageScrollList() {
        if (m_imageScrollList->GetNumberRows())
            m_imageScrollList->DeleteRows(0, m_imageScrollList->GetNumberRows());
    }


    void DisplayPanel::TEventReceived(TEvent::TEventEnum e) {
        switch (e) {

        case TEvent::TEventEnum::ImageDeleted:
        case TEvent::TEventEnum::ImageAdded:
            populateImageScrollList();
            break;
        }
    }


    ConsolePanel::ConsolePanel(TCore* core, wxWindow* parent,
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



        this->SetBackgroundColour(wxColor(50, 50, 0));



    };

    void ConsolePanel::TEventReceived(TEvent::TEventEnum e) {

    }


    CatalogPanel::CatalogPanel(TCore* core, wxWindow* parent,
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



        this->SetBackgroundColour(wxColor(0, 100, 100));

        auto sizer = new wxBoxSizer(wxVERTICAL);


        m_searchBar = new wxSearchCtrl(this, wxUSE_ANY, "");
        m_searchBar->SetMinSize(wxSize(0, 25));
        m_searchBar->Bind(wxEVT_SEARCH, &CatalogPanel::OnDisplaySearch, this);
        sizer->Add(m_searchBar, 0, wxALL | wxEXPAND | wxRIGHT | wxLEFT, 0);


        m_catalogDisplay = new wxGrid(this, wxUSE_ANY);
        m_catalogDisplay->CreateGrid(0, m_columnIndexMap.size());
        // TODO: Use the column info struct (when i make it)
        m_catalogDisplay->HideRowLabels();
        m_catalogDisplay->SetColLabelValue(m_columnIndexMap.at("Name"), "Name");
        m_catalogDisplay->SetColLabelValue(m_columnIndexMap.at("Image"), "Image");
        m_catalogDisplay->SetColLabelValue(m_columnIndexMap.at("Dim"), "Dim");
        m_catalogDisplay->SetColLabelSize(20);
        m_catalogDisplay->SetDefaultRowSize(m_imageMaxRows);
        m_catalogDisplay->SetColSize(m_columnIndexMap.at("Image"), m_imageMaxCols);
        m_catalogDisplay->SetColSize(m_columnIndexMap.at("Name"), 240);
        m_catalogDisplay->EnableEditing(false);
        //m_catalogDisplay->AutoSize();
        //m_catalogDisplay->SetColMinimalWidth(m_columnIndexMap.at("Name"), 160);
        sizer->Add(m_catalogDisplay, 1, wxALL | wxEXPAND, 0);
        //sizer->Add(m_catalogDisplay, 4, wxALL | wxEXPAND, 0);


        this->SetSizerAndFit(sizer);
    };



    void CatalogPanel::populateCatalogDisplay() {

        clearCatalogDisplay();

        m_catalogDisplay->AppendRows(m_coreptr->getCatalogItemList().size());
        int currentRow = 0;

        for (auto& item : m_coreptr->getCatalogItemList()) {
            addItemToCatalogDisplay(item.get(), currentRow);
            currentRow++;
        }

    };

    const std::map<std::string, int> CatalogPanel::m_columnIndexMap = {
       {"Image",  0},
       {"Name", 1},
       {"Dim",   2},
    };


    void CatalogPanel::addItemToCatalogDisplay(TItemTypes::TItem* item, int row) {
        if (!item)
            return;
        m_catalogDisplay->SetCellValue(row, m_columnIndexMap.at("Name"), item->getName());
        m_catalogDisplay->SetCellValue(row, m_columnIndexMap.at("Dim"), item->getDimAsString());

        cv::Mat fmtImage = formatItemImage(item,m_imageMaxRows,m_imageMaxCols);
        m_catalogDisplay->SetCellRenderer(row, m_columnIndexMap.at("Image"),
            new ImageGridCellRenderer(wxImage(fmtImage.cols, fmtImage.rows, fmtImage.data, true)));
    };


    void CatalogPanel::applyFilters() {
        for (int i = 0; i < m_catalogDisplay->GetNumberRows(); i++) {


            // Apply name filter
            std::string name = m_catalogDisplay->GetCellValue(i, m_columnIndexMap.at("Name"));
            std::transform(name.begin(), name.end(), name.begin(), ::tolower); // lower for case insensitive compare

            if (!(name.find(m_nameFilter) != std::string::npos)) {
                m_catalogDisplay->HideRow(i);
                continue;
            }
            m_catalogDisplay->ShowRow(i);
            // Apply other filters...
        }
    };

    void CatalogPanel::OnDisplaySearch(wxCommandEvent& ev) {
        m_nameFilter = ev.GetString();
        std::transform(m_nameFilter.begin(), m_nameFilter.end(), m_nameFilter.begin(), ::tolower); // lower for case insensitive compare
        applyFilters();
    };

    void CatalogPanel::clearCatalogDisplay() {
        if (m_catalogDisplay->GetNumberRows())
            m_catalogDisplay->DeleteRows(0, m_catalogDisplay->GetNumberRows());
    };


    void CatalogPanel::TEventReceived(TEvent::TEventEnum e) {
        switch (e) {

        case TEvent::TEventEnum::CatalogChanged:
            populateCatalogDisplay();
            break;
        }
    }


    SettingsDialog::SettingsDialog(TCore* core, wxWindow* parent,
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
        m_catalogSelect->SetInitialDirectory(m_coreptr->getConfigPtr()->getCATALOGS_DIR().string());
        m_catalogSelect->SetPath(m_coreptr->getConfigPtr()->getACTIVE_CATALOG().string());
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



    void ImagePanel::paintEvent(wxPaintEvent& evt)
    {
        // depending on your system you may need to look at double-buffered dcs
        wxPaintDC dc(this);
        render(dc);
    }


    void ImagePanel::paintNow()
    {
        // depending on your system you may need to look at double-buffered dcs
        wxClientDC dc(this);
        render(dc);
    }

    void ImagePanel::render(wxDC& dc)
    {
        int neww, newh;
        dc.GetSize(&neww, &newh);

        wxImage* image = m_drawDetections ? &m_sourceImageWithdetections : &m_sourceImage;

        if (neww != m_imWidth || newh != m_imHeight)
        {
            m_imageResized = wxBitmap(image->Scale(neww, newh /*, wxIMAGE_QUALITY_HIGH*/));
            m_imWidth = neww;
            m_imHeight = newh;
            dc.DrawBitmap(m_imageResized, 0, 0, false);
        }
        else {
            dc.DrawBitmap(m_imageResized, 0, 0, false);
        }
    }

    void ImagePanel::OnSize(wxSizeEvent& event) {
        Refresh();
        //skip the event.
        event.Skip();
    }


    void ImagePanel::setActiveImage(imageID id) {
        auto image = m_coreptr->getImage(id);
        if (!image)
            return;
        m_sourceImage = wxImage(image->cols, image->rows, image->data, true);
        makeImageWithDetections(id);
    }

    void ImagePanel::showDetections(bool draw) {
        m_drawDetections = draw;
    }

    void ImagePanel::makeImageWithDetections(imageID id) {

        const std::vector<TItemSupport::DetectionResult>& detRes = m_coreptr->getDetectionResults(id);
        wxBitmap bm = wxBitmap(m_sourceImage);
        wxMemoryDC dc = wxMemoryDC(bm);

        for (auto& det : detRes) {
            std::pair<cv::Point,cv::Point> locs = det.imageLoc;
            dc.SetPen(wxPen(wxColour('red'), 5, wxSOLID));
            dc.SetBrush(wxBrush(wxColour('red'), wxTRANSPARENT));
            dc.DrawRectangle(locs.first.x, locs.first.y, locs.second.x, locs.second.y);
        }
        m_sourceImageWithdetections = bm.ConvertToImage();
    }


    cv::Mat formatItemImage(const TItemTypes::TItem* item, int maxRows, int maxCols) {

        cv::Mat im;
        cv::cvtColor(item->getImage(), im, cv::COLOR_BGR2RGB);


        // Scale down the image, preserving aspect ratio.
        int numRows = maxRows;
        double scale_percent = numRows * 100 / im.rows;
        int numCols = im.cols * scale_percent / 100;

        // If the item is too long, base off max cols instead.
        if (numCols > maxCols) {
            numCols = maxCols;
            scale_percent = numCols * 100 / im.cols;
            numRows = im.rows * scale_percent / 100;
        }

        cv::Mat imresized;
        cv::resize(im, imresized, cv::Size(numCols, numRows), (0, 0), (0, 0), cv::INTER_AREA);
        return imresized;
    };
}