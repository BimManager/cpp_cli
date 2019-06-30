@echo off

set RVT_APIS="%PROGRAMFILES%\Autodesk\Revit 2019"
set DST_PATH="%APPDATA%\Autodesk\Revit\Addins\2019"

set SRCS=rvt_tmpl.cpp
set NAME=rvt_tmpl

if "%1" == "" goto:_compile
if "%1" == "clean" goto:_clean
if "%1" == "fclean" goto:_fclean

:_compile
cl %SRCS% /clr /LD /Zi /AI %RVT_APIS%
copy %NAME%.dll %DST_PATH%
copy %NAME%.addin %DST_PATH%
goto:eof

:_usage
echo usage: make file [file ...]
goto:eof

:_clean
del /f /q *.obj *.pdb
goto:eof

:_fclean
del /f /q *.obj *.pdb *.dll
goto:eof
