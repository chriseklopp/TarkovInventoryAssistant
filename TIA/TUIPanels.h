

#pragma once
//#include <wx/msw/winundef.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/fileconf.h>
#include <wx/splitter.h>
#include <wx/statline.h>
#include <wx/listctrl.h>
#include <wx/srchctrl.h>
#include <wx/popupwin.h>
#include <wx/filepicker.h>
#include <wx/grid.h>
#include "TItemTypes.h"
#include "TDataTypes.h"
#include "TCore.h"


/*
Contains definitions for UI panels and dialogs.

*/

namespace TUI {

    enum wxIDS {
        wxID_REMOVE_IMAGE = wxID_LAST +1
    };

    /* Image panel displays a selected image or a stream output.
    * Can be toggled to draw any associated detections on itself.
    */
    class ImagePanel : public wxPanel
    {

    public:


        ImagePanel(TCore* core, wxWindow* parent) :
            m_coreptr(core),
            m_imageID(-1),
            m_drawDetections(false),
            wxPanel(parent) {
        
            this->Bind(wxEVT_PAINT, &ImagePanel::paintEvent, this);
            this->Bind(wxEVT_SIZE, &ImagePanel::OnSize, this);
            paintNow();
        };


        void paintEvent(wxPaintEvent& evt);
        void paintNow();
        void OnSize(wxSizeEvent& event);

        // Render the image on the screen.
        // Force render forces the image to render regardless of the 
        // circumstances.
        void render(wxDC& dc, bool forceRender=false);


        void setActiveImage(imageID id);

        const imageID& getActiveImage() { return m_imageID; }


        void clearDisplay();

        // Toggles image to display/hide detected items.
        void showDetections(bool draw);


    private:

        void makeImageWithDetections(imageID id);

        wxBitmap m_imageResized;

        int m_imWidth;
        int m_imHeight;

        TCore* m_coreptr;

        bool m_drawDetections;

        int m_imageID;

        wxImage m_sourceImage;

        // This image is identical to the source image but has detections drawn on it.
        // This will be rendered instead of the source image if m_drawDetections is true.
        wxImage m_sourceImageWithdetections;

    };




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

        void addItemToOutputList(const TItemSupport::DetectionResult* item, int count=1);
        void addItemToOutputList(const TDataTypes::dcID id, int count=1);

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

    // DisplayPanel displays and allows management of loaded images.
    class DisplayPanel : public wxPanel, public TEvent::TObserver {

    public:
        DisplayPanel(TCore* core, wxWindow* parent,
            wxWindowID 	id = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long 	style = wxTAB_TRAVERSAL,
            const wxString& name = wxPanelNameStr

        );

        void populateImageScrollList();


        void clearImageScrollList();

        void tempSelected() { m_imagePanel->setActiveImage(0); };


        void OnImageSelect(wxGridEvent& evt);

        void OnImageRightClick(wxGridEvent& evt);

        void OnToggleDetections(wxCommandEvent& evt);

        void OnModeSelect(wxCommandEvent& evt);

        void OnRightClickMenuClicked(wxCommandEvent& evt);

        void OnAddDirBtn(wxCommandEvent& evt);
        void OnAddImageBtn(wxCommandEvent& evt);

        virtual void TEventReceived(TEvent::TEvent e) override;


        class RightClickMenu : public wxMenu {
        public:
            RightClickMenu() : m_imageID(-1) {};
            void setImageId(imageID id) { m_imageID = id; };
            const imageID& getImageId() { return m_imageID; };

        private:

            imageID m_imageID;
        };

    private:

        // Pointer to the core object.
        TCore* m_coreptr;

        static const int m_scrollListMaxRows = 120;
        static const int m_scrollListMaxCols = 260;



        // Tool bar widgets.
        wxToolBar* m_toolbar;
        wxChoice* m_modeSelect;
        wxStaticLine* m_staticLine;
        wxCheckBox* m_toggleDetections;

        //Main UI widgets
        wxGrid* m_imageScrollList;
        ImagePanel* m_imagePanel;


        // Image list header widgets
        wxButton* m_addDirBtn;
        wxButton* m_addImageBtn;
        
        RightClickMenu* m_imageScrollListRCMenu;
        // TODO: feed mode widgets if any.

        imageID m_selectedImageID;

        std::filesystem::path m_previousDirSelect;
        std::filesystem::path m_previousImageSelect;

    };


    /* ConsolePanel acts as a text console that outputs events as they happen.
    */
    class ConsolePanel : public wxPanel, public TEvent::TObserver {

    public:
        ConsolePanel(TCore* core, wxWindow* parent,
            wxWindowID 	id = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long 	style = wxTAB_TRAVERSAL,
            const wxString& name = wxPanelNameStr

        );


        virtual void TEventReceived(TEvent::TEvent e) override;

        // Pointer to the core object.
        TCore* m_coreptr;

        wxTextCtrl* m_consoleOutput;

    };

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
        // TODO: Make some sort of column info struct.
        static const std::map<std::string, int> m_columnIndexMap;


        // Pointer to the core object.
        TCore* m_coreptr;

    };


    class SettingsDialog : public wxDialog
    {

    public:
        SettingsDialog(TCore* core, wxWindow* parent,
            wxWindowID id = wxID_ANY,
            const wxString& title = wxT("Settings"),
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxSize(552, 240),
            long style = wxDEFAULT_DIALOG_STYLE);


    private:

        void OnSave(wxCommandEvent& evt);
        void OnCancel(wxCommandEvent& evt);

        wxStaticText* m_headerText;
        wxStaticLine* m_headerline;
        wxStaticText* m_dataDirText;
        wxDirPickerCtrl* m_dataDirSelect;
        wxStaticText* m_compiledCatalogText;
        wxDirPickerCtrl* m_catalogSelect;
        wxButton* m_cancelButton;
        wxButton* m_saveButton;

        // Pointer to the core object.
        TCore* m_coreptr;

    };




    class ImageGridCellRenderer : public wxGridCellStringRenderer
    {

    public:
        ImageGridCellRenderer(wxImage image) : m_image(image, -1) {}

        virtual void Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected)
        {
            wxGridCellStringRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);
            dc.DrawBitmap(m_image, rect.x, rect.y);
        }

    private:
        wxBitmap m_image;
    };


    struct columnProperties {
        int width;
        int height;
    };

    // Formats an item image so it looks proper in the display.
    // Preserving aspect ratio.
    cv::Mat formatImage(const cv::Mat image, int maxRows, int maxCols);

    // Resize Image preserving aspect ratio.
    // Will return an empty image if max values given are impossibly small
    wxImage resizeImage(wxImage& im, int maxRows, int maxCols);



}