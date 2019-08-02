@echo off
if "%1"=="" (
    if "%2"=="" (
        goto usage
    )
)
forfiles /p .\ /c "cmd /c if @fname==\"%1\" (move @path .\%2.@ext)" 
goto:eof
:usage
echo usage: ren oldname newname
