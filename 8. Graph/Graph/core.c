//
//  core.c
//  Graph
//
//  Created by Amin Benarieb on 05/12/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
//

#include <stdlib.h>
#include "core.h"

#define kInetation 1
#define maxTree 640
#define minTree 5
#define showTree false
#define autoSearch true
#define fullSearch true
#define hideSearchWord true


unsigned long long tick(void){
    unsigned long long d;
    __asm__ __volatile__ ("rdtsc": "=A" (d) );
    
    return d;
}
void rword (char *word){
    int len = rand () % 6 + 1;
    word [len] = 0;
    while (len) word [--len] = 'a' + rand () % 26;
}
int randomRange(int t1, int t2){
    int t = rand() / RAND_MAX;
    return t1 + t * (t2 - t1);
}


#pragma mark Interface functions
void show_menu(){
    
    printf("Input action: \n");
    printf("0 - Exit\n");
    printf("1 - Import data\n");
    printf("2 - Export and show graph\n");
    printf("3 - Print matrix\n");
    printf("4 - Process task\n");
}
void show_continue(){

    system( "read -n 1 -s -p \"Press any key to continue... 🌝   \"" );
    
}
void show_error_message(int error){
    
    switch (error) {
        case 0:
            break;
        case 1:
            printf("\nInvalid command code! 😱\n");
            break;
        case 2:
            printf("\nEmpty input! 😰!\n");
            break;
        case 3:
            printf("\nYou should input only numbers! 😥\n");
            break;
        case 4:
            printf("\nInvalid input! 😡\n");
            break;
        case 5:
            printf("\nToo big number. The max is %i! 😡\n", MAX_NUMBER_SIZE);
            break;
        default:
            printf("\nUnexpected error with code %d. 💀\n", error);
            break;
    }
    
}

#pragma mark Task
void f_u( double m[][N], int n){
    for (int k = 0; k < n; k++ )
        for (int i = 0; i < n; i++ )
            for (int j = 0; j < n; j++ )
                if ( m[i][j] > ( m[i][k] + m[k][j] ) )
                    m[i][j] = m[i][k] + m[k][j];
}
void process_task(const double matr[][N], int n, char cities[N][BUFFER_MAX_SIZE+1]){
    
    char city_name[BUFFER_MAX_SIZE+1], raw_input[BUFFER_MAX_SIZE+1];
    double T;
    double matrix[N][N];
    int index = -1;
    
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            matrix[i][j] = matr[i][j];
    
    printf("Input city name: ");
    scanf("%s", city_name);
    for (int i = 0; i < n; i++)
        if (strncmp(city_name, cities[i], BUFFER_MAX_SIZE) == 0)
        {
            index = i;
            break;
        }
    
    if (index < 0) {
        printf("\nError: City is invalid. \n");
        show_continue();
        return;
    }
    
    printf("Input T (min length of road): ");
    scanf("%s", raw_input);
    if (!is_number(raw_input))
    {
        printf("\nError: Invalid input. \n");
        show_continue();
        return;
    }
    T = atof(raw_input);
    
    f_u( matrix, n);

    printf("\n%*s", kMatrPadding, "");
    printf("%*s\n", kMatrPadding, cities[index]);
    for (int j = 0; j < n; j++)
        if (j != index)
        {
            printf("%*s", kMatrPadding, cities[j]);
            printf("%*.2lf", kMatrPadding,  matrix[j][index]);
            if (matrix[j][index] > T)
                printf(" ** ");
            printf("\n");
        }
    
    FILE *f = fopen("graph.gv", "w");
    if (!f) {
        printf("Error: Unable to create file!\n");
        show_continue();
        return;
    }
    printf("\nShowing graph....\n");

    fprintf(f, "digraph {\n");
    for (int i = 0; i < n; i++)
        if (matrix[i][index] > T)
            fprintf(f, "\"%s\" [shape=box,style=filled,color=\".7 .3 1.0\"]\n", cities[i]);
    
    fprintf(f, "\"%s\" [shape=circle,style=filled,color=\".3 .3 1.0\"]\n", cities[index]);
    
    for (int i = 0; i  < n; i++)
        for (int j = 0; j < n; j++)
            if (strncmp(cities[i], cities[j], BUFFER_MAX_SIZE ) &&
                i != index)
                fprintf(f, "\"%s\" -> \"%s\"[label=%0.1lf];\n", cities[i], cities[j], matr[i][j]);
    
    fprintf(f, "}\n");
    
    fclose(f);
    system("open -a Graphviz graph.gv");
    
    show_continue();
}

#pragma mark Processing
void read_matr(double matr[][N], int *n, int *m, char *fileName, Error *error){
    
    //Changed params
    *n = *m = 0;
    error->code = 0;
    
    //Local params
    int rows, cols, count = 0;
    double temp;
    char rawSign;
    
    FILE *f = fopen(fileName, "r");
    if(!f)
    {
        error->message =  "Unable to read file.";
        error->code = 1;
        return;
    }
    if (fscanf(f, "%d %d", &rows, &cols) != 2)
    {
        error->message =  "Unable to read size of matrix.";
        error->code = 2;
        return;
    }
    if ((rows <= 0) || (cols <= 0))
    {
        error->message =  "Invalid size of matrix";
        error->code = 3;
        return;
    }
    
    
    rewind(f);
    while (fscanf(f, "%c", &rawSign) != EOF){
        if (!isdigit(rawSign) && rawSign != '.' && rawSign != ' ' && rawSign != '\n')
        {
            error->message =  "Unexpected symbols in file.";
            error->code = 4;
            return;
        }
    }
    
    
    rewind(f);
    fscanf(f, "%d %d", &rows, &cols);
    while (fscanf(f, "%lf", &temp) != EOF)
        count++;
    
    if (count != rows * cols)
    {
        error->message =  "Actual size of matrix is not equal to enter size.";
        error->code = 5;
        return;
    }

    rewind(f);
    fscanf(f, "%d %d", &rows, &cols);
    
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            fscanf(f, "%lf", &matr[i][j]);
    
    *n = rows;
    *m = cols;
    
}
void print_matr(const double matrix[][N], const int n, const int m, char cities[N][BUFFER_MAX_SIZE+1]){
    
    if (n == 0) {
        printf("Matrix is empty");
        show_continue();
    }
    
    printf("%*s", kMatrPadding, "");
    for (int i = 0; i < n; printf("%*s", kMatrPadding, cities[i++]));
    
    printf("\n");
    for (int i = 0; i < n; printf("\n"), i++)
    {
        printf("%*s", kMatrPadding, cities[i]);
        for (int j = 0; j < m; j++)
            printf("%*.2lf", kMatrPadding, matrix[i][j]);
    }
}

void process_load_file(double matr[][N], int *n){
    
    Error error = {0, ""};
    char filename[BUFFER_MAX_SIZE+1];

    printf("Input filename: ");
    scanf("%s", filename);
    read_matr(matr, n, n, filename, &error);
    
    if (error.code != 0)
        printf("\nError: %s\n", error.message);
    
    show_continue();
    
}
void handle_action(int action, double matr[][N], int *n, char cities[N][BUFFER_MAX_SIZE+1]){
    
    switch (action) {
        case 0:
            break;
        case 1:
            process_load_file(matr, n);
            break;
        case 2:
        {
            FILE *f = fopen("graph.gv", "w");
            if (!f) {
                printf("Error: Unable to create file!\n");
                show_continue();
                break;
            }
            printf("\nShowing graph....\n");
            
            export_to_dot(f, "graph", matr, *n, cities);
            fclose(f);
            system("open -a Graphviz graph.gv");
            
            break;
        }
        case 3:
            print_matr(matr, *n, *n, cities);
            show_continue();
            break;
        case 4:
            process_task(matr, *n, cities);
        break;
        default:
            printf("Sorry, this functionality is unavialible at this moment 👷\n");
            show_continue();
            break;
    }
}
bool is_number(const char *str){

    while(*str)
        if (!isdigit(*str++))
            return false;
    
    return true;
    
}


#pragma mark Exporting Graph

void export_to_dot(FILE *f, const char *graph_name, const double matr[][N], const int n, char cities[N][BUFFER_MAX_SIZE+1]){
    
    fprintf(f, "digraph {\n");
    
    for (int i = 0; i  < n; i++)
        for (int j = 0; j < n; j++)
            if (strncmp(cities[i], cities[j], BUFFER_MAX_SIZE))
                fprintf(f, "\"%s\" -> \"%s\"[label=%0.1lf];\n", cities[i], cities[j], matr[i][j]);
    
    fprintf(f, "}\n");
    
}
