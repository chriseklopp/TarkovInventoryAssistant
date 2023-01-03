// TarkovInventoryAssistant.h : Include file for standard system include files,
// or project specific include files.

#pragma once
//#include <wx/msw/winundef.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/fileconf.h>
#include <wx/splitter.h>
#include <iostream>
#include <DataSupport/TItemTypes.h>

#include <Core/TCore.h>
#include <filesystem>


#include <UI/TUIPanels.h>
#include <UI/CatalogPanel.h>
#include <UI/ConsolePanel.h>
#include <UI/DisplayPanel.h>
#include <UI/OutputPanel.h>
#include <UI/DetectionValidatorWidget.h>


class TIAApp : public wxApp {
    public:
        virtual bool OnInit();


private:


};

class TIAFrame : public wxFrame
{
public:
    TIAFrame();

private:
    void OnHello(wxCommandEvent& evt);
    void OnExit(wxCommandEvent& evt);
    void OnAbout(wxCommandEvent& evt);
    void OnSettings(wxCommandEvent& evt);
    void OnCompileCatalog(wxCommandEvent& evt);
    void OnDetectionValidator(wxCommandEvent& evt);
    // Pointers to the four main window panels. These are created during TIAFrame construction.
    TUI::OutputPanel* m_outputPanel;
    TUI::DisplayPanel* m_displayPanel;
    TUI::ConsolePanel* m_consolePanel;
    TUI::CatalogPanel* m_catalogPanel;

    TCore m_core;


};
enum
{
    ID_Hello = 1,
    ID_Settings = 2,
    ID_COMPILECAT = 3,
    ID_DETVALIDATOR = 4
};

