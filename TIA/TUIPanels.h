

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

    class OutputPanel : public wxPanel {

    public:
        OutputPanel(TCore* core, wxWindow* parent,
            wxWindowID 	id = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long 	style = wxTAB_TRAVERSAL,
            const wxString& name = wxPanelNameStr

        );

        void populateOutputList();

        void addItemToOutputList(const TItemSupport::DetectionResult* item, int count);




    private:

        void populateCountMap();


        // Maps column name to column index in the outputList.
        static const std::map<std::string, int> m_columnIndexMap;
        static const int m_imageMaxRows = 64;

        static const int m_imageMaxCols = 128;

        wxGrid* m_outputList;

        // When true, detections with identical parents will be collapsed into one row with a count >=1.
        bool m_collapseSimilarItems;

        std::map<const TItemSupport::DetectionResult*, int> m_itemNameCountmap;
        // Pointer to the core object.
        TCore* m_coreptr;

    };


    class DisplayPanel : public wxPanel {

    public:
        DisplayPanel(TCore* core, wxWindow* parent,
            wxWindowID 	id = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long 	style = wxTAB_TRAVERSAL,
            const wxString& name = wxPanelNameStr

        );

        // Pointer to the core object.
        TCore* m_coreptr;
    };
    class ConsolePanel : public wxPanel {

    public:
        ConsolePanel(TCore* core, wxWindow* parent,
            wxWindowID 	id = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long 	style = wxTAB_TRAVERSAL,
            const wxString& name = wxPanelNameStr

        );

        // Pointer to the core object.
        TCore* m_coreptr;

        // List that displays loaded images as a sidebar.
        wxGrid* m_imageList;

    };
    class CatalogPanel : public wxPanel {

    public:
        CatalogPanel(TCore* core, wxWindow* parent,
            wxWindowID 	id = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long 	style = wxTAB_TRAVERSAL,
            const wxString& name = wxPanelNameStr

        );

        // Filter the catalog display by name.
        void setNameFilter(std::string filter);

        //void filterType(std::string filter);

    private:

        void applyFilters();

        // Repopulates the catalog display from our catalog data.
        // Respected any active filters.
        void populateCatalogDisplay();

        void addItemToCatalogDisplay(TItemTypes::TItem* item, int row);

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



    /* Image panel displays a selected image or a stream output.
    * Can be toggled to draw any associated detections on itself.
    */
    class ImagePanel : public wxPanel
    {

    public:


        ImagePanel(TCore* core, wxFrame* parent) :
            m_coreptr(core),
            wxPanel(parent) {};


        void paintEvent(wxPaintEvent& evt);
        void paintNow();
        void OnSize(wxSizeEvent& event);
        void render(wxDC& dc);


        void setActiveImage(imageID id);

        // Toggles image to display/hide detected items.
        void showDetections(bool draw);




    private:

        void makeImageWithDetections(imageID id);

        wxBitmap m_imageResized;

        int m_imWidth;
        int m_imHeight;

        TCore* m_coreptr;

        bool m_drawDetections;


        wxImage m_sourceImage;

        // This image is identical to the source image but has detections drawn on it.
        // This will be rendered instead of the source image if m_drawDetections is true.
        wxImage m_sourceImageWithdetections;

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
    cv::Mat formatItemImage(const TItemTypes::TItem* item, int maxRows, int maxCols);





}