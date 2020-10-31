CXX = g++
CXXFLAGS = -MD -Wall -Werror -O2
LDFLAGS = 
SRC_DIR = ./src
OBJ_DIR = ./build/obj
TARGET = build/main
INCLUDE = -I./include -L./lib \
		`/tmp/SDL2/bin/sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer \
		# -I/tmp/Lua5.4/include -L/tmp/Lua5.4/lib -llua

SRCS = $(notdir $(wildcard $(SRC_DIR)/*.cpp))
OBJS = $(patsubst %.o,$(OBJ_DIR)/%.o,$(SRCS:.cpp=.o))
DEPS = $(OBJS:.o=.d)

.PHONY: all clean run


all: $(TARGET)
	$(TARGET)

clean:
	rm -f $(OBJS) $(DEPS)

run: $(TARGET)
	$(TARGET)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	mkdir -p build/obj; \
	$(CXX) $(CXXFLAGS) -c $< -o $@ -MD $(LDFLAGS) $(INCLUDE)

$(TARGET) : $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS) $(INCLUDE)

-include $(DEPS)
