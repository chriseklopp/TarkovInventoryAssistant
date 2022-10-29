// TarkovInventoryAssistant.cpp : Contains entry point and GUI Frame.
//

#include "TarkovInventoryAssistant.h"


wxIMPLEMENT_APP(TIAApp); // wxWidgets entry point

bool TIAApp::OnInit()
{
    TIAFrame* frame = new TIAFrame();
    frame->Show(true);
    return true;
}

TIAFrame::TIAFrame()
    : wxFrame(nullptr, wxID_ANY, "Hello World")
{
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


//int main()
//{
//
//    TImageReader reader = TImageReader();
//    TDataCatalog cat = TDataCatalog();
//    //cat.compileCatalogFromRaw("C:\\MyWorkspace\\TarkovInventoryAssistant\\Data\\Catalogs\\catalog-Keyless");
//    cat.loadCatalog();
//    std::vector <std::unique_ptr<TItemTypes::TItem>> out;
//    std::string impath = "C:\\pyworkspace\\tarkovinventoryproject\\Data\\screenshots\\testimage3.png";
//
//    reader.parseFromPath(impath, out);
//
//
//    for (auto&& it : out) {
//        TItemTypes::TItem* res = cat.getBestMatch(*it);
//        if (res) {
//            //cv::imshow("Res", res->getImage());
//            //cv::imshow("IN", it->getImage());
//            //cv::waitKey(0);
//        }
//    }
//
//    return 0;
//}
