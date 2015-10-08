OBJ_DIR := build
BIN_DIR := bin

ifeq ($(SYSTEM_TARGET),MINGW)
  PLATFORM_SRC = $(wildcard src/windows/*.cc) $(wildcard src/mingw/*.cc)
  PLATFORM_LIBS = -lmingw32
  PLATFORM_POST_LIBS = -lgdi32 -lopengl32 -lwinmm
  PLATFORM_LINKFLAGS = -Wl,-subsystem,windows
  PLATFORM_MAIN_OBJECTS = 
  PLATFORM_OBJECTS = $(patsubst src/%.c,build/%.o,$(wildcard src/windows/*.c)) $(patsubst src/%.cc,build/%.o,$(wildcard src/windows/*.cc)) $(patsubst src/%.cc,build/%.o,$(wildcard src/mingw/*.cc))

include $(wildcard build/windows/*.d)
else
  CXX = g++
  PLATFORM_LIBS = -lGL
  PLATFORM_POST_LIBS = 
  PLATFORM_LINKFLAGS = 
  PLATFORM_OBJECTS =

include $(wildcard build/linux/*.d)
endif

OBJ_PLATFORM_DIR := $(OBJ_DIR)/$(SYSTEM_TARGET)
SUB_DIRS := $(shell find src -type d -print)
BUILD_DIRS := $(patsubst src/%,build/%,$(SUB_DIRS)) build/demo
INCLUDE_DIRS := -Iinclude
SRC := $(wildcard src/*.cc) $(wildcard src/framework/*.cc)
OBJECTS := $(SRC:src/%.cc=$(OBJ_DIR)/%.o) $(PLATFORM_OBJECTS)
LIBS := $(PLATFORM_LIBS) $(PLATFORM_POST_LIBS)
CFLAGS := $(INCLUDE_DIRS) -DSYSTEM_TARGET=$(SYSTEM_TARGET)

PLATFORM_LAYER_SRC := $(SRC) $(PLATFORM_SRC)
PLATFORM_LAYER_LIBS := -Llib $(PLATFORM_LIBS) $(PLATFORM_POST_LIBS)
PLATFORM_LAYER_TARGET := $(BIN_DIR)/platform.$(SYSTEM_TARGET).64.a

TEST_SRC := $(wildcard test/*.cc)
TEST_OBJECTS := $(patsubst test/%.cc,build/%.o,$(TEST_SRC))
TESTABLES_OBJECTS := $(OBJ_DIR)/demo/ticker.o
TEST_INCLUDE := -Idemo
TEST_CFLAGS := $(TEST_INCLUDE)
TEST_LIBS := 
TEST_TARGET := $(BIN_DIR)/test
TEST_LINK_FLAGS := -static-libgcc -static-libstdc++

DEMO_SRC := $(wildcard demo/*.cc)
DEMO_OBJECTS := $(patsubst demo/%.cc,build/demo/%.o,$(DEMO_SRC))
DEMO_LIBS := -Llib $(PLATFORM_LIBS) $(PLATFORM_POST_LIBS)
DEMO_TARGET := $(BIN_DIR)/demo

.DEFAULT_GOAL = release
.PHONY: test release demo
.PRECIOUS: build/%.d

clean:
	rm -R build

$(TEST_TARGET) $(PLATFORM_LAYER_TARGET): | $(BIN_DIR)
$(OBJECTS) $(TEST_OBJECTS) $(DEMO_OBJECTS): | $(OBJ_DIR) $(BUILD_DIRS)
$(BUILD_DIRS) $(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: src/%.c $(OBJ_DIR)/%.d
	@echo Building $@
	@clang -c -o $@ $(CFLAGS) $<

$(OBJ_DIR)/%.o: src/%.cc $(OBJ_DIR)/%.d
	@echo Building $@
	@clang++ -c -o $@ $(CFLAGS) -std=c++11 $<

$(OBJ_DIR)/%.o: test/%.cc $(OBJ_DIR)/%.d
	@echo '(Test) Building $@'
	@clang++ -c -o $@ $(CFLAGS) -std=c++11 $(TEST_CFLAGS) $<

$(OBJ_DIR)/demo/%.o: demo/%.cc $(OBJ_DIR)/demo/%.d
	@echo '(Demo) Building $@'
	@clang++ -c -o $@ $(CFLAGS) -std=c++11 $(TEST_CFLAGS) $<

$(OBJ_DIR)/%.d: src/%.c
#	@echo 'Building dependencies for $< -> $@'
	@clang $(CFLAGS) -MM -MT $(OBJ_DIR)/$*.o -MF $@ $<

$(OBJ_DIR)/%.d: src/%.cc
#	@echo 'Building dependencies for $< -> $@'
	@clang++ $(CFLAGS) -std=c++11 -MM -MT $(OBJ_DIR)/$*.o -MF $@ $<

$(OBJ_DIR)/%.d: test/%.cc
#	@echo '(Test) Building dependencies for $< -> $@'
	@clang++ $(CFLAGS) -std=c++11 $(TEST_CFLAGS) -MM -MT $(OBJ_DIR)/$*.o -MF $@ $<

$(OBJ_DIR)/demo/%.d: demo/%.cc
#	@echo '(Demo) Building dependencies for $< -> $@'
	@clang++ $(CFLAGS) -std=c++11 $(TEST_CFLAGS) -MM -MT $(OBJ_DIR)/$*.o -MF $@ $<

release: $(PLATFORM_LAYER_TARGET)
demo: $(DEMO_TARGET)
test: $(TEST_TARGET)
	@$(TEST_TARGET)

$(PLATFORM_LAYER_TARGET): $(OBJECTS)
	@echo Linking: $@
	ar cr $@ $^

$(DEMO_TARGET): $(DEMO_OBJECTS) $(PLATFORM_LAYER_TARGET)
	@echo '(Demo) Linking: $@'
	clang++ $(PLATFORM_LINK_FLAGS) -static-libgcc -static-libstdc++ -Iinclude -o $@ $(DEMO_OBJECTS) $(PLATFORM_LIBS) $(PLATFORM_LAYER_TARGET) $(PLATFORM_POST_LIBS)

$(TEST_TARGET): $(TEST_OBJECTS) $(TESTABLES_OBJECTS)
	@echo '(Test) Linking: $@'
	@clang++ -g $(TEST_LINK_FLAGS) -o $@ $^

include $(wildcard build/*.d)
