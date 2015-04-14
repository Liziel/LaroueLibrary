NAME		= bomberman

ifeq ($(CC),)
CC		= gcc
endif

ifeq ($(CXX),)
CXX		= g++ #may be clang++
endif

INCLUDES	= -I ./includes
###################
### CtvTy -> unity conception
CtvTy		= src/ctvty/gameObject.cpp \
		src/ctvty/component.cpp

### Serial -> Json Serialisation
Serial		= src/serialization/serializable.cpp \
		src/serialization/serial.cpp

### Main -> where we start the ctvty application
Main		= src/start/main.cpp

###################
CXXSRC		+= $(Serial) $(CtvTy)
CSRC		+=

FLAGS		= -Wall -Wextra $(INCLUDES)
CFLAGS		+= -W $(FLAGS)
CXXFLAGS	+= -W $(FLAGS) -std=c++11
LDFLAGS		=

CXXOBJ		= $(CXXSRC:.cpp=.o)
COBJ		= $(CSRC:.c=.o)
OBJ		= $(CXXOBJ) $(COBJ)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) -o $(NAME) $(OBJ) $(LDFLAGS)


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
