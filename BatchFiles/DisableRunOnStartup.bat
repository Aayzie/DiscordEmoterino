@echo off

set DiscordEmoterinoStartupKeyName=DiscordEmoterino

reg delete HKCU\Software\Microsoft\Windows\CurrentVersion\Run\ /v %DiscordEmoterinoStartupKeyName%

pause