# Discord Emoterino
A lightweight program that separates emotes from your messages on Discord

![DiscordEmoterinoDemo2](https://user-images.githubusercontent.com/16715946/115314822-90273b80-a1a8-11eb-93a1-fc3b6599b770.gif)

## Download (Windows)

https://github.com/Aayzie/DiscordEmoterino/releases/tag/1.0.0.1

## Instructions
-Run <b>DiscordEmoterino.exe</b> to manually start the program.

-Run <b>ExitDiscordEmoterino.bat</b> to manually quit the program.

-Run <b>EnableRunOnStartup.bat</b> to have this program run on System Startup.

-Run <b>DisableRunOnStartup.bat</b> to remove it from System Startup.

## Usage

Simply <b>hold down the CTRL key</b> then press <b>Enter</b> to automatically separate the emotes from your message.

For example, try typing <b>":arrow_up: TESTING MESSAGE :arrow_down:"</b>, then post the message while <b>holding down the CTRL key</b>.
The Arrow Up emote should be sent first, then "TESTING MESSAGE", then the Arrow Down emote.

## Prefix Emotes

If you want to use emotes that start with a prefix instead of two colons, specify the prefix in <b>EmotePrefixes.txt</b> in the same folder as the executable file.
For example, in order for the program to know that <b>forsenLUL</b> is an emote, you must put <b>"forsen"</b> in <b>EmotePrefixes.txt</b>.

## Notes

-This program is only activated when Discord is <i>in focus</i>.

-You can manually check if the program is running by searching for <b>DiscordEmoterino.exe</b> in <b>Task Manager</b>.

-You can manually check if the program is on <b>System Startup</b> by opening the <b>Registry Editor</b> and then going to <b>"HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run"</b>

-Feel free to let me know if you encounter a bug or something.

## これはウイルスですか？

いいえ。公式リリースのファイルが信頼できない場合は、自分でコンパイルできます。
