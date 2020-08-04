//
//  main.cpp
//  oneBody
//


#define _USE_MATH_DEFINES
#include "cuba.h"
#include "model.h"
#include "utilities.h"
#include "mdc.h"
#include "dos.h"
#include "interactive.h"
#include "density.h"

using namespace std;


void printHelp(){
    printf("\nDifferent ways to use 'oneBody':\n\n");
    #ifdef INTERACTIVE
    printf("$ ./oneBody i\n");
    printf("  Launch interactive plotting version.\n\n");
    #endif
    printf("$ ./oneBody d\n");
    printf("  Calcualte density.\n\n");
    printf("$ ./oneBody l\n");
    printf("  Calcualte density loop over mu. (output = 'n.out')\n");
    printf("  Plot in gnuplot with:\n");
    printf("  gnuplot> plot 'n.out' u 1:2 w lp\n\n");
    printf("$ ./oneBody mdc\n");
    printf("  Print mdc.out file. (output = 'mdc.dat')\n");
    printf("  Plot in gnuplot with:\n");
    printf("  gnuplot> plot 'mdc.dat' matrix with image\n\n");
    printf("$ ./oneBody dos\n");
    printf("  Print dos.out file. (output = 'dos.dat')\n");
    printf("  Plot in gnuplot with:\n");
    printf("  gnuplot> plot 'dos.dat' u 1:2\n\n");
}

int main(int argc, const char * argv[]) {
    if(argc>=2){
		  string opt(argv[1]);
        if (opt=="d") {Model model; density(model);}
        else if (opt=="l") {Model model; density_loopMU(model);}
        else if (opt=="mdc") {Model model; MDC mdc(200); mdc.printFile(model);}
        else if (opt=="dos") {Model model; DOS dos(-3.0,3.0,400); dos.printFile(model);}
        #ifdef INTERACTIVE
        else if (opt=="i") {Model model; MDC mdc(200); interactive_mdc(model, mdc);}
        #endif
        else printHelp();
    } else {
        printHelp();
    }
    return 0;
}
