@echo off

set DiscordEmoterinoStartupKeyName=DiscordEmoterino
set DiscordEmoterinoExecutablePath=%CD%\DiscordEmoterino.exe -nostartbox

reg add HKCU\Software\Microsoft\Windows\CurrentVersion\Run\ /v %DiscordEmoterinoStartupKeyName% /t REG_SZ /d "%DiscordEmoterinoExecutablePath%"

pause