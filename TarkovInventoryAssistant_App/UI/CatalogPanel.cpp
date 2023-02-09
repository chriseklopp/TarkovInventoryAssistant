#include <UI/CatalogPanel.h>

namespace TUI {

    // Catalog display column ordering
    const std::map<std::string, int> CatalogPanel::m_columnIndexMap = {
   {"Image",  0},
   {"Name", 1},
   {"Dim",   2},
   {"FleaPrice", 3},
   {"TraderPrice", 4}
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

        m_catalogDisplay->HideRowLabels();


        for (auto& c : m_columnIndexMap) {
            m_catalogDisplay->SetColLabelValue(c.second, c.first);
        }

        m_catalogDisplay->SetColLabelSize(20);
        m_catalogDisplay->SetDefaultRowSize(m_imageMaxRows);
        m_catalogDisplay->SetColSize(m_columnIndexMap.at("Image"), m_imageMaxCols);
        m_catalogDisplay->SetColSize(m_columnIndexMap.at("Name"), 240);
        m_catalogDisplay->EnableEditing(false);


        m_catalogDisplay->SetDefaultCellBackgroundColour(wxColor(20, 20, 20));
        m_catalogDisplay->SetDefaultCellTextColour(wxColor(255, 255, 255));
        m_catalogDisplay->SetDefaultCellFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        m_catalogDisplay->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
        m_catalogDisplay->SetGridLineColour(wxColour(73, 81, 84));

        sizer->Add(m_catalogDisplay, 1, wxALL | wxEXPAND, 0);



        this->SetSizerAndFit(sizer);
    };



    void CatalogPanel::populateCatalogDisplay() {

        clearCatalogDisplay();

        m_catalogDisplay->AppendRows(m_coreptr->getCatalogItemList().size());
        int currentRow = 0;

        for (const TDataTypes::dcID& id : m_coreptr->getCatalogItemList()) {
            const TItemTypes::TItem* itm = m_coreptr->getCatalogItem(id);
            if (!itm || itm->isRotated())
                continue;
            addItemToCatalogDisplay(itm, currentRow);
            currentRow++;
        }

        // Clean up extra rows we may have created due to excluded items.

        if (currentRow <= m_catalogDisplay->GetNumberRows() - 1)
            m_catalogDisplay->DeleteRows(currentRow, m_catalogDisplay->GetNumberRows() - currentRow);
    };



    void CatalogPanel::addItemToCatalogDisplay(const TItemTypes::TItem* item, int row) {
        if (!item)
            return;
        m_catalogDisplay->SetCellValue(row, m_columnIndexMap.at("Name"), item->getName());
        m_catalogDisplay->SetCellValue(row, m_columnIndexMap.at("Dim"), item->getDimAsString());

        wxString fleaString = wxString::FromUTF8(item->getPrice().getCurrencyString(true));
        if (!item->getPricePerSlot().getCurrencyString().empty())
            fleaString += "\n" + wxString::FromUTF8(item->getPricePerSlot().getCurrencyString(true));

        wxString traderString = wxString::FromUTF8(item->getTraderSellPrice().getCurrencyString(true)) + "\n" + item->getTrader();

        m_catalogDisplay->SetCellValue(row, m_columnIndexMap.at("FleaPrice"), fleaString);
        m_catalogDisplay->SetCellValue(row, m_columnIndexMap.at("TraderPrice"), traderString);


        cv::Mat fmtImage = formatImage(item->getImage(), m_imageMaxRows, m_imageMaxCols);
        m_catalogDisplay->SetCellRenderer(row, m_columnIndexMap.at("Image"),
            new ImageGridCellRenderer(wxImage(fmtImage.cols, fmtImage.rows, fmtImage.data, true)));
    };


    void CatalogPanel::applyFilters() {
        for (int i = 0; i < m_catalogDisplay->GetNumberRows(); i++) {


            // Apply name filter
            std::string name = m_catalogDisplay->GetCellValue(i, m_columnIndexMap.at("Name"));
            std::transform(name.begin(), name.end(), name.begin(), ::tolower); // lower for case insensitive compare

            if (!m_nameFilter.empty()) {
                if (!(name.find(m_nameFilter) != std::string::npos)) {
                    m_catalogDisplay->HideRow(i);
                    continue;
                }
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


    void CatalogPanel::TEventReceived(TEvent::TEvent e) {
        switch (e.getType()) {
        case TEvent::TEventEnum::CatalogChanged:
            populateCatalogDisplay();
            break;
        }
    }

}