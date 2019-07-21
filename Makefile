
vpath %.cpp src
vpath %.hpp include

CP       = cp -f -u
RM       = rm -f

OBJS     = $(patsubst %.cpp,%.o,$(notdir $(wildcard src/*.cpp)))

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Weffc++ -Wpedantic -O3 -mtune=intel -march=skylake
# CXXFLAGS = -std=c++17 -Wall -Wextra -Weffc++ -Wpedantic -Og -ggdb
CPPFLAGS = -D_GNU_SOURCE -DNDEBUG
# CPPFLAGS = -D_GNU_SOURCE
LDFLAGS  = -lm -lboost_program_options -lgtest

TARGET   = addtime

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -I include    -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -I include -c -o $@ $^

.PHONY: clean
clean:
	$(RM) $(OBJS) $(TARGET)

.PHONY: install
install: $(TARGET)
	$(CP) ./$(TARGET) /usr/bin

.PHONY: uninstall
uninstall:
	$(RM) /usr/bin/$(TARGET)

