CXX := g++
PROGNAME := client
OUTDIR := build
SRCDIR := ./
# Update these paths as necessary to match your installation
#
SDL_LIB = `sdl2-config --cflags --libs` -lSDL2_mixer -lSDL2_ttf -lSDL2_image -lcwiimote -lbluetooth -L/usr/lib -lm -lGL -lGLU
SDL_INCLUDE = -Iusr/include -I/usr/local/include/libcwiimote-0.4.0/libcwiimote
# If your compiler is a bit older you may need to change -std=c++11 to -std=c++0x
CXXFLAGS = $(SDL_INCLUDE) -std=c++11
LDFLAGS = $(SDL_LIB)
SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(addprefix $(OUTDIR),$(patsubst $(SRCDIR)%.cpp,%.o,$(SRCS)))
EXE = $(OUTDIR)/$(PROGNAME)


.PHONY: all

all: clean $(EXE)


$(EXE): $(OBJS)

	$(CXX) $^ $(LDFLAGS) -o $@

$(OBJS):
	$(CXX) $(CXXFLAGS) -c $(patsubst $(OUTDIR)%.o,$(SRCDIR)%.cpp,$@) -o $@

clean:
	rm -rf $(OUTDIR)/*~ $(OUTDIR)/*.o $(EXE)