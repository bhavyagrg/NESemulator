CXX = clang++
CXX_VERSION = c++20

NAME = libgui

INCLUDE = -Iinclude/ -Iexternal/raylib/include
SRC_DIR = src

INTERMEDIATE_DIR = ./out/obj/

TARGET_DIR = ../out/
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)

TARGET = $(TARGET_DIR)$(NAME).a

OBJECTS = $(patsubst %.cpp,$(INTERMEDIATE_DIR)%.o,$(SRC_FILES))

all: $(TARGET)
	$(info [+] Building $(NAME) done)

$(TARGET): $(OBJECTS) $(TARGET_DIR)
	$(info [+] Generating $(TARGET))
	ar rc $@ $(OBJECTS)

$(INTERMEDIATE_DIR)%.o: %.cpp $(INTERMEDIATE_DIR)
	$(CXX) -std=$(CXX_VERSION) $(INCLUDE) -fPIC  -c $< -o $@	

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


