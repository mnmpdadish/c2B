#pragma once
#include "utilities.h"

// MATRIX INVERSE LAPACK ROUTINES:
extern "C" {
    void zgetrf_(int*, int* , complex<double>*, int*, int*, int*);
    void zgetri_(int*, complex<double>*, int*, int*, complex<double>*, int*, int*);
    void zgemm_(char*, char*, int*, int*, int*, double*, complex<double>*, int*, complex<double>*, int*, double*, complex<double>*, int*);
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
        delete data_;
        delete IPIV_;
        delete WORK_;
    };

    /*
    void dataCopy(BasicMatrix &inputMatrix) {
        if(nEntry != inputMatrix.nEntry) {printf("ERROR: cannot copy the matrix'\n\n"); exit(1);}
        for(int i=0; i<nEntry; i++) data[i] = inputMatrix.data[i];
    };
    */
    
    /*
    const int size(){
        return nEntry;
    }
    */
    
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

    void leftright_MatrixMultiplication(BasicMatrix const * A) {  // data_ = A.data_^dag * data_ * A.data_
      assert(A->dim == dim);
      double one = 1.0;
      double zero = 0.0;
      char no = 'n';
      char ye = 'c';
      BasicMatrix tmp(dim);
      
      //printf("salutyo \n\n");
      //print();
      //A->print();
      //tmp.print();
      zgemm_(&ye,&no,&dim,&dim,&dim, &one, A->data_, &dim, data_, &dim, &zero, tmp.data_, &dim); 
      zgemm_(&no,&no,&dim,&dim,&dim, &one, tmp.data_, &dim, A->data_, &dim, &zero, data_, &dim); 
      //print();
      
      
      return;
    }

    
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
    complex<double> *WORK_ ;
    complex<double> * data_;      //can be assigned directly

} BasicMatrix;




