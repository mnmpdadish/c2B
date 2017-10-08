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
   MKL := /opt/intel/mkl/lib/intel64
	#MKL := /opt/intel/mkl/lib/ia32
   COMPILER := gcc
   ##LINK := -lcuba -lstdc++ $(MKL)/libmkl_blas95_lp64.a $(MKL)/libmkl_lapack95_lp64.a $(MKL)/libmkl_intel_lp64.a $(MKL)/libmkl_sequential.a $(MKL)/libmkl_core.a -lm 
   LINK := -lcuba -lstdc++  ${MKL}/libmkl_intel_lp64.a -Wl,--start-group $(MKL)/libmkl_blas95_lp64.a $(MKL)/libmkl_lapack95_lp64.a $(MKL)/libmkl_sequential.a ${MKL}/libmkl_core.a -Wl,--end-group  -lgomp -lpthread -lm -ldl
   ##LINK := -lcuba -lstdc++ -Wl,--start-group ${MKLROOT}/lib/intel64/libmkl_intel_ilp64.a $(MKL)/libmkl_lapack95_lp64.a ${MKLROOT}/lib/intel64/libmkl_gnu_thread.a $(MKL)/libmkl_sequential.a ${MKLROOT}/lib/intel64/libmkl_core.a -Wl,--end-group -lgomp -lpthread -lm -ldl
   #LINK := -lcuba -llapack -lblas -lm -lgfortran -lstdc++
   OPTIONS := -fpermissive -Wall -std=c++11 -DINTERACTIVE -DSUPRA 
   EXEC = oneBody
endif

HEADERS = src/utilities.h

all: executable
executable: src/oneBody.cpp
	$(COMPILER) $(OPTIONS) -o $(EXEC) src/oneBody.cpp $(LINK)

