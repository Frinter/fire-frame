PLATFORM := windows

ifeq ($(PLATFORM),windows)
  CXX = i686-pc-mingw32-g++
  PLATFORM_SRC = $(wildcard src/windows/*.cc)
  PLATFORM_LIBS = 
  PLATFORM_POST_LIBS = -lglew32 -lopengl32
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
OBJ_PLATFORM_DIR := build/$(PLATFORM)
BIN_DIR := bin
SUB_DIRS := $(shell find src -type d -print)
BUILD_DIRS := $(patsubst src/%,build/%,$(SUB_DIRS))
INCLUDE_DIRS := -Iinclude
SRC := $(wildcard src/*.cc) $(PLATFORM_SRC)
OBJECTS := $(SRC:src/%.cc=$(OBJ_DIR)/%.o) $(PLATFORM_OBJECTS)
LIBS := -Llib $(PLATFORM_LIBS) $(PLATFORM_POST_LIBS)
CFLAGS := $(INCLUDE_DIRS) -std=c++11 -g -DPLATFORM=$(PLATFORM)
LINK_FLAGS := -static-libgcc -static-libstdc++ $(PLATFORM_LINKFLAGS)
ARCHIVE_SRC := $(PLATFORM_SRC) src/framework/windowcontroller.cc
ARCHIVE_OBJECTS := $(patsubst src/%.cc,build/%.o,$(ARCHIVE_SRC))
ARCHIVE_TARGET := $(BIN_DIR)/libfireframe.a
TARGET := $(BIN_DIR)/test

.DEFAULT_GOAL = release
.PRECIOUS: build/%.d

$(TARGET): | $(BIN_DIR)
$(OBJECTS): | $(BUILD_DIRS)
$(BUILD_DIRS):
	mkdir -p $@

$(OBJ_DIR)/%.o: src/%.cc $(OBJ_DIR)/%.d
	@echo Building $@
	@$(CXX) -c -o $@ $(CFLAGS) $<

$(OBJ_DIR)/%.d: src/%.cc
	@$(CXX) $(CFLAGS) -MM -MT $(OBJ_DIR)/$*.o -MF $@ $<

release: $(TARGET)

$(ARCHIVE_TARGET): $(ARCHIVE_OBJECTS)
	ar cr $@ $^
$(TARGET): $(filter-out $(ARCHIVE_OBJECTS),$(OBJECTS)) $(ARCHIVE_TARGET)
	@echo Linking: $@
	$(CXX) -g $(LINK_FLAGS) -o $@ $^ $(LIBS)

clean:
	@rm -R build

include $(wildcard build/*.d)
