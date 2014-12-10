PLATFORM := windows

ifeq ($(PLATFORM),windows)
  CXX = g++
  PLATFORM_SRC = $(wildcard src/windows/*.cc)
  PLATFORM_LIBS = 
  PLATFORM_POST_LIBS = -lglew32 -lopengl32 -lwinmm
  PLATFORM_LINKFLAGS = -mwindows -mconsole
  PLATFORM_OBJECTS = 

include $(wildcard build/windows/*.d)
else
  CXX = g++
  PLATFORM_LIBS = -lGL
  PLATFORM_POST_LIBS = 
  PLATFORM_LINKFLAGS = 
  PLATFORM_OBJECTS =

include $(wildcard build/linux/*.d)
endif

OBJ_DIR := build
OBJ_PLATFORM_DIR := $(OBJ_DIR)/$(PLATFORM)
BIN_DIR := bin
SUB_DIRS := $(shell find src -type d -print)
BUILD_DIRS := $(patsubst src/%,build/%,$(SUB_DIRS))
INCLUDE_DIRS := -Iinclude
SRC := $(wildcard src/*.cc) $(wildcard src/framework/*.cc) $(PLATFORM_SRC)
TEST_SRC := $(wildcard test/*.cc)
OBJECTS := $(SRC:src/%.cc=$(OBJ_DIR)/%.o) $(PLATFORM_OBJECTS)
TEST_OBJECTS := $(patsubst test/%.cc,build/%.o,$(TEST_SRC))
LIBS := -Llib $(PLATFORM_LIBS) $(PLATFORM_POST_LIBS)
CFLAGS := $(INCLUDE_DIRS) -std=c++11 -g -DPLATFORM=$(PLATFORM)
LINK_FLAGS := -static-libgcc -static-libstdc++ $(PLATFORM_LINKFLAGS)
ARCHIVE_SRC := $(SRC)
ARCHIVE_OBJECTS := $(patsubst src/%.cc,build/%.o,$(ARCHIVE_SRC))
ARCHIVE_TARGET := $(BIN_DIR)/libfireframe.a
TEST_TARGET := $(BIN_DIR)/test

.DEFAULT_GOAL = release
.PHONY: test release
.PRECIOUS: build/%.d

$(TEST_TARGET) $(ARCHIVE_TARGET): | $(BIN_DIR)
$(OBJECTS) $(TEST_OBJECTS): | $(OBJ_DIR) $(BUILD_DIRS)
$(BUILD_DIRS) $(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: src/%.cc $(OBJ_DIR)/%.d
	@echo Building $@
	@$(CXX) -c -o $@ $(CFLAGS) $<

$(OBJ_DIR)/%.o: test/%.cc $(OBJ_DIR)/%.d
	@echo '(Test) Building $@'
	@$(CXX) -c -o $@ $(CFLAGS) $<

$(OBJ_DIR)/%.d: src/%.cc
	@echo 'Building dependencies for $< -> $@'
	@$(CXX) $(CFLAGS) -MM -MT $(OBJ_DIR)/$*.o -MF $@ $<

$(OBJ_DIR)/%.d: test/%.cc
	@echo '(Test) Building dependencies for $< -> $@'
	@$(CXX) $(CFLAGS) -MM -MT $(OBJ_DIR)/$*.o -MF $@ $<

release: $(ARCHIVE_TARGET)
test: $(TEST_TARGET)

$(ARCHIVE_TARGET): $(ARCHIVE_OBJECTS)
	@echo Linking: $@
	@ar cr $@ $^
$(TEST_TARGET): $(TEST_OBJECTS) $(ARCHIVE_TARGET)
	@echo '(Test) Linking: $@'
	@$(CXX) -g $(LINK_FLAGS) -o $@ -lmingw32 $^ $(LIBS)

clean:
	@rm -R build

include $(wildcard build/*.d)
