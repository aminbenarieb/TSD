//
//  config.h
//  Graph
//
//  Created by Amin Benarieb on 05/12/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
//

#ifndef config_h
#define config_h

#include <time.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define kMatrPadding 10
#define N 100
#define BUFFER_MAX_SIZE 100   // максимальный буфера ввода
#define MAX_NUMBER_SIZE 9999  // масимальный размер вводимого числ

#define MESSAGE_MEMORY_ERROR "Unable to allocate memory!"

#define MAX(a,b) a > b ? a : b

/*!
 @brief Структура ошибки
 */
typedef struct
{
    int code;
    char *message;
} Error;


/*!
 @brief Стркутура узла
 */
typedef struct tnode tnode;
struct tnode
{
    tnode *left;
    tnode *right;
    
};

#endif /* config_h */
