// main.cpp
// Smart Command Assistant - Safe but real actions (Trie + Windows integration)
// Compile with: g++ main.cpp -o main -std=c++11

#include <iostream>
#include <vector>
#include <unordered_map>
#include <windows.h>
#include <string>
#include <algorithm>
#include <shellapi.h>
#include <sstream>
#include <ctime>
#include <fstream>
#include <cstdio>   // for _popen, _pclose
#include <cstdlib>

using namespace std;

// --- Helpers ---
wstring intToWString(int value) {
    wstringstream wss;
    wss << value;
    return wss.str();
}

string getTimestamp() {
    time_t now = time(nullptr);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buf);
}

void appendLog(const string &entry) {
    ofstream ofs("logs.txt", ios::app);
    ofs << "[" << getTimestamp() << "] " << entry << "\n";
    ofs.close();
}

void appendHistory(const string &entry) {
    ofstream ofs("history.txt", ios::app);
    ofs << "[" << getTimestamp() << "] " << entry << "\n";
    ofs.close();
}

// Run a system command and capture stdout (safe commands only)
string runCommandCapture(const string &cmd) {
    string result;
    FILE *pipe = _popen(cmd.c_str(), "r");
    if (!pipe) return "ERROR: popen failed.";
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    _pclose(pipe);
    return result;
}

// Safe launcher wrapper for URIs / programs (uses ShellExecuteW)
void launchURIorProgram(const wstring &target) {
    // target could be an exe or URI like L"https://..."
    ShellExecuteW(NULL, L"open", target.c_str(), NULL, NULL, SW_SHOW);
}

// --- Trie implementation ---
class TrieNode {
public:
    bool isEnd;
    unordered_map<char, TrieNode*> children;
    int index;
    TrieNode() : isEnd(false), index(-1) {}
};

class Trie {
public:
    TrieNode* root;
    Trie() { root = new TrieNode(); }

    void insert(const string &word, int idx) {
        TrieNode* node = root;
        for (char ch : word) {
            char c = tolower(ch);
            if (!node->children[c]) node->children[c] = new TrieNode();
            node = node->children[c];
        }
        node->isEnd = true;
        node->index = idx;
    }

    void suggestions(const string &prefix, vector<int> &result) {
        TrieNode* node = root;
        for (char ch : prefix) {
            char c = tolower(ch);
            if (!node->children[c]) return;
            node = node->children[c];
        }
        dfs(node, result);
    }

    void dfs(TrieNode* node, vector<int> &result) {
        if (!node) return;
        if (node->isEnd) result.push_back(node->index);
        for (auto &p : node->children) dfs(p.second, result);
    }
};

// --- Commands list (safe, real implementations) ---
vector<string> commands = {
    "open notepad", "open calculator", "open paint", "open cmd", "open powershell",
    "open wordpad", "open browser", "open gmail", "open google", "open youtube",
    "show logs", "show history", "monitor usage", "scan system", "optimize settings",
    "open file", "open folder", "save file (mock)", "search data", "map route",
    "summarize text", "sort data", "share screen (open projection)", "show time", "show date",
    "open control panel", "open task manager", "open network settings", "open display settings", "open store"
};

// Forward declarations
void openNotepad();
void openCalculator();
void openPaint();
void openCMD();
void openPowerShell();
void openWordPad();
void openBrowser();
void openGmail();
void openGoogle();
void openYouTube();
void showLogs();
void showHistory();
void monitorUsage();
void scanSystem();
void optimizeSettings();
void openFile();      // mock: will open notepad for a file example
void openFolder();    // opens current folder in explorer
void saveFileMock();
void searchData();
void mapRoute();
void summarizeText();
void sortData();
void shareScreen();
void showTime();
void showDate();
void openControlPanel();
void openTaskManager();
void openNetworkSettings();
void openDisplaySettings();
void openStore();

// Map index -> function
vector<void(*)()> functions = {
    openNotepad, openCalculator, openPaint, openCMD, openPowerShell,
    openWordPad, openBrowser, openGmail, openGoogle, openYouTube,
    showLogs, showHistory, monitorUsage, scanSystem, optimizeSettings,
    openFile, openFolder, saveFileMock, searchData, mapRoute,
    summarizeText, sortData, shareScreen, showTime, showDate,
    openControlPanel, openTaskManager, openNetworkSettings, openDisplaySettings, openStore
};

// --- Implementations ---

void openNotepad() {
    launchURIorProgram(L"notepad.exe");
    appendLog("Opened Notepad");
    appendHistory("open notepad");
}

void openCalculator() {
    launchURIorProgram(L"calc.exe");
    appendLog("Opened Calculator");
    appendHistory("open calculator");
}

void openPaint() {
    launchURIorProgram(L"mspaint.exe");
    appendLog("Opened Paint");
    appendHistory("open paint");
}

void openCMD() {
    launchURIorProgram(L"cmd.exe");
    appendLog("Opened Command Prompt");
    appendHistory("open cmd");
}

void openPowerShell() {
    launchURIorProgram(L"powershell.exe");
    appendLog("Opened PowerShell");
    appendHistory("open powershell");
}

void openWordPad() {
    launchURIorProgram(L"write.exe");
    appendLog("Opened WordPad");
    appendHistory("open wordpad");
}

void openBrowser() {
    launchURIorProgram(L"https://www.google.com");
    appendLog("Opened Browser -> google.com");
    appendHistory("open browser");
}

void openGmail() {
    launchURIorProgram(L"https://mail.google.com");
    appendLog("Opened Gmail");
    appendHistory("open gmail");
}

void openGoogle() {
    launchURIorProgram(L"https://www.google.com");
    appendLog("Opened Google");
    appendHistory("open google");
}

void openYouTube() {
    launchURIorProgram(L"https://www.youtube.com");
    appendLog("Opened YouTube");
    appendHistory("open youtube");
}

// Show logs: read logs.txt and print to console (also open with notepad option)
void showLogs() {
    ifstream ifs("logs.txt");
    if (!ifs.is_open()) {
        cout << "\nNo logs found. (logs.txt missing)\n";
        appendLog("Attempted to show logs - none present");
        appendHistory("show logs (none)");
        return;
    }
    cout << "\n--- logs.txt ---\n";
    string line;
    while (getline(ifs, line)) cout << line << "\n";
    cout << "-----------------\n";
    ifs.close();
    appendLog("Displayed logs.txt content to console");
    appendHistory("show logs");
}

// Show history: read history.txt (same as logs)
void showHistory() {
    ifstream ifs("history.txt");
    if (!ifs.is_open()) {
        cout << "\nNo history found. (history.txt missing)\n";
        appendLog("Attempted to show history - none present");
        appendHistory("show history (none)");
        return;
    }
    cout << "\n--- history.txt ---\n";
    string line;
    while (getline(ifs, line)) cout << line << "\n";
    cout << "--------------------\n";
    ifs.close();
    appendLog("Displayed history.txt content to console");
    appendHistory("show history");
}

// Monitor usage: run 'tasklist' and print top portion (safe)
void monitorUsage() {
    cout << "\nGathering running processes (tasklist)...\n";
    string out = runCommandCapture("tasklist");
    if (out.size() == 0) cout << "No output from tasklist (permission issue?)\n";
    else {
        // print first ~30 lines to keep console readable
        istringstream iss(out);
        string line;
        int cnt = 0;
        while (cnt < 30 && getline(iss, line)) {
            cout << line << "\n";
            cnt++;
        }
        // append full output to logs for reviewer
        ofstream ofs("logs.txt", ios::app);
        ofs << "[" << getTimestamp() << "] tasklist output (partial):\n";
        ofs << out << "\n";
        ofs.close();
    }
    appendHistory("monitor usage");
    appendLog("Ran tasklist and logged output");
}

// Scan system (safe): run 'systeminfo' and print summary
void scanSystem() {
    cout << "\nCollecting basic system info (systeminfo)...\n";
    string out = runCommandCapture("systeminfo");
    if (out.size() == 0) cout << "No output from systeminfo (permission issue?)\n";
    else {
        // show first ~25 lines
        istringstream iss(out);
        string line;
        int cnt = 0;
        while (cnt < 25 && getline(iss, line)) {
            cout << line << "\n";
            cnt++;
        }
        ofstream ofs("logs.txt", ios::app);
        ofs << "[" << getTimestamp() << "] systeminfo output (partial):\n";
        ofs << out << "\n";
        ofs.close();
    }
    appendHistory("scan system");
    appendLog("Ran systeminfo and logged output");
}

// Optimize settings (safe): open Disk Cleanup (cleanmgr) UI
void optimizeSettings() {
    // Launch Disk Cleanup GUI (safe)
    launchURIorProgram(L"cleanmgr.exe");
    cout << "\nOpened Disk Cleanup (cleanmgr.exe). Use UI to continue.\n";
    appendHistory("optimize settings");
    appendLog("Opened Disk Cleanup (cleanmgr)");
}

// Open file: ask for path then open in Notepad
void openFile() {
    cout << "\nEnter file path to open with Notepad (or press Enter to open sample file): ";
    string path;
    getline(cin, path);
    if (path.empty()) {
        // create sample file if not exists
        ofstream ofs("sample_open.txt");
        ofs << "Sample file for SmartCommandAssistant.\nEdit this file to test open file action.\n";
        ofs.close();
        launchURIorProgram(L"notepad.exe");
        // Open specific sample file explicitly:
        ShellExecuteW(NULL, L"open", L"sample_open.txt", NULL, NULL, SW_SHOW);
        appendLog("Opened sample_open.txt in Notepad");
        appendHistory("open file (sample)");
        return;
    }
    // convert to wide string and open
    wstring wpath(path.begin(), path.end());
    ShellExecuteW(NULL, L"open", wpath.c_str(), NULL, NULL, SW_SHOW);
    appendLog("Opened file: " + path);
    appendHistory("open file: " + path);
}

// Open folder: open current folder in Explorer
void openFolder() {
    char cwd[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, cwd);
    string cur(cwd);
    wstring wcur(cur.begin(), cur.end());
    ShellExecuteW(NULL, L"open", wcur.c_str(), NULL, NULL, SW_SHOW);
    cout << "\nOpened current project folder in Explorer: " << cur << "\n";
    appendHistory("open folder");
    appendLog("Opened current folder: " + cur);
}

// Save file (mock): write a demo file to disk to show save action
void saveFileMock() {
    ofstream ofs("saved_demo.txt");
    ofs << "This file was saved by SmartCommandAssistant (demo save).\nTimestamp: " << getTimestamp() << "\n";
    ofs.close();
    cout << "\nSaved 'saved_demo.txt' in current folder.\n";
    appendHistory("save file (mock)");
    appendLog("Saved saved_demo.txt");
}

// Search data: open browser with Google search query asked from user
void searchData() {
    cout << "\nEnter search query: ";
    string q;
    getline(cin, q);
    if (q.empty()) { cout << "Empty query - cancelled.\n"; appendLog("search data - cancelled"); appendHistory("search data - cancelled"); return; }
    // form URL
    string url = "https://www.google.com/search?q=";
    // naive URL encode spaces -> +
    for (char &c : q) if (c == ' ') c = '+';
    wstring wurl(url.begin(), url.end());
    wurl += wstring(q.begin(), q.end());
    ShellExecuteW(NULL, L"open", wurl.c_str(), NULL, NULL, SW_SHOW);
    appendLog("Searched Google for: " + q);
    appendHistory("search data: " + q);
}

// Map route: open Google Maps with query
void mapRoute() {
    cout << "\nEnter place or route to open in Google Maps: ";
    string q;
    getline(cin, q);
    if (q.empty()) { cout << "Cancelled.\n"; appendLog("map route - cancelled"); appendHistory("map route - cancelled"); return; }
    string url = "https://www.google.com/maps/search/";
    for (char &c : q) if (c == ' ') c = '+';
    wstring wurl(url.begin(), url.end());
    wurl += wstring(q.begin(), q.end());
    ShellExecuteW(NULL, L"open", wurl.c_str(), NULL, NULL, SW_SHOW);
    appendLog("Opened Google Maps for: " + q);
    appendHistory("map route: " + q);
}

// Summarize text: simple summarization - read first N words from sample file
void summarizeText() {
    cout << "\nWill summarize 'docs/sample_text.txt' (create that file to test). Press Enter to continue...";
    cin.get();
    ifstream ifs("docs/sample_text.txt");
    if (!ifs) {
        cout << "sample_text.txt not found in docs/. Create docs/sample_text.txt for summarization demo.\n";
        appendLog("summarize text - sample not found");
        appendHistory("summarize text - sample not found");
        return;
    }
    string word;
    int limit = 60; // words
    int count = 0;
    cout << "\n--- Summary (first " << limit << " words) ---\n";
    while (count < limit && ifs >> word) {
        cout << word << " ";
        count++;
    }
    cout << "\n--- End Summary ---\n";
    ifs.close();
    appendLog("Summarized sample_text.txt (first 60 words)");
    appendHistory("summarize text");
}

// Sort data: read docs/data.txt (one number per line), sort, print & save sorted_output.txt
void sortData() {
    ifstream ifs("docs/data.txt");
    if (!ifs) {
        cout << "\nNo docs/data.txt found. Create it (one number per line) to demo sorting.\n";
        appendLog("sort data - data.txt missing");
        appendHistory("sort data - missing");
        return;
    }
    vector<long long> arr;
    long long val;
    while (ifs >> val) arr.push_back(val);
    ifs.close();
    if (arr.empty()) {
        cout << "\nNo numeric data found in docs/data.txt\n";
        appendLog("sort data - no numbers");
        appendHistory("sort data - empty");
        return;
    }
    sort(arr.begin(), arr.end());
    cout << "\nSorted data (ascending):\n";
    for (size_t i = 0; i < arr.size() && i < 100; ++i) cout << arr[i] << " ";
    cout << "\n";
    ofstream ofs("docs/sorted_output.txt");
    for (auto &x : arr) ofs << x << "\n";
    ofs.close();
    cout << "\nSaved sorted output to docs/sorted_output.txt\n";
    appendLog("Sorted data and saved to docs/sorted_output.txt");
    appendHistory("sort data");
}

// Share screen: open projection/settings page (safe)
void shareScreen() {
    // Opens the Projecting to this PC settings page or projection
    // Safe alternative: open ms-settings:project
    launchURIorProgram(L"ms-settings:project");
    cout << "\nOpened projection/settings page (ms-settings:project). Use Windows UI to continue.\n";
    appendLog("Opened projection settings (share screen hint)");
    appendHistory("share screen");
}

void showTime() {
    SYSTEMTIME st;
    GetLocalTime(&st);
    cout << "\nCurrent time: " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "\n";
    // small popup too
    wstring time = L"Time: " + intToWString(st.wHour) + L":" + intToWString(st.wMinute) + L":" + intToWString(st.wSecond);
    MessageBoxW(NULL, time.c_str(), L"Time", MB_OK);
    appendLog("Displayed current time");
    appendHistory("show time");
}

void showDate() {
    SYSTEMTIME st;
    GetLocalTime(&st);
    cout << "\nCurrent date: " << st.wDay << "/" << st.wMonth << "/" << st.wYear << "\n";
    wstring date = L"Date: " + intToWString(st.wDay) + L"/" + intToWString(st.wMonth) + L"/" + intToWString(st.wYear);
    MessageBoxW(NULL, date.c_str(), L"Date", MB_OK);
    appendLog("Displayed current date");
    appendHistory("show date");
}

void openControlPanel() {
    launchURIorProgram(L"control.exe");
    appendLog("Opened Control Panel");
    appendHistory("open control panel");
}

void openTaskManager() {
    launchURIorProgram(L"taskmgr.exe");
    appendLog("Opened Task Manager");
    appendHistory("open task manager");
}

void openNetworkSettings() {
    launchURIorProgram(L"ms-settings:network");
    appendLog("Opened Network Settings");
    appendHistory("open network settings");
}

void openDisplaySettings() {
    launchURIorProgram(L"ms-settings:display");
    appendLog("Opened Display Settings");
    appendHistory("open display settings");
}

void openStore() {
    launchURIorProgram(L"ms-windows-store:");
    appendLog("Opened Microsoft Store");
    appendHistory("open store");
}

// --- MAIN ---
int main() {
    // Ensure docs folder exists for demos
    CreateDirectoryA("docs", NULL);

    Trie trie;
    for (int i = 0; i < (int)commands.size(); ++i) trie.insert(commands[i], i);

    cout << "=== Smart Command Assistant (safe, real actions) ===\n";
    cout << "Type prefix (e.g., op, sh, mo) or 'exit' to quit.\n";

    while (true) {
        cout << "\nEnter prefix: ";
        string prefix;
        getline(cin, prefix);
        if (prefix == "exit") break;
        if (prefix.empty()) { cout << "Type something (e.g., 'op' for open).\n"; continue; }

        vector<int> matched;
        trie.suggestions(prefix, matched);

        if (matched.empty()) {
            cout << "No suggestions found for prefix: " << prefix << "\n";
            continue;
        }

        cout << "\nSuggestions:\n";
        for (int i = 0; i < (int)matched.size(); ++i) {
            cout << (i + 1) << ". " << commands[matched[i]] << "\n";
        }

        cout << "\nEnter choice number to run command (0 to cancel): ";
        string choiceLine;
        getline(cin, choiceLine);
        int choice = 0;
        try { choice = stoi(choiceLine); } catch (...) { choice = 0; }
        if (choice <= 0 || choice > (int)matched.size()) {
            cout << "Cancelled or invalid choice.\n";
            continue;
        }

        int cmdIndex = matched[choice - 1];
        // Execute the mapped function
        if (cmdIndex >= 0 && cmdIndex < (int)functions.size()) {
            // Run the function safely
            functions[cmdIndex]();
            cout << "\nAction completed. Check logs.txt/history.txt for details.\n";
        } else {
            cout << "Invalid command index.\n";
        }
    }

    cout << "\nExiting Smart Command Assistant. Bye.\n";
    return 0;
}
