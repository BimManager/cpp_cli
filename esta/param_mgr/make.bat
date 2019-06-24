@echo off

set out="param_manager"

if "%1" == "" goto:_usage
if "%1" == "clean" goto:_clean
if "%1" == "fclean" goto:_fclean

set LIB1="c:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.21.27702\lib\x64"

set LIB2="c:\Program Files (x86)\Windows Kits\NETFXSDK\4.6.1\Lib\um\x64"
set LIB3="c:\Program Files (x86)\Windows Kits\10\Lib\10.0.17763.0\um\x64"
set LIB4="c:\Program Files (x86)\Windows Kits\10\Lib\10.0.17763.0\ucrt\x64"

rem cl %src%.cpp /clr /LD /Zi /AI "x:\kkozlov" 

cl %1 %2 %3 /clr /Zi /AI x:\kkozlov /link /DLL ^
        /LIBPATH:%LIB1% ^
        /LIBPATH:%LIB2% ^
        /LIBPATH:%LIB3% ^
        /LIBPATH:%LIB4% ^
        /OUT:%out%.dll 

copy %out%.dll "x:\kkozlov"
:: copy %src%.addin "x:\kkozlov"

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
