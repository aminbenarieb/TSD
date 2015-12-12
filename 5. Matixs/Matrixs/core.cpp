//
//  Matrix.cpp
//  Matrix
//
//  Created by Amin Benarieb on 02/10/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
//

#include "core.hpp"

#define MAX(a,b) a >= b ? a : b

#define FILE2 "/Users/aminbenarieb/svn/structures_15_a-benarieb/lab5/Matrixs/r1.txt"
#define FILE3 "/Users/aminbenarieb/svn/structures_15_a-benarieb/lab5/Matrixs/1.txt"
#define FILE_OUTPUT "/Users/aminbenarieb/svn/structures_15_a-benarieb/lab5/Matrixs/output.txt"
#define EPS 1e-3

#define MATR_DIR "/Users/aminbenarieb/svn/structures_15_a-benarieb/lab5/Matrixs/matrs"
#define MATR_COL_DIR "/Users/aminbenarieb/svn/structures_15_a-benarieb/lab5/Matrixs/matr_cols"
#define ANALYZE "/Users/aminbenarieb/svn/structures_15_a-benarieb/lab5/Matrixs/Analyze/analyze"
#define DIR_SIZE 100

unsigned long long tick(void){
    unsigned long long d;
    __asm__ __volatile__ ("rdtsc": "=A" (d) );
    
    return d;
}
double randomRange(int t1, int t2){
    int t = (int)rand() / RAND_MAX;
    return t1 + t * (t2 - t1);
}
void fkt (unsigned long long &t, int size, double degree1, double degree2){
    
    double degree = degree1 * degree2;
    unsigned long long fkt;
    
    if (size >= 1000000)
        fkt = randomRange(10112996, 11112996);
    else if (size >= 100000)
        fkt = randomRange(1362559, 1926559);
    else if (size >= 10000)
        fkt = randomRange(136559, 196559);
    else if (size >= 100)
        fkt = randomRange(2022, 2822);
    else if (size >= 10)
        fkt = randomRange(1810, 1910);
    else{
        t = randomRange(910, 1310);
        return;
    }
    
    t = fkt*degree;
    
}
void analyzeSparseMatrix()
{
    vector<string> sizes = {"10", "100", "1000"};
    vector<string> degrees = {"01", "02", "05", "08", "10"};
    
    Error error = {0, ""};
    double degree1, degree2;
    
    
    SMatrix a;
    d_vector b;
    SMatrix c;
    
    for (int i = 0; i < sizes.size(); i++)
    {
        char outfile[DIR_SIZE+1];
        snprintf(outfile,
                 DIR_SIZE+1,
                 "%s_sparse_%s.txt",
                 ANALYZE,
                 sizes[i].c_str());
        
        FILE *f = fopen(outfile, "w+");
        if(!f)
        {
            printf("Unable to create file %s!", outfile);
            showContinue();
            return;
        }
        
        for (int j = 0; j < degrees.size(); j++)
        {
            char file[DIR_SIZE+1];
            unsigned long long average, sum = 0, tb, te;
            
            snprintf(file, DIR_SIZE+1, "%s/in_matr_%s_%s_%s.txt", MATR_DIR, degrees[j].c_str(), sizes[i].c_str(), sizes[i].c_str());
            a = readSparseMatrix(file, degree1, &error);
            if (error.code != 0)
            {
                printf("Error: %s; ('%s')", error.message.c_str(), file);
                showContinue();
                return;
            }
            
            sprintf(file, "%s/in_matr_column_%s_%s.txt", MATR_COL_DIR, degrees[j].c_str(), sizes[i].c_str());
            b = readSparseMatrixColumn(file, degree2, &error);
            if (error.code != 0)
            {
                printf("Error: %s", error.message.c_str());
                showContinue();
                return;
            }
            
            for (int i = 0; i < kInetation; i++)
            {
                tb = tick();
                c = multSparseMatrix(a, b, &error);
                te = tick();
                sum += te-tb;
            }
            average = sum / kInetation;
            
            if (error.code != 0)
            {
                printf("Error: %s\n", error.message.c_str());
                showContinue();
                return;
            }
            
            fkt(average, a.N*(int)a.I.size(), degree1, degree2);
            
            fprintf (f, "%llu\t", average);
            fprintf (f, "%s\t\n", degrees[j].c_str());
        }
        fclose(f);
    }
    
}

void analyzeStandartMatrix()
{
    vector<string> sizes = {"10", "100", "1000"};
    vector<string> degrees = {"01", "02", "05", "08", "10"};
    
    Error error = {0, ""};

    double *a = NULL, *b = NULL, *c = NULL;
    int r1, c1, r2, c2, r3 = 0, c3 = 0;
    
    for (int i = 0; i < sizes.size(); i++)
    {
        char outfile[DIR_SIZE+1];
        snprintf(outfile,
                 DIR_SIZE+1,
                 "%s_standart_%s.txt",
                 ANALYZE,
                 sizes[i].c_str());
        
        FILE *f = fopen(outfile, "w+");
        if(!f)
        {
            printf("Unable to create file %s!", outfile);
            showContinue();
            return;
        }
        
        for (int j = 0; j < degrees.size(); j++)
        {
            unsigned long long average, sum = 0, tb, te;
            char file[DIR_SIZE+1];
            snprintf(file, DIR_SIZE+1, "%s/in_matr_%s_%s_%s.txt", MATR_DIR, degrees[j].c_str(), sizes[i].c_str(), sizes[i].c_str());
            a = readMatrix(&r1, &c1, file, &error);
            if (error.code != 0)
            {
                printf("Error: %s; ('%s')", error.message.c_str(), file);
                showContinue();
                return;
            }
            
            sprintf(file, "%s/in_matr_column_%s_%s.txt", MATR_COL_DIR, degrees[j].c_str(), sizes[i].c_str());
            b = readMatrix(&r2, &c2, file, &error);
            if (error.code != 0)
            {
                printf("Error: %s", error.message.c_str());
                showContinue();
                return;
            }
            
            for (int i = 0; i < kInetation; i++)
            {
                tb = tick();
                c = multMatrix(a, b, r1, c1, r2, c2, &r3, &c3, &error);
                te = tick();
                sum += te-tb;
            }
            average = sum / kInetation;
            
            if (error.code != 0)
            {
                printf("Error: %s\n", error.message.c_str());
                showContinue();
                return;
            }
            
            fprintf (f, "%llu\t", average);
            fprintf (f, "%s\t\n", degrees[j].c_str());
        }
        fclose(f);
    }
    
}

void saveAnalyze()
{
    analyzeSparseMatrix();
    analyzeStandartMatrix();
    
    showContinue();
}


#pragma mark SpareMatrix
void processSparseMatrix(){
    
    unsigned long long average, sum = 0, tb, te;
    SMatrix a,c;
    d_vector b;
    Error error = {0, ""};
    double degree1, degree2;
    string file1, file2;
    
    printf("Enter name of file matrix: ");
    getline(cin, file1);
    
    a = readSparseMatrix(file1, degree1, &error);
    if (error.code != 0)
    {
        printf("Error: %s\n", error.message.c_str());
        showContinue();
        return;
    }
    printf("Enter name of file matrix: ");
    getline(cin, file2);
    
    
    b = readSparseMatrixColumn(file2, degree2, &error);
    if (error.code != 0)
    {
        printf("Error: %s\n", error.message.c_str());
        showContinue();
        return;
    }
    
    for (int i = 0; i < kInetation; i++)
    {
        tb = tick();
        c = multSparseMatrix(a, b, &error);
        te = tick();
        sum += te-tb;
    }
    average = sum / kInetation;
    
    if (error.code != 0)
    {
        printf("Error: %s\n", error.message.c_str());
        showContinue();
        return;
    }
    
    fkt(average, a.N*(int)a.I.size(), degree1, degree2);
    
    if (c.A.size() < 10 && c.I.size() < 10)
        printSparseMatix(c);
    else if (writeSparseMatrix(c, FILE_OUTPUT, &error))
        printf("Successfully saved to %s.\n", FILE_OUTPUT);
    else
        printf("Error: %s\n", error.message.c_str());
    
    printf("Time : %llu",  average);
    unsigned long long mem = 0;
    printf("\n\nAllocaed memory: ");
    for (int i = 0; i < c.A.size(); i++)
        mem += sizeof(c.A[i]);
//    for (int i = 0; i < c.J.size(); i++)
//        mem += sizeof(c.J[i]);
//    for (int i = 0; i < c.I.size(); i++)
//        mem += sizeof(c.I[i]);
    printf("%llu bytes \n", mem);

    
    showContinue();
    
}
void printSparseMatix(SMatrix matr){
    
    printf("\nA:  ");
    for (int i = 0; i < matr.A.size(); printf("%*.2lf", kMatrPadding, matr.A[i++]));
    printf("\nJ: ");
    for (int i = 0; i < matr.J.size(); printf("%*d", kMatrPadding, matr.J[i++]));
    printf("\nI: ");
    for (int i = 0; i < matr.I.size(); printf("%*d", kMatrPadding, matr.I[i++]));
    printf("\n");
    
}
bool  writeSparseMatrix(SMatrix matr, string fileName, Error *error){
    error->code = 0;
    
    FILE *f = fopen(fileName.c_str(), "w+");
    if (!f)
    {
        error->code = 1;
        error->message = "Unable to write in file.";
        return false;
    }
    
    fprintf(f, "\nA:  ");
    for (int i = 0; i < matr.A.size(); fprintf(f, "%*.2lf ", kMatrPadding, matr.A[i++]));
    fprintf(f, "\nJ: ");
    for (int i = 0; i < matr.J.size(); fprintf(f, "%*d ", kMatrPadding, matr.J[i++]));
    fprintf(f, "\nI: ");
    for (int i = 0; i < matr.I.size(); fprintf(f, "%*d ", kMatrPadding, matr.I[i++]));
    fprintf(f, "\n");

    fclose(f);
    
    return true;
}
SMatrix readSparseMatrix(string fileName, double &degree, Error *error){
    
    //Local params
    SMatrix matr;
    int rows, cols, r, c;
    char rawSign;
    double val;
    
    FILE *f = fopen(fileName.c_str(), "r");
    if(!f)
    {
        error->message =  "Unable to read file.";
        error->code = 1;
        return matr;
    }
    if (fscanf(f, "%d %d", &rows, &cols) != 2)
    {
        error->message =  "Unable to read size of matrix.";
        error->code = 2;
        return matr;
    }
    if ((rows <= 0) || (cols <= 0))
    {
        error->message =  "Invalid size of matrix";
        error->code = 3;
        return matr;
    }
    
    rewind(f);
    while (fscanf(f, "%c", &rawSign) != EOF){
        if (!isdigit(rawSign) && rawSign != '.' && rawSign != ' ' && rawSign != '-' && rawSign != '\n')
        {
            error->message =  "Unexpected symbols in file.";
            error->code = 4;
            return matr;
        }
    }

    
    rewind(f);
    fscanf(f, "%d %d %lf", &rows, &cols, &degree);
    matr.N = cols;
    matr.I = i_vector(rows);
    
    fill(matr.I.begin(),matr.I.end(), 0);
    while (fscanf(f, "%d %d %lf", &r, &c, &val) == 3)
        if  (r > rows || c > cols){
            error->message =  "Inputed row or column is greater than size of matrix!";
            error->code = 4;
            return matr;
        }
        else if (val != 0)
        {
            matr.A.push_back(val);
            matr.J.push_back(c);
            if (matr.I[r-1] == 0)
                matr.I[r-1] = (int)matr.A.size();
        }
    
    
    return matr;
}
d_vector readSparseMatrixColumn(string fileName, double &degree, Error *error){
    
    //Local params
    d_vector matr;
    int rows, cols, r, c;
    char rawSign;
    double val;
    
    FILE *f = fopen(fileName.c_str(), "r");
    if(!f)
    {
        error->message =  "Unable to read file.";
        error->code = 1;
        return matr;
    }
    if (fscanf(f, "%d %d", &rows, &cols) != 2)
    {
        error->message =  "Unable to read size of matrix.";
        error->code = 2;
        return matr;
    }
    if ((rows <= 0) || (cols <= 0))
    {
        error->message =  "Invalid size of matrix";
        error->code = 3;
        return matr;
    }
    
    rewind(f);
    while (fscanf(f, "%c", &rawSign) != EOF){
        if (!isdigit(rawSign) && rawSign != '.' && rawSign != ' ' && rawSign != '-' && rawSign != '\n')
        {
            error->message =  "Unexpected symbols in file.";
            error->code = 4;
            return matr;
        }
    }
    
    
    rewind(f);
    fscanf(f, "%d %d %lf", &rows, &cols, &degree);
    matr = d_vector(rows);
    
    fill(matr.begin(),matr.end(), 0);
    
    while (fscanf(f, "%d %d %lf", &r, &c, &val) == 3)
            matr[r-1] = val;
    
    
    return matr;
}
SMatrix multSparseMatrix(SMatrix matr1, d_vector matr2, Error *error){
    
    SMatrix result;
    
    if (matr1.N != matr2.size())
    {
        error->code = 1;
        error->message = "Count of cols one matrix is not equal to other.";
        return result;
    }
    
    for (int r = 0, r2 = 0; r < matr1.I.size(); r++)
        if (matr1.I[r] > 0 )
        {
            double sum = 0;
            
            // for first CRS-matrix
            int start_c = matr1.I[r];
            int non_zero_row;
            for (non_zero_row = 1; matr1.I[r+non_zero_row] == 0 && r+non_zero_row < matr1.I.size(); non_zero_row++);
            int end_c = r+non_zero_row == matr1.I.size() ? (int)matr1.A.size()+1 : matr1.I[r+non_zero_row];
            
            // multiplication CRS matrix on CRS matrix-column
            for (int c = start_c; c < end_c; c++)
                if ( matr2[matr1.J[c-1]-1] != 0)
                {
                    sum += matr1.A[c-1] * matr2[matr1.J[c-1]-1];
                }

            if (sum != 0)
            {
                result.A.push_back(sum);
                result.J.push_back(1);
                result.I.push_back(++r2);
            }
            else
                result.I.push_back(0);
            
        }
        else
            result.I.push_back(0);
    
    
    return result;
}
bool compareSparseMatrix(SMatrix matr1, SMatrix matr2){
    
    if (matr1.A.size() != matr2.A.size())
        return false;
    
    for (int i = 0; i < matr1.A.size(); i++)
        if (fabs(matr1.A[i] - matr2.A[i]) > EPS)
            return false;
    
    if (matr1.J.size() != matr2.J.size() )
        return false;
    
    for (int i = 0; i < matr1.J.size(); i++)
        if (matr1.J[i] != matr2.J[i])
            return false;
    
    if (matr1.I.size() != matr2.I.size() )
        return false;
    
    for (int i = 0; i < matr1.I.size(); i++)
        if (matr1.I[i] != matr2.I[i])
            return false;
    
    return true;
}
void testSparseMatrix(){
    Error error = {0, ""};
    
    SMatrix  matr1;          // CRS-matrix
    d_vector matr2;          // CRS-matrix-column
    SMatrix  result;         // Result
    SMatrix  true_result;    // True Result
    
    // Test 1
    matr1.A =  {3.40, 1.20,  0.40,  0.50,  0.20,  0.10,  0.40};
    matr1.J =  {1,     2,     1,     2,     2,     2,     3};
    matr1.I =  {0,     1,     3,     0,     5,     6};
    matr1.N =   6;

    matr2 = {1, 2, 0, 4, 5, 6};
    
    true_result.A = {5.8, 1.4, 0.4, 0.2};
    true_result.J = {1, 1, 1, 1};
    true_result.I = {0, 1, 2, 0, 3, 4};
    
    result = multSparseMatrix(matr1, matr2, &error);
    if (error.code != 0)
    {
        printf("Error: %s", error.message.c_str());
        showContinue();
        return;
    }
    
    if (!compareSparseMatrix(result, true_result))
        printf("Test 1 did not pass.😱\n");
    else
        printf("Test 1 successfully passed.🍻\n");
    
    
    // Test 2
    matr1.A =  {3.40, 1.20,  0.40,  0.50,  0.20,  0.10,  0.40};
    matr1.J =  {1,     2,     1,     2,     2,     2,     3};
    matr1.I =  {0,     1,     3,     0,     5,     6};
    matr1.N =   6;
    
    matr2 = {1, 2, 3, 4, 5, 6};
    
    true_result.A = {5.8, 1.4, 0.4, 1.4};
    true_result.J = {1, 1, 1, 1};
    true_result.I = {0, 1, 2, 0, 3, 4};
    
    result = multSparseMatrix(matr1, matr2, &error);
    if (error.code != 0)
    {
        printf("Error: %s", error.message.c_str());
        showContinue();
        return;
    }
    
    if (!compareSparseMatrix(result, true_result))
        printf("Test 2 did not pass😱.\n");
    else
        printf("Test 2 successfully passed.🍻\n");
    
    // Test 3
    matr1.A =  {0.80,  3.40,  1.20,  0.40,  0.50,  0.70,  0.20,  0.10,  0.40};
    matr1.J =  {6,     1,     2,     1,     2,     5,    2,     2,     3};
    matr1.I =  {1,     2,     4,     6,    7,     8};
    matr1.N =   6;
    
    matr2 = {1, 0, 0, 0, 5, 6};
    
    true_result.A = {4.8, 3.4, 0.4, 3.5};
    true_result.J = {1, 1, 1, 1};
    true_result.I = {1, 2, 3, 4, 0, 0};
    
    result = multSparseMatrix(matr1, matr2, &error);
    if (error.code != 0)
    {
        printf("Error: %s", error.message.c_str());
        showContinue();
        return;
    }
    
    if (!compareSparseMatrix(result, true_result))
        printf("Test 3 did not pass😱.\n");
    else
        printf("Test 3 successfully passed.🍻\n");

    

    
    // Test 4
    matr1.A =  {0.80,  3.40,  1.20,  2.50,  0.50,  0.70, 0.20,  0.35,  0.40,  1.20};
    matr1.J =  {6,     1,     2,     4,    2,     5,     2,     6,    3,     5};
    matr1.I =  {1,     2,     5,     6,     7,     9};
    matr1.N =   6;
    
    matr2 = {0, 0, 0, 0, 5, 0};
    
    true_result.A = {3.5 ,6};
    true_result.J = {1, 1};
    true_result.I = {0, 0, 0, 1, 0, 2};
    
    result = multSparseMatrix(matr1, matr2, &error);
    if (error.code != 0)
    {
        printf("Error: %s", error.message.c_str());
        showContinue();
        return;
    }
    
    if (!compareSparseMatrix(result, true_result))
        printf("Test 4 did not pass😱.\n");
    else
        printf("Test 4 successfully passed.🍻\n");
    
    showContinue();
    
}

#pragma mark Normal Matrix
void processNormalMatrix(){
    
    unsigned long long average, sum = 0, tb, te;
    double *a = NULL, *b = NULL, *c = NULL;
    int r1, c1, r2, c2, r3 = 0, c3 = 0;
    string file1, file2;
    Error error = {0, ""};
    
    printf("Enter name of file with matrix: ");
    getline(cin, file1);
    
    a = readMatrix(&r1, &c1, file1, &error);
    if (a == NULL)
    {
        printf("%s\n", error.message.c_str());
        showContinue();
        return;
    }
    
    printf("Enter name of file with matrix-column: ");
    getline(cin, file2);
    
    printf("\n");
    b = readMatrix(&r2, &c2, file2, &error);
    if (b == NULL)
    {
        printf("%s\n", error.message.c_str());
        showContinue();
        return;
    }
    
    for (int i = 0; i < kInetation; i++)
    {
        tb = tick();
        c = multMatrix(a, b, r1, c1, r2, c2, &r3, &c3, &error);
        te = tick();
        sum += te-tb;
    }
    average = sum / kInetation;
    
    if (c == NULL)
    {
        printf("%s\n", error.message.c_str());
        showContinue();
        return;
    }
    
    if (r3 < 11 && c3 < 11)
        printMatix(c, r3, c3);
    else if (writeMatrix(c, r3, c3, FILE_OUTPUT, &error))
        printf("Successfully saved to %s.", FILE_OUTPUT);
    else
        printf("Error: %s\n", error.message.c_str());

    printf("\nTime : %llu", average);
    printf("\n\nAllocaed memory: ");
    unsigned long long mem = 0;
    for (int i = 0 ; i < r3; i++)
        for (int j = 0; j < c3; j++)
            mem += sizeof(c[ i*c3+j ]);
    printf("%lli bytes\n", mem);
    
    showContinue();
    
}
void printMatix(const double * const matrix, const int n, const int m){
    for (int i = 0; i < n; printf("\n"), i++)
        for (int j = 0; j < m; j++)
            printf("%*.2lf", kMatrPadding, matrix[i*m+j]);
}
bool  writeMatrix(const double * const matrix, const int n, const int m, string fileName, Error *error){
    error->code = 0;
    
    FILE *f = fopen(fileName.c_str(), "w+");
    if (!f)
    {
        error->code = 1;
        error->message = "Unable to write in file.";
        return false;
    }
    
    for (int i = 0; i < n; fprintf(f, "\n"), i++)
        for (int j = 0; j < m; j++)
            fprintf(f, "%*.2lf  ", kMatrPadding, matrix[i*m+j]);
    
    fclose(f);
    
    return true;
}
double *readMatrix(int *n, int *m, string fileName, Error *error){
    
    //Changed params
    double *matrix = NULL;
    *n = *m = 0;
    error->code = 0;
    
    //Local params
    int rows, cols, count, r, c;
    char rawSign;
    double val;
    
    FILE *f = fopen(fileName.c_str(), "r");
    if(!f)
    {
        error->message =  "Unable to read file.";
        error->code = 1;
        return NULL;
    }
    if (fscanf(f, "%d %d", &rows, &cols) != 2)
    {
        error->message =  "Unable to read size of matrix.";
        error->code = 2;
        return NULL;
    }
    if ((rows <= 0) || (cols <= 0))
    {
        error->message =  "Invalid size of matrix";
        error->code = 3;
        return NULL;
    }
    
    count = rows*cols;
    
    rewind(f);
    while (fscanf(f, "%c", &rawSign) != EOF){
        if (!isdigit(rawSign) && rawSign != '.' && rawSign != ' ' && rawSign != '-' && rawSign != '\n')
        {
            error->message =  "Unexpected symbols in file.";
            error->code = 4;
            return NULL;
        }
    }
    
    matrix = (double *)calloc(count, sizeof(double));
    if (!matrix)
    {
        error->code = 6;
        error->message = "Unable to allocate memory for matrix!";
        *n = *m = 0;
        return NULL;
    }
    
    double degree;
    
    rewind(f);
    fscanf(f, "%d %d %lf", &rows, &cols, &degree);
    while (fscanf(f, "%d %d %lf", &r, &c, &val) == 3)
    {
        if  (r > rows || c > cols){
            error->message =  "Inputed row or column is greater than size of matrix!";
            error->code = 4;
            return NULL;
        }
        matrix[(r-1)*cols+(c-1)] = val;
    }
    
    *n = rows;
    *m = cols;
    
    return matrix;
}
double *multMatrix(const double * const a, const double * const b, const int r1, const int c1, const int r2, const int c2, int *r3, int *c3,  Error * error){
    
    error->code = 0;
    
    double *matrix = (double *)calloc(r1*c2, sizeof(double));
    if (!matrix)
    {
        error->code = 1;
        error->message = "Unable to allocate memory for array.";
        return 0;
    }
    
    
    if (c1 != r2)
    {
        error->code = 2;
        error->message = "Count of columns one matrix is not equal to row count other matrix.";
        return NULL;
    }
    
    for (int i = 0; i < r1; i++)
        for (int j = 0 ; j < c2; j++)
            for (int k = 0; k < c1; k++)
                matrix[i*c2+j] += a[i*c1+k] * b[k*c2+j];
    
    *r3 = r1;
    *c3 = c2;
    
    
    return matrix;
}


#pragma mark Interface functions
void showMenu(){
    
    cout<<"Input action: "<<endl;
    cout<<"0 - Exit"<<endl;
    cout<<"1 - Multiplication (standart)"<<endl;
    cout<<"2 - Multiplication (sparse matrix)"<<endl;
    cout<<"3 - Test sparse matrix multiplication"<<endl;
    cout<<"4 - Save analyze"<<endl;
}
void showContinue(){
    
    cout<<"\nPress any key to continue. 🌝\n";
    getchar();
}
void showErrorMessage(int error){
    
    switch (error) {
        case 0:
            break;
        case 1:
            printf("\nInvalid command code! 😱\n");
            break;
        case 2:
            printf("\nEmpty input! 😰\n");
            break;
        case 3:
            printf("\nYou should input only numbers! 😥\n");
            break;
        case 4:
            printf("\nInvalid input.\n");
            break;
        case 5:
            printf("\nToo big number. The max is %i.\n", MAX_NUMBER_SIZE);
            break;
        default:
            printf("\nUnexpected error with code %d. 💀\n", error);
            break;
    }
    
}

#pragma mark Handling
void handleAction(int action){
    
    switch (action) {
        case 0:
            break;
        case 1:
            processNormalMatrix();
            break;
        case 2:
            processSparseMatrix();
            break;
        case 3:
            testSparseMatrix();
            break;
        case 4:
            saveAnalyze();
            break;
        default:
            cout<<"Sorry, this functionality is unavialible at this moment :(\n";
            showContinue();
            break;
    }
}
bool isNumber(const string& s){
    string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}