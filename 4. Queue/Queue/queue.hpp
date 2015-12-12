//
//  config.h
//  Queue
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

#define ARRAY_MAX_SIZE 10000
#define MAX_NUMBER_SIZE 9999

#define T1_1 1
#define T1_2 5
#define T2_1 0
#define T2_2 3
#define T3_1 0
#define T3_2 6
#define T4_1 0
#define T4_2 1

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
 @brief Request class
 */
struct Request {
    double comeTime;
    double processTime;
    int value;
    Request *next;
};

#pragma mark QueueArray

/*!
 @brief Array implementation of Queue
 */
struct QueueArray {
    /*!
     @brief Identifier of element of Queue (unique)
     */
    int index;
    /*!
     @brief Size of queue
     */
    int size;
    /*!
     @brief Elements of queue
     */
    Request elements[ARRAY_MAX_SIZE];
    
    /*!
     @brief Adding new element at the end of Queue
     @param int value
     @param double processTime
     @param double comeTime
     @param Error request - Error code
     @return true in case of success, false and error with nonzero code otherwise
     */
    bool push(Request value, Error *error){

        if (size == ARRAY_MAX_SIZE)
        {
            error->code = 1;
            error->message = "Queue is overflow (Pin == Max array size).";
            return false;
        }
            
        
        elements[size++] = value;
        
        return true;
    }
    
    /*!
     @brief Removing last element at the Queue    
     @param Error request - Error code
     @return true in case of success, false and error with nonzero code otherwise
     */
    Request pop(Error *error){

        if (size == 0)
        {
            error->code = 1;
            error->message = "Queue is empty.";
            return elements[size];
        }
        

        Request popElement = elements[0];
        size--;
        for (int i = 0; i < size; i++)
            elements[i] = elements[i+1];
        
        return popElement;
    }
    
    /*!
     @brief QueueArray consturctor
     */
    QueueArray()
    {
        index = 0;
        size = 0;
    }
};

#pragma mark QueueList
/*!
 @brief List implementation of Queue
 */
struct QueueList {
    
    /*!
     @brief 'Head' of Queue
     */
    struct Request *top;
    /*!
     @brief 'Tail' of Queue
     */
    struct Request *bottom;
    /*!
     @brief Size of queue
     */
    int size;
    /*!
     @brief Identifier of element of Queue (unique)
     */
    int index;
    
    
    /*!
     @brief Adding new element at the end of Queue
     @param double processTime
     @param double comeTime
     @param Error request - Error code
     @return true in case of success, false and error with nonzero code otherwise
     */
    bool push(Request request, Error *error){
        
        Request *new_cell = new (nothrow) Request;
        if (new_cell == NULL)
        {
            error->code = 1;
            error->message = "Queue overflow.";
            return false;
        }
        
        new_cell->value = request.value;
        new_cell->processTime = request.processTime;
        new_cell->comeTime = request.comeTime;
        
        if (bottom == NULL && top == NULL)
        {
            bottom = top = new_cell;
        }
        else if (top == bottom)
        {
            bottom->next = new_cell;
            top = new_cell;
        }
        else
        {
            top->next = new_cell;
            top = new_cell;
        }
        size++;
        
        return true;
    }
    
    /*!
     @brief Removing last element at the Queue
     @param Error request - Error code
     @return true in case of success, false and error with nonzero code otherwise
     */
    Request *pop(Error *error, Request *request){
        
        if (bottom == NULL)
        {
            error->code = 1;
            error->message = "Queue is empty.";
            return NULL;
        }
        Request *bufferBottom = bottom;
        
//        request->value = bufferBottom->value;
//        request->comeTime = bufferBottom->comeTime;
//        request->processTime = bufferBottom->processTime;
        
        if (bottom != top)
        {
            Request *temp = bottom->next;
            delete bottom;
            bottom = temp;
        }
        else
        {
            delete bottom;
            top = bottom = NULL;
        }
        
        size--;
        
        return bufferBottom;
        
    }
    
    /*!
     @brief QueueList consturctor
     */
    QueueList()
    {
        top = bottom = NULL;
        size = 0;
    }
};


typedef vector<struct Request *> RequestAddressVector;


#endif /* config_h */
