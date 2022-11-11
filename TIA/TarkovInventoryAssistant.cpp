// TarkovInventoryAssistant.cpp : Contains entry point and GUI Frame.
//

#include "TarkovInventoryAssistant.h"


wxIMPLEMENT_APP(TIAApp); // wxWidgets entry point

bool TIAApp::OnInit()
{
    // Create ini
    //wxConfigBase::Set();


    TIAFrame* frame = new TIAFrame();
    frame->Show(true);
    frame->SetClientSize(1000, 600);

    return true;
}

TIAFrame::TIAFrame()
    : wxFrame(nullptr, wxID_ANY, "Tarkov Inventory Assistant")
{


    wxBoxSizer* rootSizer = new wxBoxSizer(wxVERTICAL);
    this->SetBackgroundColour(wxColor(255, 255, 255));


    // Make splitter for upper / lower widgets.
    wxSplitterWindow* upperLowerSplitter = new wxSplitterWindow(this, wxID_ANY);
    upperLowerSplitter->SetMinimumPaneSize(10);
    upperLowerSplitter->SetSashGravity(.5);

    rootSizer->Add(upperLowerSplitter, 1, wxEXPAND);

    // Make upper windows and their left / right splitter.
    wxSplitterWindow* upperSplitter = new wxSplitterWindow(upperLowerSplitter, wxID_ANY);
    upperSplitter->SetMinimumPaneSize(10);
    upperSplitter->SetSashGravity(1);

    m_displayPanel = new TUI::DisplayPanel(&m_core, upperSplitter,wxID_ANY, wxDefaultPosition,wxSize(200,100));
    m_outputPanel = new TUI::OutputPanel(&m_core, upperSplitter, wxID_ANY, wxDefaultPosition, wxSize(200, 100));

    upperSplitter->SplitVertically(m_displayPanel, m_outputPanel,0);


    // Make lower windows and their left / right splitter.
    wxSplitterWindow* lowerSplitter = new wxSplitterWindow(upperLowerSplitter, wxID_ANY);
    lowerSplitter->SetMinimumPaneSize(10);
    lowerSplitter->SetSashGravity(1);

    m_consolePanel = new TUI::ConsolePanel(&m_core, lowerSplitter, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
    m_catalogPanel = new TUI::CatalogPanel(&m_core, lowerSplitter, wxID_ANY, wxDefaultPosition, wxSize(200, 100));

    lowerSplitter->SplitVertically(m_consolePanel, m_catalogPanel);

    upperLowerSplitter->SplitHorizontally(upperSplitter, lowerSplitter);
    upperLowerSplitter->SetSashGravity(.7);
    this->SetSizerAndFit(rootSizer);


    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
        "Help string shown in status bar for this menu item");
    menuFile->Append(ID_Settings, "&Settings", "Application settings");
    menuFile->AppendSeparator();

    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");


    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");

    Bind(wxEVT_MENU, &TIAFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &TIAFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &TIAFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &TIAFrame::OnSettings, this, ID_Settings);


    // Register observer panels with TCore
    m_core.registerTObserver(m_displayPanel);
    m_core.registerTObserver(m_outputPanel);
    m_core.registerTObserver(m_consolePanel);
    m_core.registerTObserver(m_catalogPanel);


    // Load initial catalog defined in the config.
    m_core.loadCatalog();

    // TODO: DEBUG REMOVE
    std::unique_ptr<cv::Mat> matty =
        std::make_unique<cv::Mat>(cv::imread("C:\\pyworkspace\\tarkovinventoryproject\\Data\\screenshots\\raw2\\tucker2.png"));
    m_core.addImage(std::move(matty));

    std::unique_ptr<cv::Mat> matty1 =
        std::make_unique<cv::Mat>(cv::imread("C:\\pyworkspace\\tarkovinventoryproject\\Data\\screenshots\\testimage4.png"));
    m_core.addImage(std::move(matty1));

    std::unique_ptr<cv::Mat> matty2 =
        std::make_unique<cv::Mat>(cv::imread("C:\\pyworkspace\\tarkovinventoryproject\\Data\\screenshots\\raw2\\tucker2.png"));
    m_core.addImage(std::move(matty2));

    std::unique_ptr<cv::Mat> matty3 =
        std::make_unique<cv::Mat>(cv::imread("C:\\pyworkspace\\tarkovinventoryproject\\Data\\screenshots\\testimage4.png"));
    m_core.addImage(std::move(matty3));


}



void TIAFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void TIAFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
        "About Hello World", wxOK | wxICON_INFORMATION);
}

void TIAFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}


void TIAFrame::OnSettings(wxCommandEvent& event)
{
    m_settingsDialog = new TUI::SettingsDialog(&m_core,this);
    m_settingsDialog->ShowModal();
}

