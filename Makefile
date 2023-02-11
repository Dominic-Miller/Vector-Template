all: driver.o
	g++ -std=c++11 driver.cpp -o driver.x

driver.o: 
	g++ -std=c++11 -c driver.cpp
