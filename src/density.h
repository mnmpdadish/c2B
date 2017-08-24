//
//  density.h
//  oneBody
//
#pragma once 

#include "cuba.h"
#include "model.h"

// density (calculated on imaginary axis in complex plane) 


void iwk_Integrand(Model &model, const complex<double> z, const double pxc, const double pyc, double* output) {
    model.calculate_Gk(pxc,pyc,z);
    complex<double> integral = 0.0;
    #ifdef SUPRA
    integral += model.green.partialTrace();
    #else
    integral += model.green.trace();
    #endif
    *output = real(integral/2.0-1.0/(z-model.pole));
}

static int lowFreqCubaIntegrand(const int* ndim, const double x[],
                                const int* ncomp, double result[], void* userdata) {
    Model * p_model = (Model*)userdata;
    complex<double> z(0,x[0]*p_model->w_domain);
    iwk_Integrand(*p_model,z,x[1],x[2],result);
    return 0;
}

static int midFreqCubaIntegrand(const int* ndim, const double x[],
                                const int* ncomp, double result[], void* userdata) {
    Model * p_model = (Model*)userdata;
    complex<double> z(0,x[0]*p_model->w_domain+p_model->smallScale);
    iwk_Integrand(*p_model, z, x[1], x[2], result);
    return 0;
}

static int highFreqCubaIntegrand(const int* ndim, const double x[],
                                 const int* ncomp, double result[], void* userdata) {
    Model * p_model = (Model*)userdata;
    double iw = x[0]*p_model->w_domain;
    complex<double> z(0,1.0/iw);
    iwk_Integrand(*p_model, z, x[1], x[2], result);
    *result /= (iw*iw);
    return 0;
}

void cubaIntegrateDensity(Model & model, double* integral, double* error, int verboseIntegrate=2){
    void* userdata=&model;
    int NDIM=3;                     // const    number of dimensions
    int ncomp=1;                    // const    number of components of the integrand (could be a vector) // here, it is the argument of var integral
    int nvec=1;                     // const    number of samples passed to integrand; usually 1 (here it can be larger than the one passed to the Integrand.. why? no one knows)
    int KEY=11;                     // const    7, 9, 11 or 13 (see doc) for 3D should be 11
    void* spin=NULL;                // const    deepshit to manage internal process (keep it NULL)
    int nregions;                   //      OUT actual number of subregion
    int neval;                      //      OUT number of evaluation
    int fail;                       //      OUT error flag (see doc)
    //double error[1];              //      OUT error
    double prob[1];                 //      OUT another unreliable error (see doc)
    int verbose=0;
    
    integral[0] = 0;
    error[0] = 0;
    double integralPortion[1];
    double errorPortion[1];
    
    
    // Region 1
    model.w_domain = model.smallScale;
    
    double accur = model.EPSABS*M_PI/model.w_domain;
    
    Cuhre(NDIM, ncomp, lowFreqCubaIntegrand, userdata, nvec,
          model.EPSREL, accur, verbose, model.MINEVAL, model.MAXEVAL, KEY, NULL, spin,
          &nregions, &neval, &fail, integralPortion, errorPortion, prob);
    
    if(verboseIntegrate>=2) printf("Region 1 - Integral: %8.6f,   Error: %8.6f,   N_eval: %d \n", integralPortion[0]*model.w_domain*M_1_PI, errorPortion[0], neval);
    integral[0] += integralPortion[0]*model.w_domain*M_1_PI;
    error[0] += abs(errorPortion[0]*model.w_domain*M_1_PI);
    model.w_domain = model.largeScale-model.smallScale;
    accur = model.EPSABS*M_PI/model.w_domain;
    
    // Region 2
    Cuhre(NDIM, ncomp, midFreqCubaIntegrand, userdata, nvec,
          model.EPSREL, accur, verbose, model.MINEVAL, model.MAXEVAL, KEY,  NULL, spin,
          &nregions, &neval, &fail, integralPortion, errorPortion, prob);
    
    if(verboseIntegrate>=2) printf("Region 2 - Integral: %8.6f,   Error: %8.6f,   N_eval: %d \n", integralPortion[0]*model.w_domain*M_1_PI, errorPortion[0], neval);
    integral[0] += integralPortion[0]*model.w_domain*M_1_PI;
    error[0] += abs(errorPortion[0]*model.w_domain*M_1_PI);
    model.w_domain = 1.0/model.largeScale;
    accur = model.EPSABS*M_PI/model.w_domain;
    
    // Region 3
    Cuhre(NDIM, ncomp, highFreqCubaIntegrand, userdata, nvec,
          model.EPSREL, accur, verbose, model.MINEVAL, model.MAXEVAL, KEY,  NULL, spin,
          &nregions, &neval, &fail, integralPortion, errorPortion, prob);
    
    if(verboseIntegrate>=2) printf("Region 3 - Integral: %8.6f,   Error: %8.6f,   N_eval: %d \n", integralPortion[0]*model.w_domain*M_1_PI, errorPortion[0], neval);
    integral[0] += integralPortion[0]*model.w_domain*M_1_PI;
    error[0] += abs(errorPortion[0]*model.w_domain*M_1_PI);
    
    integral[0] *=2; // for the spin
    error[0] *=2;
    
}

void density(Model & model){
    printf("\nCalculating density, int over imaginary axis 'iw' (3 regions):\n");
    double integral[1]; //container for integral result (1D value, could be multiD)
    double error[1];
    integral[0] = 0;
    cubaIntegrateDensity(model,integral,error);
    
    //FILE *file = fopen("n.out","w");
    //fprintf(file," mu     n          err\n");
    //fprintf(file,"% 5.2f  % 8.6f  % 8.6f\n",model.MU,integral[0],error[0]);
    //fclose(file);
    printf("\n n = %8.6f \n", integral[0]);
}


void density_loopMU(Model & model){
    if(model.verbose >= 1) printf("\nPrinting density file. \nCalculating density, int over imaginary axis 'iw' (3 regions):\n");
    double integral[1]; //container for integral result
    double error[1];
    //double mu0=model.MU;
    
    FILE *file = fopen("n.out","w");
    fprintf(file," mu     n          err\n");
    printf(" mu     n          err\n");
    
    int m=0; for(m=0; m<model.nMu; m++)
    {
        double mu = model.muMin + m*(model.muMax-model.muMin)/(model.nMu-1);
        //double percentage = (100.*m)/(1.0*model.nMu);
        model.MU=mu;
        cubaIntegrateDensity(model,integral, error, model.verbose);
        fprintf(file,"% 5.2f  % 8.6f  % 8.6f\n",mu,integral[0],error[0]);
        printf("% 5.2f  % 8.6f  % 8.6f\n",mu,integral[0],error[0]);
    }
    fclose(file);
    if(model.verbose >= 1) printf("\rDensity file printed.\n");
}


