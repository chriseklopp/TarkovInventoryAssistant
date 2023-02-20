#include <UI/UISupport.h>



namespace TUI {

    /* ConsolePanel acts as a text console that outputs events as they happen.
    */
    class ConsolePanel : public wxPanel, public TEvent::TObserver {

    public:
        ConsolePanel(TAppInterface* core, wxWindow* parent,
            wxWindowID 	id = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long 	style = wxTAB_TRAVERSAL,
            const wxString& name = wxPanelNameStr

        );

        virtual void TEventReceived(TEvent::TEvent e) override;

        // Pointer to the core object.
        TAppInterface* m_coreptr;

        wxTextCtrl* m_consoleOutput;

    };

}