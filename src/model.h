#pragma once

#include "basicMatrix.h"

typedef struct Model {
// things that define the model (most can be read in para.dat)
public:

    int verbose=1;

    //hamiltonian parameters:
    double MU=0.0;
    double ETA=0.1;
    double OMEGA=0.0;    
    double t=1.0;
    double tp=0.0;
    double tpp=0.0;
    double M=0.0;
    double D=0.0;

    int nbOfTerms=8;
    
    //cuba parameters:
    double EPSREL=0.0001;
    double EPSABS=0.0001;
    int MAXEVAL=1000000;
    int MINEVAL=15000;
    int VERBOSE=0;
    
    //density parameters:
    double smallScale=0.2;
    double largeScale=10.0;
    double w_domain=1.0;
    double pole=3.0;
    double beta=50.0;
        
    //matrices
    BasicMatrix hamiltonian;
    BasicMatrix green;
    
    Model(): hamiltonian(4), green(4) 
    {
        if (not exists("para.dat")) {printf("ERROR: couldn't find file 'para.dat'\n\n"); exit(1);}
        printf("reading parameters from para.dat\n\n") ;
        ifstream file;
        file.open("para.dat");
        
        //hamiltonian parameters:
        readNumber(file,"MU",MU); 
        readNumber(file,"ETA",ETA);
        readNumber(file,"OMEGA",OMEGA);
        readNumber(file,"t",t);
        readNumber(file,"tp",tp);
        readNumber(file,"tpp",tpp);
        readNumber(file,"M",M);
        readNumber(file,"D",D);
        
        //cuba parameters:
        readNumber(file,"EPSREL",EPSREL);
        readNumber(file,"EPSABS",EPSABS);
        readNumber(file,"MAXEVAL",MAXEVAL); 
        readNumber(file,"MINEVAL",MINEVAL);
        readNumber(file,"VERBOSE",VERBOSE);
        
        //if(dbEqual(D,0.0)) printf("is not superconducter\n");
        //if(dbEqual(M,0.0)) printf("is not AFM\n");
        
        
    }
    
    bool isSuperconductive() {return dbEqual(D,0.0);};
    bool isAFM() {return dbEqual(M,0.0);};
    
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
        double kx = 2*M_PI*(px-0.5); //offset to center the data
        double ky = 2*M_PI*(py-0.5);
        
        double kxQ = 2*M_PI*px; //Qx=+0.5 : AFM SDW 
        double kyQ = 2*M_PI*py; //Qy=+0.5 : AFM SDW 

        double tk =  2*t  * (cos(kx)    + cos(ky))
                    +2*tp * (cos(kx+ky) + cos(kx-ky))
                    +2*tpp* (cos(2*kx)  + cos(2*ky));
                    
        double tkQ =  2*t  * (cos(kxQ)     + cos(kyQ))
                     +2*tp * (cos(kxQ+kyQ) + cos(kxQ-kyQ))
                     +2*tpp* (cos(2*kxQ)   + cos(2*kyQ));
                       
        double Dk  = D * (cos(kx)  - cos(ky));
        double DkQ = D * (cos(kxQ) - cos(kyQ));
    
        //assignation of the 4 by 4 Hk hamiltonian:
        //This matrix is in Nambu notation: indices 0,1 are normal and 2,3 are in the Nambu space.
        
        hamiltonian(0,0)=-tk-MU;    hamiltonian(0,1)=M;           hamiltonian(0,2)=Dk;        hamiltonian(0,3)=0.0;
        hamiltonian(1,0)=M;         hamiltonian(1,1)=-tkQ-MU;     hamiltonian(1,2)=0.0;       hamiltonian(1,3)=DkQ;
        hamiltonian(2,0)=Dk;        hamiltonian(2,1)=0.0;         hamiltonian(2,2)=tk+MU;     hamiltonian(2,3)=M;
        hamiltonian(3,0)=0.0;       hamiltonian(3,1)=DkQ;         hamiltonian(3,2)=M;         hamiltonian(3,3)=tkQ+MU;
        
        //hamiltonian.print();
    };
    
    void calculate_Gk(const double px, const double py)
    //Gk = Green matrix
    {
        // to sum up, the Green matrix is Gk = 1/(omega+i*eta-Hk)
        calculate_Hk(px, py);
        for(int i=0;i<green.dim;i++)
            for(int j=0;j<green.dim;j++)
            {
                green(i,j) = -hamiltonian(i,j);
                if(i==j) green(i,j) += complex<double>(OMEGA,ETA);    
            }
        green.invert();
    }
    
    void calculate_Gk(const double px, const double py, const complex<double> z)
    //Gk = Green matrix
    {
        // to sum up, the Green matrix is Gk = 1/(z-Hk)
        calculate_Hk(px, py);
        for(int i=0;i<green.dim;i++)
            for(int j=0;j<green.dim;j++)
            {
                green(i,j) = -hamiltonian(i,j);
                if(i==j) green(i,j) += z;    
            }
        green.invert();
    }
    
    double calculate_Ak11(const double px, const double py)
    {
        calculate_Gk(px, py);
        return (-M_1_PI*imag(green(0,0)));
    }
    
    double calculate_Aktot(const double px, const double py)
    {
        calculate_Gk(px, py);
        double sum=0.0;
        for(int i=0; i<green.nEntry; i++) sum+= -M_1_PI*imag(green.trace());  
        return sum/green.dim;
    }



//private:


    
} Model;


