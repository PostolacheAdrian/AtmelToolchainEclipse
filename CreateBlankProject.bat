@set /p Projectname="Project name: "
@mkdir %~dp0workspace\%Projectname%
@xcopy /E %~dp0tools\BlankTemplate\*.* %~dp0workspace\%Projectname%
