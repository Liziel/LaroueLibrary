NAME		= bomberman

CC		= gcc
CXX		= g++ #may be clang++

CXXSRC		=
CSRC		=
INCLUDES	= -I ./includes
###################
CtvTy		= src/ctvty/gameObject.cpp

###################
CXXSRC		= $(CtvTy)
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
