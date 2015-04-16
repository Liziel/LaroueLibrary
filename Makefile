NAME		= bomberman

ifeq ($(CC),)
CC		= gcc
endif

ifeq ($(CXX),)
CXX		= g++
endif

INCLUDES	= -I ./includes
###################
### CtvTy -> unity conception
CtvTy		= src/ctvty/gameObject.cpp \
		src/ctvty/component.cpp\
		src/ctvty/object.cpp\
		src/ctvty/event/clock.cpp\
		src/ctvty/event/delayed.cpp

### Serial -> Json Serialisation
Serial		= src/serialization/serializable.cpp \
		src/serialization/serial.cpp

### FileSystem -> Oop FieSystem
FileSystem	= src/filesystem/file.cpp \
		src/filesystem/directory.cpp

### Main -> where we start the ctvty application
Main		= src/start/main.cpp

###################
CXXSRC		+= $(Serial) $(CtvTy) $(Main) $(FileSystem)
CSRC		+=

FLAGS		= -Wall -Wextra $(INCLUDES)
CFLAGS		+= -W $(FLAGS)
CXXFLAGS	+= -W $(FLAGS) -std=c++11
LDFLAGS		=

CXXOBJ		= $(CXXSRC:.cpp=.o)
COBJ		= $(CSRC:.c=.o)
OBJ		= $(CXXOBJ) $(COBJ)

ifneq ($(CXXOBJ),)
ifeq 	($(LINKER),)
LINKER	= $(CXX)
endif
else
ifeq	($(LINKER),)
LINKER	= $(CC)
endif
endif

all: $(NAME)

$(NAME): $(OBJ)
	$(LINKER) -o $(NAME) $(OBJ) $(LDFLAGS)


clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.cpp.o:
	$(CXX) -c -o $@ $< $(CXXFLAGS)

.c.o:
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: all clean fclean re
