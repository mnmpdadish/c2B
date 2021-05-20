//
//  c2B.cpp
//  c2B
//

#define _USE_MATH_DEFINES
#include "cuba.h"
#include "model.h"
#include "utilities.h"
#include "mdc.h"
#include "dos.h"
#include "interactive.h"

using namespace std;


void printHelp(){
    printf("\nDifferent ways to use 'oneBody':\n\n");
    #ifdef INTERACTIVE
    printf("$ ./c2B i\n");
    printf("  Launch interactive plotting version.\n\n");
    #endif
    printf("$ ./c2B mdc\n");
    printf("  Print mdc.out file. (output = 'mdc.dat')\n");
    printf("  Plot in gnuplot with:\n");
    printf("  gnuplot> plot 'mdc.dat' matrix with image\n\n");
    printf("$ ./c2B dos\n");
    printf("  Print dos.out file. (output = 'dos.dat')\n");
    printf("  Plot in gnuplot with:\n");
    printf("  gnuplot> plot 'dos.dat' u 1:2\n\n");
}

int main(int argc, const char * argv[]) {
    if(argc>=2){
		  string opt(argv[1]);
        if      (opt=="mdc") {Model model; MDC mdc(200); mdc.printFile(model);}
        else if (opt=="dos") {
#ifdef CUBA
          Model model; DOS dos(model.omegaMin, model.omegaMax, model.nOmega); dos.printFile(model);
#else
          printf("\n\nPlease compile with option -DCUBA and make sure you have\n");
          printf("the integration library Cuba installed. See README for more information.\n");
#endif
        }
        
        else if (opt=="i") {
#ifdef INTERACTIVE
          Model model; MDC mdc(200); interactive_mdc(model, mdc);
#else
          printf("\n\nPlease compile with option -DINTERACTIVE and make sure you have\n");
          printf("Gnuplot 5 or above installed. See README for more information.\n");
#endif
        }
        else printHelp();
    } else {
        printHelp();
    }
    return 0;
}
