//
//  config.h
//  lab6
//
//  Created by Amin Benarieb on 21/11/15.
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

#define BUFFER_MAX_SIZE 10000   // максимальный буфера ввода
#define ARRAY_MAX_SIZE 10000    // максимальный размер массива
#define MAX_NUMBER_SIZE 9999    //масимальный размер вводимого числа
#define DATE_STRING_SIZE 11     // размер строки даты

#define MESSAGE_MEMORY_ERROR "Unable to allocate memory!"

/*!
 @brief Структура ошибки
 */
typedef struct
{
    int code;
    char *message;
} Error;

/*!
 @brief Множесто атрибутов файла
 */
typedef enum {
    unknown  = 0,
    archive  = 1,
    hidden   = 2,
    systeme  = 3,
    readonly = 4
} tattribute;
/*!
 @brief Структура даты
 */
typedef struct
{
    int day;
    int month;
    int year;
} tdate;
/*!
 @brief Структура файла (информац. часть)
 */
typedef struct {
    char name[BUFFER_MAX_SIZE+1];
    tattribute attubute;
    tdate lastused;
    
} tfile;

/*!
 @brief Стркутура узла
 */
typedef struct tnode tnode;
struct tnode
{

    tfile file;
    tnode *left;
    tnode *right;
    
};


#endif /* config_h */
