CXX := g++
PROGNAME := server
OUTDIR := build
SRCDIR := .
# Update these paths as necessary to match your installation
# 
SDL_LIB = `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lSDL2_gfx -L/usr/lib -L/usr/lib/x86_64-linux-gnu -lm
SDL_INCLUDE = -Iusr/include
# If your compiler is a bit older you may need to change -std=c++11 to -std=c++0x
CXXFLAGS = $(SDL_INCLUDE)
LDFLAGS = $(SDL_LIB) -std=c++11
SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(addprefix $(OUTDIR),$(patsubst $(SRCDIR)%.cpp,%.o,$(SRCS)))
EXE = $(OUTDIR)/$(PROGNAME)


.PHONY: all

all: $(EXE)


$(EXE): $(OBJS)

	$(CXX) $^ $(LDFLAGS) -o $@

$(OBJS):
	$(CXX) $(CXXFLAGS) -c $(patsubst $(OUTDIR)%.o,$(SRCDIR)%.cpp,$@) -o $@
