#include <UI/UISupport.h>




namespace TUI {



    /* OutputPanel display results of detections from active images.
    */
    class OutputPanel : public wxPanel, public TEvent::TObserver {

    public:
        OutputPanel(TCore* core, wxWindow* parent,
            wxWindowID 	id = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long 	style = wxTAB_TRAVERSAL,
            const wxString& name = wxPanelNameStr

        );


        // Populate Output list with information from imageID;
        void addImageInfo(imageID id);

        // Removes information from imageID;
        void removeImageInfo(imageID id);

        // Resets output list and repopulates it with detections from all activated images.
        void refreshOutputList();

        // Clear everything from output list
        void clearOutputList();

        void OnToggleCollapse(wxCommandEvent& evt);

        void OnToggleActiveOnly(wxCommandEvent& evt);

        virtual void TEventReceived(TEvent::TEvent e) override;

    private:

        void addItemToOutputList(const TItemSupport::DetectionResult* item, int row, int count = 1);
        void addItemToOutputList(const TDataTypes::dcID id, int row, int count = 1);

        void removeItemsFromOutputList(const std::vector<TDataTypes::dcID>& ids);
        void removeItemsFromOutputList(const std::vector<TItemSupport::DetectionResult>* dets);

        wxString makeFleaString(const TItemTypes::TItem* itm, int count);
        wxString makeTraderString(const TItemTypes::TItem* itm, int count);

        // Update all item counts using the count map.
        void updateCounts();

        void addToCountMap(imageID id);
        void removeFromCountMap(imageID id);

        // Returns true if item is new, false if it was already present.
        bool addToCountMap(const TItemSupport::DetectionResult& det);
        // Returns true if counter for that item reaches 0, false if count remains > 0.
        bool removeFromCountMap(const TItemSupport::DetectionResult& det);

        void addToTotalCurrency(const TItemSupport::DetectionResult& det);
        void removeFromTotalCurrency(const TItemSupport::DetectionResult& det);

        // Determines the best price from a TItem and returns a pointer to its TCurrency
        const TDataTypes::TCurrency* determineBestCurrency(const TItemTypes::TItem& catItem);

        // Swap the values of two rows in the outputList
        void swapListRows(int rowA, int rowB);

        void updateTotalValueDisplay();

        // Pointer to the core object.
        TCore* m_coreptr;


        // Maps column name to column index in the outputList.
        static const std::map<std::string, int> m_columnIndexMap;
        static const int m_imageMaxRows = 64;

        static const int m_imageMaxCols = 128;

        wxGrid* m_outputList;


        // Tool bar widgets.
        wxToolBar* m_toolbar;
        wxCheckBox* m_toggleCollapse;
        wxCheckBox* m_toggleActiveOnly;

        // Total Value display box
        wxTextCtrl* m_totalValueBox;
        std::vector<TDataTypes::TCurrency> m_totalCurrencyValues; // Store value for each currency type.
        // When true, detections with identical parents will be collapsed into one row with a count >=1.
        bool m_collapseSimilarItems;

        // When true, only active images will have their detections displayed.
        bool m_showActiveOnly;

        //std::map<const TItemTypes::TItem*, int> m_itemNameCountmap;
        std::unordered_map<TDataTypes::dcID, int> m_itemIDCountMap;


    };
}