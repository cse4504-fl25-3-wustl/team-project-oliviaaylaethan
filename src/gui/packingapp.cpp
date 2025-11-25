#include <wx/wx.h>
#include <wx/busyinfo.h>
#include <wx/filepicker.h>
#include <wx/stream.h>
#include <thread> // so loading icon can be shown while waiting for it to finish
#include "estimator.cpp"
#include <nlohmann/json.hpp>
#include <filesystem>

// for the radio buttons of whether the client accepts crates
enum YesNoChoice
{
    YES = 0, // Yes button is displayed BEFORE the No button, so it's 0
    NO  = 1
};

class WxStreamBuf : public std::streambuf {
public:
    WxStreamBuf(wxTextCtrl* ctrl) : ctrl_(ctrl) {}

protected:
    int overflow(int c) override {
        if (c != EOF) {
            wxString s(static_cast<char>(c));
            wxTheApp->CallAfter([this, s]() {
                // Set red style for cerr output
                wxTextAttr redStyle;
                redStyle.SetTextColour(*wxRED);
                ctrl_->SetDefaultStyle(redStyle);
                ctrl_->AppendText(s);

                // Reset to default style (black)
                wxTextAttr defaultStyle;
                defaultStyle.SetTextColour(*wxBLACK);
                ctrl_->SetDefaultStyle(defaultStyle);
            });
        }
        return c;
    }

private:
    wxTextCtrl* ctrl_;
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
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnRunEstimator(wxCommandEvent& event);
    void OnDownloadJson(wxCommandEvent& event);
    void OnDownloadText(wxCommandEvent& event);

    wxFilePickerCtrl* filePickerData_;
    wxChoice* choiceAcceptsCrates_;
    wxTextCtrl* logBox_;
    wxButton* downloadJsonBtn_;
    wxButton* downloadTextBtn_;

    Response* response_;
};
 
enum
{
    ID_RunEstimator = 1,
    ID_DownloadJson = 2,
    ID_DownloadText = 3
};

bool PackingApp::OnInit()
{
    PackingFrame *frame = new PackingFrame();
    frame->Show(true);
    return true;
}

PackingFrame::PackingFrame()
    : wxFrame(nullptr, wxID_ANY, "Packing Estimator", wxDefaultPosition, wxSize(1024, 768))
{
    // === Menus ===
    wxMenu* menuFile = new wxMenu;
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


    // Run Estimator button
    wxButton* runButton = new wxButton(panel, ID_RunEstimator, "Run Estimator");
    sizer->Add(runButton, 0, wxALIGN_LEFT | wxALL, 10);

    // Log box
    logBox_ = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
                             wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH2);
    sizer->Add(logBox_, 1, wxEXPAND | wxALL, 10);

    // Download json button
    downloadJsonBtn_ = new wxButton(panel, ID_DownloadJson, "Download JSON");
    sizer->Add(downloadJsonBtn_, 0, wxALIGN_LEFT | wxALL, 10);
    if (response_ == nullptr) {
        downloadJsonBtn_->Disable();
    }

    // Download text file button
    downloadTextBtn_ = new wxButton(panel, ID_DownloadText, "Download Text File");
    sizer->Add(downloadTextBtn_, 0, wxALIGN_LEFT | wxALL, 10);
    if (response_ == nullptr) {
        downloadTextBtn_->Disable();
    }

    panel->SetSizer(sizer);

    // === Event bindings ===
    Bind(wxEVT_MENU, &PackingFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &PackingFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_BUTTON, &PackingFrame::OnRunEstimator, this, ID_RunEstimator);
    Bind(wxEVT_BUTTON, &PackingFrame::OnDownloadJson, this, ID_DownloadJson);
    Bind(wxEVT_BUTTON, &PackingFrame::OnDownloadText, this, ID_DownloadText);
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

void PackingFrame::OnRunEstimator(wxCommandEvent& event)
{
    wxString dataPath = filePickerData_->GetPath();
    int crateSelection = choiceAcceptsCrates_->GetSelection();

    if (dataPath.IsEmpty() || (crateSelection == wxNOT_FOUND)) {
        wxMessageBox("Please select a data file and choose if crates are accepted.",
                    "Missing input",
                    wxOK | wxICON_WARNING);
        return;
    }

    logBox_->AppendText("Running estimator...\n");
    logBox_->AppendText("Art Data: " + dataPath + "\n");
    logBox_->AppendText("Crates allowed: " + wxString(crateSelection == YES ? "Yes" : "No") + "\n");

    // Attach cerr to logBox
    WxStreamBuf* buf = new WxStreamBuf(logBox_);
    std::cerr.rdbuf(buf);

    // Show loading icon
    wxBusyInfo* busyInfo = new wxBusyInfo(wxT("Running estimator, please wait..."), this);

    // Run computation in a separate thread
    std::thread([this, busyInfo, dataPath, crateSelection]() {

        // Convert wxString → std::string and crate selection to bool
        std::string dataInputFile = dataPath.ToStdString();
        bool cratesAllowed = crateSelection == YES;

        // Build stable argument array
        std::string arg0 = "estimator";
        std::string arg1 = dataInputFile;
        std::string arg2 = cratesAllowed ? "y" : "n";

        char* argv[] = {
            arg0.data(),
            arg1.data(),
            arg2.data()
        };

        // Call estimator
        std::optional<Response> responseOpt = Estimator::RunEstimator(3, argv);

        // Update GUI after computation
        wxTheApp->CallAfter([this, busyInfo, responseOpt]() {
            delete busyInfo; // Remove loading icon

            if (!responseOpt.has_value()) {
                wxMessageBox("Error running estimator. Please check the log for details.",
                             "Error",
                             wxOK | wxICON_ERROR);
                logBox_->AppendText("Error running estimator.\n");
                return;
            }

            response_ = new Response(responseOpt.value());
            std::vector<std::vector<std::string>> summary = response_->getPackingSummary();
            summary.push_back(response_->getWeightSummary());
            summary.push_back(response_->getBusinessIntelSummary());
            summary.push_back(response_->getEmailFormatSummary());

            // Finish up
            logBox_->AppendText("Estimation complete!\n");
            for (const auto& line : summary) {
                for (const auto& subline : line) {
                    logBox_->AppendText(subline + "\n");
                }
            }
            logBox_->AppendText("\n");

            downloadJsonBtn_->Enable();
            downloadTextBtn_->Enable();
        });
    }).detach();
}

void PackingFrame::OnDownloadJson(wxCommandEvent& event)
{
    // Ask the user where to save the JSON file
    wxFileDialog saveDialog(
        this,
        "Save JSON File",
        "",                  // default directory
        "output.json",      // default filename
        "JSON files (*.json)|*.json",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT
    );

    if (saveDialog.ShowModal() != wxID_OK)
        return; // user canceled

    wxString path = saveDialog.GetPath();

    // Convert your ResponseSummary to json
    ResponseSummary responseSummary(*response_);
    nlohmann::json j = responseSummary;

    // Save to file
    std::ofstream file(path.ToStdString());
    if (!file.good()) {
        wxMessageBox("Could not save JSON file.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    file << j.dump(4);
    file.close();

    wxMessageBox("JSON file saved successfully.", "Success", wxOK | wxICON_INFORMATION);
}

void PackingFrame::OnDownloadText(wxCommandEvent& event)
{
    // Ask the user where to save the JSON file
    wxFileDialog saveDialog(
        this,
        "Save Text File",
        "",
        "output.txt",
        "Text files (*.txt)|*.txt",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT
    );

    if (saveDialog.ShowModal() != wxID_OK)
        return;

    wxString path = saveDialog.GetPath();

    // Save to file
    std::ofstream file(path.ToStdString());
    if (!file.good()) {
        wxMessageBox("Could not save text file.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    std::vector<std::vector<std::string>> summary = response_->getPackingSummary();
    summary.push_back(response_->getWeightSummary());
    summary.push_back(response_->getBusinessIntelSummary());
    summary.push_back(response_->getEmailFormatSummary());

    for (const auto& line : summary) {
        for (const auto& subline : line) {
            file << subline << "\n";
        }
    }
    file.close();

    wxMessageBox("Text file saved successfully.", "Success", wxOK | wxICON_INFORMATION);
}
