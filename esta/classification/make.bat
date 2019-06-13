@echo off

set src=%1

rem echo %src%

if "%src%" == "" goto:_usage
if "%src%" == "clean" goto:_clean
if "%src%" == "fclean" goto:_fclean

rem cl %src%.cpp /clr /LD /Zi /AI "x:\kkozlov" 
cl %src%.cpp /clr /Zi /AI x:\kkozlov /link /DLL /OUT:%DLL_NAME%.dll
copy %src%.dll "x:\kkozlov"
copy %src%.addin "x:\kkozlov"

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
