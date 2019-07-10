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

HDRS = viewsmgr.h viewsmgr_form.h
SRCS = viewsmgr.cpp viewsmgr_form.cpp
OBJS = viewsmgr.obj viewsmgr_form.obj
NAME = viewsmgr.dll

NETASMS = "%PROGRAMFILES(x86)%\Reference Assemblies\Microsoft\Framework\.NETFramework\v4.6.1"
LIBS = "X:\kkozlov"
DST = "X:\kkozlov"

all : $(NAME) copy

$(NAME) : $(OBJS)
	$(LD) $(LDFLAGS) $? /OUT:$@

$(OBJS)	: $(SRCS)
	$(CC) $(CFLAGS) $?

.PHONY : all clean fclean re objs

clean :
	@del /q /f *.obj *.pdb

fclean : clean
	@del /q /f $(NAME)

re : fclean all

objs : $(SRCS)
	$(CC) $(CFLAGS) $**

copy :
	copy $(NAME) $(DST)
