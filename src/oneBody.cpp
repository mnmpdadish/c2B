//
//  main.cpp
//  oneBody
//


#define _USE_MATH_DEFINES
#include "cuba.h"
#include "model.h"
#include "utilities.h"
#include "mdc.h"
#include "interactive.h"
#include "density.h"

using namespace std;

//#include <iomanip>


int main(int argc, const char * argv[]) {
    printf("oneBuddy starting\n\n");
    
    Model model;
    model.calculate_Hk(0.1,0.4);
    model.calculate_Gk(0.1,0.5);
    
    MDC mdc(201);
    //mdc.printFile(model);
    
    //interactive_mdc(model,mdc);
    //density(model);
    
    
    //cout << "\n" << sizeof(double) << " :)";
    //cout << model.calculate_Ak11(0.1,0.5);
    //cout << "\n" << model.calculate_Aktot(0.1,0.5);
    
    if(argc>=2){
		string opt(argv[1]);
        if (opt=="d") {density(model);}
        else if (opt=="mdc") {mdc.printFile(model);}
        //else if (opt=="dos") {printDOS(p);}
#ifdef INTERACTIVE
        else if (opt=="i") {interactive_mdc(model,mdc);}
#endif
        else {printf("could not understand the task to do.");}
    }/* else {
        if (p.density_trigger) density_david(p);
        if (p.density_trigger_loop) density_david_loop(p);
        if (p.mdc_trigger) {calculateMDC(p); printMDC(p);}
        if (p.dos_trigger) printDOS(p);
#ifdef INTERACTIVE
//      if (p.mdc_interactive_trigger) {printMDC_interactive(p);}
#endif
    }
    cout << endl << "oneBuddy over." << endl;
*/    

    return 0;
}
