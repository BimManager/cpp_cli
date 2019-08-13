# Makefile
# $@ a target
# $** (all dependent files in the dependency)
# $? (all dependent files in the dependency with a later timestamp than the target)
# @command (prevents display of the command)
# !command (executes command for each dependent file if command uses $** or $?)

CC = cl
CFLAGS = /nologo /clr /AI $(NETASMS) /c /Zi /Wall /WL /DDEBUG
LD = link
LDFLAGS = /NOLOGO

PREFIX = wpfForm

HDRS = $(PREFIX).h
SRCS = $(PREFIX).cpp
OBJS = $(PREFIX).obj
NAME = a.exe

NETASMS = "%PROGRAMFILES(x86)%\Reference Assemblies\Microsoft\Framework\.NETFramework\v4.6.1"
#LIBS = "%PROGRAMFILES%\Autodesk\Revit 2019"
LIBS = "X:\kkozlov"

DST = "x:\kkozlov"

all : $(NAME) #copy

$(NAME) : $(OBJS)
	$(LD) $(LDFLAGS) $** /OUT:$@

$(OBJS)	: $(SRCS)
	$(CC) $(CFLAGS) $**

$(SRCS) : $(HDRS)

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
	copy /y $(PREFIX).addin $(DST)