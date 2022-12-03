#include "TUIPanels.h"



namespace TUI {


    // Catalog display column ordering
    const std::map<std::string, int> CatalogPanel::m_columnIndexMap = {
   {"Image",  0},
   {"Name", 1},
   {"Dim",   2},
   {"FleaPrice", 3},
   {"TraderPrice", 4}
    };

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

    OutputPanel::OutputPanel(TCore* core, wxWindow* parent,
        wxWindowID 	id,
        const wxPoint& pos,
        const wxSize& size,
        long 	style,
        const wxString& name) : m_coreptr(core), m_collapseSimilarItems(false),m_showActiveOnly(true), wxPanel(parent,
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
        m_outputList->SetDefaultCellFont(wxFont(10, wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL));
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

        std::string detUnit = catItem->getPrice().getUnit();
        if (detUnit == "$") {
            std::cout << "wdawd";
        }
        for (TDataTypes::TCurrency& cur : m_totalCurrencyValues) {
            if (cur.getUnit() == detUnit) {
                cur += catItem->getPrice();
                return;
            }
        }
        // This is a new currency unit weve never seen before
        m_totalCurrencyValues.push_back(catItem->getPrice());
    }
   

    void OutputPanel::removeFromTotalCurrency(const TItemSupport::DetectionResult& det) {

        const TItemTypes::TItem* catItem = m_coreptr->getCatalogItem(det.catalogItem);

        std::string detUnit = catItem->getPrice().getUnit();
        for (TDataTypes::TCurrency& cur : m_totalCurrencyValues) {
            if (cur.getUnit() == detUnit){
                cur -= catItem->getPrice();
                return;
            }
        }
    }

    void OutputPanel::addItemToOutputList(const TItemSupport::DetectionResult* det, int row, int count) {
        if (!det)
            return;

        const TItemTypes::TItem* catItem = m_coreptr->getCatalogItem(det->catalogItem);

        std::string countString = std::to_string(count);

        const std::unique_ptr<TItemTypes::TItem>& item = det->inputItem;

        m_outputList->SetCellValue(row, m_columnIndexMap.at("Name"), catItem->getName());
        m_outputList->SetCellValue(row, m_columnIndexMap.at("Dim"), catItem->getDimAsString());
        m_outputList->SetCellValue(row, m_columnIndexMap.at("Count"), countString);
        m_outputList->SetCellValue(row, m_columnIndexMap.at("CatalogItemId"), wxString::Format(wxT("%i"), det->catalogItem));
        m_outputList->SetCellValue(row, m_columnIndexMap.at("ParentImageId"), wxString::Format(wxT("%i"), det->parentImageID));

        // Add price info
        m_outputList->SetCellValue(row, m_columnIndexMap.at("FleaPrice"), makeFleaString(catItem,1));
        m_outputList->SetCellValue(row, m_columnIndexMap.at("TraderPrice"), makeTraderString(catItem,1));

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


        m_outputList->SetCellValue(row, m_columnIndexMap.at("Name"), itm->getName());
        m_outputList->SetCellValue(row, m_columnIndexMap.at("Dim"), itm->getDimAsString());
        m_outputList->SetCellValue(row, m_columnIndexMap.at("Count"), countString);
        m_outputList->SetCellValue(row, m_columnIndexMap.at("CatalogItemId"), wxString::Format(wxT("%i"), id));
        m_outputList->SetCellValue(row, m_columnIndexMap.at("ParentImageId"), wxString::Format(wxT("%i"), -1)); // -1 because catalog images are not associated with a parent image.


        // Add price info
        m_outputList->SetCellValue(row, m_columnIndexMap.at("FleaPrice"), makeFleaString(itm,count));
        m_outputList->SetCellValue(row, m_columnIndexMap.at("TraderPrice"), makeTraderString(itm,count));


        // Add catalog image
        cv::Mat fmtCatImage = formatImage(itm->getImage(), m_imageMaxRows, m_imageMaxCols);
        m_outputList->SetCellRenderer(row, m_columnIndexMap.at("CatalogImage"),
            new ImageGridCellRenderer(wxImage(fmtCatImage.cols, fmtCatImage.rows, fmtCatImage.data, true)));

    };
    void OutputPanel::removeItemsFromOutputList(const std::vector<TDataTypes::dcID>& ids){
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

    void OutputPanel::removeItemsFromOutputList(const std::vector<TItemSupport::DetectionResult>* dets){
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


    void OutputPanel::updateCounts(){
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
        }
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
            wxString(wxT("Remove Image")),wxEmptyString, wxITEM_NORMAL);
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
        m_imagePanel->showDetections(evt.GetInt()==1);
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

    void DisplayPanel::OnAddDirBtn(wxCommandEvent& evt){
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

    void DisplayPanel::OnAddImageBtn(wxCommandEvent& evt){
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

        auto sizer = new wxBoxSizer(wxVERTICAL);
        m_consoleOutput = new wxTextCtrl(this, wxUSE_ANY,wxEmptyString,wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);

        m_consoleOutput->SetBackgroundColour(wxColour(20,20,20));
        m_consoleOutput->SetForegroundColour(wxColour(255, 255, 255));
        m_consoleOutput->SetFont(wxFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

        sizer->Add(m_consoleOutput, 1, wxEXPAND);
        m_consoleOutput->AppendText("------Tarkov Inventory Assistant-----\n");
        this->SetSizerAndFit(sizer);

    };

    void ConsolePanel::TEventReceived(TEvent::TEvent e) {

        switch (e.getType()) {

            case TEvent::TEventEnum::ImageAdded:
                m_consoleOutput->AppendText("Image Added: ID=" + e.getData() +
                    " Detections=" + std::to_string(m_coreptr->getDetectionResults(std::stoi(e.getData()))->size()) + "\n");
                break;

            case TEvent::TEventEnum::ImageDeleted:
                m_consoleOutput->AppendText("Image Deleted: ID=" + e.getData() + "\n");
                break;
            case TEvent::TEventEnum::ImageActivated:
                break;
            case TEvent::TEventEnum::ImageDeactivated:
                break;
            case TEvent::TEventEnum::AllImagesDeactivated:
                break;

            case TEvent::TEventEnum::ImagesCleared:
                m_consoleOutput->AppendText("Cleared all Images \n");
                break;
            case TEvent::TEventEnum::CatalogChanged:
                if (e.getStatus() == 1)
                    m_consoleOutput->AppendText("Successfully Loaded Catalog: " + e.getData() + "\n");
                else
                    m_consoleOutput->AppendText("Failed to load catalog: " + e.getData() + "\n");
                break;
                
        }

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
            addItemToCatalogDisplay(m_coreptr->getCatalogItem(id), currentRow);
            currentRow++;
        }

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


        cv::Mat fmtImage = formatImage(item->getImage(),m_imageMaxRows,m_imageMaxCols);
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
        render(dc,true);
    }

    void ImagePanel::render(wxDC& dc, bool forceRender)
    {
        int neww, newh;
        dc.GetSize(&neww, &newh);
        if (neww <=0 || newh <= 0)
            return;

        wxImage* image = m_drawDetections ? &m_sourceImageWithdetections : &m_sourceImage;
        if (!image->IsOk())
            return;

        if (forceRender || (neww != m_imWidth || newh != m_imHeight))
        {
            wxImage resizedIm = resizeImage(*image, newh, neww);
            if (!resizedIm.IsOk())
                return;

            m_imageResized = resizedIm;

            m_imWidth = neww;
            m_imHeight = newh;
            dc.DrawBitmap(m_imageResized, 0, 0, false);
        }
        else {
            dc.DrawBitmap(m_imageResized, 0, 0, false);
        }
    }

    void ImagePanel::clearDisplay() {
        m_imageID = -1;
        m_sourceImage = wxImage();
        m_sourceImageWithdetections = wxImage();
        Refresh();
    }



    void ImagePanel::OnSize(wxSizeEvent& event) {
        Refresh();
        //skip the event.
        event.Skip();
    }


    void ImagePanel::setActiveImage(imageID id) {
        if (id == m_imageID)
            return;
        auto image = m_coreptr->getImage(id);
        if (!image)
            return;
        m_imageID = id;

        cv::Mat fmtImage;
        cv::cvtColor(*image, fmtImage, cv::COLOR_BGR2RGB);

        m_sourceImage = wxImage(fmtImage.cols, fmtImage.rows, fmtImage.data, true).Copy();
        makeImageWithDetections(id);
    }

    void ImagePanel::showDetections(bool draw) {
        m_drawDetections = draw;
        paintNow();
    }

    void ImagePanel::makeImageWithDetections(imageID id) {

        const std::vector<TItemSupport::DetectionResult>* res = m_coreptr->getDetectionResults(id);
        if (!res)
            return;

        wxBitmap bm = wxBitmap(m_sourceImage);
        wxMemoryDC dc = wxMemoryDC(bm);

        for (auto& det : *res) {
            std::pair<cv::Point,cv::Point> locs = det.imageLoc;
            dc.SetPen(wxPen(wxColour(245,24,24), 2, wxSOLID));
            dc.SetBrush(wxBrush(wxColour(245, 24, 24), wxTRANSPARENT));
            dc.DrawRectangle(locs.first.x, locs.first.y, locs.second.x- locs.first.x,locs.second.y- locs.first.y);
        }
        m_sourceImageWithdetections = bm.ConvertToImage();
    }




    cv::Mat formatImage(const cv::Mat image, int maxRows, int maxCols) {
        cv::Mat im;
        cv::cvtColor(image, im, cv::COLOR_BGR2RGB);


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
    }


    wxImage resizeImage(wxImage& im, int maxRows, int maxCols) {
        
        // Scale down the image, preserving aspect ratio.
        int numRows = maxRows;
        double scale_percent = numRows * 100 / im.GetHeight();
        int numCols = im.GetWidth() * scale_percent / 100;

        // If the item is too long, base off max cols instead.
        if (numCols > maxCols) {
            numCols = maxCols;
            scale_percent = numCols * 100 / im.GetWidth();
            numRows = im.GetHeight() * scale_percent / 100;
        }


        if (numCols == 0 || numRows == 0)
            return wxImage();
        return wxImage(im.Scale(numCols, numRows /*, wxIMAGE_QUALITY_HIGH*/));
    }


}