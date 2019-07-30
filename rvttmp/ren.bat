forfiles /p .\ /m *.* /c "cmd /c move @path .\%1.@ext" 
