EXE = camelot.exe
SOURCES = parser.cpp camelot.cpp labels.cpp macros.cpp
HEADERS = parser.hpp labels.hpp macros.hpp

all : $(EXE)

$(EXE) :
	g++ $(SOURCES) $(HEADERS) -o $(EXE) -ggdb

release :
	g++ $(SOURCES) $(HEADERS) -o $(EXE) -O3 -Os
	strip $(EXE)