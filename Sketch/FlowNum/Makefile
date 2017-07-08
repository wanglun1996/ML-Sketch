CPPFLAGS = -Wall -O3 -std=c++11 -lm -w
PROGRAMS = CF

all: $(PROGRAMS)

CF: main.cpp params.h FMsketch.h BOBHash.h
	g++ -o CF main.cpp $(CPPFLAGS)

clean:
	rm -f *.o $(PROGRAMS)
