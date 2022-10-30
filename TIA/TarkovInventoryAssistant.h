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
#include "TImageReader.h"
//#include "TScreenCapture.h" // TODO: need to figure out why window.h explodes eveything.
#include "TDataCatalog.h"
#include "TGlobal.h"
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


    // Pointers to the four main window panels. These are created during TIAFrame construction.
    OutputPanel* m_outputPanel;
    DisplayPanel* m_displayPanel;
    ConsolePanel* m_consolePanel;
    CatalogPanel* m_catalogPanel;


    // Do I want to keep the interfacing with my types in a separate class?
    // I probably do to ensure ui related code stays segregated from backend related code.
    TDataCatalog m_catalog;
    TImageReader m_imageReader;



};
enum
{
    ID_Hello = 1
};

