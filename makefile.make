all: main.exe clean

#CXXFLAGS := -O3 -march=native -mtune=native

main.exe: main.o files_handler.o Token.o xBS.o freq_handler.o rate_calculator.o
	g++ $(CXXFLAGS)  main.o files_handler.o Token.o xBS.o freq_handler.o rate_calculator.o -o main.exe


main.o: 
	g++ $(CXXFLAGS) -c main.cpp

files_handler.o: 
	g++ $(CXXFLAGS) -c  files_handler.cpp 

Token.o: 
	g++ $(CXXFLAGS) -c  Token.cpp

xBS.o:
	g++ $(CXXFLAGS) -c  xBS.cpp

freq_handler.o:
	g++ $(CXXFLAGS) -c  freq_handler.cpp

rate_calculator.o:
	g++ $(CXXFLAGS) -c  rate_calculator.cpp


clean:
	rm *.o
