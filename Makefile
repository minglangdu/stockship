all: main clean

main: main.cpp
	echo "Compiling..."
	-g++ -std=c++11 -c display.cpp -I /opt/homebrew/include
	-g++ -std=c++11 -c main.cpp -I /opt/homebrew/include
	-g++ -std=c++11 main.o display.o -o main \
	-I /opt/homebrew/include -L /opt/homebrew/lib -lSDL2
	echo "Compiled.\nRunning..."
	-./main

clean: 
	rm -f display.o
	rm -f main.o
	rm -f main
	echo "Cleaned".

.PHONY: all clean
.SILENT: main clean