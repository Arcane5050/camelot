EXE = camelot.exe

all : $(EXE)

$(EXE) :
	g++ parser.cpp camelot.cpp labels.cpp macros.cpp parser.hpp labels.hpp macros.hpp -o $(EXE) -ggdb