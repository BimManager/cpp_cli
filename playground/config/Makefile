CC = cl
LD = link

CFLAGS = /clr /c
LDFLAGS = /KEYFILE:$(KEY)

HDRS = 
SRCS = main.cpp
OBJS = main.obj
NAME = a.exe

KEY = keyfile.snk

all : $(NAME)

$(NAME) : $(OBJS)
	$(LD) $(LDFLAGS) $** /OUT:$(NAME)

$(OBJS)	: $(SRCS)
	$(CC) $(CFLAGS) $** 

clean :
	del /f/q *.obj

fclean : clean		
	del /f/q $(NAME)

re : fclean all

.PHONY : all clean fclean re
