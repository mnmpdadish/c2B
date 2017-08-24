HEADERS = src/utilities.h
#OPTIONS = -O2 -fpermissive -Wall -std=c++11 -DINTERACTIVE -DBOUND_CHECK
OPTIONS = -O2 -fpermissive -Wall -std=c++11 -DINTERACTIVE -DBOUND_CHECK -DSUPRA
EXEC = oneBody

all: executable
executable: src/oneBody.cpp
	gcc $(OPTIONS) -o $(EXEC) src/oneBody.cpp  -lcuba -llapack -lblas -lstdc++

