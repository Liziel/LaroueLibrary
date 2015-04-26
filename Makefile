NAME		= bomberman

CC		?= gcc

CXX		?= g++

INCLUDES	= -I ./includes
###################
### CtvTy -> unity conception
CtvTyEvent	=src/ctvty/event/clock.cpp\
		src/ctvty/event/delayed.cpp

CtvTyAssets	=src/ctvty/assets/assets.cpp\
		src/ctvty/assets/asset.cpp

CtvTyUtils	=src/ctvty/utils/vector3d.cpp\
		src/ctvty/utils/quaternion.cpp\
		src/ctvty/utils/boundingbox.cpp\
		src/ctvty/utils/face.cpp\
		src/ctvty/utils/contactpoint.cpp

CtvTyComponents	=src/ctvty/component/transform.cpp\
		src/ctvty/component/rigidbody.cpp\
		src/ctvty/component/collider.cpp\
		src/ctvty/component/boxcollider.cpp

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
LDFLAGS		+=

CXXOBJ		= $(CXXSRC:.cpp=.o)
COBJ		= $(CSRC:.c=.o)
OBJ		= $(CXXOBJ) $(COBJ)

CXXDEPENDENCIES	= $(CXXSRC:.cpp=.d)
CDEPENDENCIES	= $(CSRC:.c=.d)
DEPENDENCIES	= $(CDEPENDENCIES) $(CXXDEPENDENCIES)

ifneq ($(CXXOBJ),)
LINKER	?= $(CXX)
else
LINKER	?= $(CC)
endif

all: $(NAME)

$(NAME): $(OBJ)
	$(LINKER) -o $(NAME) $(OBJ) $(LDFLAGS)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME) $(DEPENDENCIES)

re: fclean dependencies all

ifneq ($@,re)
-include $(DEPENDENCIES)
endif

dependencies: $(DEPENDENCIES)
	@echo Dependencies Builded at `date`

%.d: %.cpp
	@echo -n `dirname $*` > $*.d
	@echo -n "/" >> $*.d
	@echo "Building Dependencies file for" $*.cpp
	@$(CXX) -MM	$*.cpp	 $(CXXFLAGS) >>	$*.d
	@echo "\t@echo Building " $*.cpp >> $*.d
	@echo '\t@$$(CXX) -c	$*.cpp $$(CXXFLAGS) -o $*.o' >> $*.d

%.d: %.c
	@echo -n `dirname $*` > $*.d
	@echo -n "/" >> $*.d
	@echo "Building Dependencies file for" $*.c
	@$(CC) -MM	$*.c	 $(CFLAGS) >>	$*.d
	@echo "\t@echo Building " $*.c >> $*.d
	@echo '\t@$$(CC) -c	$*.c $$(CFLAGS) -o $*.o' >> $*.d
.cpp.o:
	echo oui
%.o: %.cpp
	$(CXX) -c	$*.cpp	 $(CXXFLAGS) -o $*.o

%.o: %.c
	$(CC) -c	$*.c	 $(CFLAGS) -o $*.o


.PHONY: all clean fclean re dependencies
