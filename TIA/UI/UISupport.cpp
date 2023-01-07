#include <UI/UISupport.h>
#include <DataSupport/TItemTypes.h>
#include <DataSupport/TDataTypes.h>




namespace TUI {

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
        render(dc, true);
    }

    void ImagePanel::render(wxDC& dc, bool forceRender)
    {
        int neww, newh;
        dc.GetSize(&neww, &newh);
        if (neww <= 0 || newh <= 0)
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
            std::pair<cv::Point, cv::Point> locs = det.imageLoc;
            dc.SetPen(wxPen(wxColour(245, 24, 24), 2, wxSOLID));
            dc.SetBrush(wxBrush(wxColour(245, 24, 24), wxTRANSPARENT));
            dc.DrawRectangle(locs.first.x, locs.first.y, locs.second.x - locs.first.x, locs.second.y - locs.first.y);
        }
        m_sourceImageWithdetections = bm.ConvertToImage();
    }


    SaveFileDialog::SaveFileDialog(const std::string& extension, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : m_extension(extension), m_fileName("Unknown"), m_saved(false), wxDialog(parent, id, title, pos, size, style)
    {
        this->SetSizeHints(wxDefaultSize, wxDefaultSize);

        wxBoxSizer* mainSizer;
        mainSizer = new wxBoxSizer(wxVERTICAL);

        m_directoryText = new wxStaticText(this, wxID_ANY, wxT("Directory"), wxDefaultPosition, wxDefaultSize, 0);
        m_directoryText->Wrap(-1);
        mainSizer->Add(m_directoryText, 0, wxALL, 5);

        m_directoryPicker = new wxDirPickerCtrl(this, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE);
        mainSizer->Add(m_directoryPicker, 0, wxALL, 5);

        m_fileNameText = new wxStaticText(this, wxID_ANY, wxT("File Name"), wxDefaultPosition, wxDefaultSize, 0);
        m_fileNameText->Wrap(-1);
        mainSizer->Add(m_fileNameText, 0, wxALL, 5);

        m_fileNameBox = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
        mainSizer->Add(m_fileNameBox, 0, wxALL, 5);

        wxBoxSizer* buttonsizer;
        buttonsizer = new wxBoxSizer(wxHORIZONTAL);

        m_saveButton = new wxButton(this, wxID_OK, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0);
        m_saveButton->Bind(wxEVT_BUTTON, &SaveFileDialog::OnSave, this);

        buttonsizer->Add(m_saveButton, 0, wxALL, 5);

        m_cancelButton = new wxButton(this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
        buttonsizer->Add(m_cancelButton, 0, wxALL, 5);


        mainSizer->Add(buttonsizer, 1, wxEXPAND, 5);


        this->SetSizer(mainSizer);

        this->Centre(wxBOTH);
    }

    SaveFileDialog::~SaveFileDialog()
    {
    }
    std::filesystem::path SaveFileDialog::getFilePath() {
        return m_saved ? std::filesystem::path(m_path) / (m_fileName + m_extension) : "";
    }
    void SaveFileDialog::OnSave(wxCommandEvent& evt) {
        m_path = m_directoryPicker->GetTextCtrlValue().ToStdString();
        m_fileName = m_fileNameBox->GetValue().ToStdString();
        m_saved = true;
        EndModal(wxID_OK);
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