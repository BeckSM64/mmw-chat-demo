#include <wx/wx.h>
#include <thread>
#include <atomic>
#include <cstring>
#include <chrono>
#include <sstream>
#include "MMW.h"

struct MmwDemoChatMessage {
    char senderId[64];
    char text[256];
};

class ChatFrame : public wxFrame {
public:
    ChatFrame(const wxString& title);
    ~ChatFrame();

private:
    wxTextCtrl* textCtrlLog;
    wxTextCtrl* textCtrlInput;
    wxButton* sendButton;
    std::string clientId;
    std::thread subscriberThread;
    std::atomic<bool> running{true};

    void OnSend(wxCommandEvent& event);
    void AppendMessage(const std::string& msg);
    static void OnMessage(void* data);
};

class ChatApp : public wxApp {
public:
    bool OnInit() override {
        auto* frame = new ChatFrame("MMW Chat Demo");
        frame->Show(true);
        return true;
    }

    int OnExit() override {
        mmw_cleanup();
        return 0;
    }
};

wxIMPLEMENT_APP(ChatApp);

ChatFrame* g_instance = nullptr;

ChatFrame::ChatFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(500, 400)) {

    g_instance = this;

    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    textCtrlLog = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
                                 wxTE_MULTILINE | wxTE_READONLY);
    textCtrlInput = new wxTextCtrl(panel, wxID_ANY);
    sendButton = new wxButton(panel, wxID_ANY, "Send");

    sizer->Add(textCtrlLog, 1, wxEXPAND | wxALL, 5);
    sizer->Add(textCtrlInput, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);
    sizer->Add(sendButton, 0, wxALIGN_RIGHT | wxALL, 5);

    panel->SetSizer(sizer);

    sendButton->Bind(wxEVT_BUTTON, &ChatFrame::OnSend, this);
    textCtrlInput->Bind(wxEVT_TEXT_ENTER, &ChatFrame::OnSend, this);

    // Unique client ID
    std::stringstream ss;
    ss << std::chrono::steady_clock::now().time_since_epoch().count();
    clientId = ss.str();

    // Initialize middleware
    if (mmw_initialize("127.0.0.1", 5000) != MMW_OK) {
        wxMessageBox("Failed to initialize MMW", "Error", wxICON_ERROR);
        return;
    }

    // Create publisher
    mmw_create_publisher("chat");

    // Start subscriber thread
    subscriberThread = std::thread([this]() {
        if (mmw_create_subscriber_raw("chat", &ChatFrame::OnMessage) != MMW_OK) {
            return;
        }
        while (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
}

ChatFrame::~ChatFrame() {
    running = false;
    if (subscriberThread.joinable())
        subscriberThread.join();
    // cleanup handled by wxApp::OnExit
}

void ChatFrame::OnSend(wxCommandEvent&) {
    std::string msg = textCtrlInput->GetValue().ToStdString();
    if (msg.empty()) return;

    // Append locally first
    AppendMessage("You: " + msg);

    MmwDemoChatMessage m{};
    strncpy(m.senderId, clientId.c_str(), sizeof(m.senderId) - 1);
    strncpy(m.text, msg.c_str(), sizeof(m.text) - 1);

    mmw_publish_raw("chat", &m, sizeof(m), MMW_RELIABLE);
    textCtrlInput->Clear();
}

void ChatFrame::AppendMessage(const std::string& msg) {
    textCtrlLog->AppendText(msg + "\n");
}

void ChatFrame::OnMessage(void* data) {
    auto* msg = static_cast<MmwDemoChatMessage*>(data);
    if (!g_instance) return;

    if (strncmp(msg->senderId, g_instance->clientId.c_str(), sizeof(msg->senderId)) == 0)
        return; // Ignore own messages received from middleware

    std::string display = std::string(msg->senderId) + ": " + msg->text;
    g_instance->CallAfter(&ChatFrame::AppendMessage, display);
}
