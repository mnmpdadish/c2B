HEADERS = src/utilities.h
OPTIONS = -O2 -fpermissive -Wall -std=c++11 -DINTERACTIVE
EXEC = oneBody

all: executable
executable: src/oneBody.cpp $(HEADERS)
	gcc $(OPTIONS) -o $(EXEC) src/oneBody.cpp  -lcuba -llapack -lblas -lstdc++

