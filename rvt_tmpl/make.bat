@echo off

set src=%1

rem echo %src%

if "%src%" == "" goto:_usage
if "%src%" == "clean" goto:_clean
if "%src%" == "fclean" goto:_fclean

cl %src%.cpp /clr /LD /Zi /AI "c:\Program Files\Autodesk\Revit 2019" 
copy %src%.dll "%APPDATA%\Autodesk\Revit\Addins\2019"
copy %src%.addin "%APPDATA%\Autodesk\Revit\Addins\2019"

rem for %%a in (%*) do echo %%a

:_usage
echo usage: make file [file ...]
goto:eof

:_clean
del /f /q *.obj *.pdb
goto:eof

:_fclean
del /f /q *.obj *.pdb *.dll
goto:eof
