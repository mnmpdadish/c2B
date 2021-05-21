#pragma once
#include "cuba.h"
#include "model.h"
#include "utilities.h"
#include <vector>
using namespace std;


static int dosIntegrand(const int* ndim, const double x[],
                     const int* ncomp, double result[], void* userdata) {
    double px_continuous = M_PI * x[0];
    double py_continuous = M_PI * x[1];
    Model *model = (Model*)userdata;
    
    model->calculate_Gperiodized(px_continuous, py_continuous);
    *result = -M_1_PI*imag(model->G_per);
    
    return 0;
}

int cubaIntegrateDOS(Model &model, double* result, double* error){
    void* userdata=&model;
    int NDIM=2;                     // const    number of dimensions
    int ncomp=1;                    // const    number of components of the integrand (could be a vector) // here, it is the argument of var result
    int nvec=1;                     // const    number of samples passed to integrand; usually 1 (here it can be larger than the one passed to the Integrand.. why? no one knows)
    double EPSREL=0.001;
    if (model.EPSREL!=0) EPSREL=model.EPSREL;
    double EPSABS=0.0001;
    if (model.EPSABS!=0) EPSABS=model.EPSABS;
    int verbose=0;
    int MINEVAL= (int)(2.5/(model.ETA*model.ETA));      //(1000/((model.ETA*model.ETA)/(0.05*0.05)));
    if (model.MINEVAL!=0) MINEVAL=model.MINEVAL;
    int MAXEVAL=400000;
    if (model.MAXEVAL!=0) MAXEVAL=model.MAXEVAL;
    int KEY=13;                     // const    7, 9, 11 or 13 (see doc) for 2D should be 13
    void* spin=NULL;                // const    deepshit to manage internal process (keep it NULL)
    int nregions;                   //      OUT actual number of subregion
    int neval;                      //      OUT number of evaluation
    int fail;                       //      OUT error flag (see doc)
    double prob[ncomp];             //      OUT another unreliable error (see doc)
    
    Cuhre(NDIM, ncomp, dosIntegrand, userdata, nvec,
          EPSREL, EPSABS, verbose, MINEVAL, MAXEVAL, KEY, NULL, spin,
          &nregions, &neval, &fail, result, error, prob);
    return neval;
}



int gridIntegrateDOS(Model &model, double* integral, double* error, int verboseIntegrate=1){

    void* userdata=&model;
    int NDIM=2;                     // const    number of dimensions

    int ncomp=1;
    double result[1];
    
    int grid_nMin = 5;
    int grid_nMax = 11;
    
    int steps = 1 << (grid_nMin - 1);
    for(int n = 0; n<ncomp; n++) integral[n] = .0;

    double EPSREL=0.001;
    if (model.EPSREL!=0) EPSREL=model.EPSREL;
    double EPSABS=0.0001;
    if (model.EPSABS!=0) EPSABS=model.EPSABS;
    
    bool continueFlag=false;
    do {
        steps *= 3;	
        steps /= 2;	
        for(int n = 0; n<ncomp; n++) error[n] = integral[n]; 

        for(int n = 0; n<ncomp; n++) integral[n] = .0;
        for(int i = 0; i < steps; i++) 
           for(int j = 0; j < steps; j++) {
               double k[] = {((double)i) / ((double)(steps)) , ((double)j) / ((double)(steps))};
               dosIntegrand(&NDIM, k, &ncomp, result, userdata);
               for(int n = 0; n<ncomp; n++) integral[n] += result[n];
           }

        for(int n = 0; n<ncomp; n++) integral[n] /= ((double)(steps*steps)) ;

        continueFlag=false;
        for(int n = 0; n<ncomp; n++) {
            error[n] -= integral[n];
            if(abs(error[n]) > EPSREL*abs(integral[n]) ) {
                if(abs(error[n]) > EPSABS){
                    if(steps < (1 << grid_nMax)){
                        if(n<3) continueFlag=true;  //convergence criterion only check for the first 3 value to integrate here, the one obtained with A^3 and not the integral by part.
                    }
                }
            }
            
        }
        //printf("\n\nsteps=%d", steps);			
        //for(int n = 0; n<ncomp; n++) printf(", val%d=% 4.3e err%d=% 4.2e", n, integral[n], n, error[n]); 
    } while(continueFlag);
    
    return steps*steps;
}

typedef struct DOS {
// Density Of States (DOS). Integral of the spetral weight as a function omega.
public:

    double omega_start;
    double omega_end;
    int resolution;
    vector<double> dos_data;

    DOS(double start, double end, int res)
    {
        omega_start = start;
        omega_end = end;
        resolution = res;
        dos_data.resize(resolution);
    }

    void printFile(Model &model){
        if(model.verbose>=1) printf("printing DOS file\n");

        double integral[1]; //container for integral result
        double error[1];
        double omegaOrig=model.OMEGA;
        
        FILE *file = fopen("dos.out","w");
        fprintf(file," w      dos          err\n");
        printf(      " w      dos          err\n");
        
       for (int w=0; w<resolution; w++) {
            
            double omega = omega_start + w*(omega_end-omega_start)/(resolution-1);
            model.OMEGA = omega;
            int steps_total = cubaIntegrateDOS(model, &integral[0], &error[0]);
            fprintf(file,"% 5.2f  % 8.6f  % 8.6f\n",omega,integral[0],error[0]);
            printf("% 5.2f  % 8.6f  % 8.6f    % d\n",omega,integral[0],error[0], steps_total);
            fflush(file);
        }
        fclose(file);
        if(model.verbose >= 1) printf("\rDOS file printed.\n");
    
        model.OMEGA = omegaOrig;
    }

} DOS;

