//
//  functions.hpp
//  Records
//
//  Created by Amin Benarieb on 24/09/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
//

#ifndef functions_hpp
#define functions_hpp

#include <iostream>
#include <fstream>
#include "config.hpp"

#pragma mark Struct functions
/*!
 * @brief Adding car record to file
 */
void addCar();
/*!
 * @brief Removing car record from file
 */
void removeCar();
/*!
 * @brief Shows specific car records
 */
void filterCar();
/*!
 * @brief Sorts car records with different algorithms and show comparative info
 */
void sortCar();
/*!
 * @brief Creates file with analyzation of sorts with table keys and without
 */
void analyzeCars();

#pragma mark Interface functions
/*!
 * @brief Showing the header of table
 */
void showHeader();
/*!
 * @brief Showing the header of keys table
 */
void showHeaderKeys();
/*!
 * @brief Showing the main menu
 */
void showMenu();
/*!
 * @brief Showing the table of records with Filter
 */
/*!
 * @brief Showing the table of records with out Filter
 */
int showTable(RecordFilter, bool);
int showTable();
/*!
 * @brief Showing given record as row of table
 * @param size_t number of record
 * @param Car stucture object
 */
void showRecord(size_t, Car);
/*!
 * @brief Showing given key as row of table
 * @param size_t number of record
 * @param Car_Key stucture object
 */
void showKey(size_t id, Car_Key key);
/*!
 * @brief Delay screen until user's action
 */
void showContinue();


#pragma mark Handling

/*!
 * @brief Showing error message with given error code
 * @param int error code
 */
void showErrorMessage(int );
/*!
 * @brief Handling action
 * @param int action
*/
void handleAction(int );
/*!
 * @brief Removing extra spaces in string
 * @param string value
* @param string processed value
 */
void trim(string &);
/*!
 * @brief Handling color
 * @param Color value
 * @return string name of color
 */
string getColorName(Color);

#pragma mark Date functions

/*!
 * @brief Validation of date
 * @param int day, mondth, year
 * @return true if date is valid, false otherwise
 */
bool isDateValid(int, int, int, int &);
/*!
 * @brief Validation of date
 * @param int year
 * @return true if year is leap, false otherwise
 */
bool leapYear(int);

#pragma mark Input functions

/*!
 * @brief Input valid string
 * @param string value
 * @return void
 */
void inputString(string &);
/*!
 * @brief Input valid number
 * @param int value
 * @param bool filter
 * @param int maxValue
 * @return void
 */
bool inputNumber(int &, bool, int);
bool inputNumber(int &, int);
/*!
 * @brief Input valid color
 * @param Color value
 * @return void
 */
bool inputColor(Color &);
bool inputColor(Color &, bool);
/*!
 * @brief Input valed date
 * @param Date value
 * @return void
 */
void inputDate(Date &);
/*!
 * @brief Input car's condition
 * @return bool if car is new, false otherwise
 */
bool inputNew();
/*!
 * @brief Check if string is a number
 * @return bool if string is a number, false otherwise
 */
bool isNumber(const string& s);

#pragma mark Sorts algorithms

/*!
 * @brief Binary insection sort
 * @param void* array
 * @param size_t size of array
 * @param size_t size of one element
 * @param int(*)(constvoid*,constvoid*) compare function pointer
 * @return sorted array
 */
void binaryInsectionSort(void *, size_t, size_t, int (*) ( const void *, const void * ) );

#pragma mark Other functions
/*!
 * @brief Compare function
 * @param constvoid* first variable
 * @param constvoid* second variable
 * @return return positive number if a > b, negative if a < b, zero if a == b
 */
int compare(const void *a, const void *b);
/*!
 * @brief Compare two integer value
 * @param constvoid* a - first number
 * @param constvoid* b - second number
 * positive number if a > b, negative if  < b, 0 if a == b
 */
int compare_int(const void *a, const void *b);
/*!
 * @brief Compare two car objects by waranty
 * @param constvoid* a - first number
 * @param constvoid* b -second number
 * positive number if a > b, negative if  < b, 0 if a == b
 */
int compare_car(const void *a, const void *b);

/*!
 * @brief Compare two car keys by waranty
 * @param constvoid* a - first number
 * @param constvoid* b -second number
 * positive number if a > b, negative if  < b, 0 if a == b
 */
int compare_key(const void *a, const void *b);

#endif /* functions_hpp */