all: main clean

main: main.cpp
	echo "Compiling..."
	-g++ -std=c++11 -c display.cpp
	-g++ -std=c++11 -c main.cpp
	-g++ -std=c++11 main.o display.o -o main \
	-I include -L lib -l SDL2-2.0.0
	echo "Compiled.\nRunning..."
	-./main

clean: 
	rm -f display.o
	rm -f main.o
	rm -f main
	echo "Cleaned".

.PHONY: all clean
.SILENT: main clean