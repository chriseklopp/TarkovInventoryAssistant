#include <UI/UISupport.h>
#include <wx/srchctrl.h>
namespace TUI {


    /* CatalogPanel displays the contents of the currently loaded catalog.
   */
    class CatalogPanel : public wxPanel, public TEvent::TObserver {

    public:
        CatalogPanel(TCore* core, wxWindow* parent,
            wxWindowID 	id = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long 	style = wxTAB_TRAVERSAL,
            const wxString& name = wxPanelNameStr

        );

        //void filterType(std::string filter);
        virtual void TEventReceived(TEvent::TEvent e) override;

    private:

        void applyFilters();

        // Repopulates the catalog display from our catalog data.
        // Respected any active filters.
        void populateCatalogDisplay();

        void addItemToCatalogDisplay(const TItemTypes::TItem* item, int row);

        void clearCatalogDisplay();

        void OnDisplaySearch(wxCommandEvent& ev);


        static const int m_imageMaxRows = 75;

        static const int m_imageMaxCols = 128;


        // Handles searching the catalog display
        wxSearchCtrl* m_searchBar;

        wxGrid* m_catalogDisplay;

        // Active name filter.
        std::string m_nameFilter;

        // Maps column name to column index in the catalog display.
        static const std::map<std::string, int> m_columnIndexMap;


        // Pointer to the core object.
        TCore* m_coreptr;

    };


}