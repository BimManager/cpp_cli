# Makefile

CC = cl
LD = link

CFLAGS = /clr /AI $(LIBS) /c /I$(VMP) /I$(PMP) /I$(RBP)
LDFLAGS = /DLL /ASSEMBLYRESOURCE:$(RS)

HDRS = ribbon.h param_manager.h form1.h viewsmgr.h viewsmgr_form.h
SRCS = ribbon.cpp param_manager.cpp form1.cpp viewsmgr.cpp viewsmgr_form.cpp
OBJS = ribbon.obj param_manager.obj form1.obj viewsmgr.obj viewsmgr_form.obj
RS = icons.resources
NAME = esta.dll

NETASMS = "%PROGRAMFILES(x86)%\Reference Assemblies\Microsoft\Framework\.NETFramework\v4.6.1"
LIBS = "X:\kkozlov"
VMP = ".\viewsmgr"
PMP = ".\param_manager"
RBP = ".\ribbon"

.PHONY : all clean fclean re

vpath %.cpp ./ribbon:./viewsmgr:./param_mgr

all : $(NAME)

$(NAME) : $(OBJS)
	$(LD) $(LDFLAGS) /OUT:$@

$(OBJS)	: $(SRCS)
	$(CC) $(CFLAGS) .\ribbon\$(SRCS)

clean :
	del /q /f *.obj

fclean : clean
	del /q /f $(NAME)

re : fclean re	