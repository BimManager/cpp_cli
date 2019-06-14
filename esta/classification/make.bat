@echo off

set src=%1

rem echo %src%

if "%src%" == "" goto:_usage
if "%src%" == "clean" goto:_clean
if "%src%" == "fclean" goto:_fclean

set LIB="c:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.21.27702\lib\x64"

set LIB2="c:\Program Files (x86)\Windows Kits\NETFXSDK\4.6.1\Lib\um\x64"
set LIB3="c:\Program Files (x86)\Windows Kits\10\Lib\10.0.17763.0\um\x64"
set LIB4="c:\Program Files (x86)\Windows Kits\10\Lib\10.0.17763.0\ucrt\x64"

rem cl %src%.cpp /clr /LD /Zi /AI "x:\kkozlov" 

cl %src%.cpp /clr /Zi /AI x:\kkozlov /link /DLL /LIBPATH:%LIB% ^
        /LIBPATH:%LIB2% /LIBPATH:%LIB3% /LIBPATH:%LIB4% /OUT:%DLL_NAME%.dll 

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
