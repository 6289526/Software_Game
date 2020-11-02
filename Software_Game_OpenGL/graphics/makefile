CXX = g++
# Update these paths as necessary to match your installation
SDL_LIB = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_gfx
OpenGL_LIB = -lopengl32 -lglu32 -lm
OpenGL_LIB_WSL = -lglut -lGLU -lGL
# If your compiler is a bit older you may need to change -std=c++11 to -std=c++0x
CXXFLAGS = -Wall
LDFLAGS = -lmingw32 -mwindows -mconsole $(SDL_LIB)
SRCS = $(wildcard ./*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))
GRAPHIC_SRCS = graphic.cpp graphic_main.cpp client_map.cpp graphic_test.cpp image.cpp
GRAPHIC_OBJS = $(patsubst %.cpp,%.o,$(GRAPHIC_SRCS))
TARGET = client
GRAPHIC_TARGET = graphic
GRAPHIC_TARGET_WSL = $(GRAPHIC_TARGET)

all:$(TARGET)

graphic-wsl:$(GRAPHIC_TARGET_WSL)

graphics:$(GRAPHIC_TARGET)

$(GRAPHIC_TARGET):$(GRAPHIC_SRCS) $(GRAPHIC_OBJS)

	$(CXX) $(GRAPHIC_OBJS) $(LDFLAGS) $(OpenGL_LIB) -o $@ 

$(GRAPHIC_TARGET_WSL):$(GRAPHIC_SRCS) $(GRAPHIC_OBJS)

	$(CXX) $(GRAPHIC_OBJS) $(SDL_LIB) $(OpenGL_LIB_WSL) -o $@ 

$(GRAPHIC_OBJS):

	$(CXX) $(CXXFLAGS) -c $(patsubst %.o,%.cpp,$@) -o $@


$(TARGET): $(SRCS) $(OBJS)

	$(CXX) $(OBJS) $(LDFLAGS) -o $@ 

.cpp.o: 
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -rf *~ *.o $(TARGET) $(GRAPHIC_TARGET)