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
#include "TItemTypes.h"

#include "TGlobal.h"
#include "TCore.h"
#include <filesystem>


#include "TUIPanels.h"



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
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnSettings(wxCommandEvent& event);

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
    ID_Settings =2
};

