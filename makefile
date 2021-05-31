
COMPILER := gcc

LINK := -llapack -lblas -lm -lstdc++ #-lcuba 

OPTIONS := -Wall -std=c++11 -O2 #-DCUBA
# other possible options:
#  -DCUBA : to compile with cuba library (necessary for the dos task).
#           Cuba must be installed and '-lcuba' should be added to 
#           the LINK variable. See README file for details.
#
#  -DAZERTY : to use the AZERTY keyboard layout intead of the QWERTY.
#
#  -DTERMINAL : specify the terminal you want. For example:
#               -DTERMINAL=\"x11\" force the terminal x11 instead 
#               of the default terminal. See README file for details.


EXEC = c2B

all: src/c2B.cpp
	$(COMPILER) $(OPTIONS) -o $(EXEC) src/c2B.cpp $(LINK)

