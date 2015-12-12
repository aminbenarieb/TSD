//
//  config.h
//  Stack
//
//  Created by Amin Benarieb on 04/10/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
//

#ifndef config_h
#define config_h

#include <iostream>
#include <string>
#include <time.h>
#include <math.h>
#include <vector>

#define kInetation 1
#define MAX_SIZE 5
#define kMAX_POINTER 0x7fd0a3d00020

using namespace std;


/*!
 @brief Error class
 */
typedef struct
{
    int code;
    string message;
} Error;

/*!
 @brief Array implementation of stack
 */

struct StackArray {
    /*!
     @brief Current size of stack
     */
    int size;
    /*!
     @brief Elements of stack
     */
    char elements[MAX_SIZE];
    
    /*!
     @brief Adding new element at the end of stack
     @param char brace - New element to add
     @param Error brace - Error code
     @return true in case of success, false and error with nonzero code otherwise
     */
    bool push(char element, Error *error){
        if (size == MAX_SIZE)
        {
            error->code = 1;
            error->message = "Stack is overflow.";
            return false;
        }
        elements[size++] = element;
        return true;
    }
    
    /*!
     @brief Removing last element at the stack    
     @param Error brace - Error code
     @return true in case of success, false and error with nonzero code otherwise
     */
    bool pop(Error *error){
        if (size == 0)
        {
            error->code = 1;
            error->message = "Stack is empty.";
            return false;
        }
        elements[size--] = '\0';
        return true;
    }
    
    /*!
     @brief StackArray consturctor
     */
    StackArray()
    {
        size = 0;
    }
};

/*!
 @brief Brace class
 */
struct Brace {
    char brace;
    struct Brace *prev;
};

/*!
 @brief List implementation of stack
 */
struct StackList {
    
    /*!
     @brief Last element of stack
     */
    struct Brace *top;
    const Brace *MAX_POINTER;
    
    /*!
     @brief Adding new element at the end of stack
     @param char brace - New element to add
     @param Error brace - Error code
     @return true in case of success, false and error with nonzero code otherwise
     */
    bool push(char element, Error *error){
        
        Brace *brace = new (nothrow) Brace;
        if (brace == NULL /* || brace >= MAX_POINTER */ )
        {
            error->code = 1;
            error->message = "Stack overflow.";
            return false;
        }
        
        brace->brace = element;
        brace->prev = top;

        top = brace;
        
        return true;
    }
    
    /*!
     @brief Removing last element at the stack
     @param Error brace - Error code
     @return true in case of success, false and error with nonzero code otherwise
     */
    Brace *pop(Error *error){
        
        if (top == NULL)
        {
            error->code = 1;
            error->message = "Stack is empty.";
            return NULL;
        }

        Brace *temp = top->prev;
        Brace *bufferTop = top;
        delete top;
        top = temp;
        
        return bufferTop;
    }
    
    /*!
     @brief StackList consturctor
     */
    StackList()
    {
        top = NULL;
        MAX_POINTER = reinterpret_cast<Brace*>(kMAX_POINTER);
    }
};


typedef vector<struct Brace *> BraceAddressVector;



#endif /* config_h */
