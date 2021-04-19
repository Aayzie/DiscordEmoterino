@echo off

set DiscordEmoterinoProcessName=DiscordEmoterino.exe

taskkill /im "%DiscordEmoterinoProcessName%" /f 2>nul

if %errorlevel% == 0 (
	echo Discord Emoterino was successfully closed.
) else (
	if %errorlevel% == 128  (
		echo Discord Emoterino is not running.
	) ELSE (
		echo Unable to close Discord Emoterino. Error Code: %errorlevel%
	)
)

pause