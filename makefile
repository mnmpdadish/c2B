
COMPILER := gcc
#MKL := /opt/intel/mkl/lib/intel64  #only to compile with MKL
#LINK := -lcuba -lstdc++  ${MKL}/libmkl_intel_lp64.a -Wl,--start-group $(MKL)/libmkl_blas95_lp64.a $(MKL)/libmkl_lapack95_lp64.a $(MKL)/libmkl_sequential.a ${MKL}/libmkl_core.a -Wl,--end-group  -lgomp -lpthread -lm -ldl
 
LINK := -lcuba -llapack -lblas -lm -lstdc++   
OPTIONS := -Wall -Wpedantic -Wextra -Wshadow -std=c++11 -DINTERACTIVE -O2 -DWXT -DCUBA
#OPTIONS := -Wall -Wpedantic -Wextra -Wshadow -std=c++11 -DINTERACTIVE -O2 -DQT
# other possibles options:
#  -DWXT : use wxt terminal for gnuplot
#  -DX11 : use x11 termianl for gnuplot
#  -DQT  : use qt  terminal for gnuplot
#
#  -DAZERTY : to use the azerty keyboard layout 
EXEC = c2B

HEADERS = src/utilities.h

all: executable
executable: src/c2B.cpp
	$(COMPILER) $(OPTIONS) -o $(EXEC) src/c2B.cpp $(LINK)

