//
//  stack.hpp
//  Stack
//
//  Created by Amin Benarieb on 02/10/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
//

#ifndef stack_hpp
#define stack_hpp

#include "stack.hpp"

#pragma mark Analyze
/*!
 * @brief Counting ticks for push & pop of stack-array, stack-list
 */
void analyzeStack();


#pragma mark Stack functions 
/*!
 * @brief Display given stack-array and stack-list
 */
void showStack(const StackArray, const StackList, const BraceAddressVector);
/*!
 * @brief Handle adding a new element to stack-array
 */
void pushStackArray(StackArray *);
/*!
 * @brief Handle removing the last element from stack-array
 */
void popStackArray(StackArray *);
/*!
 * @brief Handle adding a new element to stack-array
 */
void pushStackList(StackList *, BraceAddressVector *removedList);
/*!
 * @brief Handle removing the last element from stack-array
 */
void popStackList(StackList *, StackList *);
/*!
 * @brief Check the correct placement of the brackets
 */
void checkStack(const StackArray, const BraceAddressVector);


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
/*!
 * @brief Delay screen until user's action
 */
void showContinue();


#pragma mark Handling
/*!
 * @brief Showing error message with given error code
 * @param int error code
 */
void showErrorMessage(int);
/*!
 * @brief Handling action
 * @param int action
 * @param StackArray variable
 * @param StackList variable
 */
void handleAction(int, StackArray *, StackList *, BraceAddressVector *);
/*!
 * @brief Removing extra spaces in string
 * @param string value
 * @param string processed value
 */
void trim(string &);
/*!
 * @brief Handling braces
 * @param string with sign
 * @return true if sign is belongs to '(','[','{',')',']','}' and false otherwise
 */
bool isBrace(string);
/*!
 * @brief Check if string is a number
 * @return bool if string is a number, false otherwise
 */
bool isNumber(const string&);
/*!
 * @brief Check if two braces is balanced
 * @return bool if balanced, false otherwise
 */
bool isBalanced(const char, const char);

#endif /* stack_hpp */
