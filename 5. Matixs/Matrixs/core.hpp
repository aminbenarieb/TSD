//
//  Matrix.hpp
//  Matrix
//
//  Created by Amin Benarieb on 02/10/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
//

#ifndef Matrix_hpp
#define Matrix_hpp

#include "matrix.hpp"

#define NUMBER_MARGIN 2

#pragma mark Interface functions
/*!
 * @brief Delay screen until user's action
 */
void showContinue();
/*!
 * @brief Showing error message with given error code
 * @param int error code
 */
void showErrorMessage(int);
/*!
 * @brief Showing main menu
 */
void showMenu();
/*!
 * @brief Processing action for normal matrix
 */
void processNormalMatrix();

#pragma mark Sparse Matrix

/*!
 * @brief Print matrix on console screen
 */
void printSparseMatix(const SMatrix);
/*!
 * @brief Print matrix in file
 */
bool writeSparseMatrix(SMatrix, string, Error *);
/*!
 * @brief Read array from file and return sparse matrix
 */
SMatrix readSparseMatrix(string, double&, Error *);
/*!
 * @brief Read array from file and return vector-column
 */
d_vector readSparseMatrixColumn(string, double&, Error *);
/*!
 * @brief Multipliation of sparse matrixs
 */
SMatrix multSparseMatrix(SMatrix, d_vector, Error *);

#pragma mark Normal Matrix

/*!
 * @brief Print matrix on console screen
 */
void printMatix(const double * const, const int, const int);

/*!
 * @brief Print matrix in file
 */
bool writeMatrix(const double * const, const int, const int, string, Error *);

/*!
 * @brief Read array from file
 */
double *readMatrix(int *, int *, string, Error *);

/*!
 * @brief Multipliation two matrix
 */
double *multMatrix(const double * const, const double * const, const int, const int, const int, const int, int *, int *, Error *);



#pragma mark Handling
/*!
 * @brief Handling action
 * @param int action
 */
void handleAction(int);
/*!
 * @brief Check if string is a number
 * @return bool if string is a number, false otherwise
 */
bool isNumber(const string&);
#endif /* Matrix_hpp */
