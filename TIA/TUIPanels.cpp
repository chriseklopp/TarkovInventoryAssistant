#include "TUIPanels.h"




OutputPanel::OutputPanel(wxWindow* parent,
    wxWindowID 	id,
    const wxPoint& pos,
    const wxSize& size,
    long 	style,
    const wxString& name) : wxPanel(parent,
        id,
        pos,
        size,
        style,
        name) {

    // Code goes here :)

    this->SetBackgroundColour(wxColor(100, 200, 200));

    // Add output list
    m_outputList = new wxListView(this);

    // Add columns
    for (auto& col : m_columnIndexMap){
        m_outputList->InsertColumn(col.second, col.first);
}
    

    m_outputList->SetColumnWidth(0, 80);
    m_outputList->SetColumnWidth(1, 120);
    m_outputList->SetColumnWidth(2, 200);

    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_outputList, 1, wxALL | wxEXPAND, 0);
    this->SetSizerAndFit(sizer);

    populateOutputList();

};


void OutputPanel::populateOutputList() {
    wxListItem* it = new wxListItem();

    m_outputList->InsertItem(0, "123");
    m_outputList->SetItem(0,1, "Some Item");
    m_outputList->SetItem(0, 2, "1x1");

    m_outputList->InsertItem(1,"test");
    m_outputList->SetItem(1, 1, "Cool Item");
    m_outputList->SetItem(1, 2, "2x2");

    m_outputList->InsertItem(2, "69969");
    m_outputList->SetItem(2, 1, "Testy Item");
    m_outputList->SetItem(2, 2, "3x2");
}; 


void OutputPanel::addItemToOutputList(const TItemTypes::TItem* item, int count) {

    int index = m_outputList->GetItemCount();
    m_outputList->InsertItem(index, "");

    m_outputList->SetItem(index, m_columnIndexMap.at("Name"), item->getName());
    m_outputList->SetItem(index, m_columnIndexMap.at("Dim"), item->getDimAsString());
    m_outputList->SetItem(index, m_columnIndexMap.at("Count"), std::to_string(count));

    wxImage aswxImage = wxImage(item->getImage().cols, item->getImage().rows, item->getImage().data, true);
    //m_outputList->SetItemImage(index, aswxImage);

};


const std::map<std::string, int> OutputPanel::m_columnIndexMap = {
   {"Name",  0},
   {"Image", 1},
   {"Dim",   2},
   {"Count", 3}
};

DisplayPanel::DisplayPanel(wxWindow* parent,
    wxWindowID 	id,
    const wxPoint& pos,
    const wxSize& size,
    long 	style,
    const wxString& name) : wxPanel(parent,
        id,
        pos,
        size,
        style,
        name) {

    // Code goes here :)
    this->SetBackgroundColour(wxColor(0, 0, 200));


};
ConsolePanel::ConsolePanel(wxWindow* parent,
    wxWindowID 	id,
    const wxPoint& pos,
    const wxSize& size,
    long 	style,
    const wxString& name) : wxPanel(parent,
        id,
        pos,
        size,
        style,
        name) {

    // Code goes here :)
    this->SetBackgroundColour(wxColor(50, 50, 0));



};
CatalogPanel::CatalogPanel(wxWindow* parent,
    wxWindowID 	id,
    const wxPoint& pos,
    const wxSize& size,
    long 	style,
    const wxString& name) : wxPanel(parent,
        id,
        pos,
        size,
        style,
        name) {

    // Code goes here :)
    this->SetBackgroundColour(wxColor(0, 100, 100));
};