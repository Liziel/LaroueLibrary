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
CtvTyEvent	=src/ctvty/event/clock.cpp\
		src/ctvty/event/delayed.cpp

CtvTyAssets	=src/ctvty/assets/assets.cpp\
		src/ctvty/assets/asset.cpp

CtvTyUtils	=src/ctvty/utils/vector3d.cpp\
		src/ctvty/utils/quaternion.cpp

CtvTyComponents	=src/ctvty/component/transform.cpp\
		src/ctvty/component/rigidbody.cpp

CtvTy		= src/ctvty/gameObject.cpp \
		src/ctvty/component.cpp\
		src/ctvty/object.cpp\
		src/ctvty/behaviour.cpp\
		src/ctvty/application.cpp\
		$(CtvTyEvent) $(CtvTyAssets) $(CtvTyComponents) $(CtvTyUtils)

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

FLAGS		= -W -Wextra $(INCLUDES)
CFLAGS		+= $(FLAGS) -Wall
CXXFLAGS	+= $(FLAGS) -std=c++11 # skip bad warning for c++11 
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
	$(CXX) -c -o $@		 $< $(CXXFLAGS)

.c.o:
	$(CC) -c -o $@		 $< $(CFLAGS)

.PHONY: all clean fclean re
