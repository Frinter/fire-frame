OBJ_DIR := build
BIN_DIR := bin

ifeq ($(SYSTEM_TARGET),MINGW)
  PLATFORM_SRC = $(wildcard src/windows/*.cc) $(wildcard src/mingw/*.cc)
  PLATFORM_LIBS = -lmingw32
  PLATFORM_POST_LIBS = -lgdi32 -lopengl32 -lwinmm
  PLATFORM_LINKFLAGS = -Wl,-subsystem,windows
  PLATFORM_MAIN_OBJECTS =
  PLATFORM_OBJECTS = $(patsubst src/%.c,build/%.o,$(wildcard src/windows/*.c)) $(patsubst src/%.cc,build/%.o,$(wildcard src/windows/*.cc)) $(patsubst src/%.cc,build/%.o,$(wildcard src/mingw/*.cc))
  CPP = x86_64-w64-mingw32-c++
  GCC = x86_64-w64-mingw32-gcc
  AR  = x86_64-w64-mingw32-ar

include $(wildcard build/windows/*.d)
else
  CXX = g++
  PLATFORM_LIBS =
  PLATFORM_POST_LIBS = -lGL -lX11 -pthread
  PLATFORM_LINKFLAGS =
  PLATFORM_OBJECTS = $(patsubst src/%.cc,build/%.o,$(wildcard src/linux/*.cc))
  CPP = c++
  GCC = gcc
  AR  = ar

include $(wildcard build/linux/*.d)
endif

OBJ_PLATFORM_DIR := $(OBJ_DIR)/$(SYSTEM_TARGET)
SUB_DIRS := $(filter-out src,$(shell find src -type d -print))
BUILD_DIRS := $(patsubst src/%,build/%,$(SUB_DIRS)) $(patsubst test/%,build/%,$(SUB_TEST_DIRS)) build/demo
INCLUDE_DIRS := -Iinclude
SRC := $(wildcard src/*.cc) $(wildcard src/framework/*.cc)
OBJECTS := $(SRC:src/%.cc=$(OBJ_DIR)/%.o) $(PLATFORM_OBJECTS)
LIBS := $(PLATFORM_LIBS) $(PLATFORM_POST_LIBS)
CFLAGS := $(INCLUDE_DIRS) -DSYSTEM_TARGET=$(SYSTEM_TARGET) -std=c++11

PLATFORM_LAYER_SRC := $(SRC) $(PLATFORM_SRC)
PLATFORM_LAYER_LIBS := -Llib $(PLATFORM_LIBS) $(PLATFORM_POST_LIBS)
PLATFORM_LAYER_TARGET := $(BIN_DIR)/platform.$(SYSTEM_TARGET).64.a

TEST_SUB_DIRS := $(filter-out test,$(shell find test -type d -print))
TEST_OBJ_DIRS := build/test $(patsubst test/%,build/test/%,$(TEST_SUB_DIRS))
TEST_SRC := $(shell find test -type f -name "*.cc" -print)
TEST_OBJECTS := $(TEST_SRC:test/%.cc=$(OBJ_DIR)/test/%.o)
TESTABLES_OBJECTS := $(PLATFORM_LAYER_TARGET) $(OBJ_DIR)/demo/ticker.o
TEST_INCLUDE := -Idemo -Itest
TEST_CFLAGS := $(TEST_INCLUDE)
TEST_LIBS :=
TEST_TARGET := $(BIN_DIR)/test
TEST_LINK_FLAGS := -static-libgcc -static-libstdc++

DEMO_SRC := $(wildcard demo/*.cc)
DEMO_OBJECTS := $(patsubst demo/%.cc,build/demo/%.o,$(DEMO_SRC))
DEMO_LIBS := -Llib $(PLATFORM_LIBS) $(PLATFORM_POST_LIBS)
DEMO_TARGET := $(BIN_DIR)/demo.exe

.DEFAULT_GOAL = release
.PHONY: test release demo
.PRECIOUS: build/%.d

debug-make:
	@echo $(PLATFORM_OBJECTS)
clean:
	@rm -Rf build

$(TEST_TARGET) $(PLATFORM_LAYER_TARGET): | $(BIN_DIR)
$(OBJECTS) $(DEMO_OBJECTS): | $(OBJ_DIR) $(BUILD_DIRS)
$(TEST_OBJECTS): | $(TEST_OBJ_DIRS)
$(BUILD_DIRS) $(TEST_OBJ_DIRS) $(OBJ_DIR) $(BIN_DIR):
	@mkdir -p $@

$(OBJ_DIR)/%.o: src/%.c $(OBJ_DIR)/%.d
	@echo Building: $<
	@$(GCC) -c -o $@ $(CFLAGS) $<

$(OBJ_DIR)/%.o: src/%.cc $(OBJ_DIR)/%.d
	@echo Building: $<
	@$(CPP) -c -o $@ $(CFLAGS) $<

$(OBJ_DIR)/test/%.o: test/%.cc $(OBJ_DIR)/test/%.d
	@echo Building: $<
	@$(CPP) -c -o $@ $(CFLAGS) $(TEST_CFLAGS) $<

$(OBJ_DIR)/demo/%.o: demo/%.cc $(OBJ_DIR)/demo/%.d
	@echo Building: $<
	@$(CPP) -c -o $@ $(CFLAGS) $(TEST_CFLAGS) $<

$(OBJ_DIR)/%.d: src/%.c
	@$(GCC) $(CFLAGS) -MM -MT $(OBJ_DIR)/$*.o -MF $@ $<

$(OBJ_DIR)/%.d: src/%.cc
	@$(CPP) $(CFLAGS) -MM -MT $(OBJ_DIR)/$*.o -MF $@ $<

$(OBJ_DIR)/test/%.d: test/%.cc
	@$(CPP) $(CFLAGS) $(TEST_CFLAGS) -MM -MT $(OBJ_DIR)/$*.o -MF $@ $<

$(OBJ_DIR)/demo/%.d: demo/%.cc
	@$(CPP) $(CFLAGS) $(TEST_CFLAGS) -MM -MT $(OBJ_DIR)/$*.o -MF $@ $<

release: $(PLATFORM_LAYER_TARGET)
demo: $(DEMO_TARGET)
test: $(TEST_TARGET)
	@$(TEST_TARGET)

$(PLATFORM_LAYER_TARGET): $(OBJECTS)
	$(AR) cr $@ $^

$(DEMO_TARGET): $(DEMO_OBJECTS) $(PLATFORM_LAYER_TARGET)
	$(CPP) $(PLATFORM_LINK_FLAGS) -static-libgcc -static-libstdc++ -Iinclude -o $@ $(DEMO_OBJECTS) $(PLATFORM_LIBS) $(PLATFORM_LAYER_TARGET) $(PLATFORM_POST_LIBS)

$(TEST_TARGET): $(TEST_OBJECTS) $(TESTABLES_OBJECTS)
	$(CPP) -g $(TEST_LINK_FLAGS) -o $@ $^

include $(wildcard build/*.d)
