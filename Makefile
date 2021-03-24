CXX := g++
BIN = fountain
SRCS = $(shell find src -name *.cpp)
OBJS = $(SRCS:%=build/%.o)
DEPS = $(OBJS:.o=.d)
CFLAGS = -Iinclude -MMD -MP -O3 -std=c++17 -Wall -Wextra -pedantic
ifeq ($(PDF_SUPPORT), 1)
CFLAGS += -DPDF_SUPPORT
LDFLAGS = -Llib -lwkhtmltox -Wl,-rpath='$$ORIGIN'
endif

build/$(BIN): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)
	find lib -name *.so* -exec cp -pr {} build ';'
	find lib -name *.dll* -exec cp -pr {} build ';'

build/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -c $< -o $@

.PHONY: clean test

clean:
	rm -rf build
	rm -rf test/output

test: build/$(BIN)
	sh test/run.sh

-include $(DEPS)
