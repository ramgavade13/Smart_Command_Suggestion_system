
#include <iostream>
#include <vector>
#include <unordered_map>
#include <windows.h>
#include <string>
#include <algorithm>
#include <shellapi.h>
#include <sstream> // Added for int to wstring conversion

using namespace std;

// Helper to convert int to wstring
wstring intToWString(int value) {
    wstringstream wss;
    wss << value;
    return wss.str();
}

// Expanded list of commands
vector<string> commands = {
    "open notepad", "open calculator", "open paint", "open cmd", "open powershell",
    "open wordpad", "open browser", "open gmail", "open google", "open youtube",
    "shutdown system", "restart system", "lock system", "maximize window", "minimize window",
    "open file", "open folder", "save file", "search data", "map route",
    "monitor usage", "system restore", "optimize settings", "mail to client", "send email",
    "summarize text", "scan system", "share screen", "show logs", "sort data",
    "show time", "show date", "open control panel", "open device manager", "open task manager",
    "open network settings", "open display settings", "open windows settings", "open store", "open calendar"
};

// Trie Node
class TrieNode {
public:
    bool isEnd;
    unordered_map<char, TrieNode*> children;
    int index;

    TrieNode() { isEnd=false; index=-1; }
};

class Trie {
public:
    TrieNode* root;
    Trie() { root = new TrieNode(); }

    void insert(string word, int idx) {
        TrieNode* node = root;
        for(char c : word) {
            c = tolower(c);
            if(!node->children[c]) node->children[c] = new TrieNode();
            node = node->children[c];
        }
        node->isEnd = true;
        node->index = idx;
    }

    void suggestions(string prefix, vector<int>& result) {
        TrieNode* node = root;
        for(char c : prefix) {
            c = tolower(c);
            if(!node->children[c]) return;
            node = node->children[c];
        }
        dfs(node, result);
    }

    void dfs(TrieNode* node, vector<int>& result) {
        if(node->isEnd) result.push_back(node->index);
        for(auto &p: node->children) dfs(p.second, result);
    }
};

// ==== Command Functions ====
void openNotepad() { ShellExecuteW(NULL,L"open",L"notepad.exe",NULL,NULL,SW_SHOW); }
void openCalculator() { ShellExecuteW(NULL,L"open",L"calc.exe",NULL,NULL,SW_SHOW); }
void openPaint() { ShellExecuteW(NULL,L"open",L"mspaint.exe",NULL,NULL,SW_SHOW); }
void openCMD() { ShellExecuteW(NULL,L"open",L"cmd.exe",NULL,NULL,SW_SHOW); }
void openPowerShell() { ShellExecuteW(NULL,L"open",L"powershell.exe",NULL,NULL,SW_SHOW); }
void openWordPad() { ShellExecuteW(NULL,L"open",L"write.exe",NULL,NULL,SW_SHOW); }
void openBrowser() { ShellExecuteW(NULL,L"open",L"https://www.google.com",NULL,NULL,SW_SHOW); }
void openGmail() { ShellExecuteW(NULL,L"open",L"https://mail.google.com",NULL,NULL,SW_SHOW); }
void openGoogle() { ShellExecuteW(NULL,L"open",L"https://www.google.com",NULL,NULL,SW_SHOW); }
void openYouTube() { ShellExecuteW(NULL,L"open",L"https://www.youtube.com",NULL,NULL,SW_SHOW); }
void shutdownSystem() { system("shutdown /s /t 5"); }
void restartSystem() { system("shutdown /r /t 5"); }
void lockSystem() { LockWorkStation(); }
void maximizeWindow() { ShowWindow(GetConsoleWindow(),SW_MAXIMIZE); }
void minimizeWindow() { ShowWindow(GetConsoleWindow(),SW_MINIMIZE); }
void openFile() { MessageBoxW(NULL,L"Simulating Open File...",L"Action",MB_OK); }
void openFolder() { MessageBoxW(NULL,L"Simulating Open Folder...",L"Action",MB_OK); }
void saveFile() { MessageBoxW(NULL,L"Saving file...",L"Action",MB_OK); }
void searchData() { ShellExecuteW(NULL,0,L"https://www.google.com",0,0,SW_SHOW); }
void mapRoute() { ShellExecuteW(NULL,0,L"https://www.google.com/maps",0,0,SW_SHOW); }
void monitorUsage() { MessageBoxW(NULL,L"Monitoring CPU/Memory usage...",L"Action",MB_OK); }
void systemRestore() { MessageBoxW(NULL,L"System restore simulated.",L"Action",MB_OK); }
void optimizeSettings() { MessageBoxW(NULL,L"Optimizing system settings...",L"Action",MB_OK); }
void mailToClient() { ShellExecuteW(NULL,0,L"mailto:client@example.com",0,0,SW_SHOW); }
void sendEmail() { ShellExecuteW(NULL,0,L"mailto:test@example.com?subject=Hello",0,0,SW_SHOW); }
void summarizeText() { MessageBoxW(NULL,L"Summarizing text...",L"Action",MB_OK); }
void scanSystem() { MessageBoxW(NULL,L"Scanning system...",L"Action",MB_OK); }
void shareScreen() { MessageBoxW(NULL,L"Sharing screen (mock)...",L"Action",MB_OK); }
void showLogs() { MessageBoxW(NULL,L"Showing logs...",L"Action",MB_OK); }
void sortData() { MessageBoxW(NULL,L"Sorting data...",L"Action",MB_OK); }

// Fixed showTime and showDate
void showTime() {
    SYSTEMTIME st; GetLocalTime(&st);
    wstring time = L"Time: " + intToWString(st.wHour) + L":" + intToWString(st.wMinute) + L":" + intToWString(st.wSecond);
    MessageBoxW(NULL,time.c_str(),L"Time",MB_OK);
}

void showDate() {
    SYSTEMTIME st; GetLocalTime(&st);
    wstring date = L"Date: " + intToWString(st.wDay) + L"/" + intToWString(st.wMonth) + L"/" + intToWString(st.wYear);
    MessageBoxW(NULL,date.c_str(),L"Date",MB_OK);
}

void openControlPanel() { ShellExecuteW(NULL,L"open",L"control.exe",NULL,NULL,SW_SHOW); }
void openDeviceManager() { ShellExecuteW(NULL,L"open",L"devmgmt.msc",NULL,NULL,SW_SHOW); }
void openTaskManager() { ShellExecuteW(NULL,L"open",L"taskmgr.exe",NULL,NULL,SW_SHOW); }
void openNetworkSettings() { ShellExecuteW(NULL,L"open",L"ms-settings:network",NULL,NULL,SW_SHOW); }
void openDisplaySettings() { ShellExecuteW(NULL,L"open",L"ms-settings:display",NULL,NULL,SW_SHOW); }
void openWindowsSettings() { ShellExecuteW(NULL,L"open",L"ms-settings:",NULL,NULL,SW_SHOW); }
void openStore() { ShellExecuteW(NULL,L"open",L"ms-windows-store:",NULL,NULL,SW_SHOW); }
void openCalendar() { ShellExecuteW(NULL,L"open",L"outlookcal:",NULL,NULL,SW_SHOW); }

vector<void(*)()> functions = {
    openNotepad, openCalculator, openPaint, openCMD, openPowerShell,
    openWordPad, openBrowser, openGmail, openGoogle, openYouTube,
    shutdownSystem, restartSystem, lockSystem, maximizeWindow, minimizeWindow,
    openFile, openFolder, saveFile, searchData, mapRoute,
    monitorUsage, systemRestore, optimizeSettings, mailToClient, sendEmail,
    summarizeText, scanSystem, shareScreen, showLogs, sortData,
    showTime, showDate, openControlPanel, openDeviceManager, openTaskManager,
    openNetworkSettings, openDisplaySettings, openWindowsSettings, openStore, openCalendar
};

// ==== MAIN ====
int main() {
    Trie trie;
    for(int i=0;i<commands.size();++i) trie.insert(commands[i],i);

    while(true) {
        system("cls");
        cout << "=== AI-like Smart Command Assistant ===\n";
        cout << "Type prefix (e.g., op, sh, ma) or 'exit' to quit:\n";
        cout << "Enter prefix: ";
        string prefix;
        getline(cin,prefix);
        if(prefix=="exit") break;

        vector<int> matched;
        trie.suggestions(prefix,matched);

        if(matched.empty()) cout << "\nNo suggestions found.\n";
        else {
            cout << "\nSuggestions:\n";
            for(int i=0;i<matched.size();++i) cout << i+1 << ". " << commands[matched[i]] << endl;

            int choice;
            cout << "\nEnter choice number to run command: ";
            cin >> choice;
            cin.ignore();
            if(choice>0 && choice<=matched.size()) functions[matched[choice-1]]();
            else cout << "Invalid choice.\n";
        }
        cout << "\nPress Enter to continue...";
        cin.get();
    }
    return 0;
}
