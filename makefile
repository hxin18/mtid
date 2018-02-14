main: update.o data.o strtokenizer.o main.cpp
	g++ -O2 -fopenmp  update.o data.o strtokenizer.o main.cpp -o main

update.o: update.cpp update.h
	g++ -c -O2 -fopenmp update.cpp

data.o: data.cpp data.h
	g++ -c -O2 -fopenmp data.cpp

strtokenizer.o: strtokenizer.cpp strtokenizer.h
	g++ -c -O2 -fopenmp strtokenizer.cpp

clean:
	rm *.o