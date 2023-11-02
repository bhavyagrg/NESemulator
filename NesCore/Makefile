CXX = clang++
CXX_VERSION = c++20

NAME = libnescore

INCLUDE = -Iinclude/ -I../Gui/ -I../Gui/external/raylib/include
SRC_DIR = src

INTERMEDIATE_DIR = ./out/obj/

TARGET_DIR = ./out/
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)

TARGET = $(TARGET_DIR)$(NAME).so

OBJECTS=$(patsubst %.cpp,$(INTERMEDIATE_DIR)%.o,$(SRC_FILES))

LDFLAGS = -lstdc++

all: $(TARGET)
	$(info [+] Building libnescore done)

$(TARGET): $(OBJECTS) $(TARGET_DIR)
	$(info [+] Generating $(TARGET) to $(TARGET_DIR))
	$(CXX) -std=$(CXX_VERSION) $(LDFLAGS) -shared $(OBJECTS) -o $@ 

$(INTERMEDIATE_DIR)%.o: %.cpp $(INTERMEDIATE_DIR)
	$(CXX) -std=$(CXX_VERSION) $(INCLUDE) -fPIC -c $< -o $@	

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


