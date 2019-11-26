# cmd Makefile

CC = cl
CFLAGS = /nologo /clr /c /AI $(NET_ASMBS) /AI $(RVT_ASMBS) \
	/I$(LIB6) /I$(LIB4) /I.\$(INCLDIR) \
	/Fo.\$(OBJDIR)\ /FIc:\users\k.kozlov\samoletrvt\$(INCLDIR)\stdafx.h \
	/FUSystem.dll /FUSystem.Configuration.dll /FUSystem.Diagnostics.Debug.dll \
	/FUSystem.Windows.Forms.dll /FUSystem.Drawing.dll \
	/FUPresentationCore.dll /FUPresentationFramework.dll /FUWindowsBase.dll /FUSystem.Xaml.dll \
	/FURevitAPI.dll /FURevitAPIUI.dll \
	/FUSystem.Web.Extensions.dll \
!ifndef release
	/DDEBUG \
!endif
!if defined(warnings)
	/Wall /Zi
!endif

LD = link
LDFLAGS = /NOLOGO /DLL /SUBSYSTEM:WINDOWS /MACHINE:X64 \
	/LIBPATH:$(LIB1) /LIBPATH:$(LIB2) /LIBPATH:$(LIB3) /LIBPATH:$(LIB5) \
	/ASSEMBLYRESOURCE:$(RSFILE)

HDRS = $(SRCS:cpp=h)

SRCS = \
	app.cpp assembly_info.cpp cmdtmp.cpp\
	events.cpp failures.cpp family_params_cmd.cpp\
	family_params_form.cpp groups.cpp gui_utils.cpp\
	internal_storage.cpp params_data_parser.cpp\
	rvt_elem_to_json.cpp selectable_value.cpp\
	select_by_unique_id_cmd.cpp select_by_unique_id_form.cpp\
	select_values_cmd.cpp select_values_form.cpp\
	set_of_values.cpp shared_params_manager_cmd.cpp\
	shared_params_manager_form.cpp utils.cpp\
	validation.cpp web.cpp serialization_utils.cpp\
	binary_tree.cpp feedback_cmd.cpp\
	cut_openings_cmd.cpp

_OBJS = $(SRCS:.cpp=.obj)
OBJS = build\$(_OBJS: = build\)

NAME = samolet.dll

DSA_HDRS = $(DSA_SRCS:cpp=h)
DSA_SRCS = binary_tree.cpp
_DSA_OBJS = $(DSA_SRCS:cpp=obj)
DSA_OBJS = build\$(_DSA_OBJS: = build\)
DSA_NAME = dsa.dll

INCLDIR = include
SRCDIR = src
OBJDIR = build
RESDIR = resources
BINDIR = bin

RSFILE = .\resources\icons.resources
DST = "%APPDATA%\Autodesk\Revit\Addins\2020"
SERV = "\\sd-revit2020\RevitAddins"

LIB1 = "f:\Program Files\Windows Kits\10\Lib\10.0.18362.0\um\x64"
LIB2 = "f:\Program Files\Windows Kits\10\Lib\10.0.18362.0\ucrt\x64"
LIB3 = "f:\Program Files\Windows Kits\NETFXSDK\4.8\Lib\um\x64"
LIB4 = "f:\Program Files\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.20.27508\include"
LIB5 = "f:\Program Files\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.20.27508\lib\x64"
LIB6 = "f:\Program Files\Windows Kits\10\Include\10.0.18362.0\ucrt"

NET_ASMBS = "%PROGRAMFILES(x86)%\Reference Assemblies\Microsoft\Framework\.NETFramework\v4.6.1"
RVT_ASMBS= "%PROGRAMFILES%\Autodesk\Revit 2020"

usage :
	@echo usage: 
	@echo 1. To build in a debug mode, type in: nmake all
	@echo 2. To build in a release mode, type in: nmake all release=1
	@echo 3. To copy the files to %APPDATA%\Autodesk\Revit\Addins\2020,
	@echo type in: nmake cp
	@echo 4. To copy the files to the server, type in: nmake cpserv
	@echo 5. To rebuild, type in: nmake re
	@echo 6. To clean up, type in: nmake fclean

all : $(NAME)

$(NAME) : $(OBJS)
	$(LD) $(LDFLAGS) $** /OUT:$(BINDIR)\$@

{$(SRCDIR)}.cpp{$(OBJDIR)}.obj::
	$(CC) $(CFLAGS) $<
$(OBJS) : $(OBJDIR)

$(OBJDIR) :
	@if not exist $@ mkdir $@

$(DSA_NAME) : $(DSA_OBJS)
	$(LD) $(LDFLAGS) $** /OUT:$(BINDIR)\$@

{$(SRCDIR)}.cpp{$(OBJDIR)}.obj::
	$(CC) $(CFLAGS) $<
$(DSA_OBJS) :

clean :
	@if exist $(OBJDIR) rmdir/s/q $(OBJDIR)

fclean : clean
	@del $(BINDIR)\$(NAME)

re : fclean all

cp :
	copy $(BINDIR)\$(NAME) $(DST)
	copy $(BINDIR)\*.addin $(DST)
	copy $(BINDIR)\*.config $(DST)

cpserv :
	copy $(BINDIR)\$(NAME) $(SERV)
	copy $(BINDIR)\*.addin $(SERV)
	copy $(BINDIR)\*.config $(SERV)

.PHONY : all clean fclean re cp cpserv
