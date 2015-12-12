//
//  main.c
//  Matrix
//
//  Created by Amin Benarieb on 02/10/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
//
/*
 
     Вариант 5
     
     Разреженная (содержащая много нулей) матрица хранится в форме 3-х объектов:
     - вектор A содержит значения ненулевых элементов;
     - вектор JA содержит номера столбцов для элементов вектора A;
     - связный список IA, в элементе Nk которого находится номер компонент
     в A и JA, с которых начинается описание строки Nk матрицы A.
     
     1. Смоделировать операцию умножения хранящихся в этой форме матрицы и вектора-cтолбца с получением результата в той же форме.
     2. Произвести операцию умножения, применяя  стандартный алгоритм работы с матрицами.
     3. Сравнить время выполнения операций и объем памяти при использовании этих 2-х алгоритмов при различном проценте заполнения матриц.
 
    Считывание матрицы:
     n_rows n_cols
     x1 y1 value1
     x2 y2 value2
     ...
     x_n y_n value_n
     (с проверкой правильности ввода)
 
 */

#include <iostream>
#include "core.hpp"

int main(int argc, const char * argv[]) {
    
    string rawInput;
    int input = -1;
    int errorCode = 0;
    do
    {
        input = -1;
        system("clear");
        showMenu();
        
        // Processing error
        if (errorCode)
            showErrorMessage(errorCode);
        
        cout<<"❱ ";
        getline(cin, rawInput);
        
        // Processing input
        if (isNumber(rawInput) && rawInput < "5")
        {
            input = stoi(rawInput);
            errorCode = 0;
            handleAction(input);
        }
        else if (rawInput.length() == 0)
            errorCode = 2;
        else
            errorCode = 1;
        
        
    }
    while (rawInput != "0");

    
    return 0;
}
