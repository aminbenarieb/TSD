
//
//  module.h
//  TSD_2
//
//  Created by Amin Benarieb on 23/09/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
//

#ifndef module_h
#define module_h

using namespace std;

#pragma mark Constants

#define maxStringSize 12
#define maxPrice 10000000
#define maxWaranty 9
#define maxRun 999999
#define maxRepairs 9
#define filename "/Users/aminbenarieb/OneDrive/2Year/Labs/TSD/Records/Records/cars.am"
#define directory "/Users/aminbenarieb/OneDrive/2Year/Labs/TSD/Records/Records/"
#define tempfile "./_temp.am"
#define minYear 1700
#define maxYear 2100
#define disableFilter "n"

#pragma mark Macro


/*!
 * @brief Enuma of colors
 */
typedef enum {
    unknown = 0,
    white = 1,
    red = 2,
    orange = 3,
    yellow = 4,
    green = 5,
    blue = 6,
    purple = 7,
    black = 8,
    gray = 9
} Color;


/*!
 * @brief Filter for Car records
*/
typedef struct
{
    bool enable;
    bool modernEnable;
    bool colorEnable;
    bool runEnable;
    bool repairsEnable;
    int modern;
    Color color;
    int run;
    int repairs;
} RecordFilter;

/*!
 * @brief Structure of date
 */
typedef struct
{
    int day;
    int month;
    int year;
} Date;

/*!
 * @brief Stucture of car
 */
typedef struct
{
    /*!
     * @brief Brand name of car
     */
    string brand;
    string producer;
    int price;
    Color color;
    bool modern;
    union {
        int waranty;
        struct
        {
            int run;
            int repairs;
            Date date;
        } old;
        
    }condition;
} Car;


/*!
 * @brief Stucture of car key
 */
typedef struct
{
    int number;
    int price;
} Car_Key;


#endif /* module_h */
