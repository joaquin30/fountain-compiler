CXX := g++
BIN = fountain
SRCS = $(shell find src -name *.cpp)
OBJS = $(SRCS:%=build/%.o)
DEPS = $(OBJS:.o=.d)
CFLAGS = -Iinclude -MMD -MP -O3 -std=c++17 -Wall -Wextra -pedantic
LDFLAGS = -Llib -lm -lwkhtmltox -Wl,-rpath='$$ORIGIN'
ifeq ($(PDF_SUPPORT), 1)
PFLAG = PDF_SUPPORT_TRUE
else
PFLAG = PDF_SUPPORT_FALSE
endif

build/$(BIN): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)
	find lib -name *.so* -exec cp -prv {} build ';'
	find lib -name *.dll* -exec cp -prv {} build ';'

build/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -c $< -o $@ -D $(PFLAG)

.PHONY: clean test

clean:
	rm -rf build
	rm -f test/*.html test/*.pdf test/*.fdx

test: build/$(BIN)
	sh test/test.sh

-include $(DEPS)
