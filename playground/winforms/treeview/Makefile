# Winforms Makefile

CC = cl
LD = link
CFLAGS = /clr /c /Zi
LDFLAGS = /OUT:$(NAME) 

HDRS = treeview.h
SRCS = treeview.cpp main.cpp
OBJS = treeview.obj main.obj
NAME = a.exe

all : $(NAME)

$(NAME) : $(OBJS)
	$(LD) $(LDFLAGS) $**

$(OBJS) : $(SRCS)
	$(CC) $(CFLAGS) $**

clean :
	del /f/q *.obj *.pdb

fclean : clean
	del /f/q $(NAME)

re : fclean all

.PHONY : all clean fclean re