all: predictors

predictors: main.o Predictor.o Instruction.o
	g++ *.o -std=c++17 -o predictors

main.o: main.cpp
	g++ -c -g -Wall -std=c++17 main.cpp -o main.o

Predictor.o: Predictor.h Predictor.cpp
	g++ -c -g -Wall -std=c++17 Predictor.cpp -o Predictor.o

Instruction.o: Instruction.h Instruction.cpp
	g++ -c -g -Wall -std=c++17 Instruction.cpp -o Instruction.o

clean:
	rm -f *.o predictors
