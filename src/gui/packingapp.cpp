#include <wx/wx.h>
#include <wx/busyinfo.h>
#include <wx/filepicker.h>
#include <wx/stream.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <thread> // so loading icon can be shown while waiting for it to finish
#include <wx/utils.h> // wxLaunchDefaultBrowser
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
                // Detect background color
                wxColour bgColor = ctrl_->GetBackgroundColour();
                wxColour textColor;

                // Set text color based on background color
                if (bgColor.IsOk() && bgColor.GetLuminance() < 0.5) {
                    // Dark background → use light text
                    textColor = *wxWHITE;
                } else {
                    // Light background → use dark text
                    textColor = *wxBLACK;
                }

                // Set red style for cerr output
                wxTextAttr redStyle;
                redStyle.SetTextColour(*wxRED);
                ctrl_->SetDefaultStyle(redStyle);
                ctrl_->AppendText(s);

                // Reset to default style (dynamic text color)
                wxTextAttr defaultStyle;
                defaultStyle.SetTextColour(textColor);
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
    void OnOpenEmail(wxCommandEvent& event);
    void OnInputChanged(wxCommandEvent& event);

    wxFilePickerCtrl* filePickerData_;
    wxChoice* choiceAcceptsCrates_;
    wxTextCtrl* logBox_;
    wxButton* downloadJsonBtn_;
    wxButton* downloadTextBtn_;
    wxButton* openEmailBtn_;
    wxNotebook* notebook_;

    Response* response_;
};
 
enum
{
    ID_RunEstimator = 1,
    ID_DownloadJson = 2,
    ID_DownloadText = 3,
    ID_OpenEmail = 4
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
    // Create a notebook for tabs
    notebook_ = new wxNotebook(panel, wxID_ANY);

    // Terminal Output Tab
    wxPanel* terminalPanel = new wxPanel(notebook_);
    wxBoxSizer* terminalSizer = new wxBoxSizer(wxVERTICAL);
    logBox_ = new wxTextCtrl(terminalPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
                             wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH2);
    terminalSizer->Add(logBox_, 1, wxEXPAND | wxALL, 10);
    terminalPanel->SetSizer(terminalSizer);
    notebook_->AddPage(terminalPanel, "Terminal Output");

    // JSON Tab
    wxPanel* jsonPanel = new wxPanel(notebook_);
    wxBoxSizer* jsonSizer = new wxBoxSizer(wxVERTICAL);
    wxTextCtrl* jsonBox = new wxTextCtrl(jsonPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
                                         wxTE_MULTILINE | wxTE_READONLY);
    jsonSizer->Add(jsonBox, 1, wxEXPAND | wxALL, 10);
    jsonPanel->SetSizer(jsonSizer);
    notebook_->AddPage(jsonPanel, "JSON");

    // Packing Summary Tab
    wxPanel* packingSummaryPanel = new wxPanel(notebook_);
    wxBoxSizer* packingSummarySizer = new wxBoxSizer(wxVERTICAL);
    wxTextCtrl* packingSummaryBox = new wxTextCtrl(packingSummaryPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
                                                   wxTE_MULTILINE | wxTE_READONLY);
    packingSummarySizer->Add(packingSummaryBox, 1, wxEXPAND | wxALL, 10);
    packingSummaryPanel->SetSizer(packingSummarySizer);
    notebook_->AddPage(packingSummaryPanel, "Packing Summary");

    // Weight Summary Tab
    wxPanel* weightSummaryPanel = new wxPanel(notebook_);
    wxBoxSizer* weightSummarySizer = new wxBoxSizer(wxVERTICAL);
    wxTextCtrl* weightSummaryBox = new wxTextCtrl(weightSummaryPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
                                                  wxTE_MULTILINE | wxTE_READONLY);
    weightSummarySizer->Add(weightSummaryBox, 1, wxEXPAND | wxALL, 10);
    weightSummaryPanel->SetSizer(weightSummarySizer);
    notebook_->AddPage(weightSummaryPanel, "Weight Summary");

    // Business Intel Summary Tab
    wxPanel* businessIntelPanel = new wxPanel(notebook_);
    wxBoxSizer* businessIntelSizer = new wxBoxSizer(wxVERTICAL);
    wxTextCtrl* businessIntelBox = new wxTextCtrl(businessIntelPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
                                                  wxTE_MULTILINE | wxTE_READONLY);
    businessIntelSizer->Add(businessIntelBox, 1, wxEXPAND | wxALL, 10);
    businessIntelPanel->SetSizer(businessIntelSizer);
    notebook_->AddPage(businessIntelPanel, "Business Intel Summary");

    // Email Format Tab
    wxPanel* emailFormatPanel = new wxPanel(notebook_);
    wxBoxSizer* emailFormatSizer = new wxBoxSizer(wxVERTICAL);
    wxTextCtrl* emailFormatBox = new wxTextCtrl(emailFormatPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
                                                wxTE_MULTILINE | wxTE_READONLY);
    emailFormatSizer->Add(emailFormatBox, 1, wxEXPAND | wxALL, 10);
    emailFormatPanel->SetSizer(emailFormatSizer);
    notebook_->AddPage(emailFormatPanel, "Email Format");

    // Add notebook to the main sizer
    sizer->Add(notebook_, 1, wxEXPAND | wxALL, 10);

    // Add download buttons below the notebook
    downloadJsonBtn_ = new wxButton(panel, ID_DownloadJson, "Download JSON");
    sizer->Add(downloadJsonBtn_, 0, wxALIGN_LEFT | wxALL, 10);
    if (response_ == nullptr) {
        downloadJsonBtn_->Disable();
    }

    downloadTextBtn_ = new wxButton(panel, ID_DownloadText, "Download Text File");
    sizer->Add(downloadTextBtn_, 0, wxALIGN_LEFT | wxALL, 10);
    if (response_ == nullptr) {
        downloadTextBtn_->Disable();
    }

    // Open Email Client button
    openEmailBtn_ = new wxButton(panel, ID_OpenEmail, "Open Email Client");
    sizer->Add(openEmailBtn_, 0, wxALIGN_LEFT | wxALL, 10);
    if (response_ == nullptr) {
        openEmailBtn_->Disable();
    }

    panel->SetSizer(sizer);

    // === Event bindings ===
    Bind(wxEVT_MENU, &PackingFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &PackingFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_BUTTON, &PackingFrame::OnRunEstimator, this, ID_RunEstimator);
    Bind(wxEVT_BUTTON, &PackingFrame::OnDownloadJson, this, ID_DownloadJson);
    Bind(wxEVT_BUTTON, &PackingFrame::OnDownloadText, this, ID_DownloadText);
    Bind(wxEVT_BUTTON, &PackingFrame::OnOpenEmail, this, ID_OpenEmail);

    // Bind events for file picker and dropdown menu
    filePickerData_->Bind(wxEVT_FILEPICKER_CHANGED, &PackingFrame::OnInputChanged, this);
    choiceAcceptsCrates_->Bind(wxEVT_CHOICE, &PackingFrame::OnInputChanged, this);

    // Ensure buttons are initially disabled
    FindWindow(ID_RunEstimator)->Disable();
    downloadJsonBtn_->Disable();
    downloadTextBtn_->Disable();
    openEmailBtn_->Disable();
}

void PackingFrame::OnExit(wxCommandEvent& event)
{
    // Ensure the app only closes when explicitly requested
    Close(true);
}

void PackingFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageDialog dialog(
        this,
        "This application estimates packing efficiency from a file.\n\n"
        "Example valid input file:\n\n"
        "Line #,Quantity,Tag #,Final Medium,Outer Width,Outer Height,Glazing,Moulding,Hardware\n"
        "1,30,1,Paper Print - Framed,43,43,Regular Glass,N/A,N/A\n"
        "2,6,2,Paper Print - Framed,36,43,Regular Glass,N/A,N/A",
        "About Packing Estimator",
        wxOK | wxICON_INFORMATION
    );

    dialog.SetSize(800, 400); // Set a larger size for the dialog box to prevent overflow
    dialog.ShowModal();
}

void PackingFrame::OnRunEstimator(wxCommandEvent& event)
{
    logBox_->Clear(); // clear output between runs
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
            logBox_->AppendText("Estimation complete!\nTo see the results, click the download buttons or view in the other tabs.");
            logBox_->AppendText("\n");

            downloadJsonBtn_->Enable();
            downloadTextBtn_->Enable();
            openEmailBtn_->Enable();
            // Update JSON tab
            wxTextCtrl* jsonBox = dynamic_cast<wxTextCtrl*>(notebook_->GetPage(1)->GetChildren()[0]);
            if (jsonBox) {
                jsonBox->Clear();
                ResponseSummary responseSummary(*response_);
                nlohmann::json j = responseSummary;
                jsonBox->AppendText(j.dump(4));
            }

            // Update Packing Summary tab
            wxTextCtrl* packingSummaryBox = dynamic_cast<wxTextCtrl*>(notebook_->GetPage(2)->GetChildren()[0]);
            if (packingSummaryBox) {
                packingSummaryBox->Clear();
                for (const auto& line : response_->getPackingSummary()) {
                    for (const auto& subline : line) {
                        packingSummaryBox->AppendText(subline + "\n");
                    }
                }
            }

            // Update Weight Summary tab
            wxTextCtrl* weightSummaryBox = dynamic_cast<wxTextCtrl*>(notebook_->GetPage(3)->GetChildren()[0]);
            if (weightSummaryBox) {
                weightSummaryBox->Clear();
                for (const auto& line : response_->getWeightSummary()) {
                    weightSummaryBox->AppendText(line + "\n");
                }
            }

            // Update Business Intel Summary tab
            wxTextCtrl* businessIntelBox = dynamic_cast<wxTextCtrl*>(notebook_->GetPage(4)->GetChildren()[0]);
            if (businessIntelBox) {
                businessIntelBox->Clear();
                for (const auto& line : response_->getBusinessIntelSummary()) {
                    businessIntelBox->AppendText(line + "\n");
                }
            }

            // Update Email Format tab
            wxTextCtrl* emailFormatBox = dynamic_cast<wxTextCtrl*>(notebook_->GetPage(5)->GetChildren()[0]);
            if (emailFormatBox) {
                emailFormatBox->Clear();
                for (const auto& line : response_->getEmailFormatSummary()) {
                    emailFormatBox->AppendText(line + "\n");
                }
            }
        });
    }).detach();
}

void PackingFrame::OnDownloadJson(wxCommandEvent& event)
{
    // Log the start of the OnDownloadJson event
    wxLogMessage("OnDownloadJson event triggered.");

    // Log the response_ pointer state
    if (response_ == nullptr) {
        wxLogMessage("response_ is null. Cannot proceed with JSON download.");
    } else {
        wxLogMessage("response_ is valid. Proceeding with JSON download.");
    }

    // Check for null pointers before using response_
    if (response_ == nullptr) {
        wxMessageBox("Response object is null. Please ensure the estimator ran successfully.", "Error", wxOK | wxICON_ERROR);
        return;
    }

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
    // Log the start of the OnDownloadText event
    wxLogMessage("OnDownloadText event triggered.");

    // Log the response_ pointer state
    if (response_ == nullptr) {
        wxLogMessage("response_ is null. Cannot proceed with text file download.");
    } else {
        wxLogMessage("response_ is valid. Proceeding with text file download.");
    }

    // Check for null pointers before using response_
    if (response_ == nullptr) {
        wxMessageBox("Response object is null. Please ensure the estimator ran successfully.", "Error", wxOK | wxICON_ERROR);
        return;
    }

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

void PackingFrame::OnOpenEmail(wxCommandEvent& event)
{
    if (response_ == nullptr) {
        wxMessageBox("No response available. Run the estimator first.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Build mailto subject and body from the Email Format summary
    try {
         auto lines = response_->getEmailFormatSummary();
        if (lines.empty()) {
            wxMessageBox("Email format summary is empty.", "Error", wxOK | wxICON_ERROR);
            return;
        }
        lines.erase(lines.begin());
        wxString subject;
        wxString body;
        for (const auto& sline : lines) {
            wxString line = wxString::FromUTF8(sline.c_str());
            if (line.StartsWith("Subject:")) {
                // Extract everything after "Subject: " (including potential space)
                wxString rest = line.Mid(8); // position after 'Subject:'
                rest.Trim(true).Trim(false);
                // If it starts with a space, remove it
                if (rest.StartsWith(" ")) {
                    rest = rest.Mid(1);
                }
                subject = rest;
            } else {
                body += line + "\n";
            }
        }

        // URL-encode subject and body minimally (replace spaces and newlines)
        subject.Replace(" ", "%20");
        body.Replace("\n", "%0A");
        body.Replace(" ", "%20");

        wxString mailto = "mailto:?subject=" + subject + "&body=" + body;

        if (!wxLaunchDefaultBrowser(mailto)) {
            wxMessageBox("Failed to open the default mail client.", "Error", wxOK | wxICON_ERROR);
        }
    } catch (const std::exception& e) {
        wxMessageBox(wxString::Format("Error constructing email: %s", e.what()), "Error", wxOK | wxICON_ERROR);
        return;
    }
}

void PackingFrame::OnInputChanged(wxCommandEvent& event)
{
    // Check if both inputs are valid
    bool isDataFileSelected = !filePickerData_->GetPath().IsEmpty();
    bool isCrateSelectionValid = choiceAcceptsCrates_->GetSelection() != wxNOT_FOUND;

    // Enable or disable buttons based on input validity
    bool enableButtons = isDataFileSelected && isCrateSelectionValid;
    if (auto* btn = FindWindow(ID_RunEstimator)) btn->Enable(enableButtons);
}
