NAME		= bomberman

CC		= gcc
CXX		= g++ #may be clang++

CXXSRC		=
CSRC		=
INCLUDES	= -I ./includes
###################
### CtvTy -> unity conception
CtvTy		= src/ctvty/gameObject.cpp \
		src/ctvty/component.cpp

### Serial -> Json Serialisation
Serial		= src/serialization/Serializable.cpp

###################
CXXSRC		= $(CtvTy) $(Serial)
CSRC		=

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
	$(CC) -c -o $@ $< $(CXXFLAGS)

.PHONY: all clean fclean re
