# comment these to toggle them as one sees fit.
# WARNINGS will spam hundreds of warnings, mostly safe, if turned on
# DEBUG is best turned on if you plan to debug in gdb -- please do!
# PROFILE is for use with gprof or a similar program -- don't bother generally
#WARNINGS = -Wall
DEBUG = -g
#PROFILE = -pg
#additional defs
DEFS = -DLOCALEDIR=\"locale\" -DPACKAGE=\"cataclysm\" -DTEMPERATURE_IN_CELSIUS

ODIR = obj
DDIR = .deps

TARGET = cataclysm

OS  = $(shell uname -o)
CXX = g++

CFLAGS = $(WARNINGS) $(DEBUG) $(PROFILE)

ifeq ($(OS), Msys)
#LDFLAGS = -static -lpdcurses
LDFLAGS = -Wl,-stack,12000000 -lpdcurses
else 
LDFLAGS = -lncursesw
endif

SOURCES = $(wildcard *.cpp)
_OBJS = $(SOURCES:.cpp=.o)
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

all: $(TARGET)
	@

$(TARGET): $(ODIR) $(DDIR) $(OBJS)
	$(CXX) -o $(TARGET) $(CFLAGS) $(OBJS) $(LDFLAGS) 

$(ODIR):
	mkdir $(ODIR)

$(DDIR):
	@mkdir $(DDIR)

$(ODIR)/%.o: %.cpp
	$(CXX) $(CFLAGS) $(DEFS) -c $< -o $@

clean:
	rm -f $(TARGET) $(ODIR)/*.o

-include $(SOURCES:%.cpp=$(DEPDIR)/%.P)
