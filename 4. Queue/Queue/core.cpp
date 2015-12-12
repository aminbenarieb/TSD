//
//  Queue.cpp
//  Queue
//
//  Created by Amin Benarieb on 02/10/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
//

#include "core.hpp"

// ****************************
// ******** Simulation ********
// ****************************
#pragma mark Simulate

#define eps 1e-3
#define MAX(a,b) a > b ? a : b
bool fcompare(double x, double y)
{
    return fabs(x -y) <= eps;
}

double randomRange(double t1, double t2)
{
    double t = (double)rand() / RAND_MAX;
    return t1 + t * (t2 - t1);
}

/*!
 * @brief Enum of Device
 */
typedef enum {
    Empty = 0,
    ModeQueue1 = 1,
    ModeQueue2 = 2
} Device;

void SimulateQueueArray(){
    
    Error error = {0, ""};
    
    QueueArray queue1;
    QueueArray queue2;
    
    Request request = {0,0,0};
    Device device = Empty;
    
    double time = 0;
    double q1ComeTime = 0.0;
    double q2ComeTime  = 0.0;
    double averageQ1ComeTime = 0.0;
    double averageQ2ComeTime  = 0.0;
    int q1ComeCounter = 0;
    int q2ComeCounter = 0;
    double averageQ1OutTime = 0.0;
    double averageQ2OutTime  = 0.0;
    int qOutCounter = 0;
    
    int numberOutQueue1 = 0;
    int numberOutQueue2 = 0;
    int numberOutQueues = 0;
    int lastNumberOutQueues = 0;
    int counter100times = 0;
    int queue1Size100 = 0;
    int queue2Size100 = 0;
    
    double expectedTime= 0;
    
    while (numberOutQueue1 < 1000)
    {
        
        if (q1ComeTime <= time)
        {
            q1ComeTime = time + randomRange(T1_1, T1_2);
            averageQ1ComeTime += q1ComeTime;
            averageQ1ComeTime /= ++q1ComeCounter;
            queue1.push({q1ComeTime, randomRange(T3_1, T3_2), ++queue1.index}, &error);
            if (error.code)
            {
                printf("\nUnexpected error: %s\n", error.message.c_str());
                return;
            }
        }
        
        if (time - q2ComeTime >= 0)
        {
            q2ComeTime = time + randomRange(T2_1, T2_2);
            averageQ2ComeTime += q2ComeTime;
            averageQ2ComeTime /= ++q2ComeCounter;
            queue2.push({q2ComeTime, randomRange(T4_1, T4_2), ++queue2.index}, &error);
            if (error.code)
            {
                printf("\nUnexpected error: %s\n", error.message.c_str());
                return;
            }
        }

        // Choosing queues
        if (device == Empty)
        {
            if ( queue1.size == 0)
                device = ModeQueue2;
            else  if ( queue2.size == 0)
                device = ModeQueue1;
            else
                device = q1ComeTime - q2ComeTime <= 0 ? ModeQueue1 : ModeQueue2;
            
        }
        
        
        // *****
        // Processing queues mode for device & Counting out queues
        // *****
        if (device == ModeQueue1 && queue1.size != 0 && request.value == 0 )
        {
            request = queue1.pop(&error);
            request.processTime += time;
            averageQ1OutTime += request.processTime;
            averageQ1OutTime /= ++qOutCounter;
            request.value = 1;
            if (error.code)
            {
                printf("\nUnexpected error for queue1: %s\n", error.message.c_str());
                return;
            }
        }
        else if (device == ModeQueue2 && queue2.size != 0  && request.value == 0)
        {
            request = queue2.pop(&error);
            request.processTime += time;
            averageQ2OutTime += request.processTime;
            averageQ2OutTime /= ++qOutCounter;
            request.value = 1;
            if (error.code)
            {
                printf("\nUnexpected error queue2: %s\n", error.message.c_str());
                return;
            }
        }
        if (device != Empty && fcompare(time,request.processTime) && request.value != 0) {
            
            if (device == ModeQueue1)
                numberOutQueue1++;
            else if (device == ModeQueue2)
                numberOutQueue2++;
            
            request.value = 0;
            
            if (queue1.size == 0 || queue2.size == 0)
                device = Empty;

            numberOutQueues++;
        }
        // *****
        // *****
        
        if (numberOutQueues != lastNumberOutQueues && numberOutQueues % 100 == 0)
        {
            lastNumberOutQueues = numberOutQueues;

            counter100times++;
            queue1Size100 += queue1.size;
            queue2Size100 += queue2.size;
            
            printf("\n\nProcessed requests: %d\n", numberOutQueues);
            printf("\tLength of first queue: %d\n", queue1.size);
            printf("\tLength of second queue: %d\n", queue2.size);
            printf("\tAverage length of first queue: %lf\n", double(queue1Size100) / counter100times);
            printf("\tAverage length of second queue: %lf\n\n", double(queue2Size100) / counter100times);
            
        }
        
        time += eps;
        
    }
    
    
    /*
     
     Методичка
     ---
     При проверке работы программы по входу заявок
     ожидаемое время моделирования равно
     среднему интервалу между приходом заявок, 
     умноженному на количество вошедших заявок. 
     Если есть две очереди, то проверка ведется 
     по каждой из очередей.
     
     Если среднее время обработки заявок больше среднего интервала между их поступлением, то очередь будет расти и время моделирования будет определяться временем обработки заявок. При этом количество вошедших заявок должно быть равно времени моделирования, деленному на средний интервал между приходом заявок.
     
     Если средний интервал между поступлением заявок больше или равен среднему времени их обработки, то длина очереди стабилизируется (может стремиться к нулю, единице или к какой-то другой величине) и время моделирования будет определяться временем прихода заявок.
     
     Среднее время прихода или обработки заявок можно подсчитать как среднее арифметическое временного диапазона (например, если t1 = 1, а t2 = 4, то среднее время будет равно tср = (1+4)/2 = 2.5). Аналогичным образом рассчитывается время окончания обработки заявок и время выхода заявок из системы.

     
     Задание
     -------
     
     Расчетное время моделирование (по входу) = среднее время прихода заявки* количество вошедших заявок.

     Если есть 2 очереди, то расчет идет  для каждой из очередей.
     
     Расчетное время моделирование (по выходу) =
     среднее время обработки заявки* количество обработанных заявок.
     
     Если есть 2 очереди и один аппарат, то время моделирования будет суммой времен обслуживаний заявок каждого типа.
     
     Если время обработки больше времени прихода, то очередь будет расти, и время моделирования будет определяться временем обработки, а количество вошедших заявок будет = времени моделирования /  среднее время прихода заявки.
     Если время прихода больше времени обработки, то очереди не будет, и время моделирования будет определяться временем прихода заявок.
     
     
     */
    double expectedTimeCome = MAX(averageQ1ComeTime*queue1.index,averageQ2ComeTime*queue2.index);
    double expectedTimeOut = averageQ1OutTime*numberOutQueue1 + averageQ2OutTime*numberOutQueue2;
    expectedTime = MAX(expectedTimeCome,expectedTimeOut);

    
    printf("\nNumber of came request of first type %d", queue1.index);
    printf("\nNumber of came request of second type %d", queue2.index);
    printf("\nNumber of processed request of first type %d", numberOutQueue1);
    printf("\nNumber of processed request of second type %d", numberOutQueue2);
    printf("\nTime of simulation %lf", time);
    printf("\nExpected time of simulation %lf", expectedTime);
    
    
    showContinue();
    
}
void SimulateQueueList(){
    
    Error error = {0, ""};
    
    QueueList queue1;
    QueueList queue2;
    
    Request request = {0,0,0};
    Device device = Empty;
    
    double time = 0;
    double q1ComeTime = 0.0;
    double q2ComeTime  = 0.0;
    double averageQ1ComeTime = 0.0;
    double averageQ2ComeTime  = 0.0;
    int q1ComeCounter = 0;
    int q2ComeCounter = 0;
    double averageQ1OutTime = 0.0;
    double averageQ2OutTime  = 0.0;
    int qOutCounter = 0;
    
    int numberOutQueue1 = 0;
    int numberOutQueue2 = 0;
    int numberOutQueues = 0;
    int lastNumberOutQueues = 0;
    int counter100times = 0;
    int queue1Size100 = 0;
    int queue2Size100 = 0;
    
    double expectedTime= 0;
    
    while (numberOutQueue1 < 1000)
    {
        
        if (q1ComeTime <= time)
        {
            q1ComeTime = time + randomRange(T1_1, T1_2);
            averageQ1ComeTime += q1ComeTime;
            averageQ1ComeTime /= ++q1ComeCounter;
            queue1.push({q1ComeTime, randomRange(T3_1, T3_2), ++queue1.index}, &error);
            if (error.code)
            {
                printf("\nUnexpected error: %s\n", error.message.c_str());
                return;
            }
        }
        
        if (time - q2ComeTime >= 0)
        {
            q2ComeTime = time + randomRange(T2_1, T2_2);
            averageQ2ComeTime += q2ComeTime;
            averageQ2ComeTime /= ++q2ComeCounter;
            queue2.push({q2ComeTime, randomRange(T4_1, T4_2), ++queue2.index}, &error);
            if (error.code)
            {
                printf("\nUnexpected error: %s\n", error.message.c_str());
                return;
            }
        }
        
        // Choosing queues
        if (device == Empty)
        {
            if ( queue1.size == 0)
                device = ModeQueue2;
            else  if ( queue2.size == 0)
                device = ModeQueue1;
            else
                device = q1ComeTime - q2ComeTime <= 0 ? ModeQueue1 : ModeQueue2;
            
        }
        
        
        // *****
        // Processing queues mode for device & Counting out queues
        // *****
        if (device == ModeQueue1 && queue1.size != 0 && request.value == 0 )
        {
            queue1.pop(&error, &request);
            request.processTime += time;
            averageQ1OutTime += request.processTime;
            averageQ1OutTime /= ++qOutCounter;
            request.value = 1;
            if (error.code)
            {
                printf("\nUnexpected error for queue1: %s\n", error.message.c_str());
                return;
            }
        }
        else if (device == ModeQueue2 && queue2.size != 0  && request.value == 0)
        {
            queue2.pop(&error, &request);
            request.processTime += time;
            averageQ2OutTime += request.processTime;
            averageQ2OutTime /= ++qOutCounter;
            request.value = 1;
            if (error.code)
            {
                printf("\nUnexpected error queue2: %s\n", error.message.c_str());
                return;
            }
        }
        if (device != Empty && fcompare(time,request.processTime) && request.value != 0) {
            
            if (device == ModeQueue1)
                numberOutQueue1++;
            else if (device == ModeQueue2)
                numberOutQueue2++;
            
            request.value = 0;
            
            if (queue1.size == 0 || queue2.size == 0)
                device = Empty;
            
            numberOutQueues++;
        }
        // *****
        // *****
        
        if (numberOutQueues != lastNumberOutQueues && numberOutQueues % 100 == 0)
        {
            lastNumberOutQueues = numberOutQueues;
            
            counter100times++;
            queue1Size100 += queue1.size;
            queue2Size100 += queue2.size;
            
            printf("\n\nProcessed requests: %d\n", numberOutQueues);
            printf("\tLength of first queue: %d\n", queue1.size);
            printf("\tLength of second queue: %d\n", queue2.size);
            printf("\tAverage length of first queue: %lf\n", double(queue1Size100) / counter100times);
            printf("\tAverage length of second queue: %lf\n\n", double(queue2Size100) / counter100times);
            
        }
        
        time += eps;
        
    }
    
    
    /*
     
     Методичка
     ---
     При проверке работы программы по входу заявок
     ожидаемое время моделирования равно
     среднему интервалу между приходом заявок,
     умноженному на количество вошедших заявок.
     Если есть две очереди, то проверка ведется
     по каждой из очередей.
     
     Если среднее время обработки заявок больше среднего интервала между их поступлением, то очередь будет расти и время моделирования будет определяться временем обработки заявок. При этом количество вошедших заявок должно быть равно времени моделирования, деленному на средний интервал между приходом заявок.
     
     Если средний интервал между поступлением заявок больше или равен среднему времени их обработки, то длина очереди стабилизируется (может стремиться к нулю, единице или к какой-то другой величине) и время моделирования будет определяться временем прихода заявок.
     
     Среднее время прихода или обработки заявок можно подсчитать как среднее арифметическое временного диапазона (например, если t1 = 1, а t2 = 4, то среднее время будет равно tср = (1+4)/2 = 2.5). Аналогичным образом рассчитывается время окончания обработки заявок и время выхода заявок из системы.
     
     
     Задание
     -------
     
     Расчетное время моделирование (по входу) = среднее время прихода заявки* количество вошедших заявок.
     
     Если есть 2 очереди, то расчет идет  для каждой из очередей.
     
     Расчетное время моделирование (по выходу) =
     среднее время обработки заявки* количество обработанных заявок.
     
     Если есть 2 очереди и один аппарат, то время моделирования будет суммой времен обслуживаний заявок каждого типа.
     
     Если время обработки больше времени прихода, то очередь будет расти, и время моделирования будет определяться временем обработки, а количество вошедших заявок будет = времени моделирования /  среднее время прихода заявки.
     Если время прихода больше времени обработки, то очереди не будет, и время моделирования будет определяться временем прихода заявок.
     
     
     */
    double expectedTimeCome = MAX(averageQ1ComeTime*queue1.index,averageQ2ComeTime*queue2.index);
    double expectedTimeOut = averageQ1OutTime*numberOutQueue1 + averageQ2OutTime*numberOutQueue2;
    expectedTime = MAX(expectedTimeCome,expectedTimeOut);
    
    
    printf("\nNumber of came request of first type %d", queue1.index);
    printf("\nNumber of came request of second type %d", queue2.index);
    printf("\nNumber of processed request of first type %d", numberOutQueue1);
    printf("\nNumber of processed request of second type %d", numberOutQueue2);
    printf("\nTime of simulation %lf", time);
    printf("\nExpected time of simulation %lf", expectedTime);
    
    
    error = {0, ""};
    Request *iReq = queue1.pop(&error, &request);
    while (iReq != queue1.top)
        iReq = queue1.pop(&error, &request);
    
    iReq = queue2.pop(&error, &request);
    while (iReq != queue2.top)
        iReq = queue2.pop(&error, &request);

    
    
    showContinue();
    
}
// ***************************
// **** End of Simulation ****
// ***************************


// ***************************
// ******** Analyzing ********
// ***************************
#pragma mark Analyze
unsigned long long tick(void){
    unsigned long long d;
    __asm__ __volatile__ ("rdtsc": "=A" (d) );
    
    return d;
}
void AnalyzeQueue(){
    
    unsigned long long sum = 0, tb, te;
    unsigned long long average = 0;
    
    QueueArray queueArray;
    QueueList queueList;
    Request request = {0,0,0};
    Error error = {0, ""};
    
    
    printf("Analyzing time of queues (ticks).\n\n");
    
    
    printf("Push function:\n");
    for (int i = 0; i < kInetation; i++)
    {
        tb = tick();
        queueArray.push(request, &error);
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
        queueList.push(request, &error);
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
        queueArray.pop(&error);
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
        queueList.pop(&error, &request);
        te = tick();
        sum += te-tb;
    }
    average = sum / kInetation;
    printf("\tList: %llu\n", average);
    
    
    printf("\n\nAllocaed memory (bytes):\n");
    printf("\tArray: %lu", sizeof(QueueArray));
    printf("\tList: %lu\n", sizeof(QueueList));
    
    
    showContinue();
    
}
// ***************************
// ***** End of Analyzing ****
// ***************************

#pragma mark Queue functions
void pushQueueArray(QueueArray *queueArray){
    
    Error error = {0, ""};
    string rawInput;
    
    printf("Queue injection.\n\tPress any key to add or '0' to exit.\n");
    
    while (rawInput != "0")
    {
        Request request = {0,0, ++queueArray->index};
        
        if(queueArray->push(request, &error))
            printf("\n\n\tSucessfuly added '%d'.\n 🍻", request.value);
        else
        {
            printf("\n\tError: %s\n", error.message.c_str());
            break;
        }
        
        printf("\n\tCurrent queue: ");
        for (int i = 0; i < queueArray->size; i++)
            printf("%i ", queueArray->elements[i].value);
        
        cout<<"\n\t❱";
        getline(cin, rawInput);
        
    }
    
    
    showContinue();
    
}
void popQueueArray(QueueArray *queueArray){
    
    Error error = {0, ""};
    string rawInput;
    
    printf("Queue ejection.\n\tPress any key to add or '0' to exit.\n");
    
    while (rawInput != "0")
    {
        Request request = queueArray->pop(&error);
        
        if (!error.code)
            printf("\n\n\tSuccessfuly ejected  %d. 🍻", request.value);
        else
        {
            printf("\n\tError: %s", error.message.c_str());
            break;
        }

        if ( queueArray->size == 0 )
            break;
        
        printf("\n\tCurrent queue: ");
        for (int i = 0; i < queueArray->size; i++)
            printf("%i ", queueArray->elements[i].value);
        
        cout<<"\n\t❱";
        getline(cin, rawInput);
        
    }


    
    showContinue();
}

void pushQueueList(QueueList *queueList, RequestAddressVector *removedList){

    Error error = {0, ""};
    string rawInput;
    
    printf("Queue injection.\n\tPress any key to add or '0' to exit.\n");
    
    while (rawInput != "0")
    {
        Request request = {0,0, ++queueList->index};
        
        if(queueList->push(request, &error))
            printf("\n\n\tSucessfuly added '%d'. 🍻\n", queueList->top->value);
        else
        {
            printf("\n\tError: %s\n", error.message.c_str());
            break;
        }
        
        for (size_t i = removedList->size(); i > 0; i--)
            if ( (struct Request *)(*removedList)[i-1] == queueList->top)
            {
                
                printf("\n\t\tFree memory: %p", (struct request *)(*removedList)[i-1]);
                printf("\n\t\tRealocated memory: %p  \n", queueList->top);
                
                RequestAddressVector _removedList;
                for (size_t i = removedList->size(); i > 0; i--)
                    if ( (struct Request *)(*removedList)[i-1] != queueList->top)
                    {
                        _removedList.push_back((*removedList)[i-1]);
                    }
                *removedList = _removedList;
            }
        
        if ( queueList->bottom != NULL)
        {
            printf("\n\tCurrent queue: ");
            Request *iReq = queueList->bottom;
            while (iReq != queueList->top)
            {
                printf("%d ", iReq->value);
                iReq = iReq->next;
            }
            printf("%d ", iReq->value);
            
            if ( removedList->size() != 0 )
            {
                printf("\n\n\tFree memory addresses: ");
                for (size_t i = removedList->size(); i > 0; i--)
                    printf("%p ", (struct request *)(*removedList)[i-1]);
            }
            
            printf("\n\n\tAllocated memory addresses: ");
            iReq = queueList->bottom;
            while (iReq != queueList->top)
            {
                printf("%p ", iReq);
                iReq = iReq->next;
            }
            printf("%p ", iReq);
        }
        
        cout<<"\n\t❱";
        getline(cin, rawInput);
        
    }
    
    
    showContinue();
}
void popQueueList(QueueList *queueList, RequestAddressVector *removedList){
    
    Error error = {0, ""};
    string rawInput;
    Request req;
    
    while (rawInput != "0")
    {
        
        if (Request *request = queueList->pop(&error, &req))
        {
            removedList->push_back(request);
            printf("Successfuly removed first element. 🍻\n");
        }
        else
            printf("Error: %s", error.message.c_str());

        if ( queueList->bottom != NULL)
        {
            printf("\n\tCurrent queue: ");
            Request *iReq = queueList->bottom;
            while (iReq != queueList->top)
            {
                printf("%d ", iReq->value);
                iReq = iReq->next;
            }
            printf("%d ", iReq->value);
        }
        
        if ( removedList->size() != 0 )
        {
            printf("\n\n\tFree memory addresses: ");
            for (size_t i = removedList->size(); i > 0; i--)
                printf("%p ", (struct request *)(*removedList)[i-1]);
        }
        
        
        cout<<"\n\t❱";
        getline(cin, rawInput);
        
    }
    
    showContinue();
    
}

#pragma mark Interface functions
void showMenu(){
    
    cout<<"Input action: "<<endl;
    cout<<"0 - Exit"<<endl;
    cout<<"1 - Push (array)"<<endl;
    cout<<"2 - Pop  (array)"<<endl;
    cout<<"3 - Simulate (array)"<<endl;
    cout<<"4 - Push (list)"<<endl;
    cout<<"5 - Pop  (list)"<<endl;
    cout<<"6 - Simulate (list)"<<endl;
    cout<<"7 - Analyze"<<endl;
}
void showContinue(){
    
    cout<<"\nPress any key to continue. 🌝\n";
    getchar();
}
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
        case 5:
            printf("\nToo big number. The max is %i.\n", MAX_NUMBER_SIZE);
            break;
        default:
            printf("\nUnexpected error with code %d. 💀\n", error);
            break;
    }
    
}

#pragma mark Handling
void handleAction(int action, QueueArray * QueueArray, QueueList * QueueList, RequestAddressVector *removedList){
    
    switch (action) {
        case 0:
            break;
        case 1:
            pushQueueArray(QueueArray);
            break;
        case 2:
            popQueueArray(QueueArray);
            break;
        case 3:
        case 6:
            SimulateQueueArray();
            break;
        case 4:
            pushQueueList(QueueList, removedList);
            break;
        case 5:
            popQueueList(QueueList, removedList);
            break;
        case 10:
            SimulateQueueList();
            break;
        case 7:
            AnalyzeQueue();
            break;
        default:
            cout<<"Sorry, this functionality is unavialible at this moment :(\n";
            showContinue();
            break;
    }
}
bool isNumber(const string& s){
    string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}