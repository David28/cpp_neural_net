OBJECTOS = utils.o plot.o activation.o layer.o neural_network.o main.o

all: link open

%.o: src/%.cpp $($@)
	g++ -I inc -o obj/$@ -c $<	

link: $(OBJECTOS)
	g++ $(addprefix obj/,$(OBJECTOS)) -o main -L lib -l sfml-graphics -l sfml-window -l sfml-system
open:
	main.exe	
