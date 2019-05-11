set PATH=%windir%\system32;
set PATH=%PATH%;%~dp0tools\shellutils;%~dp0tools\avr8\avr8-gnu-toolchain\bin
cd %~dp0eclipse
start eclipse.exe -data ..\workspace