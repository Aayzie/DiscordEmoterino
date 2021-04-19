#include <iostream>
#include <windows.h>
#include <thread>
#include <tlhelp32.h>
#include <tchar.h>

const wchar_t* DiscordProcessName = L"Discord.exe";
const int FindProcessDelay = 300;
const int ControlCheckDelay = 20;
const int OperationDelay = 5;

bool _isExecuting = false;
std::string _savedClipboardText = "";

void ShowStartBox ();
bool IsDiscordFocused ();

void Update ();
void LoadClipboardText ();
void SaveClipboardText ();
std::string GetOriginalMessage ();
int GetPreEmotes (const std::string& message, std::string& preEmotes);
int GetPostEmotes (const std::string& message, std::string& postEmotes);
const std::string GetTrimmedMessage (std::string message, int preEmotesLastIndex, int postEmotesLastIndex);
void PostMessagesSequentially (const std::string& preEmotes, const std::string& trimmedMessage, const std::string& postEmotes);

std::string GetClipboardText ();
void SetClipboardText (const std::string& text);
void TrimEnd (std::string& text);
void PostMessage (const std::string& text);

int main(int argc, char* argv[])
{
    bool noStartBox = false;

    for (int i = 0; i < argc; ++i)
    {
        if (std::string(argv[i]).compare("-nostartbox") == 0)
        {
            noStartBox = true;
        }
    }

    std::thread showStartBoxThread;
    if (!noStartBox)
    {
        showStartBoxThread = std::thread (ShowStartBox);
    }

    while (true)
    {
        if (GetAsyncKeyState (VK_CONTROL) >= 0 || GetAsyncKeyState (VK_RETURN) >= 0)
        {
            Sleep (ControlCheckDelay);
            continue;
        }

        if (IsDiscordFocused ())
        {
            Update ();
        }

        Sleep (FindProcessDelay);
    }
}

void ShowStartBox ()
{
    MessageBox (nullptr, TEXT ("Discord Emoterino is now running~!"), TEXT ("Discord Emoterino"), MB_OK);
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

void Update ()
{
    SaveClipboardText ();

    std::string message = GetOriginalMessage ();
    TrimEnd (message);

    if (message.size () == 0 || _savedClipboardText == message)
    {
        return;
    }

    std::string preEmotes = "";
    int preEmotesLastIndex = GetPreEmotes (message, preEmotes);

    std::string postEmotes = "";
    int postEmotesLastIndex = GetPostEmotes (message, postEmotes);

    const std::string trimmedMessage = GetTrimmedMessage (message, preEmotesLastIndex, postEmotesLastIndex);

    PostMessagesSequentially (preEmotes, trimmedMessage, postEmotes);

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

int GetPreEmotes (const std::string& message, std::string& preEmotes)
{
    int preEmotesLastIndex = 0;

    if (message[0] != ':')
    {
        return preEmotesLastIndex;
    }

    bool isGettingEmote = true;
    int emoteBeginIndex = 0;
    for (int i = 1; i < message.length (); i++)
    {
        if (isGettingEmote && message[i] == ':')
        {
            preEmotes += message.substr (emoteBeginIndex, i - emoteBeginIndex + 1) + " ";
            preEmotesLastIndex = i + 1;
            isGettingEmote = false;
        }
        else if (!isGettingEmote)
        {
            if (message[i] != ':' && !isspace (message[i]))
            {
                break;
            }
            else if (message[i] == ':')
            {
                emoteBeginIndex = i;
                isGettingEmote = true;
            }
        }
    }

    return preEmotesLastIndex;
}

int GetPostEmotes (const std::string& message, std::string& postEmotes)
{
    int postEmotesLastIndex = message.length ();

    if (message[message.length () - 1] != ':')
    {
        return postEmotesLastIndex;
    }

    bool isGettingEmote = true;
    int emoteEndIndex = message.length () - 1;
    for (int i = message.length () - 2; i >= 0; i--)
    {
        if (isGettingEmote && message[i] == ':')
        {
            postEmotes += message.substr (i, emoteEndIndex - i + 1) + " ";
            postEmotesLastIndex = i;
            isGettingEmote = false;
        }
        else if (!isGettingEmote)
        {
            if (message[i] != ':' && !isspace (message[i]))
            {
                break;
            }
            else if (message[i] == ':')
            {
                emoteEndIndex = i;
                isGettingEmote = true;
            }
        }
    }

    return postEmotesLastIndex;
}

const std::string GetTrimmedMessage (std::string message, int preEmotesLastIndex, int postEmotesLastIndex)
{
    return message.substr (preEmotesLastIndex, postEmotesLastIndex - preEmotesLastIndex) + " ";
}

void PostMessagesSequentially (const std::string& preEmotes, const std::string& trimmedMessage, const std::string& postEmotes)
{
    if (preEmotes.size () > 0)
    {
        PostMessage (preEmotes);
        Sleep (OperationDelay);
    }
    PostMessage (trimmedMessage);
    Sleep (OperationDelay);
    if (preEmotes != postEmotes && postEmotes.size () > 0)
    {
        PostMessage (postEmotes);
        Sleep (OperationDelay);
    }
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

void TrimEnd (std::string& text)
{
    text.erase (std::find_if (text.rbegin (), text.rend (), [] (unsigned char ch)
    {
        return !isspace (ch);
    }).base (), text.end ());
}

void PostMessage (const std::string& message)
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
}