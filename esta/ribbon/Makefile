# imgtores Makefile

CC = cl /c
LD = link
CFLAGS = /clr /c /AI "%PROGRAMFILES(x86)%\Reference Assemblies\Microsoft\Framework\.NETFramework\v4.6.1"
LDFLAGS = /OUT:$(NAME).exe

SRCS = imgtores.cpp
OBJS = imgtores.obj
NAME = imgtores

all : $(NAME)

$(NAME) : $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS)

$(OBJS)	: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS)

.PHONY : all clean fclean re

clean :
	del *.obj 

fclean : clean
	del *.exe	