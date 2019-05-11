@set /p Projectname="Project name: "
@mkdir %~dp0workspace\%Projectname%
@xcopy /E %~dp0tools\PlatformTemplate\*.* %~dp0workspace\%Projectname%
