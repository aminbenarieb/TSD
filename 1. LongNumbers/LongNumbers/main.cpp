#include <iostream>
#include <stdlib.h>
#include <vector>
#include <unistd.h>

#define base 10
#define max_size 30

using namespace std;
typedef vector<int> DigitsVector;
struct longNum {
    DigitsVector  vector;
    int degree  = 0;
    bool positive = true;
    bool flag_zero = true;
};


longNum multiplylongNums(longNum &x, longNum &y);
void  normalize(longNum &x);
void  inputlongNum(longNum &num, int &);
void  printlongNum(longNum &num, int &);
void  processError(int error);
int main(){

    int error;
    cout<<"╔═══════════════════════════════════════════════════════════════════════════════════════════╗\n";
    cout<<"║  Умножение двух чисел, где порядок имеет до 5 знаков: от –99999 до +99999, а мантисса -   ║\n";
    cout<<"║  до 30 знаков. Программа должна осуществлять ввод чисел и выдавать либо в указанном       ║\n";
    cout<<"║  формате (при корректных данных), либо сообщение о невозможности произвести счет          ║\n";
    cout<<"║  Формат ввода: ❬мантисса Е порядок❭. В случае нуля выводится 0.                           ║\n";
    cout<<"╚═══════════════════════════════════════════════════════════════════════════════════════════╝\n";
    
    cout<<"┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n";
    cout<<"┃Примеры ввода .00025, +123001., –123.456, 1234567E–20 или 1234567e20: ┃\n";
    cout<<"┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n\n";
    cout << "                               ↓\n";
    cout << "                               | ← Размер мантисы \n";
    cout << "                               ↑\n";
    
    do
    {
        longNum a,b,c;
        error = 0;
        
        inputlongNum(a, error);

        if (error != 0)
        {
            processError(error);
            cout<<"\nВведите число в указанном формате:\n";
            continue;
        }
        
        inputlongNum(b, error);
        if (error != 0)
        {
            processError(error);
            cout<<"\nВведите число в указанном формате:\n";
            continue;
        }

        c = multiplylongNums(a,b);
        
        cout << "Результат:\t";
        printlongNum(c, error);
        if (error != 0)
        {
            processError(error);
            continue;
        }
    }
    while (error);
    
    cout<<endl;
    
    return 0;
}

longNum multiplylongNums(longNum &x, longNum &y){
    longNum res;
    if (!x.flag_zero && !y.flag_zero)
    {
        res.flag_zero = false;
        res.positive = !(x.positive ^ y.positive);
        if (x.vector.size() < y.vector.size())
            swap(x, y);
        
        DigitsVector one = {1};
        
        if (y.vector != one)
        {
            res.vector.resize(x.vector.size() + y.vector.size());
        
            for (unsigned int i = 0; i < x.vector.size(); i++)
                for (unsigned int j = 0; j < y.vector.size(); j++)
                    res.vector[i+j] += x.vector[i] * y.vector[j];
            res.degree = x.degree + y.degree;
        }
        else
        {
            res = x;
        }
    }
    else res.flag_zero = true;
    
    return res;
}
void  normalize(longNum &x){
    if (!x.flag_zero)
    {
        for (unsigned int i = 0; i < x.vector.size(); i++) {
            x.vector[i+1] += x.vector[i] / base;
            x.vector[i] %= base;
        }
        
        size_t n = x.vector.size();
        int i = 0;
        for (; n > max_size; n--, i++)
            if (x.vector[i] >= 5) {
                x.vector[i+1] += 1;
                if (x.vector[i+1] == 10)
                    for (int k = i+1; x.vector[k] == 10; k++){
                        x.vector[k+1] += 1;
                        x.vector[k] = 0;
                    }
            }
        if (i) {
            rotate(x.vector.begin(), x.vector.begin() + i, x.vector.end());
            x.vector.resize(x.vector.size() - i);
            x.degree += i;
        }
    }
    
    if (!x.flag_zero)
    {
        // Убираем нули с начала
        while (x.vector.back() == 0)
            x.vector.pop_back();
        
        size_t n = x.vector.size();
        x.vector.push_back(0);
        x.degree += n;
        reverse(x.vector.begin(), x.vector.end());

        // Убираем нули с конца
        while (x.vector.back() == 0)
            x.vector.pop_back();
    }
}
void  printlongNum(longNum &num, int &err){
    normalize(num);
    
    if (num.flag_zero)
        cout << '0';
    else
    {
        if (num.degree < -99999 || num.degree > 99999)
            err = 3;
        else {
            if (num.positive)
                cout<< '+';
            else
                cout<< '-';
            cout<<num.vector[0]<<'.';
            
            for (unsigned int i = 1; i < num.vector.size(); i++)
                cout<< num.vector[i];
            cout<<'E'<<num.degree;
        }
    }
}
void  inputlongNum(longNum &num, int &err){
    cout<<"❱ ";
    string s;
    getline(cin, s);
    cout << endl;

    size_t pointPos = -1;
    if (s.size() == 0)
        err = 4;
    if (s.find('.') > s.find('E') || s.find('.') > s.find('e'))
        err = 1;
    if (err != 4) {
        if (s[0] != '.') {
            if (s.find('.') != string::npos){
                pointPos = s.find('.');
                s.erase(pointPos,1);
            }
            
            if (isdigit(s[0])) {
                if (s[0] != '0')
                    num.flag_zero = false;
                num.vector.push_back(s[0] - '0');
            }
            else if (s[0] ==  '-')
                num.positive = false;
            else if (s[0] != '+')
                err = 1;
        }
        else
            pointPos = 0;
        
        
        bool e = false;
        unsigned int i = 0;
        if (!err) {
            for (i = 1; i < s.size(); i++)
                if (isdigit(s[i])) {
                    if (num.flag_zero &&  s[i] != '0')
                        num.flag_zero = false;
                    num.vector.push_back(s[i] - '0');
                }
                else if (tolower(s[i]) == 'e'){
                    e = true;
                    break;
                }
                else
                    err = 1;
        }
        
        if (!err)
        {
            reverse(num.vector.begin(), num.vector.end());
            if (i > 30)
                err = 2;
            else if (e){
                s = s.substr(i+1, s.size()-i-1);
                num.degree = atoi(s.c_str());
            }
            
            if (num.degree < -99999 || num.degree > 99999)
                err = 3;
            else if (pointPos != -1)
                num.degree -= i - pointPos;
        }
    }
}
void  processError(int error){
    switch (error) {
        case 1:
            cout<<"Некорректные символы или неверный формат.\n";
            break;
        case 2:
            cout<<"Превышен размер мантиссы!\n";
            break;
        case 3:
            cout<<"Превышен размер порядка!\n";
            break;
        case 4:
            cout<<"Пустой ввод!\n";
            break;
        default:
            break;
    }
}
