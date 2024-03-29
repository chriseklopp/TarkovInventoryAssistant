﻿// TarkovInventoryAssistant.cpp : Contains entry point and GUI Frame.
//

#include <TarkovInventoryAssistant.h>


wxIMPLEMENT_APP(TIAApp); // wxWidgets entry point

bool TIAApp::OnInit()
{
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

    // Create file menu
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
        "Help string shown in status bar for this menu item");
    menuFile->Append(ID_Settings, "&Settings", "Application settings");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);


    // Create tools menu
    wxMenu* menuTools = new wxMenu;
    menuTools->Append(ID_COMPILECAT, "&Catalog Compiler", "Create a compiled catalog from raw");
    menuTools->Append(ID_DETVALIDATOR, "&Detection Validator", "Compare detections against a reference set for accuracy.");

    // Create help menu
    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);


    // Set menu bar.
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuTools, "&Tools");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);


    CreateStatusBar();
    SetStatusText("Welcome to Tarkov Inventory Assistant!");

    // Create bindings.
    Bind(wxEVT_MENU, &TIAFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &TIAFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &TIAFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &TIAFrame::OnSettings, this, ID_Settings);
    Bind(wxEVT_MENU, &TIAFrame::OnCompileCatalog, this, ID_COMPILECAT);
    Bind(wxEVT_MENU, &TIAFrame::OnDetectionValidator, this, ID_DETVALIDATOR);

    // Register observer panels with TCore
    m_core.registerTObserver(m_displayPanel);
    m_core.registerTObserver(m_outputPanel);
    m_core.registerTObserver(m_consolePanel);
    m_core.registerTObserver(m_catalogPanel);

    m_core.loadCatalog(m_core.getConfigPtr()->getACTIVE_CATALOG());
}



void TIAFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void TIAFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("Documentation Coming Soon (TM)",
        "Tarkov Inventory Assistant", wxOK | wxICON_INFORMATION);
}

void TIAFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}


void TIAFrame::OnSettings(wxCommandEvent& event)
{
    TUI::SettingsDialog settingsDialog = TUI::SettingsDialog(&m_core,this);
    settingsDialog.ShowModal();
}

void TIAFrame::OnCompileCatalog(wxCommandEvent& evt) {
    TUI::CompileCatalogDialog compileCatalogDialog = TUI::CompileCatalogDialog(&m_core, this);
    compileCatalogDialog.ShowModal();
}

void TIAFrame::OnDetectionValidator(wxCommandEvent& evt) {
    TUI::DetectionValidatorWidget detValidatorWidget = TUI::DetectionValidatorWidget(&m_core, this);
    detValidatorWidget.ShowModal();
}