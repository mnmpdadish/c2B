/* 
*  basicMatrix.h
*    Project: c2B
*    Authors: Maxime Charlebois, Simon Verret
*    License: MIT License
*/


#pragma once
#include "utilities.h"

// MATRIX INVERSE LAPACK ROUTINES:
extern "C" {
  void zgetrf_(int*, int* , complex<double>*, int*, int*, int*);
  void zgetri_(int*, complex<double>*, int*, int*, complex<double>*, int*, int*);
}

typedef struct BasicMatrix {
  public:
    int dim; 
    int nEntry;

    BasicMatrix() {};
    BasicMatrix(int inputDim)
    {
      dim = inputDim;
      nEntry = dim*dim;
      data_ = new complex<double>[nEntry];

      //some arrays used by Lapack:
      IPIV_ = new int[dim+1];
      WORK_ = new complex<double>[nEntry];
    };

    ~BasicMatrix()
    {
      delete [] data_;
      delete [] IPIV_;
      delete [] WORK_;
    };

    //next function, useful to assign or get
    complex<double>& operator()(int i, int j) 
    {
#ifdef BOUND_CHECK
      assert(i < dim and j < dim); 
#endif    
      return data_[dim*i + j]; // return the pointer to the position in the matrix
    };

    complex<double> trace()
    {
      complex<double> sum=0.0;
      for(int i=0; i<dim; i++) sum+= data_[i*dim+i];  //sum on diagonal 
      return sum;
    };

    complex<double> partialTrace()
    {
      complex<double> sum=0.0;
      for(int i=0; i<(dim/2); i++) sum+= data_[i*dim+i];  //sum on half of the diagonal (must be even)
      return sum;
    };

    void invert() {
      int INFO;
      zgetrf_(&dim,&dim,data_,&dim,IPIV_,&INFO);
      zgetri_(&dim,data_,&dim,IPIV_,WORK_,&nEntry,&INFO);
      if( !(INFO == 0) ) {
        printf( "The algorithm failed to invert the matrix.\n" );
        exit( 1 );
      }
    };

    void print() {
      int i, j;
      for( i = 0; i < dim; i++ ) {
        for( j = 0; j < dim; j++ )
          printf( " (% 4.2f,% 4.2f)", real(data_[i+j*dim]), imag(data_[i+j*dim]) );
        printf( "\n" );
      }
      printf( "\n\n" );
      fflush(stdout);
    }    


  public: // meh
    int *IPIV_ ;
    complex<double> * WORK_;
    complex<double> * data_;   // can be assigned directly

} BasicMatrix;




