CC = gcc
# Update these paths as necessary to match your installation
SDL_LIB = -L/usr/local/lib -I/usr/local/include/SDL2 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_gfx
# If your compiler is a bit older you may need to change -std=c++11 to -std=c++0x
CXXFLAGS = -Wall
LDFLAGS =  $(SDL_LIB)
SRCS = $(wildcard ./*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))
TARGET = client




$(TARGET): $(SRCS) $(OBJS)

	$(CC) $(OBJS) $(LDFLAGS) -o $@ 

.cpp.o: 
	$(CC) $(CXXFLAGS) -c $<

clean:
	rm -rf *~ *.o $(TARGET)