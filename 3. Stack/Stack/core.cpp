//
//  stack.cpp
//  Stack
//
//  Created by Amin Benarieb on 02/10/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
//

#include "core.hpp"


#pragma mark Analyze
unsigned long long tick(void){
    unsigned long long d;
    __asm__ __volatile__ ("rdtsc": "=A" (d) );
    
    return d;
}
void analyzeStack(){
    
    unsigned long long sum = 0, tb, te;
    unsigned long long average = 0;
    
    StackArray stackArray;
    StackList stackList;
    Error error = {0, ""};
    
    
    printf("Analyzing time of stacks (ticks).\n\n");
    
    
    printf("Push function:\n");
    for (int i = 0; i < kInetation; i++)
    {
        tb = tick();
        stackArray.push('(', &error);
        te = tick();
        sum += te-tb;
    }
    average = sum / kInetation;
    printf("\tArray: %llu", average);
    
    sum = 0;
    average = 0;
    for (int i = 0; i < kInetation; i++)
    {
        tb = tick();
        stackList.push('(', &error);
        te = tick();
        sum += te-tb;
    }
    average = sum / kInetation;
    printf("\tList: %llu", average);
    
    sum = 0;
    average = 0;
    printf("\n\nPop function:\n");
    for (int i = 0; i < kInetation; i++)
    {
        tb = tick();
        stackArray.pop(&error);
        te = tick();
        sum += te-tb;
    }
    average = sum / kInetation;
    printf("\tArray: %llu", average);
    
    sum = 0;
    average = 0;
    for (int i = 0; i < kInetation; i++)
    {
        tb = tick();
        stackList.pop(&error);
        te = tick();
        sum += te-tb;
    }
    average = sum / kInetation;
    printf("\tList: %llu\n", average);
    
    
    printf("\n\nAllocaed memory (bytes):\n");
    printf("\tArray: %lu", sizeof(StackArray));
    printf("\tList: %lu\n", sizeof(StackList));
    
    showContinue();
    
    
}

#pragma mark Stack functions
void showStack(const StackArray stackArray, const  StackList stackList, const BraceAddressVector removedList){
    
    if (stackArray.size == 0)
        printf("\nStack-array is empty!\n");
    else
    {
        printf("\nStack-array: ");
        for (int i = 0; i < stackArray.size; printf("%c",stackArray.elements[i++]));
    }
    printf("\n");
    

    printf("Stack-list.\n");
    printf("\n\tRemoved addresses:");
    for (size_t i = removedList.size(); i > 0; i--)
    {
        printf("%p  ", (struct Brace *)removedList[i-1]);
    }
    Brace *tempPointer = stackList.top;
    string list;
    printf("\n\tAddresses: ");
    while(tempPointer != NULL)
    {
        printf("%p  ", tempPointer);
        list.append(&tempPointer->brace);
        tempPointer = tempPointer->prev;
    }
    printf("\n\tValues: ");
    reverse(list.begin(), list.end());
    printf("%s", list.c_str());

    printf("\n");
    
    showContinue();

    
}
void pushStackArray(StackArray *stackArray){
    
    
    string sign;
    int errorCode = 0;
    Error error = {0, ""};
    
    printf("Input of braces.\n");
    printf("The list of valid braces:  '(' , ')' , '{' , '}' , '[' , ']'.\n");
    printf("To finish input press 'n'.\n");

    do
    {
        // Processing error
        if (errorCode > 0)
            showErrorMessage(errorCode);
        
        
        errorCode = -1;
        
        cout<<"❱ ";
        getline(cin, sign);
        
        // Processing input
        if (isBrace(sign))
        {
            if(stackArray->push(sign[0], &error))
                printf("\nSucessfuly added '%c'.\n", sign[0]);
            else{
                printf("\nError: %s\n", error.message.c_str());
                break;
            }
            errorCode = 0;
        }
        else if (sign.length() == 0)
            errorCode = 2;
        else if ( sign == "n")
            errorCode = -1;
        else
            errorCode = 4;
        
        
    }
    while (errorCode != -1);
    
    showContinue();
    
}
void popStackArray(StackArray *stackArray){
    
    Error error = {0, ""};
    
    if (stackArray->pop(&error))
        printf("Successfuly removed last element from stack-array.");
    else
        printf("Error: %s", error.message.c_str());
    
    showContinue();
}
void pushStackList(StackList *stackList, BraceAddressVector *removedList){
    
    string sign;
    int errorCode = 0;
    Error error = {0, ""};
    
    printf("Input of braces.\n");
    printf("The list of valid braces:  '(' , ')' , '{' , '}' , '[' , ']':\n");
    printf("To finish input press 'n'.\n");
    do
    {
        // Processing error
        if (errorCode > 0)
            showErrorMessage(errorCode);
        
        
        errorCode = -1;
        
        cout<<"❱ ";
        getline(cin, sign);
        // Processing input
        if (isBrace(sign))
        {
            if (stackList->push(sign[0],  &error))
            {
                printf("\nSucessfuly added '%c'.\n", sign[0]);
                errorCode = 0;
                
                for (size_t i = removedList->size(); i > 0; i--)
                {
                    if ( (struct Brace *)(*removedList)[i-1] == stackList->top)
                    {
                        
                        printf("\tFree memory: %p", (struct Brace *)(*removedList)[i-1]);
                        printf("\n\tRealocated memory: %p  \n", stackList->top);
                        
                        BraceAddressVector _removedList;
                        for (size_t i = removedList->size(); i > 0; i--)
                             if ( (struct Brace *)(*removedList)[i-1] != stackList->top)
                             {
                                 _removedList.push_back((*removedList)[i-1]);
                             }
                        *removedList = _removedList;
                    }
                }
            }
            else
            {
                printf("Error: %s", error.message.c_str());
                errorCode = -1;
            }
        }
        else if (sign.length() == 0)
            errorCode = 2;
        else if ( sign == "n")
            errorCode = -1;
        else
            errorCode = 4;
        
        
    }
    while (errorCode != -1);
    
    showContinue();
}
void popStackList(StackList *stackList, BraceAddressVector *removedList){
    
    Error error = {0, ""};
    
    if (Brace *brace = stackList->pop(&error))
    {
        removedList->push_back(brace);
        printf("Successfuly removed last element from stack-list.");
    }
    else
        printf("Error: %s", error.message.c_str());

    
    showContinue();
    
}
void checkStack(const StackArray stackArray, const  StackList stackList){
    
    if (stackArray.size == 0)
        printf("\nStack-array is empty!\n");
    else
    {
        Error error = {0, ""};
        short int kOpen = 0, i;
        StackList buffer;
        for (i = stackArray.size-1; i >= 0; i--)
        {
            if ( (stackArray.elements[i] == ')' || stackArray.elements[i] == '}'  || stackArray.elements[i] == ']'))
            {
                buffer.push(stackArray.elements[i],  &error);
                kOpen++;
            }
            else
            {
                
                if (buffer.top != NULL && kOpen != 0)
                {
                    if (isBalanced(stackArray.elements[i], buffer.top->brace))
                    {
                        buffer.pop(&error);
                        kOpen--;
                    }
                    else
                        break;
                }
                else
                    break;
            }
            
        }
        printf("\nStack-array braces is%sbalanced. 🍻", i != -1 || kOpen != 0 ? " not " : " ");
    }
    
    printf("\n");
    
    if (stackList.top == NULL)
        printf("\nStack-list is empty!\n");
    else
    {
        StackList buffer;
        Error error = {0, ""};
        Brace *ptr = stackList.top;
        short int kOpen = 0;
        while(ptr != NULL )
        {
            if ( (ptr->brace == ')' || ptr->brace == '}'  || ptr->brace == ']'))
            {
                buffer.push(ptr->brace, &error);
                kOpen++;
            }
            else
            {
                
                if (buffer.top != NULL && kOpen != 0)
                {
                    if (isBalanced(ptr->brace, buffer.top->brace))
                    {
                        buffer.pop(&error);
                        kOpen--;
                    }
                    else
                        break;
                }
                else
                    break;
            }
            
            ptr = ptr->prev;
        }
        printf("\nStack-list braces is%sbalanced. 🍻", ptr != NULL || kOpen != 0 ? " not " : " ");
    }
    
    printf("\n");
    
    showContinue();

}

#pragma mark Interface functions
void showMenu(){
    
    cout<<"Input action: "<<endl;
    cout<<"0 - Exit"<<endl;
    cout<<"1 - Push (array)"<<endl;
    cout<<"2 - Pop  (array)"<<endl;
    cout<<"3 - Push (list)"<<endl;
    cout<<"4 - Pop  (list)"<<endl;
    cout<<"5 - Show"<<endl;
    cout<<"6 - Check"<<endl;
    cout<<"7 - Analyze"<<endl;
}
void showContinue(){
    
    cout<<"\nPress any key to continue. 🌝\n";
    getchar();
}

#pragma mark Handling
void showErrorMessage(int error){
    
    switch (error) {
        case 0:
            break;
        case 1:
            printf("\nInvalid command code! 😱\n");
            break;
        case 2:
            printf("\nEmpty input! 😰\n");
            break;
        case 3:
            printf("\nYou should input only numbers! 😥\n");
            break;
        case 4:
            printf("\nInvalid input.\n");
            break;
        default:
            printf("\nUnexpected error with code %d. 💀\n", error);
            break;
    }
    
}
void handleAction(int action, StackArray * stackArray, StackList * stackList, BraceAddressVector *removedList){
    
    switch (action) {
        case 0:
            break;
        case 1:
            pushStackArray(stackArray);
            break;
        case 2:
            popStackArray(stackArray);
            break;
        case 3:
            pushStackList(stackList, removedList);
            break;
        case 4:
            popStackList(stackList, removedList);
            break;
        case 5:
            showStack(*stackArray, *stackList, *removedList);
            break;
        case 6:
            checkStack(*stackArray, *stackList);
            break;
        case 7:
            analyzeStack();
            break;
        default:
            cout<<"Sorry, this functionality is unavialible at this moment :(\n";
            showContinue();
            break;
    }
}
void trim(string &s){
    string::size_type pos = s.find_last_not_of(' ');
    if(pos != string::npos)
    {
        if (s.length()!=pos+1)//if there are trailing whitespaces erase them
            s.erase(pos+1);
        pos = s.find_first_not_of(' ');
        if(pos!=0) //if there are leading whitespaces erase them
            s.erase(0, pos);
    }
    else s="";
}
bool isNumber(const string& s){
    string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}
bool isBrace(string sign){
    
    return sign == "(" || sign == ")" || sign == "[" || sign == "]" || sign == "{" || sign == "}";
}
bool isBalanced(const char a, const char b){
    
    if ( (a == '(' && b == ')') || (a == '{' && b == '}') || (a == '[' && b == ']')  )
        return true;

    return false;
}