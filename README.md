# Discord Emoterino
A lightweight, seamless program that separates emotes from your messages on Discord

![DiscordEmoterinoDemo3](https://user-images.githubusercontent.com/16715946/115318781-06c83700-a1b1-11eb-946a-576c109d194d.gif)

## Download (Windows-only)

https://github.com/Aayzie/DiscordEmoterino/releases/tag/1.0.0.1

## Instructions
Run <b>DiscordEmoterino.exe</b> to manually start the program.

Run <b>ExitDiscordEmoterino.bat</b> to quit the program.

Run <b>EnableRunOnStartup.bat</b> to have this program run on system startup.

Run <b>DisableRunOnStartup.bat</b> to remove it from system startup.

### Usage

Simply <b>hold down the CTRL key</b> then press <b>Enter</b> to automatically separate the emotes from your message.

For example, try typing <b>":arrow_up: TESTING MESSAGE :arrow_down:"</b>, then post the message while <b>holding down the CTRL key</b>.
The Arrow Up emote should be sent first, then "TESTING MESSAGE", then the Arrow Down emote.

### Prefix Emotes

If you want to use emotes that start with a prefix instead of a colon, specify the prefix in <b>EmotePrefixes.txt</b> in the same folder as the executable file.
e.g. in order for the program to know that <b>forsenLUL</b> is an emote, you must put <b>"forsen"</b> in <b>EmotePrefixes.txt</b>.

## Notes

-This program is only activated when Discord is <i>in focus</i>.

-You can manually check if the program is running by searching for <b>DiscordEmoterino.exe</b> in <b>Task Manager</b>.

-You can manually check if the program is on <b>system startup</b> by opening the <b>Registry Editor</b> and then going to <b>"HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run"</b>

-Feel free to let me know if you encounter a bug or something.

-If you don't trust the release files, you can compile it yourself Okayeg
