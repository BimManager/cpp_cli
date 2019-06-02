echo OFF

set src=%1

rem goto:EOF

cl %src%.cpp /clr /LD /AI "c:\Program Files\Autodesk\Revit 2019" 
copy %src%.dll "%APPDATA%\Autodesk\Revit\Addins\2019"
copy %src%.addin "%APPDATA%\Autodesk\Revit\Addins\2019"

rem for %%a in (%*) do echo %%a

:EOF