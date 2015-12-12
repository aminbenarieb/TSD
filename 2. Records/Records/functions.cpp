//
//  functions.cpp
//  Records
//
//  Created by Amin Benarieb on 24/09/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
//

#include "functions.hpp"
#include <algorithm>
#include <time.h>
#include <math.h>

#pragma mark Test Time

#define kInetation 50
#define FileNameTable "table.txt"
#define FileNameKeys "keys.txt"

//Счетчик тактов
unsigned long long tick(void);unsigned long long tick(void)
{
    unsigned long long d;
    __asm__ __volatile__ ("rdtsc": "=A" (d) );
    
    return d;
}

void testTime(void *arr1, void* arr2, size_t size, int n, const char *fileName,  int (*compare) ( const void *, const void * ) )
{
    
    FILE *f = fopen(fileName, "w+");
    unsigned long long sum = 0, tb, te;
    unsigned long long average = 0;
    
    for (int i = 0; i < n; i++)
    {
        fprintf (f, "%d\t", i+1);
        for (int j = 0; j < kInetation; j++)
        {
            tb = tick();
            binaryInsectionSort(arr1, i, size, compare);
            te = tick();
            sum += te-tb;
            for (int k = 0; k < i; k++)
               memcpy( ((char *)arr1+k), ((char *)arr2+k), size);
        }
        average = sum / kInetation;
        fprintf (f, "%llu\t", average);
        sum = 0;
        average = 0;
        for (int j = 0; j < kInetation; j++)
        {
            tb = tick();
            qsort (arr1, i, size, compare);
            te = tick();
            sum += te-tb;
            for (int k = 0; k < i; k++)
                memcpy( ((char *)arr1+k), ((char *)arr2+k), size);
        }
        average = sum / kInetation;
        fprintf (f, "%llu\n", average);
    }
    
    fclose (f);
}

#pragma mark Struct functions

void addCar(){
    
    cout<<"Adding a new car. Press any key to start or 'n' to return to main menu.";
    string add;
    getline(cin, add);
    if (add == "n")
        return;
    
    Car car;
    printf("\nInput brand of car (no more %d symbols): ", maxStringSize);
    inputString(car.brand);
    printf("\nInput country-producer of car (no more %d symbols): ", maxStringSize);
    inputString(car.producer);
    printf("\nInput price of car (max %d euro): ", maxPrice);
    inputNumber(car.price, maxPrice);
    printf("\nInput color of car (1 - white, 2 - red, 3 - orange, 4 - yellow, 5 - green, 6 - blue, 7 - purple, 8 - black, 9 - silver): ");
    inputColor(car.color);
    printf("\nInput condition of car (0 - new, 1 - used): ");
    car.modern = inputNew();
    if (car.modern)
    {
    	printf("\nInput waranty of car (max %d): ", maxWaranty);
        inputNumber(car.condition.waranty, maxWaranty);
    }
    else
    {
    	printf("\nInput value of car run in km (max %d): ", maxRun);
        inputNumber(car.condition.old.run, maxRun);
    	printf("\nInput repaires count of car (max %d): ", maxRepairs);
        inputNumber(car.condition.old.repairs, maxRepairs);
    	printf("\nInput car production date: \n");
        inputDate(car.condition.old.date);
    }
    
    
    FILE *file = fopen(filename, "a");
    if (!file )
    {
        file = fopen(filename,"w");
        if (!file)
        {
            printf("\nUnable to create file %s.", filename);
            showContinue();
            return;
        }
    }
    
    if ( fwrite(&car, sizeof(Car), 1, file) != 1)
    {
        printf("\nUnexpeted error during saving file %s", filename);
        showContinue();
        return;
    }
        
    fclose(file);
    
    
    printf("\nRecord added to file. 🍻\n");
    showContinue();
}
void filterCar(){
    
    RecordFilter filter;
    filter.enable = true;
    filter.modernEnable = true;
    filter.colorEnable = true;
    filter.runEnable = true;
    filter.repairsEnable = true;
    
    cout<<"\nEnter filter settings (to disable specific filter print 'n')\n";
    
    cout<<"Car condtion: 0 - new, 1 - old)\n";
    if (!inputNumber(filter.modern, true, 1))
    {
        cout<<"Condition filter disabled.\n";
        filter.modernEnable = false;
    }
    printf("\nColor of car (1 - white, 2 - red, 3 - orange, 4 - yellow, 5 - green, 6 - blue, 7 - purple, 8 - black, 9 - silver): \n");
    if (!inputColor(filter.color, true))
    {
        cout<<"Color filter disabled.\n";
        filter.colorEnable = false;
    }
    printf("\nCar run in km (max %d): \n", maxRun);
    if (!inputNumber(filter.run, true, maxRun))
    {
        cout<<"Run filter disabled.\n";
        filter.runEnable = false;
    }
    printf("\nRepairs count of car (max %d)\n", maxRepairs);
    if (!inputNumber(filter.repairs, true, maxRepairs))
    {
        cout<<"Repair filter disabled.\n";
        filter.repairsEnable = false;
    }
    
    showTable(filter, true);
}
void removeCar(){
    
    RecordFilter filter;
    int number, n;
    
    filter.enable = false;
    n = showTable(filter, false);
    
    cout<<"\nInput number of car to delete or 'n' to return to main menu:";
    if(!inputNumber(number, true, n))
        return;
    
    FILE* inFile = fopen(filename, "r");
    FILE* outFile = fopen(tempfile, "w+");
    Car car;
    int recordCount = 0;
    
    if( inFile == NULL )
    {
        printf("Error: unable to open %s", filename);
        showContinue();
        return;
    }
    
    
    while (fread(&car, sizeof(Car), 1, inFile))
        if (recordCount++ != number-1)
            fwrite(&car, sizeof(Car), 1, outFile);
    
    fclose(inFile);
    fclose(outFile);
    
    if( rename(tempfile, filename) != 0)
    {
        printf("Error: unable to rename %s to %s", tempfile, filename);
        showContinue();
        return;
    }
    
    printf("\nRecord successfully removed! 🍻\n");
    showContinue();
    
}
void sortCar(){
    int sortIndex;
    
    system("clear");
    cout<<"Choose sort algorithm. To return to main menu enter 'n'.\n";
    cout<<"1 - Quicke Sort \n";
    cout<<"2 - Improved Bubble Sort \n";
    
    if(!inputNumber(sortIndex, true, 3))
        return;
    
    
    system("clear");
    printf("\e[8;25;180t");
    FILE *file = fopen(filename, "rb");
    if (!file )
    {
        printf("\nTable is empty.");
        showContinue();
        return;
    }
    int n = 0;
    Car rawCar;
    
    // Counting
    while (fread(&rawCar, sizeof(Car), 1, file))
        n++;
    
    if ( n == 0)
    {
        cout<<"\nTable is empty!";
        showContinue();
        return;
    }
    
    int i = 0;
    Car *cars = new Car[n];
    Car_Key car_keys[n];
    
    rewind(file);
    // Reading file
    while (fread(&cars[i], sizeof(Car), 1, file))
    {
        car_keys[i].number = i;
        car_keys[i].price = cars[i].price;
        i++;
    }
    
    fclose(file);
    
    const clock_t begin_time_key = clock();
    switch (sortIndex) {
        case 1:
            qsort(car_keys, n, sizeof(Car_Key), compare_key);
            break;
        case 2:
            binaryInsectionSort(car_keys, n, sizeof(Car_Key), compare_key);
            break;
        default:
            cout<<"Undefined sort selected!  😣";
            return;
    }
    const float time_key = float( clock () - begin_time_key ) /  CLOCKS_PER_SEC;
    
    showHeaderKeys();
    for (int i = 0; i < n; i++)
        showKey(i+1, car_keys[i]);
    cout <<endl<<endl;
    showHeader();
    for (int i = 0; i < n; i++)
            showRecord(i+1, cars[car_keys[i].number]);
    printf("\n\nSorting time of table keys: %lf seconds\n\n", time_key);
    
    const clock_t begin_time = clock();
    switch (sortIndex) {
        case 1:
            qsort(cars, n, sizeof(Car), compare_car);
            break;
        case 2:
            binaryInsectionSort(cars, n, sizeof(Car), compare_car);
            break;
        default:
            cout<<"Undefined sort selected!  😣";
            return;
    }
    const float time = float( clock () - begin_time ) /  CLOCKS_PER_SEC;
    
    showHeader();
    for (int i = 0; i < n; i++)
        showRecord(i+1, cars[i]);
    printf("\n\nSorting time of table: %lf seconds.\n\n", time);
    

    if (fabs(time_key - time) > 1e-5 * fmax(fabs(time_key), fabs(time)))
        printf("Sorting keys is %d procents faster. \n", (int)((time/time_key)*100) );
    else if (fabs(time_key - time) < 1e-5 * fmax(fabs(time_key), fabs(time)))
        printf("Sorting table is %d procents faster.\n", (int)((time_key/time)*100) );
    else
        printf("Times of sorting table and keys are equal.\n");
    
    printf("Used memory for table keys is %lu bytes and for table is %lu bytes.\n", sizeof(Car_Key)*n, sizeof(Car)*n);
    
    delete [] cars;
    
    showContinue();
}
void analyzeCars()
{
    
    system("clear");
    printf("\e[8;25;180t");
    FILE *file = fopen(filename, "rb");
    if (!file )
    {
        printf("\nTable is empty.");
        showContinue();
        return;
    }
    int n = 0;
    Car rawCar;
    
    // Counting
    while (fread(&rawCar, sizeof(Car), 1, file))
        n++;
    
    if ( n == 0)
    {
        cout<<"\nTable is empty!";
        showContinue();
        return;
    }
    
    int i = 0;
    Car *cars = new Car[n];
    Car *cars_copy = new Car[n];
    Car_Key car_keys[n];
    Car_Key car_keys_copy[n];
    
    rewind(file);
    // Reading file
    while (fread(&cars[i], sizeof(Car), 1, file))
    {
        car_keys[i].number = i;
        car_keys[i].price = cars[i].price;
        
        cars_copy[i] = cars[i];
        car_keys_copy[i] = car_keys[i];
        
        i++;
        
    }
    
    testTime(cars, cars_copy, sizeof(Car), n, FileNameTable, compare_car);
    testTime(car_keys, car_keys_copy, sizeof(Car_Key), n, FileNameKeys, compare_key);
    
    delete [] cars;
    delete [] cars_copy;
    
    printf("\nSuccessfuly analyzed 🍻.\nData saved to %s\n", directory);
    
    showContinue();
    
    fclose(file);
    
}

#pragma mark Interface functions

void f(char const *format, int w, char const *s)
{
    printf(format,w+strlen(s)/2,s,w-strlen(s)/2,"|");
}

void showHeader(){
    cout<<endl;
    f("%*s%*s", 3, "id");
    f("%*s%*s",  maxStringSize, "Brand");
    f("%*s%*s",  maxStringSize, "Country");
    f("%*s%*s",  8, "Price");
    f("%*s%*s",  7, "Color");
    f("%*s%*s",  8, "Condition");
    f("%*s%*s",  7, "Waranty");
    f("%*s%*s",  10, "Run");
    f("%*s%*s",  9, "Repairs");
    f("%*s%*s\n",  12, "Date");
    for (int i = 0; i < 177; printf("-"), i++);
}

void showHeaderKeys(){
    f("%*s%*s", 3, "id");
    f("%*s%*s", 11, "table index");
    f("%*s%*s\n",  8, "Price");
    for (int i = 0; i < 44; printf("-"), i++);
}

void showRecord(size_t id, Car car){
    
    cout<<endl;
    f("%*s%*s",  3, to_string(id).c_str());
    f("%*s%*s",  maxStringSize, car.brand.c_str());
    f("%*s%*s",  maxStringSize, car.producer.c_str());
    f("%*s%*s",  8, to_string(car.price).c_str());
    f("%*s%*s",  7, getColorName(car.color).c_str());
    f("%*s%*s",  8, car.modern ? "New" : "Old");
    f("%*s%*s",  7, car.modern ? to_string(car.condition.waranty).c_str() : "-");
    f("%*s%*s",  10, !car.modern ? to_string(car.condition.old.run).c_str() : "-");
    f("%*s%*s",  9, !car.modern ? to_string(car.condition.old.repairs).c_str() : "-");
    if (!car.modern)
        printf("%*s%02d/%02d/%*d%*s|", 7, "", car.condition.old.date.day, car.condition.old.date.month, 4, car.condition.old.date.year, 6, "");
    else
        f("%*s%*s", 12, "-");
    
}
void showKey(size_t id, Car_Key key){
    
    cout<<endl;
    f("%*s%*s",  3, to_string(id).c_str());
    f("%*s%*s",  11, to_string(key.number).c_str());
    f("%*s%*s",  8, to_string(key.price).c_str());
    
}
void showMenu(){
    
    cout<<"Input action: "<<endl;
    cout<<"0 - exit"<<endl;
    cout<<"1 - show table"<<endl;
    cout<<"2 - add record"<<endl;
    cout<<"3 - remove record"<<endl;
    cout<<"4 - filter record"<<endl;
    cout<<"5 - sort table"<<endl;
    cout<<"6 - analyze table"<<endl;
}
int  showTable(){
    int n;

    RecordFilter filter;
    filter.enable = false;
    n = showTable(filter, false);
    showContinue();
    return n;
}
int  showTable(RecordFilter filter, bool flagContinue){
    
    system("clear");
    printf("\e[8;25;180t");
    FILE *file = fopen(filename, "rb");
    if (!file )
    {
        printf("\nTable is empty.");
        if (flagContinue)
            showContinue();
        return 0;
    }
    int n = 0;
    Car rawCar;
    
    // Counting
    while (fread(&rawCar, sizeof(Car), 1, file))
        n++;
    
    if ( n == 0)
    {
        cout<<"\nTable is empty!";
        if (flagContinue)
            showContinue();
        return 0;
    }
    
    int i = 0;
    Car *cars = new Car[n];
    Car_Key car_keys[n];
    
    rewind(file);
    // Reading file
    while (fread(&cars[i], sizeof(Car), 1, file))
    {
        car_keys[i].number = i;
        car_keys[i].price = cars[i].price;
        i++;
        
    }
    
    showHeader();
    for (int i = 0, j = 0; i < n; i++)
    {
        Car car = cars[i];
        
        if (!filter.enable)
            showRecord(i+1, car);
        else
        {
            bool modern;
            if (filter.modern == 0)
                modern = car.modern == true;
            else if (filter.modern != 0)
                modern = car.modern == false;
            modern = modern || !filter.modernEnable;
            
            bool color = filter.color == car.color || !filter.colorEnable;
            bool run = filter.run >= car.condition.old.run || !filter.runEnable || car.modern;
            bool repairs = filter.repairs >= car.condition.old.repairs || !filter.repairsEnable || car.modern;
            
            if (modern && color && run && repairs)
                showRecord((j++)+1, car);
        }
    }
    
        
    cout<<endl;
    
    if (flagContinue)
        showContinue();
    
    delete [] cars;
    
    return n;
    
}
void showContinue(){

    cout<<"\nPress any key to continue. 🌝\n";
    getchar();
}

#pragma mark Handling

void   showErrorMessage(int error){
    
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
            printf("\nInvalid date! 👮🏼\n");
            break;
        default:
            printf("\nUnexpected error with code %d. 💀\n", error);
            break;
    }
    
}
void   handleAction(int action){
    
    switch (action) {
        case 0:
            break;
        case 1:
            showTable();
            break;
        case 2:
            addCar();
            break;
        case 3:
            removeCar();
            break;
        case 4:
            filterCar();
            break;
        case 5:
            sortCar();
            break;
        case 6:
            analyzeCars();
            break;
        default:
            cout<<"Sorry, this functionality is unavialible at this moment :(\n";
            showContinue();
            break;
    }
}
void   trim(string &s){
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
string getColorName(Color color){
    
    switch (color) {
        case white:
            return "White";
        case red:
            return "Red";
        case yellow:
            return "Yellow";
        case orange:
            return "Orange";
        case green:
            return "Green";
        case blue:
            return "Blue";
        case purple:
            return "Purple";
        case black:
            return "Black";
        case gray:
            return "Silver";
        default:
            return "Unknown";
    }
}

#pragma mark Date functions

bool leapYear(int year){
    
    if(((year%4) == 0) && (((year%100)!=0) || ((year%400) == 0)))
        return true;
       
   return false;
}
bool isDateValid(int day, int month, int year, int &error){

    if(year < minYear || year > maxYear)
    {
        error = 1;
        return false;
    }
    
    if(day < 1)
    {
        error = 2;
        return false;
    }
    
    switch(month)
    {
        case 2:
            if(day > 29 && leapYear(year))
            {
                error = 3;
                return false;
            }
            else if(day > 28)
            {
                error = 4;
                return false;
            }
            break;
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            if(day > 31)
            {
                error = 5;
                return false;
            }
            break;
        case 4: case 6: case 9: case 11:
            if(day > 30)
            {
            	error = 6;
                return false;
            }
            break;
        default: // the month is not between 1 and 12
	        error = 7;
            return false;
            break;
    }
    return true;
}

#pragma mark Input functions

void inputString(string &str){
    
    do
    {
        string rawInput;
        cout<<"❱ ";
        getline(cin, rawInput);
        
        trim(rawInput);
        
        if (rawInput.length() == 0)
        {
            cout<<"Empty input!\n";
        }
        else if (rawInput.length() > maxStringSize)
        {
            cout<<"Too long string!\n";
        }
        else{
            str = rawInput;
            break;
        }
        
    }
    while (true);
    
}
bool inputNumber(int &number, int maxVal){
    return inputNumber(number, false, maxVal);
}
bool inputNumber(int &number, bool filter, int maxVal){
    
    do
    {
        string rawInput;
        cout<<"❱ ";
        getline(cin, rawInput);
        
        trim(rawInput);
        
        if (rawInput.empty())
        {
            cout<<"Empty input!\n";
        }
        else if ( rawInput == disableFilter && filter )
            return false;
        else if (rawInput.length() > maxStringSize)
        {
            cout<<"Too long number!\n";
        }
        else
        {
            string::const_iterator it = rawInput.begin();
            while (it != rawInput.end() && isdigit(*it)) ++it;
            if (it != rawInput.end() )
            {
                 cout<<"Input positive numbers!\n";
            }
            else
            {
                number = stoi(rawInput);
                if (number > maxVal)
                {
                    printf("Too big number. Max value is %d!\n", maxVal);
                }
                else break;
            }
        }
        
    }
    while (true);
    
    return true;
    
}
bool inputColor(Color &color){
    return inputColor(color, false);
}
bool inputColor(Color &color, bool filter){
    
    do
    {
        string rawInput;
        cout<<"❱ ";
        getline(cin, rawInput);
        
        trim(rawInput);
        
        if (rawInput.empty())
        {
            cout<<"Empty input!\n";
        }
        else if ( rawInput == disableFilter && filter )
            return false;
        else
        {
            if (!isNumber(rawInput))
            {
            	cout<<"Input numbers!\n";
            }
            else if (1 > stoi(rawInput) || stoi(rawInput) > 9 )
            {
                cout<<"Numbers should be from 1 to 9!\n";
            }
            else{
            	color = static_cast<Color>(stoi(rawInput));
            	break;
	        }
        }
        
    }
    while (true);
    
    return  true;
    
}
void inputDate(Date &date){

	int year, month, day;
	while (true)
	{
		printf("Input year: ");
		inputNumber(year, maxYear);
		printf("Input month: ");
		inputNumber(month, 12);
		printf("Input day: ");
		inputNumber(day, 31);

        int error = 0;
        
        string yearMessage = "is";
        time_t theTime = time(NULL);
        struct tm *aTime = localtime(&theTime);
        int cyear = aTime->tm_year + 1900;
        if (cyear > year) yearMessage = "was";
        if (cyear < year) yearMessage = "will be";

        if (!isDateValid(day, month, year, error)){
			 switch (error) {
		        case 1:
		            printf("\nInvalid year. Enter year between %d and %d!\n", minYear, maxYear);
		            break;
		        case 2:
		            printf("\nInvalid day!\n");
		            break;
		        case 3:
		            printf("\nThere is no more 29 days in February!\n");
		            break;
		        case 4:
		            printf("\nThere %s 28 days in February in this year!\n", yearMessage.c_str());
		            break;
		        case 5:
		            printf("\nDay can not be more 31!\n");
		            break;
		        case 6:
		            printf("\nThere %s 30 days in this month!\n", yearMessage.c_str());
		            break;
	           case 7:
		            printf("\nInvalid month. Enter month between 1 and 12!\n");
		            break;
		        default:
		            printf("\nUnexpected error with code %d.\n", error);
		            break;
			    }
		}
		else 
		{
            date.day = day;
            date.month = month;
            date.year = year;
            
			break;
		}
	}
}
bool inputNew(){

	do
    {
        string rawInput;
        cout<<"❱ ";
        getline(cin, rawInput);
        
        trim(rawInput);
        
        if (rawInput.empty())
        {
            cout<<"Empty input!\n";
        }
        else
        {
            if (!isNumber(rawInput))
            {
            	cout<<"Input numbers!\n";
            }
            else if (0 > stoi(rawInput) || stoi(rawInput) > 1 )
            {
                cout<<"Numbers should be 0 or 1!\n";
            }
            else{
            	return stoi(rawInput) == 0;
	        }
        }
        
    }
    while (true);
}
bool isNumber(const string& s){
    string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

#pragma mark Sorts algorithms

void binaryInsectionSort(void *first, size_t number, size_t size, int (*compare) ( const void *, const void * ) ){
    char *pArr = (char*)first;
    char *pArrNext = pArr + number * size;
    char temp[size];
    char *pLeft;
    char *pRight;
    char *pMiddle;
    char *p1, *p2;

    for (p1 = pArr + size; p1 < pArrNext; p1+=size)
    {
        memcpy (temp, p1, size);
        pLeft = pArr;
        pRight = p1;
        while (pLeft < pRight)
        {
            pMiddle = pLeft + ((pRight - pLeft)/(size*2)) * size;
            if (compare (temp, pMiddle) < 0)
                pRight = pMiddle;
            else
                pLeft= pMiddle + size;
        }
        for (p2 = p1-size; p2 >= pLeft; p2-=size)
            memcpy (p2 + size, p2, size);
        memcpy(pRight, temp, size);
    }
}

#pragma mark Other functions

int compare_key(const void *a, const void *b){
    
    
    Car_Key car_a = *(Car_Key *)a;
    Car_Key car_b = *(Car_Key *)b;
    
    return car_a.price - car_b.price;

    
}
int compare_car(const void *a, const void *b){

    Car car_a = *(Car *)a;
    Car car_b = *(Car *)b;
    
    return car_a.price - car_b.price;
    
}

