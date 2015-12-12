//
//  main.c
//  Queue
//
//  Created by Amin Benarieb on 02/10/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
//
// Условие задачи: Проверить правильность расстановки скобок трех типов (круглых, квадратных и фигурных) в выражении.

#include <iostream>
#include "core.hpp"

int main(int argc, const char * argv[]) {
    
    string rawInput;
    int input = -1;
    int errorCode = 0;
    QueueArray queueArray;
    QueueList queueList;
    RequestAddressVector removedList;
    
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
        if (isNumber(rawInput) && rawInput < "9")
        {
            input = stoi(rawInput);
            errorCode = 0;
            handleAction(input, &queueArray, &queueList, &removedList);
        }
        else if (rawInput.length() == 0)
            errorCode = 2;
        else
            errorCode = 1;
        
        
    }
    while (rawInput != "0");

    Error error = {0, ""};
    Request request;
    Request *iReq = queueList.pop(&error, &request);
    while (iReq != queueList.top)
        iReq = queueList.pop(&error, &request);
    
    return 0;
}
