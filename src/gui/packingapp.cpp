#include <wx/wx.h>
#include <wx/filepicker.h>
#include "estimator.cpp"
#include <nlohmann/json.hpp>
#include <filesystem>

// for the radio buttons of whether the client accepts crates
enum YesNoChoice
{
    YES = 0, // Yes button is displayed BEFORE the No button, so it's 0
    NO  = 1
};
 
class PackingApp : public wxApp
{
public:
    bool OnInit() override;
};
 
wxIMPLEMENT_APP(PackingApp);

// Packing frame
class PackingFrame : public wxFrame
{
public:
    PackingFrame();
 
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnRunEstimator(wxCommandEvent& event);

    wxFilePickerCtrl* filePickerData_;
    wxChoice* choiceAcceptsCrates_;
    wxDirPickerCtrl* folderPickerOut_;
    wxTextCtrl* logBox_;
};
 
enum
{
    ID_Hello = 1,
    ID_RunEstimator = 2
};

bool PackingApp::OnInit()
{
    PackingFrame *frame = new PackingFrame();
    frame->Show(true);
    return true;
}

PackingFrame::PackingFrame()
    : wxFrame(nullptr, wxID_ANY, "Packing Estimator", wxDefaultPosition, wxSize(600, 400))
{
    // === Menus ===
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl+H", "Help string shown in status bar");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("Ready to estimate packing!");

    // === Layout ===
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // File picker for data
    filePickerData_ = new wxFilePickerCtrl(
        panel, wxID_ANY, "", "Select data file", "*.*",
        wxDefaultPosition, wxDefaultSize, wxFLP_OPEN | wxFLP_FILE_MUST_EXIST | wxFLP_USE_TEXTCTRL
    );
    sizer->Add(new wxStaticText(panel, wxID_ANY, "Select data file:"), 
           0, wxLEFT | wxTOP, 10);
    sizer->Add(filePickerData_, 0, wxEXPAND | wxALL, 10);

    // Dropdown menu for whether crates are accepted (radio buttons or checkbox selects an option by default)
    wxArrayString options;
    options.Add("Yes");
    options.Add("No");

    choiceAcceptsCrates_ = new wxChoice(
        panel, wxID_ANY,
        wxDefaultPosition, wxDefaultSize, options);

    choiceAcceptsCrates_->SetSelection(wxNOT_FOUND); // Ensure no default

    sizer->Add(new wxStaticText(panel, wxID_ANY, "Does the client accept crates?"), 
           0, wxLEFT | wxTOP, 10);
    sizer->Add(choiceAcceptsCrates_, 0, wxEXPAND | wxALL, 10);


    // Folder picker for output location
    folderPickerOut_ = new wxDirPickerCtrl(
        panel, wxID_ANY, "", "Select folder to save output.json",
        wxDefaultPosition, wxDefaultSize,
        wxDIRP_USE_TEXTCTRL | wxDIRP_DIR_MUST_EXIST
    );
    sizer->Add(new wxStaticText(panel, wxID_ANY, "Select output folder:"), 
            0, wxLEFT | wxTOP, 10);
    sizer->Add(folderPickerOut_, 0, wxEXPAND | wxALL, 10);


    // Run Estimator button
    wxButton* runButton = new wxButton(panel, ID_RunEstimator, "Run Estimator");
    sizer->Add(runButton, 0, wxALIGN_LEFT | wxALL, 10);

    // Log box
    logBox_ = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
                             wxTE_MULTILINE | wxTE_READONLY);
    sizer->Add(logBox_, 1, wxEXPAND | wxALL, 10);

    panel->SetSizer(sizer);

    // === Event bindings ===
    Bind(wxEVT_MENU, &PackingFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &PackingFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &PackingFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_BUTTON, &PackingFrame::OnRunEstimator, this, ID_RunEstimator);
}

void PackingFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void PackingFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This application estimates packing efficiency from a file.",
                 "About Packing Estimator", wxOK | wxICON_INFORMATION);
}

void PackingFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello from wxWidgets!");
}

void PackingFrame::OnRunEstimator(wxCommandEvent& event)
{
    wxString dataPath = filePickerData_->GetPath();
    int crateSelection = choiceAcceptsCrates_->GetSelection();
    wxString outputPath = folderPickerOut_->GetPath();

    if (dataPath.IsEmpty() || (crateSelection == wxNOT_FOUND) || outputPath.IsEmpty()) {
        wxMessageBox("Please select a data file and output path, and choose if crates are accepted.",
                    "Missing input",
                    wxOK | wxICON_WARNING);
        return;
    }

    logBox_->AppendText("Running estimator...\n");
    logBox_->AppendText("Art Data: " + dataPath + "\n");
    logBox_->AppendText("Creates allowed: " + wxString(crateSelection == YES ? "Yes" : "No") + "\n");
    logBox_->AppendText("Output Path: " + outputPath + "\n");

    // Convert wxString → std::string and crate selection to bool
    std::string dataInputFile = dataPath.ToStdString();
    bool cratesAllowed = crateSelection == YES;
    std::string outputFilePath = outputPath.ToStdString() + "/output.json";

    // Build stable argument array
    std::string arg0 = "estimator";
    std::string arg1 = dataInputFile;
    std::string arg2 = cratesAllowed ? "y" : "n";
    std::string arg3 = outputFilePath;

    char* argv[] = {
        arg0.data(),
        arg1.data(),
        arg2.data(),
        arg3.data()
    };

    // Call estimator
    std::optional<Response> responseOpt = Estimator::RunEstimator(4, argv);
    if (!responseOpt.has_value()) {
        logBox_->AppendText("Error running estimator.\n");
        return;
    }

    Response response = responseOpt.value();

    // ---- Read JSON from file ----
    std::ifstream inFile(outputFilePath);
    if (!inFile.is_open()) {
        logBox_->AppendText("Error: Could not open output file.\n");
        return;
    }

    // Finish up
    logBox_->AppendText("Estimation complete!\n");
    logBox_->AppendText("Output saved to: " +
                        wxString(outputFilePath) + "\n");

    for (const auto& line : response.getWeightSummary()) {
        logBox_->AppendText(line + "\n");
    }

}

