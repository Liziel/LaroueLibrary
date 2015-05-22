NAME		= renderer.a
OUTPUT		?= $(NAME)

CC		?= gcc
CXX		?= g++

INCLUDES	+= -I ./includes
##############################
### Gdl Implementation
GdlImpl		= src/gdlimpl/renderer.cpp\
		src/gdlimpl/model3d.cpp\
		src/gdlimpl/camera.cpp\
		src/gdlimpl/texture.cpp\
		src/gdlimpl/hud.cpp

##############################
CXXSRC		+= $(GdlImpl)
CSRC		+=

FLAGS		= -W -Wall -Wextra $(INCLUDES) -I libraries/GdlLibrary/includes/
CFLAGS		+= $(FLAGS) -Wall
CXXFLAGS	+= $(FLAGS) -std=c++11 # skip bad warning for c++11 

CXXOBJ		= $(CXXSRC:.cpp=.o)
COBJ		= $(CSRC:.c=.o)
OBJ		= $(CXXOBJ) $(COBJ)

CXXDEPENDENCIES	= $(CXXSRC:.cpp=.d)
CDEPENDENCIES	= $(CSRC:.c=.d)
DEPENDENCIES	= $(CDEPENDENCIES) $(CXXDEPENDENCIES)

LINKER	?= ar rcs

all: $(OUTPUT)

$(OUTPUT): $(OBJ)
ifeq ($(OUTPUT),$(NAME))
	$(LINKER) $(OUTPUT) $(OBJ)
else
	@echo "Alternate output:" $(OUTPUT) "build"
	@$(LINKER) -o $(OUTPUT) $(OBJ)
endif

clean:  clean_dependencies
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

-include $(DEPENDENCIES)

clean_dependencies:
	rm -f $(DEPENDENCIES)

%.d: %.cpp
	@$(CXX) -MM	$*.cpp	 $(CXXFLAGS) 1>> .build_log
	@echo -n `dirname $*` > $*.d
	@echo -n "/" >> $*.d
	@echo "Building Dependencies file for" $*.cpp
	@$(CXX) -MM	$*.cpp	 $(CXXFLAGS) >>	$*.d

%.d: %.c
	@echo -n `dirname $*` > $*.d
	@echo -n "/" >> $*.d
	@echo "Building Dependencies file for" $*.c
	@$(CC) -MM	$*.c	 $(CFLAGS) >>	$*.d

%.o: %.cpp
	$(CXX) -c	$*.cpp	 $(CXXFLAGS) -o $*.o

%.o: %.c
	$(CC) -c	$*.c	 $(CFLAGS) -o $*.o


.PHONY: all clean fclean re clean_dependencies

