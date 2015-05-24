NAME		= bomberman
OUTPUT		?= $(NAME)

CC		?= gcc

CXX		?= g++

INCLUDES	+= -I ./includes
###################
### CtvTy -> unity conception
CtvTyEvent	=src/ctvty/event/clock.cpp\
		src/ctvty/event/delayed.cpp

CtvTyAssets	=src/ctvty/assets/assets.cpp\
		src/ctvty/assets/asset.cpp\
		src/ctvty/assets/texture.cpp\
		src/ctvty/assets/model3d.cpp


CtvTyUtils	=src/ctvty/utils/vector3d.cpp\
		src/ctvty/utils/quaternion.cpp\
		src/ctvty/utils/boundingbox.cpp\
		src/ctvty/utils/face.cpp\
		src/ctvty/utils/contactpoint.cpp\
		src/ctvty/utils/collision.cpp

CtvTyPhysics	= src/ctvty/physics/material.cpp

CtvTyComponents	=src/ctvty/component/transform.cpp\
		src/ctvty/component/rigidbody.cpp\
		src/ctvty/component/collider.cpp\
		src/ctvty/component/boxcollider.cpp\
		src/ctvty/component/renderer.cpp\
		src/ctvty/component/animator.cpp\
		src/ctvty/component/camera.cpp \
		src/ctvty/component/hud.cpp

CtvTy		= src/ctvty/gameObject.cpp \
		src/ctvty/component.cpp\
		src/ctvty/object.cpp\
		src/ctvty/behaviour.cpp\
		src/ctvty/application.cpp\
		src/ctvty/event.cpp\
		src/ctvty/input.cpp\
		$(CtvTyEvent) $(CtvTyAssets) $(CtvTyUtils) $(CtvTyComponents) $(CtvTyPhysics)

### Serial -> Json Serialisation
Serial		= src/serialization/serializable.cpp \
		src/serialization/serial.cpp

### FileSystem -> Oop FieSystem
FileSystem	= src/filesystem/file.cpp \
		src/filesystem/directory.cpp

### UserDefined
UDComponent	= src/user_defined/component/eventtester.cpp

UserDefined	= $(UDComponent)

### Main -> where we start the ctvty application
Main		= src/start/main.cpp

###################
CXXSRC		+= $(Serial) $(CtvTy) $(Main) $(FileSystem) $(UserDefined)
CSRC		+=

STATIC_LIBRARY	?= renderer/renderer.a

FLAGS		= -W -Wall -Wextra $(INCLUDES)  -I renderer/libraries/GdlLibrary/includes/
CFLAGS		+= $(FLAGS) -Wall
CXXFLAGS	+= $(FLAGS) -std=c++11
LDFLAGS		+= -lpthread
-include	renderer/.ld_path

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

all: $(OUTPUT)

$(OUTPUT): $(OBJ) $(STATIC_LIBRARY)
ifeq ($(OUTPUT),$(NAME))
	$(LINKER) -o $(OUTPUT) $(OBJ) $(STATIC_LIBRARY) $(LDFLAGS)
else
	@echo "Alternate output:" $(OUTPUT) "build"
	@$(LINKER) -o $(OUTPUT) $(OBJ) $(LDFLAGS)
endif

clean:  clean_dependencies
	$(RM) $(OBJ) $(STATIC_LIBRARY)

fclean: clean
	$(RM) $(NAME)

re: fclean all

-include $(DEPENDENCIES)

clean_dependencies:
	rm -f $(DEPENDENCIES)
	rm -f .build_log

renderer/renderer.a:
	OUTPUT='renderer.a' INCLUDES="-I `pwd`/includes/" make -C renderer

%.d: %.cpp
	@$(CXX) -MM	$*.cpp	 $(CXXFLAGS) 1>> .build_log
	@echo 1>> .build_log
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
