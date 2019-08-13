@echo off   
if "%1"=="" goto usage
forfiles /p .\ /m *.* /c ^
"cmd /c if not @ext==\"\" ( if not @ext==\"dll\" ( if not @ext==\"bat\" (move @path .\%1.@ext )))" 
goto:eof

:usage
echo setfnames newprefix