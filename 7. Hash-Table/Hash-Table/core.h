//
//  core.h
//  lab6
//
//  Created by Amin Benarieb on 21/11/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
//

#ifndef core_h
#define core_h

#include <stdio.h>
#include "config.h"
#include "binarytree.h"
#include "hashmap.h"

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
/*!
 * @brief Показывает информацтивную часть узла
 * @param tnode* узел
 * @param void* параметр (по идеи формат вывода, но он не учитывается)
 */
void show_node_info(tnode * node, void *param);



#pragma mark Task functionality
/*!
 * @brief Удаляет все слова начинающююеся на заданную букквы
 */
void process_task(tnode **, tnode **, hashmap *, hashmap_closed *);



#pragma mark Processing

/*!
 * @brief Обрабатывает ввод с консоли для добавления узла
 * @param tnode* указатель корень дерева
 */
void process_input_insert_node(tnode **);

/*!
 * @brief Обрабатывает ввод с консоли для извлечения узла
 * @param tnode** корень дерева
 */
void process_input_eject_node(tnode **);

/*!
 * @brief Обрабатывает ввод данных из файла
 */
void process_load_file(tnode **, tnode **, hashmap*, hashmap_closed*);


/*!
 * @brief Ищет узел по заданному ключу (названию файла)
 * @param tnode* корень дерева
 */
void process_lookup(tnode *root);

/*!
 * @brief Обрабатывает команду пользователя
 * @param int номер  команды
 * @param tnode** корень дерева
 */
void handle_action(int, tnode **, tnode **, hashmap*, hashmap_closed*);
/*!
 * @brief Проверяет является ли строка числом
 * @param char* строка
 * @return bool флаг - true если строка число, false иначе
 */
bool is_number(const char*);

#endif /* core_h */

#pragma mark Exporting to DOT
/*
 * @brief Печатает узел в файл
 * @param *void формат
 */
void to_dot(tnode *, void *);
/*
 * @brief Экспорт дерева в DOT файл
 * @param FILE* указатель на файловую переменную
 * @param char* строка - имя файла
 * @param tnode *дерево
 */
void export_to_dot(FILE *, const char *, tnode *);
