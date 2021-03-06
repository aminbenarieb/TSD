//
//  main.cpp
//  Records
//
//  Created by Amin Benarieb on 23/09/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
/*
 
>>>>Текст задание (вариант 4)
    • Ввести список машин, имеющихся  в автомагазине, содержащий марку автомобиля, страну-производитель, цену, цвет и состояние: новый – гарантия (в годах); нет - год выпуска, пробег, количество ремонтов.
    • Вывести все марки не новых машин указанного цвета с пробегом меньше 10 тыс. км и отсутствием ремонта.

-------
>>>>Задание
 1) Создать таблицу, содержащую не менее 40 записей с вариантной частью.
 2) Произвести сравнительный анализ реализации алгоритма поиска информации по вариантному полю.
 3) Упорядочить таблицу по возрастанию ключей (любое невариантное поле по выбору программиста), используя:
     а) исходную таблицу;
     б) массив ключей.
 4) Сравнить различные алгоритмы сортировки массива при использовании таблиц записей с большим числом полей и таблиц ключей.
   5) Оценить эффективность (по времени и по используемому объему памяти) при  различной реализации программы, то есть в случаях а) и б). Обосновать выбор алгоритма упорядочивания. Оценка эффективности должна быть относительной (в %).
-------
 
>>>>Указания к выполнению работы
     !Интерфейс программы должен быть понятен неподготовленному пользователю. При разработке интерфейса программы следует предусмотреть:
     • указание формата и диапазона данных при вводе и (или) добавлении записей;
     • указание операций производимых программой;
     • наличие пояснений при выводе результата;
     • возможность  добавления и удаления записей;
 -------
     • просмотр отсортированной таблицы ключей при несортированной исходной таблице;
     • вывод упорядоченной исходной таблицы;
     • вывод результатов сравнения эффективности работы программы при  обработке данных в  исходной таблице и в таблице ключей;
     • вывод результатов использования различных алгоритмов сортировок.
     
     !Одним из результатов работы программы должна быть количественная информация с указанием времени, затраченного на обработку исходной таблицы и таблицы ключей (при этом не забыть оценить так же время выборки данных из основной таблицы с использованием таблицы ключей), а  так же объем занимаемой при этом оперативной  памяти.
  -------
 
     При тестировании программы необходимо:
     • проверить правильность ввода и вывода данных (в том числе, отследить попытки ввода неверных по типу данных в вариантную часть записи);
     • обеспечить вывод сообщений при отсутствии входных данных («пустой ввод»);
     • проверить правильность выполнения операций;
     • отследить переполнение таблицы.
     !При хранении исходных данных в файлах необходимо также проверить наличие файла и изменения информации в нем при удалении и добавлении данных в таблицу.

*/


#include <iostream>
#include <stdlib.h>
#include "functions.hpp"


int main() {
    
    string rawInput;
    int input = -1;
    int error = 0;
    
    do
    {
        input = -1;
        system("clear");
        showMenu();
        
        // Processing error
        if (error) showErrorMessage(error);

        cout<<"❱ ";
        getline(cin, rawInput);
        
        // Processing input
        if (isNumber(rawInput) && rawInput < "7")
        {
            input = stoi(rawInput);
            error = 0;
            handleAction(input);
        }
        else if (rawInput.length() == 0)
            error = 2;
        else
            error = 1;
        
        
    }
    while (rawInput != "0");
    
    
    return 0;
}

