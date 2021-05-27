
COMPILER := gcc

LINK := -llapack -lblas -lm -lstdc++ #-lcuba 

OPTIONS := -Wall -std=c++11 -O2 #-DCUBA
# other possible options:
#  -DWXT : use wxt terminal for gnuplot
#  -DX11 : use x11 terminal for gnuplot
#        without these two flags, default terminal is 'qt')
#
#  -DCUBA : to compile with cuba library (necessary for the dos task).
#           Cuba must be installed and '-lcuba' should be added to 
#           the LINK variable. See README file for details.
#
#  -DAZERTY : to use the azerty keyboard layout

EXEC = c2B

all: src/c2B.cpp
	$(COMPILER) $(OPTIONS) -o $(EXEC) src/c2B.cpp $(LINK)

