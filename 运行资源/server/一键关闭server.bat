:functionstops

rem 6602���

	call taskkill /im ServiceLoader.exe /t /f 
	call taskkill /im ControlServer.exe /t /f 
	call taskkill /im LogonServer.exe /t /f 
	call taskkill /im CenterServer.exe /t /f 

goto :eof