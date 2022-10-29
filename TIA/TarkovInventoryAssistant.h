// TarkovInventoryAssistant.h : Include file for standard system include files,
// or project specific include files.

#pragma once
//#include <wx/msw/winundef.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include <iostream>
#include "TItemTypes.h"
#include "TImageReader.h"
//#include "TScreenCapture.h" // TODO: need to figure out why window.h explodes eveything.
#include "TDataCatalog.h"
#include "TGlobal.h"
#include <filesystem>






class TIAApp : public wxApp {
    public:
        virtual bool OnInit();
};

class TIAFrame : public wxFrame
{
public:
    TIAFrame();

private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};
enum
{
    ID_Hello = 1
};

//#include <WinSock2.h>
//#include <wx/window.h>
//class MyApp : public wxApp
//{
//public:
//    virtual bool OnInit();
//};
// TODO: Reference additional headers your program requires here.
