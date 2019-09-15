#
# TODO:
#

CC := g++
# CC := gcc
CFLAGS := -g -Wall -Wextra -Werror -I $(INCDIR)
LD := ld
LDFLAGS :=

SRCDIR := src
INCDIR := include
BUILDDIR := build
TARGET := bin/a.exe
SRCEXT := cpp

HDRS := $(shell find $(INCDIR) -type f -name *.h)
SRCS := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJS := $(shell find $(BUILDDIR) -type f -name *
TARGET := bin/a.exe

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

$(BUILDDIR)/%.o : $(SRCDIR)/%.$(SRCEXT)
	$(CC) $(CFLAGS) $^
 
clean :
	@rm -rf $(BUILDIR)/*.o

fclean : clean
	@rm -rf $(TARGET)

re : fclean all

.PHONY : all clean fclean re
