#include "MainFrame.h"

// ID cho các widget
enum {
    ID_NEXT = 1,
    ID_PREV,
    ID_ADD,
    ID_UPD,
    ID_REM,
    ID_SRCH,
    ID_REMLEARN,
    ID_STATS,
    ID_LRN           // checkbox learned
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(ID_NEXT, MainFrame::OnNext)
    EVT_BUTTON(ID_PREV, MainFrame::OnPrev)
    EVT_BUTTON(ID_ADD, MainFrame::OnAdd)
    EVT_BUTTON(ID_UPD, MainFrame::OnUpdate)
    EVT_BUTTON(ID_REM, MainFrame::OnRemove)
    EVT_BUTTON(ID_SRCH, MainFrame::OnSearch)
    EVT_BUTTON(ID_REMLEARN, MainFrame::OnRemoveLearned)
    EVT_BUTTON(ID_STATS, MainFrame::OnStats)
    EVT_CHECKBOX(ID_LRN, MainFrame::OnToggle)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(wxWindow* p)
    : wxFrame(p, wxID_ANY, "FlashCard App", wxDefaultPosition, wxSize(700,500))
{
    static wxLocale locale;
    locale.Init(wxLANGUAGE_VIETNAMESE);

    wxPanel* pan = new wxPanel(this);

    list.loadFromFile("flashcards.txt");

    wxBoxSizer* v = new wxBoxSizer(wxVERTICAL);
    wxFlexGridSizer* g = new wxFlexGridSizer(2, 8, 8);

    g->Add(new wxStaticText(pan, wxID_ANY, "Word:"));
    txtWord = new wxTextCtrl(pan, wxID_ANY);
    g->Add(txtWord, 1, wxEXPAND);

    g->Add(new wxStaticText(pan, wxID_ANY, "Pronunciation:"));
    txtPron = new wxTextCtrl(pan, wxID_ANY);
    g->Add(txtPron, 1, wxEXPAND);

    g->Add(new wxStaticText(pan, wxID_ANY, "Meaning:"));
    txtMeaning = new wxTextCtrl(pan, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    g->Add(txtMeaning, 1, wxEXPAND);

    g->Add(new wxStaticText(pan, wxID_ANY, "Example:"));
    txtExample = new wxTextCtrl(pan, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    g->Add(txtExample, 1, wxEXPAND);

    g->Add(new wxStaticText(pan, wxID_ANY, "Topic:"));
    txtTopic = new wxTextCtrl(pan, wxID_ANY);
    g->Add(txtTopic, 1, wxEXPAND);

    g->Add(new wxStaticText(pan, wxID_ANY, "Learned:"));
    chkLearned = new wxCheckBox(pan, ID_LRN, "Đã học");
    g->Add(chkLearned);

    v->Add(g, 1, wxEXPAND | wxALL, 10);

    wxBoxSizer* nav = new wxBoxSizer(wxHORIZONTAL);
    nav->Add(new wxButton(pan, ID_PREV, "<- Prev"));
    nav->Add(new wxButton(pan, ID_NEXT, "Next ->"));
    v->Add(nav, 0, wxALIGN_CENTER | wxALL, 5);

    wxBoxSizer* act = new wxBoxSizer(wxHORIZONTAL);
    act->Add(new wxButton(pan, ID_ADD, "Add"));
    act->Add(new wxButton(pan, ID_UPD, "Update"));
    act->Add(new wxButton(pan, ID_REM, "Remove"));
    act->Add(new wxButton(pan, ID_SRCH, "Search"));
    act->Add(new wxButton(pan, ID_REMLEARN, "Remove Learned"));
    act->Add(new wxButton(pan, ID_STATS, "Statistics"));
    v->Add(act, 0, wxALIGN_CENTER | wxALL, 5);

    pan->SetSizer(v);

    updateUI();
}

void MainFrame::updateUI() {
    Flashcard* c = list.current();
    if (!c) {
        txtWord->Clear();
        txtPron->Clear();
        txtMeaning->Clear();
        txtExample->Clear();
        txtTopic->Clear();
        chkLearned->SetValue(false);
        return;
    }
    txtWord->SetValue(c->word);
    txtPron->SetValue(c->pronunciation);
    txtMeaning->SetValue(c->meaning);
    txtExample->SetValue(c->example);
    txtTopic->SetValue(c->topic);
    chkLearned->SetValue(c->learned);
}

void MainFrame::save() { list.saveToFile("flashcards.txt"); }
void MainFrame::load() { list.loadFromFile("flashcards.txt"); }

void MainFrame::OnNext(wxCommandEvent&) { if (list.next()) updateUI(); }
void MainFrame::OnPrev(wxCommandEvent&) { if (list.prev()) updateUI(); }

void MainFrame::OnAdd(wxCommandEvent&) {
    Flashcard c{
        list.count() + 1,
        txtWord->GetValue().ToStdString(),
        txtMeaning->GetValue().ToStdString(),
        txtPron->GetValue().ToStdString(),
        txtExample->GetValue().ToStdString(),
        txtTopic->GetValue().ToStdString(),
        chkLearned->GetValue()
    };
    list.add(c);
    save();
    updateUI();
}

void MainFrame::OnUpdate(wxCommandEvent&) {
    Flashcard* c = list.current();
    if (!c) return;
    c->word = txtWord->GetValue().ToStdString();
    c->meaning = txtMeaning->GetValue().ToStdString();
    c->pronunciation = txtPron->GetValue().ToStdString();
    c->example = txtExample->GetValue().ToStdString();
    c->topic = txtTopic->GetValue().ToStdString();
    c->learned = chkLearned->GetValue();
    save();
    updateUI();
}

void MainFrame::OnRemove(wxCommandEvent&) {
    list.removeCurrent();
    save();
    updateUI();
}

void MainFrame::OnSearch(wxCommandEvent&) {
    wxString w = wxGetTextFromUser("Enter word:");
    if (list.search(w.ToStdString()))
        updateUI();
    else
        wxMessageBox("Not found");
}

void MainFrame::OnRemoveLearned(wxCommandEvent&) {
    list.removeLearned();
    save();
    updateUI();
}

void MainFrame::OnStats(wxCommandEvent&) {
    wxString msg;
    msg << "Total: " << list.count() << "\nLearned: " << list.countLearned();
    wxMessageBox(msg);
}

void MainFrame::OnToggle(wxCommandEvent&) {
    Flashcard* c = list.current();
    if (!c) return;
    c->learned = chkLearned->GetValue();
    save();
}
