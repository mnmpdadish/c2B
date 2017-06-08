//
//  main.cpp
//  oneBuddy
//

#include <iostream>
#include <fstream>
#include <complex>
#include <cassert>
#include <ctime>
#include <vector>
#include <queue>
#include <math.h>
#define _USE_MATH_DEFINES
#include "cuba.h"
#include "utilities.h"
using namespace std;

#include <iomanip>


//complex<double> green[LARGE_VALUE_2];
//complex<double> hamiltonian[LARGE_VALUE_2];




// MATRIX INVERSE:

extern "C" {
    void zgetrf_(int*, int* , complex<double>*, int*, int*, int*);
    void zgetri_(int*, complex<double>*, int*, int*, complex<double>*, int*, int*);
}

typedef struct BasicMatrix {
public:
    int dim; 
    int nEntry;
    complex<double> * data;      //can be assigned directly

    BasicMatrix() {};
    BasicMatrix(int inputDim)
    {
        dim = inputDim;
        nEntry = dim*dim;
        data = new complex<double>[nEntry];
        
        IPIV_ = new int[dim+1];
        WORK_ = new complex<double>[nEntry];
    };
    
    ~BasicMatrix()
    {
        delete data;
        delete IPIV_;
        delete WORK_;
    };

    void dataCopy(BasicMatrix &inputMatrix) {
        if(nEntry != inputMatrix.nEntry) {printf("ERROR: cannot copy the matrix'\n\n"); exit(1);}
        for(int i=0; i<nEntry; i++) data[i] = inputMatrix.data[i];
    };
    
    /*
    const int size(){
        return nEntry;
    }
    */
    
    complex<double> trace()
    {
        complex<double> sum=0.0;
        for(int i=0; i<nEntry; i++) sum+= data[i*dim+i];  //sum on diagonal 
        return sum;
    };
    

    void invert() {
        int INFO;
        zgetrf_(&dim,&dim,data,&dim,IPIV_,&INFO);
        zgetri_(&dim,data,&dim,IPIV_,WORK_,&nEntry,&INFO);
        if( !(INFO == 0) ) {
            printf( "The algorithm failed to invert the matrix.\n" );
            exit( 1 );
        }
    };
    
    void print() {
        int i, j;
        for( i = 0; i < dim; i++ ) {
            for( j = 0; j < dim; j++ )
                printf( " (%4.2f,%4.2f)", real(data[i+j*dim]), imag(data[i+j*dim]) );
            printf( "\n" );
        }
        printf( "\n\n" );
        fflush(stdout);
    }    


private:    
    int *IPIV_ ;
    complex<double> *WORK_ ;

} BasicMatrix;



















typedef struct Model {
// things that define the model (most can be read in para.dat)
public:
    
    Model(): hamiltonian_(4), green_(4)   
    {
        if (not exists("para.dat")) {printf("ERROR: couldn't find file 'para.dat'\n\n"); exit(1);}
        printf("reading parameters from para.dat\n\n") ;
        ifstream file;
        file.open("para.dat");
        
        //hamiltonian parameters:
        readNumber(file,"MU",MU_); 
        readNumber(file,"ETA",ETA_);
        readNumber(file,"OMEGA",OMEGA_);
        readNumber(file,"t",t_);
        readNumber(file,"tp",tp_);
        readNumber(file,"tpp",tpp_);
        readNumber(file,"M",M_);
        readNumber(file,"D",D_);
        
        
        /*
        //cuba parameters:
        readNumber(file,"EPSREL",EPSREL_);
        readNumber(file,"EPSABS",EPSABS_);
        readNumber(file,"MAXEVAL",MAXEVAL_); 
        readNumber(file,"MINEVAL",MINEVAL_);
        readNumber(file,"VERBOSE",VERBOSE_);
        */
        
        if(dbEqual(D_,0.0)) printf("is not superconductive\n");
        if(dbEqual(M_,0.0)) printf("is not AFM\n");
        
    }
    
    bool isSuperconductive() {return dbEqual(D_,0.0);};
    bool isAFM() {return dbEqual(M_,0.0);};
    
    /*
    unsigned int matricesDimension()
    {
        unsigned int dim=1;
        if(isSuperconductive()) dim*=2;
        if(isAFM()) dim*=2;
        return dim;
    }
    */
    
    void calculate_Hk(const double px, const double py)
    //Hk = Hamiltonian matrix
    {
        double kx = 2*M_PI*px;
        double ky = 2*M_PI*py;
        
        double kxpp = 2*M_PI*(px+0.5); //0.5 = AFM SDW Q vector
        double kypp = 2*M_PI*(py+0.5); //0.5 = AFM SDW Q vector

        double t_k = 2*t_  * (cos(kx)    + cos(ky))
                    +2*tp_ * (cos(kx+ky) + cos(kx-ky))
                    +2*tpp_* (cos(2*kx)  + cos(2*ky));
                    
        double t_kpp = 2*t_  * (cos(kxpp)      + cos(kypp))
                      +2*tp_ * (cos(kxpp+kypp) + cos(kxpp-kypp))
                      +2*tpp_* (cos(2*kxpp)    + cos(2*kypp));
                       
        double D_k   = D_ * (cos(kx)   - cos(ky));
        double D_kpp = D_ * (cos(kxpp) - cos(kypp));
    
        //assignation of the 4 by 4 Hk hamiltonian:
        //This matrix is in Nambu notation: indices 0,1 are normal and 2,3 are in the Nambu space.
        
        hamiltonian_.data[0] =-t_k-MU_;   hamiltonian_.data[1] =M_;           hamiltonian_.data[2] =D_k;       hamiltonian_.data[3] =0.0;
        hamiltonian_.data[4] =M_;         hamiltonian_.data[5] =-t_kpp-MU_;   hamiltonian_.data[6] =0.0;       hamiltonian_.data[7] =D_kpp;
        hamiltonian_.data[8] =D_k;        hamiltonian_.data[9] =0;            hamiltonian_.data[10]=t_k+MU_;   hamiltonian_.data[11]=M_;
        hamiltonian_.data[12]=0.0;        hamiltonian_.data[13]=D_kpp;        hamiltonian_.data[14]=M_;        hamiltonian_.data[15]=t_kpp+MU_;
        
        hamiltonian_.print();
    };
    
    void calculate_Gk(const double px, const double py)
    //Gk = Green matrix
    {
        // to sum up, the Green matrix is Gk = 1/(omega+i*eta-Hk)
        calculate_Hk(px, py);
        for(int i=0;i<green_.dim;i++)
            for(int j=0;j<green_.dim;j++)
            {
                green_.data[i*green_.dim +j] = -hamiltonian_.data[i*green_.dim +j];
                if(i==j) green_.data[i*green_.dim +j] += complex<double>(OMEGA_,ETA_);    
            }
        green_.invert();
        green_.print();
    }
    
    double calculate_Ak11(const double px, const double py)
    {
        calculate_Gk(px, py);
        return (-M_1_PI*imag(green_.data[0]));
    }
    
    double calculate_Aktot(const double px, const double py)
    {
        calculate_Gk(px, py);
        double sum=0.0;
        for(int i=0; i<green_.nEntry; i++) sum+= -M_1_PI*imag(green_.trace());  
        return sum/green_.dim;
    }



private:

    int verbose_=1;

    //hamiltonian parameters:
    double MU_=0.0;
    double ETA_=0.1;
    double OMEGA_=0.0;    
    double t_=1.0;
    double tp_=0.0;
    double tpp_=0.0;
    double M_=0.0;
    double D_=0.0;

    //cuba parameters:
    double EPSREL_=0.0001;
    double EPSABS_=0.1;
    int MAXEVAL_=4000;
    int MINEVAL_=40;
    int VERBOSE_=0;
    
    //matrices
    BasicMatrix hamiltonian_;
    BasicMatrix green_;
    
} Model;






int main(int argc, const char * argv[]) {
    printf("oneBuddy starting\n\n");
    
    Model model;
//    model.calculate_Hk(0.1,0.4);
    model.calculate_Gk(0.1,0.5);
    //cout << model.calculate_Ak11(0.1,0.5);

    //cout << "\n" << model.calculate_Aktot(0.1,0.5);
    
/*    
    if(argc>=2){
		string opt(argv[1]);
        if (opt=="density") {density_david(p);}
        else if (opt=="mdc") {calculateMDC(p);printMDC(p);}
        else if (opt=="dos") {printDOS(p);}
#ifdef INTERACTIVE
//        else if (opt=="i") {printMDC_interactive(p);}
#endif
        else {printf("could not understand the task to do.");}
    } else {
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
