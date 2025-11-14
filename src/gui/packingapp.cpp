#include <wx/wx.h>
#include <wx/filepicker.h>
#include "../interactors/packingInteractor.h"
#include "../parser/csvParser.h"
#include "../responses/response.h"
#include "../responses/responseSummary.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <filesystem>
 
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
    wxFilePickerCtrl* filePickerReq_;
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

    // File picker for requirements
    filePickerReq_ = new wxFilePickerCtrl(
        panel, wxID_ANY, "", "Select requirements file", "*.*",
        wxDefaultPosition, wxDefaultSize, wxFLP_OPEN | wxFLP_FILE_MUST_EXIST | wxFLP_USE_TEXTCTRL
    );
    sizer->Add(new wxStaticText(panel, wxID_ANY, "Select requirements file:"), 
           0, wxLEFT | wxTOP, 10);
    sizer->Add(filePickerReq_, 0, wxEXPAND | wxALL, 10);

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
    wxString reqPath  = filePickerReq_->GetPath();

    if (dataPath.IsEmpty() || reqPath.IsEmpty()) {
        wxMessageBox("Please select both input files first.",
                     "Missing input",
                     wxOK | wxICON_WARNING);
        return;
    }

    logBox_->AppendText("Running estimator...\n");
    logBox_->AppendText("Art Data: " + dataPath + "\n");
    logBox_->AppendText("Requirements: " + reqPath + "\n");

    // Convert wxString → std::string
    std::string dataInputFile = dataPath.ToStdString();
    std::string requirementsInputFile = reqPath.ToStdString();

    // Configure output path
    std::string outputFilePath =
        std::filesystem::current_path().string() + "/output.json";

    try
    {
        CsvParser parser;

        if (!parser.isValidFile(dataInputFile) ||
            !parser.isValidFile(requirementsInputFile))
        {
            logBox_->AppendText("Error: One or both input files are invalid.\n");
            return;
        }

        Request request = parser.parseFiles(dataInputFile, requirementsInputFile);

        PackingInteractor packingInteractor;
        Response response = packingInteractor.packAllArt(request);

        // JSON output
        ResponseSummary responseSummary(response);
        nlohmann::json j = responseSummary;

        std::ofstream file(outputFilePath);
        file << j.dump(4);
        file.close();

        logBox_->AppendText("Estimation complete!\n");
        logBox_->AppendText("Output saved to: " +
                            wxString(outputFilePath) + "\n");

        logBox_->AppendText("JSON Output:\n");
        logBox_->AppendText(wxString(j.dump(4)) + "\n");
    }
    catch (const std::exception& e)
    {
        logBox_->AppendText("ERROR: ");
        logBox_->AppendText(e.what());
        logBox_->AppendText("\n");
    }
}

