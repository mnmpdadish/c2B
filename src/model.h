#pragma once

#include "basicMatrix.h"

typedef struct Model {
// things that define the model (most can be read in para.dat)
public:

    int verbose=1;

    //tk parameters:
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
    
    int periodization = 0; //0=Green, 1=Cumulant, 2=Compact Assembly, 3=exact
    int model = 0; //0=afm, 1=YRZ

    int nOmega=200;
    double omegaMin=-4.0;
    double omegaMax=4.0;

    int nMu=200;
    double muMin=-4.0;
    double muMax=4.0;
        
    //matrices
    BasicMatrix tc;
    BasicMatrix tc0;
    BasicMatrix dtk;
    BasicMatrix tc2;
    BasicMatrix dtk2;
    BasicMatrix dtk3;
    BasicMatrix dtk4;
    BasicMatrix green;
    BasicMatrix cumul;
    BasicMatrix sigma;
    BasicMatrix delta;
    BasicMatrix deltak;
    complex<double> G_per;
    complex<double> M_per;


    
    Model():
    
    tc(4), tc0(4), dtk(4), tc2(4), dtk2(4), dtk3(4), dtk4(4), green(4), cumul(4), sigma(4), delta(4), deltak(4)

    {
        if (not exists("para.dat")) {printf("ERROR: couldn't find file 'para.dat'\n\n"); exit(1);}
        printf("reading parameters from para.dat\n\n") ;
        ifstream file;
        file.open("para.dat");
        
        //tk parameters:
        readNumber(file,"MU",MU); 
        readNumber(file,"ETA",ETA);
        readNumber(file,"OMEGA",OMEGA);
        readNumber(file,"t",t);
        readNumber(file,"tp",tp);
        readNumber(file,"tpp",tpp);
        readNumber(file,"M",M);
        readNumber(file,"D",D);
        readNumber(file,"periodization",periodization);
        
        //cuba parameters:
        readNumber(file,"EPSREL",EPSREL);
        readNumber(file,"EPSABS",EPSABS);
        readNumber(file,"MAXEVAL",MAXEVAL); 
        readNumber(file,"MINEVAL",MINEVAL);
        readNumber(file,"VERBOSE",VERBOSE);
        
        readNumber(file,"nOmega",nOmega);
        readNumber(file,"omegaMin",omegaMin);
        readNumber(file,"omegaMax",omegaMax);

        //density loop parameters:
        readNumber(file,"nMu",nMu);
        readNumber(file,"muMin",muMin);
        readNumber(file,"muMax",muMax); 
        
        
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
    
    void calculate_dtk(const double kx, const double ky)
    //Hk = tk matrix
    {

        tc(0,0)=  0.; tc(0,1)= -t;  tc(0,2)= -tp; tc(0,3)= -t;
        tc(1,0)= -t;  tc(1,1)=  0.; tc(1,2)= -t;  tc(1,3)= -tp;
        tc(2,0)= -tp; tc(2,1)= -t;  tc(2,2)=  0.; tc(2,3)= -t;
        tc(3,0)= -t;  tc(3,1)= -tp; tc(3,2)= -t;  tc(3,3)=  0.;

        tc2(0,0)=  0.; tc2(0,1)=  t;  tc2(0,2)= -tp; tc2(0,3)=  t;
        tc2(1,0)=  t;  tc2(1,1)=  0.; tc2(1,2)=  t;  tc2(1,3)= -tp;
        tc2(2,0)= -tp; tc2(2,1)=  t;  tc2(2,2)=  0.; tc2(2,3)=  t;
        tc2(3,0)=  t;  tc2(3,1)= -tp; tc2(3,2)=  t;  tc2(3,3)=  0.;

        tc0(0,0)=  0.; tc0(0,1)= -t;  tc0(0,2)=  0.; tc0(0,3)= -t;
        tc0(1,0)= -t;  tc0(1,1)=  0.; tc0(1,2)= -t;  tc0(1,3)=  0.;
        tc0(2,0)=  0.; tc0(2,1)= -t;  tc0(2,2)=  0.; tc0(2,3)= -t;
        tc0(3,0)= -t;  tc0(3,1)=  0.; tc0(3,2)= -t;  tc0(3,3)=  0.;

        complex<double> ex(cos(-kx*2.), sin(-kx*2.));
        complex<double> emx = conj(ex);
        complex<double> ey(cos(-ky*2.), sin(-ky*2.));
        complex<double> emy = conj(ey);  
        
        //assignation of the 4 by 4 Hk tk:
        dtk(0,0)=-tpp*(emx+ex+ey+emy);       dtk(0,1)=-t*ex;                    dtk(0,2)=-tp*(ex + ey + ex*ey);  dtk(0,3)=-t*ey;
        dtk(1,0)=-t*emx;                     dtk(1,1)=-tpp*(emx+ex+ey+emy);     dtk(1,2)=-t*ey;                  dtk(1,3)=-tp*(emx + ey + emx*ey) ;
        dtk(2,0)=-tp*(emx + emy + emx*emy);  dtk(2,1)=-t*emy;                   dtk(2,2)=-tpp*(emx+ex+ey+emy);   dtk(2,3)=-t*emx;
        dtk(3,0)=-t*emy;                     dtk(3,1)=-tp*(ex + emy + ex*emy);  dtk(3,2)=-t*ex;                  dtk(3,3)=-tpp*(emx+ex+ey+emy);

        if(model==1){//this is for YRZ
          //assignation of the 4 by 4 Hk tk:
          dtk4(0,0)=0;                          dtk4(0,1)=-t*ex;                    dtk4(0,2)=0;                      dtk4(0,3)=-t*ey;
          dtk4(1,0)=-t*emx;                     dtk4(1,1)=0;                        dtk4(1,2)=-t*ey;                  dtk4(1,3)=0;
          dtk4(2,0)=0;                          dtk4(2,1)=-t*emy;                   dtk4(2,2)=0;                      dtk4(2,3)=-t*emx;
          dtk4(3,0)=-t*emy;                     dtk4(3,1)=0;                        dtk4(3,2)=-t*ex;                  dtk4(3,3)=0;

         if (periodization>=2){
            deltak(0,0)= 0.;          deltak(0,1)=  M*(1.+ex);   deltak(0,2)= 0.;          deltak(0,3)= -M*(1.+ey);
            deltak(1,0)=  M*(1.+emx); deltak(1,1)= 0.;           deltak(1,2)= -M*(1.+ey);  deltak(1,3)= 0.;
            deltak(2,0)= 0.;          deltak(2,1)= -M*(1.+emy);  deltak(2,2)= 0.;          deltak(2,3)=  M*(1.+emx);
            deltak(3,0)= -M*(1.+emy); deltak(3,1)= 0.;           deltak(3,2)=  M*(1.+ex);  deltak(3,3)= 0.;
          
          }
          else{
            //this is for YRZ
            delta(0,0)= 0.; delta(0,1)=  M; delta(0,2)= 0.; delta(0,3)= -M;
            delta(1,0)=  M; delta(1,1)= 0.; delta(1,2)= -M; delta(1,3)= 0.;
            delta(2,0)= 0.; delta(2,1)= -M; delta(2,2)= 0.; delta(2,3)=  M;
            delta(3,0)= -M; delta(3,1)= 0.; delta(3,2)=  M; delta(3,3)= 0.;
          }
        }
        else{
          // e^(i*pi) = -1;
          dtk2(0,0)=-tpp*(emx+ex+ey+emy);      dtk2(0,1)= t*ex;                   dtk2(0,2)=-tp*(ex + ey + ex*ey); dtk2(0,3)= t*ey;
          dtk2(1,0)= t*emx;                    dtk2(1,1)=-tpp*(emx+ex+ey+emy);    dtk2(1,2)= t*ey;                 dtk2(1,3)=-tp*(emx + ey + emx*ey) ;
          dtk2(2,0)=-tp*(emx + emy + emx*emy); dtk2(2,1)= t*emy;                  dtk2(2,2)=-tpp*(emx+ex+ey+emy);  dtk2(2,3)= t*emx;
          dtk2(3,0)= t*emy;                    dtk2(3,1)=-tp*(ex + emy + ex*emy); dtk2(3,2)= t*ex;                 dtk2(3,3)=-tpp*(emx+ex+ey+emy);
          //tk.print();

          dtk3(0,0)= 0;                        dtk3(0,1)= t*ex;                   dtk3(0,2)=-tp*(ex + ey + ex*ey); dtk3(0,3)= t*ey;
          dtk3(1,0)= t*emx;                    dtk3(1,1)= 0;                      dtk3(1,2)= t*ey;                 dtk3(1,3)=-tp*(emx + ey + emx*ey) ;
          dtk3(2,0)=-tp*(emx + emy + emx*emy); dtk3(2,1)= t*emy;                  dtk3(2,2)= 0;                    dtk3(2,3)= t*emx;
          dtk3(3,0)= t*emy;                    dtk3(3,1)=-tp*(ex + emy + ex*emy); dtk3(3,2)= t*ex;                 dtk3(3,3)= 0;
        }
    };
    
    //*
    void calculate_sigma(const complex<double> z)
    //S = sigma matrix
    {
        // the self-energy matrix is S = 1/(z+mu-tc2)
        for(int i=0;i<sigma.dim;i++)
            for(int j=0;j<sigma.dim;j++)
            {
                sigma(i,j) = -tc2(i,j);
                if (i==j) sigma(i,j) += z + MU;  
                if (periodization==2) sigma(i,j) += -dtk3(i,j);
                if (periodization==3) sigma(i,j) += -dtk2(i,j);
            }

        sigma.invert();

        for(int i=0;i<sigma.dim;i++)
            for(int j=0;j<sigma.dim;j++)
            {
                sigma(i,j) = M*M*sigma(i,j);
            }
    }

    //*
    void calculate_sigmaYRZ(const complex<double> z)
    //S = sigma matrix
    {
        // the self-energy matrix is S = 1/(z+mu-tc2)
        for(int i=0;i<sigma.dim;i++)
            for(int j=0;j<sigma.dim;j++)
            {
                sigma(i,j) = tc0(i,j);
                if (i==j) sigma(i,j) += z;
                if (periodization>=2) sigma(i,j) += dtk4(i,j);
            }

        sigma.invert();
        //deltak.print();
        //delta.print();
        if(periodization>=2){ 
          sigma.leftright_MatrixMultiplication(&deltak);
        }
        else{ 
          sigma.leftright_MatrixMultiplication(&delta);
        }
    }


    void calculate_cumulant(const complex<double> z)
    //M = cumul matrix
    {
        // the cumulant matrix is M = 1/(z+mu-Sigma)
        for(int i=0;i<cumul.dim;i++)
            for(int j=0;j<cumul.dim;j++)
            {
                cumul(i,j) = -sigma(i,j);
                if(i==j) cumul(i,j) += z + MU;
            }
        cumul.invert();
    }


    void calculate_Gk(const double px, const double py, const complex<double> z)
    //Gk = Green matrix
    {   
        // the Green matrix is Gk = 1/(z-Hk)
        for(int i=0;i<green.dim;i++)
            for(int j=0;j<green.dim;j++)
            {
                green(i,j) = -tc(i,j) -dtk(i,j) -sigma(i,j);
                if(i==j) green(i,j) += z + MU;
            }
        green.invert();
    }


    void calculate_Gperiodized(const double px, const double py)
    {
        double Ry[4] = {0.,0.,1.,1.};
        double Rx[4] = {0.,1.,1.,0.};
        complex<double> z(OMEGA,ETA);
        double epsilon_k = 0;


        calculate_dtk(px, py);
        if(model==0)
          calculate_sigma(z); // could be omitted when z doesn't change
        else
          calculate_sigmaYRZ(z);
        
        if (periodization==1){
            calculate_cumulant(z); // could be omitted when z doesn't change
            M_per = 0;
            for (int ii=0; ii<4; ++ii) {
                for (int jj=0; jj<4; ++jj) {
                    double arg = ((Rx[jj]-Rx[ii])*px + (Ry[jj]-Ry[ii])*py);
                    complex<double> phase(cos(arg), sin(arg));
                    M_per += 0.25 * cumul(ii,jj) * phase; 
                }
            }
            epsilon_k = -2*t*(cos(px)+cos(py)) -4*tp*cos(px)*cos(py) -2*tpp*(cos(2*px)+cos(2*py));
            G_per = 1./((1./M_per) - epsilon_k);
        }
        else {
            calculate_Gk(px, py, z);
            G_per = 0;
            for (int ii=0; ii<4; ++ii) {
                for (int jj=0; jj<4; ++jj) {
                    double arg = ((Rx[jj]-Rx[ii])*px + (Ry[jj]-Ry[ii])*py);
                    complex<double> phase(cos(arg), sin(arg));
                    G_per += 0.25 * green(ii,jj) * phase; 
                }
            }
        }
    }

    // void calculate_Gk(const double px, const double py, const complex<double> z)
    // //Gk = Green matrix
    // {
    //     // the Green matrix is Gk = 1/(z-Hk)
    //     calculate_dtk(px, py);
    //     for(int i=0;i<sigma.dim;i++)
    //         for(int j=0;j<sigma.dim;j++)
    //         {
    //             // compact assembly:
    //             //sigma(i,j) = -tc2(i,j) -dtk2(i,j);
                
    //             // exercise simon:
    //             sigma(i,j) = -tc2(i,j);
                
    //             if(i==j) sigma(i,j) += z + MU;    
    //         }
        
    //     sigma.invert();
        
    //     for(int i=0;i<green.dim;i++)
    //         for(int j=0;j<green.dim;j++)
    //         {
    //             green(i,j) = -tc(i,j) -dtk(i,j) - M*M*sigma(i,j);
    //             if(i==j) green(i,j) += z + MU;
    //         }
        
    //     green.invert();
    // }

    // void calculate_Gk(const double px, const double py)
    // {
    //     calculate_Gk(px, py, complex<double>(OMEGA,ETA));
    // }

} Model;


