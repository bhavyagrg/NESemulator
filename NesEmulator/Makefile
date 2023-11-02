CXX = clang++
CXX_VERSION = c++20

NAME = nesemulator

TARGET_DIR = ../out/

SRC_DIR = src

INCLUDE = -Iinclude/ -Iexternal/raylib/include -I../Gui -I../NesCore -I../Gui/external/raylib/include

LIB_INCLUDE_DIRS = -L$(TARGET_DIR) -Lexternal/raylib/lib 

LIBS = -lraylib -lGL -lgui -lnescore 

INTERMEDIATE_DIR = ./out/obj/

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)

TARGET = $(TARGET_DIR)$(NAME)

OBJECTS = $(patsubst %.cpp,$(INTERMEDIATE_DIR)%.o,$(SRC_FILES))

INT_COMPILE_FLAGS=-fPIC -W

LDFLAGS = $(LIB_INCLUDE_DIRS) $(LIBS)
COMPILE_FLAGS = -W -Wall 

all: $(TARGET)
	$(info [+] Building $(TARGET) done)

$(TARGET): $(OBJECTS) $(TARGET_DIR)
	$(info [+] Generating $(TARGET))
	$(CXX) -std=$(CXX_VERSION) -o $@ $(OBJECTS) $(COMPILE_FLAGS) $(LDFLAGS) 

$(INTERMEDIATE_DIR)%.o: %.cpp $(INTERMEDIATE_DIR)
	$(CXX) -std=$(CXX_VERSION) $(INCLUDE) $(INT_COMPILE_FLAGS) -c $< -o $@	

# path checks
CHECK_PATH = $(shell test -e $(path))

ifeq ($(CHECK_PATH),)
$(TARGET_DIR):
	mkdir -p $(TARGET_DIR)
endif

ifeq ($(CHECK_PATH),)
$(INTERMEDIATE_DIR):
	mkdir -p $(INTERMEDIATE_DIR)src
endif


