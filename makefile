PLATFORM := windows

ifeq ($(PLATFORM),windows)
  CXX = i686-pc-mingw32-g++
  PLATFORM_SRC = $(wildcard src/windows/*.cc)
  PLATFORM_LIBS = 
  PLATFORM_POST_LIBS = -lopengl32
  PLATFORM_LINKFLAGS = -mwindows #-mconsole
  PLATFORM_OBJECTS = $(wildcard build/windows/*.res)

#build/glew.o: src/windows/glew.c
#	$(CXX) -c -o $@  -DGLEW_STATIC $(CFLAGS) $<
else
  CXX = g++
  PLATFORM_LIBS = -lGL
  PLATFORM_POST_LIBS = 
  PLATFORM_LINKFLAGS = 
  PLATFORM_OBJECTS =
endif

OBJ_DIR := build
OBJ_PLATFORM_DIR := build/$(PLATFORM)
BIN_DIR := bin
INCLUDE_DIRS := -Iinclude
SRC := $(wildcard src/*.cc) $(PLATFORM_SRC)
TEST_SRC := $(wildcard test-src/*.cc)
OBJECTS := $(SRC:src/%.cc=$(OBJ_DIR)/%.o) $(PLATFORM_OBJECTS)
TEST_OBJECTS := $(TEST_SRC:test/%.cc=build/%.o)
LIBS := -Llib $(PLATFORM_LIBS) $(PLATFORM_POST_LIBS)
CFLAGS := $(INCLUDE_DIRS) -std=c++0x -g -DPLATFORM=$(PLATFORM)
LINK_FLAGS := -static-libgcc -static-libstdc++ $(PLATFORM_LINKFLAGS)
TARGET := $(BIN_DIR)/test
TEST := $(BIN_DIR)/runtests

.DEFAULT_GOAL = release
.PRECIOUS: build/%.d

$(TARGET) $(TEST): | $(BIN_DIR)
$(OBJECTS): | $(OBJ_DIR) $(OBJ_PLATFORM_DIR)
$(OBJ_DIR) $(OBJ_PLATFORM_DIR) $(BIN_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: src/%.cc $(OBJ_DIR)/%.d
	$(CXX) -c -o $@ $(CFLAGS) $<

test-build/%.o: test-src/%.cc test-build/%.d
	$(CXX) -c -o $@ $(CFLAGS) $<

$(OBJ_DIR)/%.d: src/%.cc
	$(CXX) $(INCLUDE_DIRS) -MM -MT $(OBJ_DIR)/$*.o -MF $@ $<

test-build/%.d: test-src/%.cc
	$(CXX) $(INCLUDE_DIRS) -MM -MT $(OBJ_DIR)/$*.o -MF $@ $<

release: $(TARGET)
test: $(TEST)
	echo "Running tests now..."
	$(TEST)

$(TEST): $(filter-out build/main%.o, $(OBJECTS)) $(TEST_OBJECTS)
	$(CXX) -g $(LINK_FLAGS) -o $@ $^ $(LIBS)
$(TARGET): $(OBJECTS)
	$(CXX) -g $(LINK_FLAGS) -o $@ $^ $(LIBS)

clean:
	rm -R build

include $(wildcard build/*.d)