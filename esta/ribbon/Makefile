# Makefile
# $@ a target
# $** (all dependent files in the dependency)
# $? (all dependent files in the dependency with a later timestamp than the target)
# @command (prevents display of the command)
# !command (executes command for each dependent file if command uses $** or $?)

CC = cl
LD = link

CFLAGS = /clr /AI $(LIBS) /c /Zi
LDFLAGS = /DLL 

HDRS = ribbon.h
SRCS = ribbon.cpp
OBJS = ribbon.obj
RES = icons.resources
NAME = ribbon.dll

NETASMS = "%PROGRAMFILES(x86)%\Reference Assemblies\Microsoft\Framework\.NETFramework\v4.6.1"
LIBS = "X:\kkozlov"
DST = "\\192.168.10.167\bim\addins\esta"

all : $(NAME) copy

$(NAME) : $(OBJS)
	$(LD) $(LDFLAGS) /ASSEMBLYRESOURCE:$(RES) $? /OUT:$@

$(OBJS)	: $(SRCS)
	$(CC) $(CFLAGS) /AI $(NETASMS) $?

.PHONY : all clean fclean re objs

clean :
	@del /q /f *.obj *.pdb

fclean : clean
	@del /q /f $(NAME)

re : fclean all

objs : $(SRCS)
	$(CC) $(CFLAGS) $**

copy : 
	copy /y $(NAME) $(DST)