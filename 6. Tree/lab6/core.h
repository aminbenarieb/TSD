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
void process_load_file();

/*!
 * @brief Удаляет все файлы, обращение к которым происходило до этой заданной даты
 * @param tnode** указатель корень дерева
 */
void process_task(tnode **root);

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
void handle_action(int, tnode **);
/*!
 * @brief Проверяет является ли строка числом
 * @param char* строка
 * @return bool флаг - true если строка число, false иначе
 */
bool is_number(const char*);

#endif /* core_h */


#pragma mark Tree Node

/*!
 * @brief Создает узел
 * @param tfile - информационная часть (файл)
 * @param *Error - указатель на переменную ошибки
 * @return *tnode узел
 */
tnode* create_node(tfile, Error *);
/*!
 * @brief Очищает дерево
 * @param tnode*  дерево
 * @param void* параметр (формат)
 */
void free_node(tnode *, void *);
/*!
 * @brief Добавляет узел в дерево
 * @param tnode*  дерево
 * @param tnode*  узел
 * @param Error*  указатель на переменную ошибки
 * @return tnode* измененное дерево
 */
tnode* insert(tnode *, tnode *, Error *);
/*
 * @brief Поиск в дереве
 * @param tnode* измененное дерево
 * @param tfile  информационная часть
 * @return tnode* функция сравнения
 */
tnode* lookup(tnode *, tfile,  int (*)(tfile , tfile ));

/*
 * @brief Извлечение узла из дерева и возвращение его адреса
 * @param tnode* измененное дерево
 * @param tfile  информационная часть
 * @return tnode* функция сравнения
 */
tnode *eject(tnode *, tfile, int (*)(tfile, tfile), Error *);

#pragma mark Task functionality
/*
 * @brief Обход дереве и удаление заданных узлов (см. задание)
 * @param tnode** указатель на старое дерево
 * @param tnode** указатель на новое дерево
 * @param tfile информационная часть
 * @param Error *указатель на стуртуру ошибки
 */
void remove_nodes_predate(tnode **, tnode **, tfile, Error *);

/*
 * @brief Копирует дерево в пустое дерево
 * @param tnode * копируемое дерево
 * @param Error *указатель на стуктуру ошибки
 * @retnr tnode* новое дерево
 */
tnode *copy_tree(tnode *, Error *);

#pragma mark Traversal
/*
 * @brief Прямой обход дерева
 * @param tnode *дерево
 * @param void(*f)(tnode*,void*) функция сравнения
 * @param void * формат
 */
void apply_pre(tnode *, void (*f)(tnode*, void*), void *);
/*
 * @brief Обратный обход дерева
 * @param tnode *дерево
 * @param void(*f)(tnode*,void*) функция сравнения
 * @param void * формат
 */
void apply_post(tnode *tree, void (*f)(tnode*, void*), void *arg);
void apply_in(tnode *, void (*)(tnode*, void*), void *);

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
