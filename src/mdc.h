#pragma once

#include "utilities.h"
#include "model.h"
#include <vector>
using namespace std;

typedef struct MDC {
// Moment Density Curve (MDC). Spetral weight as a function of kx and ky.
public:

    int dimension;
    vector<double> mdc_data;
    
    
    MDC(int inputDim)
    {
        resize(inputDim);
    }
    
    void calculate(Model &model){
        if(model.verbose>=1) printf("printing MDC file\n");
        
        for (int px=0; px<dimension; ++px) {
            for (int py=0; py<dimension; ++py) {
                double px_continuous = M_PI * px/(double)(dimension-1.);
                double py_continuous = M_PI * py/(double)(dimension-1.);
                
                model.calculate_Gperiodized(px_continuous, py_continuous);
                mdc_data[dimension*px+py] = -M_1_PI*imag(model.G_per);
                
            }
        }
    }
    
    void printFile(Model &model)
    {
        calculate(model);
        FILE *file = fopen("mdc.dat","w");
        for (int py=0; py<dimension; ++py) {
            for (int px=0; px<dimension; ++px) 
            {
                fprintf(file,"% 4.8f  ", mdc_data[dimension*px + py]);
            }
            fprintf(file,"\n");
        }
        fclose(file);
        if(model.verbose>=1) printf("\rMDC file printed.\n");
    }
    
    void resize(int inputDim)
    {
        dimension = inputDim;
        nEntry_= dimension*dimension;
        mdc_data.resize(nEntry_);
        //gor_data_re.resize(mdc_numberOfKx*mdc_numberOfKy);
        //gor_data_im.resize(mdc_numberOfKx*mdc_numberOfKy);
    }
    
private:
    
    int nEntry_;
    // int lastUpdate=0;
    
    
} MDC;


