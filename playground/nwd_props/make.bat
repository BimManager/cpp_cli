@echo off

set NWD_PLUGINS="%PROGRAMFILES%\Autodesk\Navisworks Manage 2019\Plugins"
set NWD_APIS="%PROGRAMFILES%\Autodesk\Navisworks Manage 2019"
set SRCS=nwd_props.cpp
set NAME=nwd_props

if "%1"=="" goto compile
if "%1"=="clean" goto clean
if "%1"=="fclean" goto fclean
if "%1"=="wipeNAME" goto wipeNAME
if "BUILD"=="" goto error1

:compile
cl /clr /Zi %SRCS% /AI %NWD_APIS% /link /DLL /OUT:%NAME%.dll

if not exist %NWD_PLUGINS%\%NAME% (
    mkdir %NWD_PLUGINS%\%NAME%
)
copy %NAME%.dll %NWD_PLUGINS%\%NAME%

goto:eof

:usage
echo usage: make sourcefile (withNAME extention)
goto:eof

:clean
del /f /q *.obj *.pdb
goto:eof

:fclean
del /f /q *.obj *.pdb *.dll *.exe
goto:eof

:wipeNAME
rmdir /q /s %NWD_PLUGINS%\%NAME%
goto:eof

:error1
echo set BUILD environment variable
goto:eof
