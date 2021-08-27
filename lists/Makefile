CXX=/usr/bin/g++
CC=/usr/bin/gcc
CXXFLAGS += -fPIC

	CXXFLAGS += -Wno-deprecated
	CXXFLAGS += -Wall
	CXXFLAGS += -ggdb
	CXXFLAGS += -std=gnu++11
# The following are not included in -Wall
	CXXFLAGS += -W
	CXXFLAGS += -Wfloat-equal
	CXXFLAGS += -Winline
#	CXXFLAGS += -w -O3 -march=core2

srcs = $(wildcard *.cpp)
objs = $(srcs:.cpp=.o)
deps = $(srcs:.cpp=.d)

test: $(objs)
	$(CXX) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

.PHONY: clean

# $(RM) is rm -f by default
clean:
	$(RM) $(objs) $(deps) test

-include $(deps)
