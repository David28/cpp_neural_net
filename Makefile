OBJECTOS = utils.o plot.o activation.o layer.o neural_network.o main.o

SFML_INC = /usr/include
SFML_LIB = /usr/lib

SRC_DIR = src
OBJ_DIR = obj

SOURCES = $(wildcard $(SRC_DIR)/**.cpp)

OBJECTOS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

all: link

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ -I inc -o $@ -c $<

link: $(OBJECTOS)
	g++ $(OBJECTOS) -o main -l sfml-graphics -l sfml-window -l sfml-system

clean:
	rm -f $(OBJ_DIR)/*.o main
