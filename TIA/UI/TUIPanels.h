
#pragma once
#include <UI/UISupport.h>

#include <wx/statline.h>
#include <wx/filepicker.h>
/*
Main frame dialogs are defined here.
*/

namespace TUI {

    class SettingsDialog : public wxDialog
    {

    public:
        SettingsDialog(TCore* core, wxWindow* parent,
            wxWindowID id = wxID_ANY,
            const wxString& title = wxT("Settings"),
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxSize(552, 240),
            long style = wxDEFAULT_DIALOG_STYLE);


    private:

        void OnSave(wxCommandEvent& evt);
        void OnCancel(wxCommandEvent& evt);

        wxStaticText* m_headerText;
        wxStaticLine* m_headerline;
        wxStaticText* m_dataDirText;
        wxDirPickerCtrl* m_dataDirSelect;
        wxStaticText* m_compiledCatalogText;
        wxDirPickerCtrl* m_catalogSelect;
        wxButton* m_cancelButton;
        wxButton* m_saveButton;

        // Pointer to the core object.
        TCore* m_coreptr;

    };

    class CompileCatalogDialog : public wxDialog
    {

    public:

        CompileCatalogDialog(TCore* core, wxWindow* parent,
            wxWindowID id = wxID_ANY,
            const wxString& title = wxT("Catalog Compiler"),
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxSize(552, 240),
            long style = wxDEFAULT_DIALOG_STYLE);

    private:
        void OnCompile(wxCommandEvent& evt);
        void OnCancel(wxCommandEvent& evt);
        void OnRawPathSelected(wxFileDirPickerEvent& evt);
        void OnToggleRotations(wxCommandEvent& evt);

        wxStaticLine* m_headerLine;
        wxStaticText* m_rawCatalogPathText;
        wxDirPickerCtrl* m_rawCatalogPathSelect;
        wxStaticLine* m_optionLine;
        wxStaticText* m_catalogNameText;
        wxTextCtrl* m_catalogNameSelect;
        wxCheckBox* m_toggleGenerateRotations;
        wxStaticLine* m_footerLine;
        wxButton* m_cancelButton;
        wxButton* m_compileButton;

        wxStaticBoxSizer* m_optionSizer;



        // Pointer to the core object.
        TCore* m_coreptr;

    };

}