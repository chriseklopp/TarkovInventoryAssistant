#include <UI/OutputPanel.h>



namespace TUI {

    // Output display column ordering
    const std::map<std::string, int> OutputPanel::m_columnIndexMap = {
   {"CatalogImage",  0},
   {"Count", 1},
   {"Name",   2},
   {"Dim", 3},
   {"SourceImage", 4},
   {"FleaPrice", 5},
   {"TraderPrice", 6},
   {"CatalogItemId",7},
   {"ParentImageId", 8}
    };

    OutputPanel::OutputPanel(TAppInterface* core, wxWindow* parent,
        wxWindowID 	id,
        const wxPoint& pos,
        const wxSize& size,
        long 	style,
        const wxString& name) : m_coreptr(core), m_collapseSimilarItems(false), m_showActiveOnly(true),m_highlightThreshold(.80), wxPanel(parent,
            id,
            pos,
            size,
            style,
            name) {

        this->SetBackgroundColour(wxColor(100, 200, 200));

        auto sizer = new wxBoxSizer(wxVERTICAL);


        // Add header toolbar
        m_toolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL);

        m_toggleCollapse = new wxCheckBox(m_toolbar, wxID_ANY, wxT("Collapse Similar"),
            wxDefaultPosition, wxDefaultSize, 0);
        m_toggleActiveOnly = new wxCheckBox(m_toolbar, wxID_ANY, wxT("Show Active Only"),
            wxDefaultPosition, wxDefaultSize, 0);
        m_toggleCollapse->SetValue(m_collapseSimilarItems);
        m_toggleActiveOnly->SetValue(m_showActiveOnly);

        m_toolbar->AddControl(m_toggleCollapse);
        m_toolbar->AddControl(m_toggleActiveOnly);

        m_toggleCollapse->Bind(wxEVT_CHECKBOX, &OutputPanel::OnToggleCollapse, this);
        m_toggleActiveOnly->Bind(wxEVT_CHECKBOX, &OutputPanel::OnToggleActiveOnly, this);
        m_toolbar->Realize();

        sizer->Add(m_toolbar, 0, wxEXPAND, 5);

        // Add total value box
        m_totalValueBox = new wxTextCtrl(this, wxUSE_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
        sizer->Add(m_totalValueBox, 0, wxEXPAND, 5);

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
        m_outputList->SetDefaultCellBackgroundColour(wxColor(20, 20, 20));
        m_outputList->SetDefaultCellTextColour(wxColor(255, 255, 255));
        m_outputList->SetDefaultCellFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        m_outputList->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
        m_outputList->SetGridLineColour(wxColour(73, 81, 84));
        m_outputList->EnableEditing(false);


        m_outputList->SetColSize(m_columnIndexMap.at("CatalogItemId"), 0);
        m_outputList->SetColSize(m_columnIndexMap.at("ParentImageId"), 0);

        sizer->Add(m_outputList, 1, wxALL | wxEXPAND, 0);
        this->SetSizerAndFit(sizer);

    };


    void OutputPanel::addImageInfo(imageID id) {

        const std::vector<TItemSupport::DetectionResult>* res = m_coreptr->getDetectionResults(id);
        if (!res)
            return;
        if (m_collapseSimilarItems) {
            // Populate collapsed list.
            std::vector<TDataTypes::dcID> newItems;
            for (auto& det : *res) {
                if (addToCountMap(det))
                    newItems.push_back(det.catalogItem);
                addToTotalCurrency(det);
            }

            // Get previous final row.
            int row = m_outputList->GetNumberRows() - 1;
            m_outputList->AppendRows(newItems.size());

            // Append any new items to the outputList
            for (TDataTypes::dcID& itm : newItems) {
                row++;
                addItemToOutputList(itm, row, 1);
            }

            // Update counts of all existing items.
            updateCounts();
        }

        else {

            // Get previous final row.
            int row = m_outputList->GetNumberRows() - 1;

            m_outputList->AppendRows(res->size());

            for (auto& det : *res) {
                row++;
                addToCountMap(det);
                addToTotalCurrency(det);
                addItemToOutputList(&det, row, 1);
            }
        }
        updateTotalValueDisplay();
    }

    void OutputPanel::removeImageInfo(imageID id) {


        if (m_showActiveOnly) {
            const std::vector<imageID> activeIds = m_coreptr->getActivatedIDs();

            if (std::find(activeIds.begin(), activeIds.end(), id) == activeIds.end())
                return;

            if (activeIds.size() < 2) {
                clearOutputList(); // More efficient to clear everything if theres only one image.
                return;
            }
        }

        const std::vector<TItemSupport::DetectionResult>* res = m_coreptr->getDetectionResults(id);
        if (!res)
            return;

        if (m_collapseSimilarItems) {
            // Depopulate collapsed list.
            std::vector<TDataTypes::dcID> itemsToRemove;
            for (auto& det : *res) {
                if (removeFromCountMap(det))
                    itemsToRemove.push_back(det.catalogItem);
                removeFromTotalCurrency(det);
            }
            removeItemsFromOutputList(itemsToRemove);

            // Update counts of all existing items.
            updateCounts();
        }

        else {
            for (auto& det : *res) {
                removeFromCountMap(det);
                removeFromTotalCurrency(det);
            }
            removeItemsFromOutputList(res);
        }
        updateTotalValueDisplay();
    }

    void OutputPanel::refreshOutputList() {
        clearOutputList();

        if (m_showActiveOnly) {
            for (int id : m_coreptr->getActivatedIDs()) {
                addImageInfo(id);
            }
        }
        else {
            for (int id : m_coreptr->getLoadedImageIDs()) {
                addImageInfo(id);
            }
        }
        if (m_collapseSimilarItems)
            m_outputList->HideCol(m_columnIndexMap.at("SourceImage"));
        else
            m_outputList->ShowCol(m_columnIndexMap.at("SourceImage"));

    }
    void OutputPanel::clearOutputList() {
        if (m_outputList->GetNumberRows())
            m_outputList->DeleteRows(0, m_outputList->GetNumberRows());
        m_itemIDCountMap.clear();
        m_totalCurrencyValues.clear();
        updateTotalValueDisplay();

    }

    void OutputPanel::OnToggleCollapse(wxCommandEvent& evt) {
        m_collapseSimilarItems = (evt.GetInt() == 1);
        refreshOutputList();
    }

    void OutputPanel::OnToggleActiveOnly(wxCommandEvent& evt) {
        m_showActiveOnly = (evt.GetInt() == 1);
        refreshOutputList();
    }

    void OutputPanel::setHighlightThreshold(int thresh) {
        m_highlightThreshold = thresh;
    }

    bool OutputPanel::addToCountMap(const TItemSupport::DetectionResult& det) {
        if (m_itemIDCountMap.find(det.catalogItem) != m_itemIDCountMap.end()) {
            m_itemIDCountMap.at(det.catalogItem)++;
            return false;
        }
        else {
            m_itemIDCountMap.insert({ det.catalogItem, 1 });
            return true;
        }
    }

    bool OutputPanel::removeFromCountMap(const TItemSupport::DetectionResult& det) {
        if (m_itemIDCountMap.find(det.catalogItem) != m_itemIDCountMap.end()) {
            m_itemIDCountMap.at(det.catalogItem)--;
            if (m_itemIDCountMap.at(det.catalogItem) == 0) {
                m_itemIDCountMap.erase(det.catalogItem);
                return true;
            }
        }
        return false;
    }


    void OutputPanel::addToTotalCurrency(const TItemSupport::DetectionResult& det) {

        const TItemTypes::TItem* catItem = m_coreptr->getCatalogItem(det.catalogItem);

        const TDataTypes::TCurrency* currency = determineBestCurrency(*catItem);
        if (!currency)
            return;
        std::string detUnit = currency->getUnit();

        for (TDataTypes::TCurrency& cur : m_totalCurrencyValues) {
            if (cur.getUnit() == detUnit) {
                cur += *currency;
                return;
            }
        }
        // This is a new currency unit weve never seen before
        m_totalCurrencyValues.push_back(catItem->getPrice());
    }


    void OutputPanel::removeFromTotalCurrency(const TItemSupport::DetectionResult& det) {

        const TItemTypes::TItem* catItem = m_coreptr->getCatalogItem(det.catalogItem);

        const TDataTypes::TCurrency* currency = determineBestCurrency(*catItem);
        if (!currency)
            return;
        std::string detUnit = currency->getUnit();
        for (TDataTypes::TCurrency& cur : m_totalCurrencyValues) {
            if (cur.getUnit() == detUnit) {
                cur -= *currency;
                return;
            }
        }
    }

    void OutputPanel::applyTraderSellHighlight(int row, const TItemTypes::TItem* itm) {
        if (!itm)
            return;

        if (double(itm->getTraderSellPrice().getValue()) / double(itm->getPrice().getValue()) >= m_highlightThreshold) {
            m_outputList->SetCellBackgroundColour(row, m_columnIndexMap.at("FleaPrice"), wxColor(0, 200, 0));
            m_outputList->SetCellBackgroundColour(row, m_columnIndexMap.at("TraderPrice"), wxColor(0, 200, 0));
        }

    }

    const TDataTypes::TCurrency* OutputPanel::determineBestCurrency(const TItemTypes::TItem& catItem) {
        const TDataTypes::TCurrency* currency;

        const TDataTypes::TCurrency& fleaVal = catItem.getPrice();
        const TDataTypes::TCurrency& traderVal = catItem.getTraderSellPrice();

        if (fleaVal.getValue() != 0) {
            // If same units, use the maximum or else use flea
            if (fleaVal.getUnit() == traderVal.getUnit())
                currency = fleaVal.getValue() > traderVal.getValue() ? &fleaVal : &traderVal;
            else
                currency = &catItem.getPrice();
        }
        else { // If flea price is 0, use trader price instead
            currency = &catItem.getTraderSellPrice();
        }
        return currency;
    }

    void OutputPanel::addItemToOutputList(const TItemSupport::DetectionResult* det, int row, int count) {
        if (!det)
            return;

        const TItemTypes::TItem* catItem = m_coreptr->getCatalogItem(det->catalogItem);

        std::string countString = std::to_string(count);

        applyTraderSellHighlight(row, catItem);

        const std::unique_ptr<TItemTypes::TItem>& item = det->inputItem;

        m_outputList->SetCellValue(row, m_columnIndexMap.at("Name"), catItem->getName());
        m_outputList->SetCellValue(row, m_columnIndexMap.at("Dim"), catItem->getDimAsString());
        m_outputList->SetCellValue(row, m_columnIndexMap.at("Count"), countString);
        m_outputList->SetCellValue(row, m_columnIndexMap.at("CatalogItemId"), wxString::Format(wxT("%i"), det->catalogItem));
        m_outputList->SetCellValue(row, m_columnIndexMap.at("ParentImageId"), wxString::Format(wxT("%i"), det->parentImageID));

        // Add price info
        m_outputList->SetCellValue(row, m_columnIndexMap.at("FleaPrice"), makeFleaString(catItem, 1));
        m_outputList->SetCellValue(row, m_columnIndexMap.at("TraderPrice"), makeTraderString(catItem, 1));

        // Add source image
        cv::Mat fmtSourceImage = formatImage(item->getImage(), m_imageMaxRows, m_imageMaxCols);
        m_outputList->SetCellRenderer(row, m_columnIndexMap.at("SourceImage"),
            new ImageGridCellRenderer(wxImage(fmtSourceImage.cols, fmtSourceImage.rows, fmtSourceImage.data, true)));

        // Add catalog image
        cv::Mat fmtCatImage = formatImage(catItem->getImage(), m_imageMaxRows, m_imageMaxCols);
        m_outputList->SetCellRenderer(row, m_columnIndexMap.at("CatalogImage"),
            new ImageGridCellRenderer(wxImage(fmtCatImage.cols, fmtCatImage.rows, fmtCatImage.data, true)));

    };

    // TODO: make it more clear that this is for when m_collapseSimilar is true...
    void OutputPanel::addItemToOutputList(const TDataTypes::dcID id, int row, int count) {

        const TItemTypes::TItem* itm = m_coreptr->getCatalogItem(id);

        if (!itm)
            return;

        std::string countString = std::to_string(count);


        applyTraderSellHighlight(row, itm);

        m_outputList->SetCellValue(row, m_columnIndexMap.at("Name"), itm->getName());
        m_outputList->SetCellValue(row, m_columnIndexMap.at("Dim"), itm->getDimAsString());
        m_outputList->SetCellValue(row, m_columnIndexMap.at("Count"), countString);
        m_outputList->SetCellValue(row, m_columnIndexMap.at("CatalogItemId"), wxString::Format(wxT("%i"), id));
        m_outputList->SetCellValue(row, m_columnIndexMap.at("ParentImageId"), wxString::Format(wxT("%i"), -1)); // -1 because catalog images are not associated with a parent image.


        // Add price info
        m_outputList->SetCellValue(row, m_columnIndexMap.at("FleaPrice"), makeFleaString(itm, count));
        m_outputList->SetCellValue(row, m_columnIndexMap.at("TraderPrice"), makeTraderString(itm, count));


        // Add catalog image
        cv::Mat fmtCatImage = formatImage(itm->getImage(), m_imageMaxRows, m_imageMaxCols);
        m_outputList->SetCellRenderer(row, m_columnIndexMap.at("CatalogImage"),
            new ImageGridCellRenderer(wxImage(fmtCatImage.cols, fmtCatImage.rows, fmtCatImage.data, true)));

    };
    void OutputPanel::removeItemsFromOutputList(const std::vector<TDataTypes::dcID>& ids) {
        if (!ids.size())
            return;

        std::set<TDataTypes::dcID> idSet(ids.begin(), ids.end());

        int endIdx = m_outputList->GetNumberRows() - 1;
        int r = 0;
        while (r < endIdx) {
            TDataTypes::dcID rowCatID = wxAtoi(m_outputList->GetCellValue(r, m_columnIndexMap.at("CatalogItemId")));
            if (idSet.find(rowCatID) != idSet.end()) { // This is a row that must be removed
                swapListRows(r, endIdx); // Swap row to be removed to the back.
                endIdx--;
            }
            else { // This row does not need to be removed
                r++;
            }
        }

        int idxOfRemoval = endIdx + 1; // DeleteRows is start index inclusive.

        // Remove rows from the back
        if (idxOfRemoval <= m_outputList->GetNumberRows() - 1)
            m_outputList->DeleteRows(idxOfRemoval, m_outputList->GetNumberRows() - idxOfRemoval);

    }

    void OutputPanel::removeItemsFromOutputList(const std::vector<TItemSupport::DetectionResult>* dets) {
        if (!dets || !dets->size())
            return;

        int idToRemove = dets->at(0).parentImageID; // All dets should have the same parentImageID.

        int endIdx = m_outputList->GetNumberRows() - 1;
        int r = 0;
        while (r < endIdx) {
            imageID parentID = wxAtoi(m_outputList->GetCellValue(r, m_columnIndexMap.at("ParentImageId")));
            if (parentID == idToRemove) { // This is a row that must be removed
                swapListRows(r, endIdx); // Swap row to be removed to the back.
                endIdx--;
            }
            else { // This row does not need to be removed
                r++;
            }
        }

        int idxOfRemoval = endIdx + 1; // DeleteRows is start index inclusive.

        // Remove rows from the back
        if (idxOfRemoval <= m_outputList->GetNumberRows() - 1)
            m_outputList->DeleteRows(idxOfRemoval, m_outputList->GetNumberRows() - idxOfRemoval);
    }


    wxString OutputPanel::makeFleaString(const TItemTypes::TItem* itm, int count) {
        wxString fleaString = wxString::FromUTF8(itm->getPrice().getValueMultipledBy(count, true));

        if (!itm->getPricePerSlot().getCurrencyString().empty())
            fleaString += "\n" + wxString::FromUTF8(itm->getPricePerSlot().getValueMultipledBy(count, true));
        return fleaString;
    }

    wxString OutputPanel::makeTraderString(const TItemTypes::TItem* itm, int count) {
        return wxString::FromUTF8(itm->getTraderSellPrice().getValueMultipledBy(count, true)) + "\n" + itm->getTrader();
    }


    // Swap the values of two rows in the outputList
    void OutputPanel::swapListRows(int rowA, int rowB) {

        // Temp store values from rowA.
        std::vector<std::pair<int, wxString>> temp; // store column idx, and value from that col.
        for (auto& c : m_columnIndexMap) {
            temp.push_back(std::make_pair(c.second, m_outputList->GetCellValue(rowA, c.second)));
        }

        // Store image renders.
        wxGridCellRenderer* tempCatRender = m_outputList->GetCellRenderer(rowA, m_columnIndexMap.at("CatalogImage"));
        wxGridCellRenderer* tempSourceRender = m_outputList->GetCellRenderer(rowA, m_columnIndexMap.at("SourceImage"));


        // Copy values from rowB to rowA
        for (auto& c : m_columnIndexMap) {
            m_outputList->SetCellValue(rowA, c.second, m_outputList->GetCellValue(rowB, c.second));
        }
        // Copy image renderers.
        m_outputList->SetCellRenderer(rowA, m_columnIndexMap.at("CatalogImage"), m_outputList->GetCellRenderer(rowB, m_columnIndexMap.at("CatalogImage")));
        m_outputList->SetCellRenderer(rowA, m_columnIndexMap.at("SourceImage"), m_outputList->GetCellRenderer(rowB, m_columnIndexMap.at("SourceImage")));


        // Copy temp values to rowB
        for (auto& p : temp) {
            m_outputList->SetCellValue(rowB, p.first, p.second);
        }

        // Swap background colors.
        wxColour tempFlea = m_outputList->GetCellBackgroundColour(rowA, m_columnIndexMap.at("FleaPrice"));
        wxColour tempTrader = m_outputList->GetCellBackgroundColour(rowA, m_columnIndexMap.at("TraderPrice"));


        m_outputList->SetCellBackgroundColour(rowA, m_columnIndexMap.at("FleaPrice"), m_outputList->GetCellBackgroundColour(rowB, m_columnIndexMap.at("FleaPrice")));
        m_outputList->SetCellBackgroundColour(rowA, m_columnIndexMap.at("TraderPrice"), m_outputList->GetCellBackgroundColour(rowB, m_columnIndexMap.at("TraderPrice")));

        m_outputList->SetCellBackgroundColour(rowB, m_columnIndexMap.at("FleaPrice"), tempFlea);
        m_outputList->SetCellBackgroundColour(rowB, m_columnIndexMap.at("TraderPrice"), tempTrader);

        m_outputList->SetCellRenderer(rowB, m_columnIndexMap.at("CatalogImage"), tempCatRender);
        m_outputList->SetCellRenderer(rowB, m_columnIndexMap.at("SourceImage"), tempSourceRender);

    }

    void OutputPanel::updateTotalValueDisplay() {
        wxString displayString = "Total Value: ";
        for (TDataTypes::TCurrency& currency : m_totalCurrencyValues) {

            displayString += (wxString::FromUTF8(currency.getCurrencyString(true)) + "     ");
        }
        m_totalValueBox->Clear();
        m_totalValueBox->AppendText(displayString);
    }


    void OutputPanel::updateCounts() {
        int numRows = m_outputList->GetNumberRows();

        for (int row = 0; row < numRows - 1; row++) {
            TDataTypes::dcID id = wxAtoi(m_outputList->GetCellValue(row, m_columnIndexMap.at("CatalogItemId")));
            int count = (m_itemIDCountMap.find(id) != m_itemIDCountMap.end()) ? m_itemIDCountMap.at(id) : -1;
            m_outputList->SetCellValue(row, m_columnIndexMap.at("Count"), wxString::Format(wxT("%i"), count));

            const TItemTypes::TItem* itm = m_coreptr->getCatalogItem(id);
            if (!itm)
                return;

            // Update monetary counts.
            m_outputList->SetCellValue(row, m_columnIndexMap.at("FleaPrice"), makeFleaString(itm, count));
            m_outputList->SetCellValue(row, m_columnIndexMap.at("TraderPrice"), makeTraderString(itm, count));

        }

    }

    void OutputPanel::TEventReceived(TEvent::TEvent e) {

        switch (e.getType()) {

        case TEvent::TEventEnum::ImageAdded:
            if (m_showActiveOnly) {
                break;
            }
            else {
                addImageInfo(std::stoi(e.getData()));
                break;
            }

        case TEvent::TEventEnum::ImageActivated:
            if (m_showActiveOnly) {
                addImageInfo(std::stoi(e.getData()));
                break;
            }
            else {
                break;
            }

        case TEvent::TEventEnum::ImageDeactivated:
            if (m_showActiveOnly)
                removeImageInfo(std::stoi(e.getData()));
            else {
                break;
            }

        case TEvent::TEventEnum::ImageDeleted:
            removeImageInfo(std::stoi(e.getData()));
            break;

        case TEvent::TEventEnum::AllImagesDeactivated:
            clearOutputList();
            break;

        case TEvent::TEventEnum::CatalogChanged:
            refreshOutputList();
            break;
        }
    };

}