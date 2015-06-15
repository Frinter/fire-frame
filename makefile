ifeq ($(SYSTEM_TARGET),MINGW)
  CC = clang
  CXX = clang++
  PLATFORM_SRC = $(wildcard src/windows/*.cc) $(wildcard src/mingw/*.cc)
  PLATFORM_LIBS = #-lmingw32
  PLATFORM_POST_LIBS = -lopengl32 -lwinmm
  PLATFORM_LINKFLAGS = 
  PLATFORM_OBJECTS =

include $(wildcard build/windows/*.d)
else
  CXX = g++
  PLATFORM_LIBS = -lGL
  PLATFORM_POST_LIBS = 
  PLATFORM_LINKFLAGS = 
  PLATFORM_OBJECTS =

include $(wildcard build/linux/*.d
endif

OBJ_DIR := build
OBJ_PLATFORM_DIR := $(OBJ_DIR)/$(PLATFORM)
BIN_DIR := bin
SUB_DIRS := $(shell find src -type d -print)
BUILD_DIRS := $(patsubst src/%,build/%,$(SUB_DIRS)) build/demo
INCLUDE_DIRS := -Iinclude
SRC := $(wildcard src/*.cc) $(wildcard src/framework/*.cc) $(PLATFORM_SRC) src/gl_core_3_3.c
OBJECTS := $(SRC:src/%.cc=$(OBJ_DIR)/%.o) $(PLATFORM_OBJECTS)
LIBS := $(PLATFORM_LIBS) $(PLATFORM_POST_LIBS)
CXXFLAGS := -std=c++11 -stdlib=libstdc++
CCFLAGS :=
CFLAGS := $(INCLUDE_DIRS) -DPLATFORM=$(PLATFORM)
LINK_FLAGS := -static-libgcc -static-libstdc++ $(PLATFORM_LINKFLAGS) 

PLATFORM_LAYER_SRC := $(SRC)
PLATFORM_LAYER_OBJECTS := $(patsubst src/%.cc,build/%.o,$(PLATFORM_LAYER_SRC)) $(PLATFORM_OBJECTS) $(OBJ_DIR)/gl_core_3_3.o
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
DEMO_LIBS := $(PLATFORM_LAYER_LIBS)
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
	@$(CC) -c -o $@ $(CFLAGS) $<

$(OBJ_DIR)/%.o: src/%.cc $(OBJ_DIR)/%.d
	@echo Building $@
	@$(CXX) -c -o $@ $(CFLAGS) $(CXXFLAGS) $<

$(OBJ_DIR)/%.o: test/%.cc $(OBJ_DIR)/%.d
	@echo '(Test) Building $@'
	@$(CXX) -c -o $@ $(CFLAGS) $(CXXFLAGS) $(TEST_CFLAGS) $<

$(OBJ_DIR)/demo/%.o: demo/%.cc $(OBJ_DIR)/demo/%.d
	@echo '(Demo) Building $@'
	@$(CXX) -c -o $@ $(CFLAGS) $(CXXFLAGS) $(TEST_CFLAGS) $<

$(OBJ_DIR)/%.d: src/%.c
	@echo 'Building dependencies for $< -> $@'
	@$(CC) $(CFLAGS) -MM -MT $(OBJ_DIR)/$*.o -MF $@ $<

$(OBJ_DIR)/%.d: src/%.cc
	@echo 'Building dependencies for $< -> $@'
	@$(CXX) $(CFLAGS) $(CXXFLAGS) -MM -MT $(OBJ_DIR)/$*.o -MF $@ $<

$(OBJ_DIR)/%.d: test/%.cc
	@echo '(Test) Building dependencies for $< -> $@'
	@$(CXX) $(CFLAGS) $(CXXFLAGS) $(TEST_CFLAGS) -MM -MT $(OBJ_DIR)/$*.o -MF $@ $<

$(OBJ_DIR)/demo/%.d: demo/%.cc
	@echo '(Demo) Building dependencies for $< -> $@'
	@$(CXX) $(CFLAGS) $(CXXFLAGS) $(TEST_CFLAGS) -MM -MT $(OBJ_DIR)/$*.o -MF $@ $<

release: $(PLATFORM_LAYER_TARGET)
demo: $(DEMO_TARGET)
test: $(TEST_TARGET)
	@$(TEST_TARGET)

$(PLATFORM_LAYER_TARGET): $(PLATFORM_LAYER_OBJECTS)
	@echo Linking: $@
	ar cr $@ $^

$(DEMO_TARGET): $(DEMO_OBJECTS) $(PLATFORM_LAYER_TARGET)
	@echo '(Demo) Linking: $@'
	$(CXX) $(LINK_FLAGS) -o $@ $^ $(DEMO_LIBS)

$(TEST_TARGET): $(TEST_OBJECTS) $(TESTABLES_OBJECTS)
	@echo '(Test) Linking: $@'
	@$(CXX) -g $(TEST_LINK_FLAGS) -o $@ $^

include $(wildcard build/*.d)
