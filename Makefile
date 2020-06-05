# Makefile for KTH's code.

include Makefile.inc

SOURCES = src/extra/*.cpp src/extra/tweeny/*.cpp src/extra/coldet/*.cpp src/*.cpp

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard $(SOURCES)))
DEPENDS = $(patsubst %.cpp, %.d, $(wildcard $(SOURCES)))

SDL_LIB = -lSDL2 
GLUT_LIB = -lGL -lGLU 

LIBS = $(SDL_LIB) $(GLUT_LIB)

all:	main

main:	$(DEPENDS) $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LIBS) -o $@

%.d: %.cpp
	@$(CXX) -M -MT "$*.o $@" $(CPPFLAGS) $<  > $@
	@echo Generating new dependencies for $<

run:
	./main

clean:
	rm -f $(OBJECTS) $(DEPENDS) main *.pyc

-include $(SOURCES:.cpp=.d)

