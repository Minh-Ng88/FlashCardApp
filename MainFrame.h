#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include "FlashcardList.h"

class MainFrame : public wxFrame {
public:
    MainFrame(wxWindow* parent);

private:
    FlashcardList list;

    wxTextCtrl *txtWord, *txtPron, *txtMeaning, *txtExample, *txtTopic;
    wxCheckBox* chkLearned;

    void load();
    void save();
    void updateUI();

    void OnNext(wxCommandEvent&);
    void OnPrev(wxCommandEvent&);
    void OnAdd(wxCommandEvent&);
    void OnUpdate(wxCommandEvent&);
    void OnRemove(wxCommandEvent&);
    void OnSearch(wxCommandEvent&);
    void OnRemoveLearned(wxCommandEvent&);
    void OnStats(wxCommandEvent&);
    void OnToggle(wxCommandEvent&);

    wxDECLARE_EVENT_TABLE();
};

#endif
