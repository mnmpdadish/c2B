MAMMOUTH := $(shell echo $(pbsserver)) # should return empty, 'ms' or 'mp2'

ifeq ($(MAMMOUTH),ms )
   COMPILER :=g++
   LINK := -lmkl_lapack95_lp64 -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lcuba
   OPTIONS = -O2 -fpermissive -std=c++0x -DSUPRA
   EXEC = $(HOME)/bin/oneBody_ms

else ifeq ($(MAMMOUTH),mp2 )
   COMPILER :=g++
   LINK := -lmkl_lapack95_lp64 -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lcuba
   OPTIONS := -O2 -fpermissive -std=c++11 -DSUPRA
   EXEC = $(HOME)/bin/oneBody_mp

else
   COMPILER := gcc
   #LINK := -lcuba -llapack -lstdc++
   LINK := -lcuba -llapack -lblas -lm -lgfortran -lstdc++
   OPTIONS := -O2 -fpermissive -Wall -std=c++11 -DINTERACTIVE -DSUPRA
   EXEC = oneBody
endif

HEADERS = src/utilities.h

all: executable
executable: src/oneBody.cpp
	$(COMPILER) $(OPTIONS) -o $(EXEC) src/oneBody.cpp $(LINK)

