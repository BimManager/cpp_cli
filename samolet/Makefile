# cmd Makefile

nCC = cl
CFLAGS = /nologo /clr /c /AI $(NET_ASMBS) /AI $(RVT_ASMBS) /Wall /Zi /DDEBUG /I$(LIB6) /I$(LIB4)
LD = link
LDFLAGS = /NOLOGO /DLL /SUBSYSTEM:WINDOWS /MACHINE:X64 \
	/LIBPATH:$(LIB1) /LIBPATH:$(LIB2) /LIBPATH:$(LIB3) /LIBPATH:$(LIB5) \
	/ASSEMBLYRESOURCE:$(RSFILE)

HDRS = stdafx.h app.h setSpCmd.h setParamsCmdForm.h 
SRCS = app.cpp setSpCmd.cpp setParamsCmdForm.cpp
OBJS = app.obj setSpCmd.obj setParamsCmdForm.obj
NAME = samolet.dll

RSDIR = resources
CMDDIR = commands
APPDIR = applications

RSFILE = .\$(RSDIR)\icons.resources
DST = "%APPDATA%\Autodesk\Revit\Addins\2020"

LIB1 = "f:\Program Files\Windows Kits\10\Lib\10.0.18362.0\um\x64"
LIB2 = "f:\Program Files\Windows Kits\10\Lib\10.0.18362.0\ucrt\x64"
LIB3 = "f:\Program Files\Windows Kits\NETFXSDK\4.8\Lib\um\x64"
LIB4 = "f:\Program Files\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.20.27508\include"
LIB5 = "f:\Program Files\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.20.27508\lib\x64"
LIB6 = "f:\Program Files\Windows Kits\10\Include\10.0.18362.0\ucrt"

NET_ASMBS = "%PROGRAMFILES(x86)%\Reference Assemblies\Microsoft\Framework\.NETFramework\v4.6.1"
RVT_ASMBS= "%PROGRAMFILES%\Autodesk\Revit 2020"

all : $(NAME)

$(NAME) : $(OBJS)
	$(LD) $(LDFLAGS) $** /OUT:$(NAME)

$(OBJS)	: $(SRCS)
	$(CC) $(CFLAGS) $**

clean :
	@del/f/q *.obj *.pdb

fclean : clean
	@del $(NAME)

re : fclean all

cp :
	copy *.dll $(DST)
	copy *.addin $(DST)
	copy *.config $(DST)

.PHONY : all clean fclean re cp
