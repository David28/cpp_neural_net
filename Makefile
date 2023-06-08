all: compile link open

compile:
	g++ -I inc -c src/main.cpp -o obj/main.o
	g++ -I inc -c src/plot.cpp -o obj/plot.o
	g++ -I inc -c src/utils.cpp -o obj/utils.o
link:
	g++ obj/main.o obj/utils.o obj/plot.o -o main -L lib -l sfml-graphics -l sfml-window -l sfml-system
open:
	main.exe	
