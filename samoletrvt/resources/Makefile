# imgtores Makefile

CC = cl
LD = link
CFLAGS = /clr /c /AI $(NET_PATH)
LDFLAGS = /NOLOGO /MACHINE:X64

#TYPE = chkrs
TYPE = genrs

SRCS = $(TYPE).cpp
OBJS = $(TYPE).obj
NAME = $(TYPE)

NET_PATH = "%PROGRAMFILES(x86)%\Reference Assemblies\Microsoft\Framework\.NETFramework\v4.6.1"

all : $(NAME)

$(NAME) : $(OBJS)
	$(LD) $(LDFLAGS) $** /OUT:$(NAME).exe

$(OBJS)	: $(SRCS)
	$(CC) $(CFLAGS) $**

.PHONY : all clean fclean re

clean :
	@del *.obj

fclean : clean
	@del *.exe

re : fclean all
