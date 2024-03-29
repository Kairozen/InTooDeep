LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

CC_FLAGS = -c -O2 -Wall -std=c++11

CXX := g++

all: application

%.o: %.cpp
		$(CXX) $(CC_FLAGS) $< -o $@; mv src/*.o obj/

%.o: %.hpp
		$(CXX) $(CC_FLAGS) $< -o $@; mv src/*.o obj/

application: src/main.o src/Game.o src/Tilemap.o src/Character.o src/UserInput.o src/Entity.o src/Monster.o src/Audio.o src/ClockManager.o
		@echo "Building the game"
		$(CXX) -o application obj/*.o $(LIBS)


clean:
		@echo "Removing object files and executable..."
		rm application obj/*.o