# Winforms Makefile

CC = cl
LD = link
CFLAGS = /nologo /clr /c /Zi
LDFLAGS = /OUT:$(NAME) 

HDRS = dataGridView.h
SRCS = dataGridView.cpp main.cpp
OBJS = dataGridView.obj main.obj
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