# post Makefile

CC = cl
LD = link
CFLAGS = /nologo /clr /I.\$(INCLDIR) /Fo.\$(OBJDIR)\ /c
LDFLAGS = /nologo

INCLDIR = include
SRCDIR = src
OBJDIR = build
BINDIR = bin

HDRS = main.h
SRCS = main.cpp
OBJS = $(OBJDIR)\main.obj
NAME = a.exe

all : $(NAME)

$(NAME) : $(OBJS)
	$(LD) $(LDFLAGS) $? /OUT:$(BINDIR)\$(NAME)

{$(SRCDIR)}.cpp{$(OBJDIR)}.obj::
	mkdir $(OBJDIR)
	$(CC) $(CFLAGS) $<
$(OBJS) :

clean :
	rmdir/s/q $(OBJDIR)

fclean : clean
	del/q $(BINDIR)\$(NAME)

re : fclean all

.PHONY : all clean fclean re
