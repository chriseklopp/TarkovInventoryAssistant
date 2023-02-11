
#pragma once
//#include <wx/msw/winundef.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


#include <wx/grid.h>
#include <wx/filepicker.h>
#include <Interfaces/TAppInterface.h>


namespace TUI {

    enum wxIDS {
        wxID_REMOVE_IMAGE = wxID_LAST + 1
    };

    /* Image panel displays a selected image or a stream output.
   * Can be toggled to draw any associated detections on itself.
   */
    class ImagePanel : public wxPanel
    {

    public:


        ImagePanel(TAppInterface* core, wxWindow* parent) :
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
        void render(wxDC& dc, bool forceRender = false);


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

        TAppInterface* m_coreptr;

        bool m_drawDetections;

        int m_imageID;

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


    /*
    * Dialog that allows a user to select a path and a file name.
    * Extension will be added to the end of the file name.
    * The path and filename will be combined and the user is responsible for getting it and using it.
    */
    class SaveFileDialog : public wxDialog
    {

    public:

        SaveFileDialog(const std::string& extension, wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Save File"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(301, 182), long style = wxDEFAULT_DIALOG_STYLE);

        ~SaveFileDialog();

        std::filesystem::path getFilePath();

    protected:
        wxStaticText* m_directoryText;
        wxDirPickerCtrl* m_directoryPicker;
        wxStaticText* m_fileNameText;
        wxTextCtrl* m_fileNameBox;
        wxButton* m_saveButton;
        wxButton* m_cancelButton;

    private:

        void OnSave(wxCommandEvent& evt);
        std::string m_fileName;
        std::string m_path;
        std::string m_extension;
        bool m_saved;
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