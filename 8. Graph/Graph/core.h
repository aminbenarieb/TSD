//
//  core.h
//  Graph
//
//  Created by Amin Benarieb on 05/12/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
//

#ifndef core_h
#define core_h

#include <stdio.h>
#include "config.h"

#pragma mark Other
unsigned long long tick(void);

#pragma mark Interface functions
/*!
 * @brief Задерживает экран и ждет ввода пользователя
 */
void show_continue();
/*!
 * @brief Показывать сообищение ошибки с заданным кодом ошибки
 * @param int код ошибки
 */
void show_error_message(int);
/*!
 * @brief Показывает главное меню
 */
void show_menu();


#pragma mark Task functionality

/*!
 * @brief Алгоритм Флойда-Уоршалла
 */
void f_u( double [][N], int );

/*!
 * @brief Найти множество городов, расстояние от которых до выделенного города ( столицы ) больше, чем Т.
 */
void process_task(const double [][N], int, char [N][BUFFER_MAX_SIZE+1]);

#pragma mark Processing

void read_matr(double [][N], int *, int *, char *, Error *);
void print_matr(const double [][N], const int , const int , char [N][BUFFER_MAX_SIZE+1]);

/*!
 * @brief Обрабатывает ввод данных из файла
 */
void process_load_file(double [][N], int *);

/*!
 * @brief Обрабатывает команду пользователя
 * @param int номер  команды
 * @param tnode** корень дерева
 */
void handle_action(int, double [][N], int *, char [N][BUFFER_MAX_SIZE+1]);
/*!
 * @brief Проверяет является ли строка числом
 * @param char* строка
 * @return bool флаг - true если строка число, false иначе
 */
bool is_number(const char*);

#endif /* core_h */

#pragma mark Exporting to Graph

/*
 * @brief Экспорт дерева в DOT файл
 * @param FILE* указатель на файловую переменную
 * @param char* строка - имя файла
 */
void export_to_dot(FILE *, const char *, const double [][N], const int, char [N][BUFFER_MAX_SIZE+1]);
