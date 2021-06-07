/* 
*  mdc.h
*    Project: c2B
*    Authors: Maxime Charlebois, Simon Verret
*    License: MIT License
*/

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

    void printFile(Model &model, bool default_name=true)
    {
      calculate(model);
      char filename[100];
      if (default_name){
        sprintf(filename, "mdc.dat");
      } else if (model.periodization == 0) {
        sprintf(filename, "mdc_pG_eta%5.3f_mu%5.3f.dat", model.ETA, model.MU);
      } else if (model.periodization == 1) {
        sprintf(filename, "mdc_pM_eta%5.3f_mu%5.3f.dat", model.ETA, model.MU);
      } else if (model.periodization == 2) {
        sprintf(filename, "mdc_pC_eta%5.3f_mu%5.3f.dat", model.ETA, model.MU);
      } else {
        sprintf(filename, "mdc_eta%5.3f_mu%5.3f.dat", model.ETA, model.MU);
      }
      FILE *file = fopen(filename, "w");
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
    }

  private:

    int nEntry_;

} MDC;


