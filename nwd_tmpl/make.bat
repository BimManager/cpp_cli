@echo off

set SRCS=nwd_tmpl.cpp
set OUT=nwd_tmpl

:: set NWD_PLUGINS="%PROGRAMFILES%\Autodesk\Navisworks Manage 2019\Plugins"
:: set NWD_APIS="%PROGRAMFILES%\Autodesk\Navisworks Manage 2019"

set DST=X:\kkozlov
set NWD_APIS=X:\kkozlov

if "%1"=="" goto compile
if "%1"=="clean" goto clean
if "%1"=="fclean" goto fclean
if "%1"=="wipeout" goto wipeout

:compile
cl /clr /Zi %SRCS% /AI %NWD_APIS% /link /DLL /out:%OUT%.dll
if not exist %DST%\%OUT% (
    mkdir %DST%\%OUT%
)
copy %OUT%.dll %DST%\%OUT%
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
rmdir /q /s %DST%\%OUT%
goto:eof
