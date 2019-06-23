@echo off

set NWD_PLUGINS="%PROGRAMFILES%\Autodesk\Navisworks Manage 2019\Plugins"
set NWD_APIS="%PROGRAMFILES%\Autodesk\Navisworks Manage 2019"
set OUT=nwd_tmpl

if "%1"=="" goto usage
if "%1"=="clean" goto clean
if "%1"=="fclean" goto fclean
if "%1"=="wipeout" goto wipeout
if "BUILD"=="" goto error1

cl /clr /Zi %1.cpp /AI %NWD_APIS% /link /DLL /out:%OUT%.dll

if not exist %NWD_PLUGINS%\%OUT% (
    mkdir %NWD_PLUGINS%\%OUT%
)
copy %OUT%.dll %NWD_PLUGINS%\%OUT%

goto:eof

:usage
echo usage: make sourcefile (without extention)
goto:eof

:clean
del /f /q *.obj *.pdb
goto:eof

:fclean
del /f /q *.obj *.pdb *.dll
goto:eof

:wipeout
rmdir /q /s %NWD_PLUGINS%\%OUT%
goto:eof

:error1
echo set BUILD environment variable
goto:eof
