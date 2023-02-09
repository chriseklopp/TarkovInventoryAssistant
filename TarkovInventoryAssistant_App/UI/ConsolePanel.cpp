#include <UI/ConsolePanel.h>



namespace TUI {

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
        m_consoleOutput = new wxTextCtrl(this, wxUSE_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);

        m_consoleOutput->SetBackgroundColour(wxColour(20, 20, 20));
        m_consoleOutput->SetForegroundColour(wxColour(255, 255, 255));
        m_consoleOutput->SetFont(wxFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

        sizer->Add(m_consoleOutput, 1, wxEXPAND);
        m_consoleOutput->AppendText("------Tarkov Inventory Assistant-----\n");
        this->SetSizerAndFit(sizer);

    };

    void ConsolePanel::TEventReceived(TEvent::TEvent e) {

        switch (e.getType()) {

            // Unlike other events, console message data is posted directly to the console.
        case TEvent::TEventEnum::ConsoleMessage:
            m_consoleOutput->AppendText(e.getData() + "\n");
            break;

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
                m_consoleOutput->AppendText("Successfully Loaded Catalog: " + e.getData() + " (Items: " + wxString::Format(wxT("%i"), int(m_coreptr->getCatalogItemList().size())) + ")\n");
            else
                m_consoleOutput->AppendText("Failed to load catalog: " + e.getData() + "\n");
            break;

        }

    }

}