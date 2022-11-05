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
            for (auto& det : m_coreptr->getDetectionResults()) {
                addItemToOutputList(&det, 1);
            }
        }

    };
    void OutputPanel::populateCountMap() {
        for (auto& det : m_coreptr->getDetectionResults()) {


            if (m_itemNameCountmap.find(&det) != m_itemNameCountmap.end()) {
                m_itemNameCountmap.at(&det)++;
            }
            else {
                m_itemNameCountmap.insert({&det ,1});
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


    };

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

        populateCatalogDisplay();

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


    SettingsPanel::SettingsPanel(TCore* core, wxWindow* parent,
        wxWindowID 	id,
        const wxPoint& pos,
        const wxSize& size,
        long 	style,
        const wxString& name

    ) : m_coreptr(core), wxPanel(parent,
        id,
        pos,
        size,
        style,
        name) {



        wxBoxSizer* bSizer1;
        bSizer1 = new wxBoxSizer(wxVERTICAL);

        m_headerText = new wxStaticText(this, wxID_ANY, wxT("Settings"), wxDefaultPosition, wxDefaultSize, 0);
        m_headerText->Wrap(-1);
        bSizer1->Add(m_headerText, 0, wxALL, 5);

        m_headerline = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
        bSizer1->Add(m_headerline, 0, wxEXPAND | wxALL, 5);

        m_dataDirText = new wxStaticText(this, wxID_ANY, wxT("Data Directory"), wxDefaultPosition, wxDefaultSize, 0);
        m_dataDirText->Wrap(-1);
        bSizer1->Add(m_dataDirText, 0, 0, 5);

        m_dataDirSelect = new wxDirPickerCtrl(this, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE);
        bSizer1->Add(m_dataDirSelect, 0, wxALL, 5);

        m_compiledCatalogText = new wxStaticText(this, wxID_ANY, wxT("Compiled Catalog"), wxDefaultPosition, wxDefaultSize, 0);
        m_compiledCatalogText->Wrap(-1);
        bSizer1->Add(m_compiledCatalogText, 0, wxALL, 5);

        m_catalogSelect = new wxDirPickerCtrl(this, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE);
        bSizer1->Add(m_catalogSelect, 0, wxALL, 5);


        bSizer1->Add(0, 0, 1, wxEXPAND, 5);

        wxBoxSizer* bSizer4;
        bSizer4 = new wxBoxSizer(wxHORIZONTAL);


        bSizer4->Add(0, 0, 1, wxEXPAND, 5);

        m_cancelButton = new wxButton(this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
        bSizer4->Add(m_cancelButton, 0, wxALL, 5);

        m_saveButton = new wxButton(this, wxID_ANY, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0);
        bSizer4->Add(m_saveButton, 0, wxALL, 5);


        bSizer1->Add(bSizer4, 1, wxEXPAND, 5);


        this->SetSizer(bSizer1);
        this->Layout();


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