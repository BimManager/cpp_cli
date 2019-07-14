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

HDRS = rvt_tmpl.h
SRCS = rvt_tmpl.cpp
OBJS = rvt_tmpl.obj
NAME = rvt_tmpl.dll

NETASMS = "%PROGRAMFILES(x86)%\Reference Assemblies\Microsoft\Framework\.NETFramework\v4.6.1"
LIBS = "X:\kkozlov"

all : $(NAME)

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
