@echo off

set RVT_APIS="X:\kkozlov"
set DST_PATH="X:\kkozlov"

set SRCS=ribbon.cpp
set NAME=ribbon

if "%1" == "" goto:_compile
if "%1" == "clean" goto:_clean
if "%1" == "fclean" goto:_fclean

:_compile
cl %SRCS% /clr /Zi /AI %RVT_APIS% /link /DLL /OUT:%NAME%.dll
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
