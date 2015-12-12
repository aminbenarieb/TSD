//
//  binarytree.h
//  Hash-Table
//
//  Created by Amin Benarieb on 01/12/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
//

#ifndef binarytree_h
#define binarytree_h

#include <stdio.h>
#include "config.h"


#pragma mark Functions for Avl-Tree

unsigned char avl_height(tnode* );
tnode* avl_insert(tnode* , tnode *, Error *);

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
tnode* lookup(tnode *, tfile,  int (*)(tfile , tfile ), unsigned long long *);

/*
 * @brief Извлечение узла из дерева и возвращение его адреса
 * @param tnode* измененное дерево
 * @param tfile  информационная часть
 * @return tnode* функция сравнения
 */
tnode *eject(tnode *, tfile, int (*)(tfile, tfile), Error *);

#pragma mark Task
/*
 * @brief Обход дереве и удаление заданных узлов (см. задание)
 * @param tnode** указатель на старое дерево
 * @param tnode** указатель на новое дерево
 * @param tfile информационная часть
 * @param Error *указатель на стуртуру ошибки
 */
void remove_nodes_predate(tnode **, tnode **, char, Error *, tnode* (*_insert)(tnode *, tnode *, Error *));
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
void apply_post(tnode *, void (*f)(tnode*, void*), void *);
/*
 * @brief Поперечный обход дерева
 * @param tnode *дерево
 * @param void(*f)(tnode*,void*) функция сравнения
 * @param void * формат
 */
void apply_in(tnode *, void (*f)(tnode*, void*), void *);


#endif /* binarytree_h */
