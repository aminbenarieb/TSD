//
//  config.h
//  Matrix
//
//  Created by Amin Benarieb on 04/10/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
//

#ifndef config_h
#define config_h

#include <iostream>
#include <string>
#include <time.h>
#include <math.h>
#include <vector>

#define ARRAY_MAX_SIZE 10000
#define MAX_NUMBER_SIZE 9999
#define kInetation 50
#define kMatrPadding 6

using namespace std;

typedef vector<double> d_vector;
typedef vector<int> i_vector;

/*!
 @brief Error class
 */
typedef struct
{
    int code;
    string message;
} Error;

/*!
 @brief Sparse matrix class
 */
/*
 Advantages of the CSR format
     • efficient arithmetic operations CSR + CSR, CSR * CSR, etc.
     • efficient row slicing
     • fast matrix vector products
 Disadvantages of the CSR format
     • slow column slicing operations (consider CSC)
     • changes to the sparsity structure are expensive (consider LIL or DOK)
 */
struct SMatrix
{
    int N;       //number of cols
    
    d_vector A;  //vector of non-zero elements
    i_vector J; //vector of column indexes of A elements
    i_vector I; //vector of JA elements indexes of first non-zero element in row
};


#endif /* config_h */
