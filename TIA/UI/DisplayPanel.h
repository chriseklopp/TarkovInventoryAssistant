#include <UI/UISupport.h>

namespace TUI {

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


}