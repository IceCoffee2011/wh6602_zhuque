@echo off

Rem ���ķ�����
start .\CenterServer.exe   .\centerserver.ini

Rem �ȴ�3�� ping����3�β�ʹ��Ĭ�ϵĴ���,nul�ǲ���ʾping��Ϣ:
@ping -n 3 127.0.0.1>nul

rem ��¼������
start .\LogonServer.exe   .\logonserver.ini
@ping -n 3 127.0.0.1>nul

start .\ServiceLoader.exe   .\gameserver.ini
@ping -n 3 127.0.0.1>nul

echo �������
pause


