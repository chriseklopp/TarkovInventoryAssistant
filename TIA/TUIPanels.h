

#pragma once
//#include <wx/msw/winundef.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/fileconf.h>
#include <wx/splitter.h>
#include <wx/listctrl.h>
#include "TItemTypes.h"
/*
Code for the 4 main panels in the TIA GUI.
Each inherits wxPanel
*/

class OutputPanel : public wxPanel {

public:
    OutputPanel(wxWindow* parent,
        wxWindowID 	id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long 	style = wxTAB_TRAVERSAL,
        const wxString& name = wxPanelNameStr

    );

    void populateOutputList();

    void addItemToOutputList(const TItemTypes::TItem* item, int count);



private:

    // Maps column name to column index in the outputList.
    // TODO: Might need another way to contain this info.
    static const std::map<std::string, int> m_columnIndexMap;

    wxListView* m_outputList;


};


class DisplayPanel : public wxPanel {

public:
    DisplayPanel(wxWindow* parent,
        wxWindowID 	id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long 	style = wxTAB_TRAVERSAL,
        const wxString& name = wxPanelNameStr

    );

};
class ConsolePanel : public wxPanel {

public:
    ConsolePanel(wxWindow* parent,
        wxWindowID 	id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long 	style = wxTAB_TRAVERSAL,
        const wxString& name = wxPanelNameStr

    );

};
class CatalogPanel : public wxPanel {

public:
    CatalogPanel(wxWindow* parent,
        wxWindowID 	id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long 	style = wxTAB_TRAVERSAL,
        const wxString& name = wxPanelNameStr

    );

};
