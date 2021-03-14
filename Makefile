BIN = fountain
SRCS = $(shell find src -name *.cpp)
OBJS = $(SRCS:%=build/%.o)
DEPS = $(OBJS:.o=.d)
FLAGS = -Iinclude -MMD -MP -O3 -std=c++17
CXX := g++

build/$(BIN): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

build/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(FLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)
