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
    upperSplitter->SetSashGravity(.5);

    m_displayPanel = new DisplayPanel(upperSplitter,wxID_ANY, wxDefaultPosition,wxSize(200,100));
    m_outputPanel = new OutputPanel(upperSplitter, wxID_ANY, wxDefaultPosition, wxSize(200, 100));

    upperSplitter->SplitVertically(m_displayPanel, m_outputPanel,0);


    // Make lower windows and their left / right splitter.
    wxSplitterWindow* lowerSplitter = new wxSplitterWindow(upperLowerSplitter, wxID_ANY);
    lowerSplitter->SetMinimumPaneSize(10);
    lowerSplitter->SetSashGravity(.5);

    m_consolePanel = new ConsolePanel(lowerSplitter, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
    m_catalogPanel = new CatalogPanel(lowerSplitter, wxID_ANY, wxDefaultPosition, wxSize(200, 100));

    lowerSplitter->SplitVertically(m_consolePanel, m_catalogPanel);

    upperLowerSplitter->SplitHorizontally(upperSplitter, lowerSplitter);
    upperLowerSplitter->SetSashGravity(.7);
    this->SetSizerAndFit(rootSizer);


    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
        "Help string shown in status bar for this menu item");
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


