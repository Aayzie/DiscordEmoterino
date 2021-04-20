#include <fstream>
#include <iostream>
#include <string>
#include <tchar.h>
#include <thread>
#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <sstream>

const wchar_t* DiscordProcessName = L"Discord.exe";
const char* EmotePrefixesFileName = "EmotePrefixes.txt";
const int FindProcessDelay = 300;
const int ControlCheckDelay = 20;
const int OperationDelay = 5;

HANDLE mutex;

std::vector<std::string> _emotePrefixes;
std::string _savedClipboardText = "";

void ShowAlreadyExistsBox ();
void ShowStartBox ();
void LoadEmotePrefixes ();
bool IsDiscordFocused ();

void Emoterize ();
void LoadClipboardText ();
void SaveClipboardText ();
std::string GetOriginalMessage ();

std::string GetClipboardText ();
void SetClipboardText (const std::string& text);
void EnterMessage (const std::string& text);

bool IsFileExists (const char* fileName);
bool IsEmote (const std::string text);
bool IsPrefix (const std::string text, const std::string prefix);
void TrimEnd (std::string& text);

int main (int argc, char* argv[])
{
    mutex = CreateMutex (NULL, TRUE, L"DiscordEmoterino");
    if (GetLastError () == ERROR_ALREADY_EXISTS)
    {
        ShowAlreadyExistsBox ();
        return 0;
    }

    bool noStartBox = false;

    for (int i = 0; i < argc; ++i)
    {
        if (std::string (argv[i]).compare ("-nostartbox") == 0)
        {
            noStartBox = true;
        }
    }

    std::thread showStartBoxThread;
    if (!noStartBox)
    {
        showStartBoxThread = std::thread (ShowStartBox);
    }

    LoadEmotePrefixes ();

    while (true)
    {
        if (GetAsyncKeyState (VK_CONTROL) >= 0 || GetAsyncKeyState (VK_RETURN) >= 0)
        {
            Sleep (ControlCheckDelay);
            continue;
        }

        if (IsDiscordFocused ())
        {
            Emoterize ();
        }

        Sleep (FindProcessDelay);
    }

    CloseHandle (mutex);
}

void ShowAlreadyExistsBox ()
{
    MessageBox (nullptr, TEXT ("Discord Emoterino is already running!"), TEXT ("Discord Emoterino"), MB_OK);
}

void ShowStartBox ()
{
    MessageBox (nullptr, TEXT ("Discord Emoterino is now running~!\n\nYou may exit this program by running \"ExitDiscordEmoterino.bat\".\nIf you haven't yet, please read \"ReadMe.txt\"!"), TEXT ("Discord Emoterino"), MB_OK);
}

void LoadEmotePrefixes ()
{
    if (IsFileExists (EmotePrefixesFileName))
    {
        std::string emotePrefix;
        std::ifstream inFile;
        inFile.open (EmotePrefixesFileName);
        while (!inFile.eof ())
        {
            std::getline (inFile, emotePrefix);
            _emotePrefixes.push_back (emotePrefix);
        }
        inFile.close ();
    }
}

bool IsDiscordFocused ()
{
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof (PROCESSENTRY32);
    const auto snapshot = CreateToolhelp32Snapshot (TH32CS_SNAPPROCESS, NULL);
    if (!Process32First (snapshot, &entry))
    {
        CloseHandle (snapshot);
        return false;
    }
    do
    {
        if (!_tcsicmp (entry.szExeFile, DiscordProcessName))
        {
            CloseHandle (snapshot);
            HWND hwnd = GetForegroundWindow ();
            if (hwnd == NULL) continue;
            DWORD foregroundPid;
            if (GetWindowThreadProcessId (hwnd, &foregroundPid) == 0) continue;
            if (foregroundPid == entry.th32ProcessID)
            {
                return true;
            }
        }
    }
    while (Process32Next (snapshot, &entry));
    return false;
}

void Emoterize ()
{
    SaveClipboardText ();

    std::string message = GetOriginalMessage ();
    TrimEnd (message);

    if (message.size () == 0 || _savedClipboardText == message)
    {
        return;
    }

    bool isFirstWord = true;
    bool isGettingEmotes = false;
    std::string parsedMessage = "";
    std::string lastWord = "";
    std::stringstream ss (message);
    std::string word;
    while (ss >> word)
    {
        if (IsEmote (word))
        {
            if (!isGettingEmotes && !isFirstWord)
            {
                EnterMessage (parsedMessage);
                parsedMessage = "";
            }
            isGettingEmotes = true;
            parsedMessage += word + " ";
        }
        else
        {
            if (isGettingEmotes && !isFirstWord)
            {
                EnterMessage (parsedMessage);
                parsedMessage = "";
            }
            isGettingEmotes = false;
            parsedMessage += word + " ";
        }

        if (isFirstWord)
        {
            isFirstWord = false;
        }

        lastWord = word;
    }
    if (parsedMessage.length () > 0)
    {
        EnterMessage (parsedMessage);
    }
    else
    {
        EnterMessage (lastWord);
    }

    LoadClipboardText ();
}

void LoadClipboardText ()
{
    SetClipboardText (_savedClipboardText);
}

void SaveClipboardText ()
{
    _savedClipboardText = GetClipboardText ();
}

std::string GetOriginalMessage ()
{
    Sleep (OperationDelay);

    keybd_event ('A', 0x9C, 0, 0);
    keybd_event ('A', 0x9C, KEYEVENTF_KEYUP, 0);

    Sleep (OperationDelay);

    keybd_event ('C', 0x9C, 0, 0);
    keybd_event ('C', 0x9C, KEYEVENTF_KEYUP, 0);

    Sleep (OperationDelay);

    return GetClipboardText ();
}

std::string GetClipboardText ()
{
    HANDLE clip;
    std::string clip_text = "";
    if (OpenClipboard (NULL))
    {
        if (IsClipboardFormatAvailable (CF_TEXT))
        {
            clip = GetClipboardData (CF_TEXT);
            clip_text = (char*) clip;
        }
        CloseClipboard ();
    }
    return clip_text;
}

void SetClipboardText (const std::string& text)
{
    OpenClipboard (0);
    EmptyClipboard ();
    HGLOBAL hg = GlobalAlloc (GMEM_MOVEABLE, text.size () + 1);
    if (!hg)
    {
        CloseClipboard ();
        return;
    }
    memcpy (GlobalLock (hg), text.c_str (), text.size () + 1);
    GlobalUnlock (hg);
    SetClipboardData (CF_TEXT, hg);
    CloseClipboard ();
    GlobalFree (hg);
}

void EnterMessage (const std::string& message)
{
    SetClipboardText (message);

    Sleep (OperationDelay);

    keybd_event (VK_CONTROL, 0x9C, 0, 0);

    keybd_event ('V', 0x9C, 0, 0);
    keybd_event ('V', 0x9C, KEYEVENTF_KEYUP, 0);

    keybd_event (VK_CONTROL, 0x9C, KEYEVENTF_KEYUP, 0);

    Sleep (OperationDelay);

    keybd_event (VK_RETURN, 0x9C, 0, 0);
    keybd_event (VK_RETURN, 0x9C, KEYEVENTF_KEYUP, 0);

    Sleep (OperationDelay);
}

bool IsFileExists (const char* fileName)
{
    std::ifstream infile (fileName);
    return infile.good ();
}

bool IsEmote (const std::string text)
{
    if (text.length () == 0)
    {
        return false;
    }
    if ((text[0] == ':' && text[text.length () - 1] == ':'))
    {
        return true;
    }
    else
    {
        for (std::string& prefix : _emotePrefixes)
        {
            if (IsPrefix (text, prefix))
            {
                return true;
            }
        }
    }
    return false;
}

bool IsPrefix (const std::string text, const std::string prefix)
{
    if (text == prefix)
    {
        return false;
    }
    return text.rfind (prefix, 0) == 0;
}

void TrimEnd (std::string& text)
{
    text.erase (std::find_if (text.rbegin (), text.rend (), [] (unsigned char ch)
        {
            return !isspace (ch);
        }).base (), text.end ());
}