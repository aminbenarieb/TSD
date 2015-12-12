//
//  Queue.hpp
//  Queue
//
//  Created by Amin Benarieb on 02/10/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
//

#ifndef Queue_hpp
#define Queue_hpp

#include "queue.hpp"

#define NUMBER_MARGIN 2
#define kInetation 50

#pragma mark Simulate
/*!
 * @brief Counting ticks for push & pop of Queue-array, Queue-list
 */
void SimulateQueue();


#pragma mark Queue functions 
/*!
 * @brief Handle adding a new element to Queue-array
 */
void pushQueueArray(QueueArray *);
/*!
 * @brief Handle removing the last element from Queue-array
 */
void popQueueArray(QueueArray *);
/*!
 * @brief Handle adding a new element to Queue-array
 */
void pushQueueList(QueueList *, RequestAddressVector *removedList);
/*!
 * @brief Handle removing the last element from Queue-array
 */
void popQueueList(QueueList *, QueueList *);

#pragma mark Interface functions
/*!
 * @brief Delay screen until user's action
 */
void showContinue();
/*!
 * @brief Showing error message with given error code
 * @param int error code
 */
void showErrorMessage(int);
/*!
 * @brief Showing main menu
 */
void showMenu();


#pragma mark Handling
/*!
 * @brief Handling action
 * @param int action
 * @param QueueArray variable
 * @param QueueList variable
 */
void handleAction(int, QueueArray *, QueueList *, RequestAddressVector *);
/*!
 * @brief Check if string is a number
 * @return bool if string is a number, false otherwise
 */
bool isNumber(const string&);
#endif /* Queue_hpp */
