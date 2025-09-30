#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using std::cout;
using std::endl;

// // Класс для сохранения даты создания какого-то события, имя события. 
// class Date
// {
//     private:
//         std::string
//     public:
//         // день
//         unsigned getDateDay()
//         {

//         }

//         // месяц
//         unsigned getDateMonth()
//         {


//         }

//         // год
//         unsigned getDateYear()
//         {
            
//         }

//         std::string
// };

// класс для хранения времени (например, для отслеживания времени начала и конца)
class Time
{
    private:
        // хранение времени (например, 20 или 18) начала и следующей датой конец
        unsigned start_end_month[2];
        unsigned start_end_day[2];
        unsigned start_end_hour[2];
        unsigned start_end_minute[2];
        unsigned start_end_seconds[2];
        unsigned all_time_work_minutes;

        double fewMinutes; // сколько прошло минут на выполнение задачи
    public:
        Time()
        {
            
        }
        void startTime()
        {
            // Засекаем начальное время
            std::time_t current_time = std::time(nullptr);
            std::tm* local_time = std::localtime(&current_time);

            start_end_month[0] = local_time -> tm_mon;
            start_end_day[0] = local_time -> tm_mday;
            start_end_hour[0] = local_time -> tm_hour;
            start_end_minute[0] = local_time -> tm_min;
            start_end_seconds[0] = local_time -> tm_sec;

        }
        void endTime()
        {
            // Засекаем конец времени и добавляем его к общему времени на выполнение задачи
            std::time_t current_time = std::time(nullptr);
            std::tm* local_time = std::localtime(&current_time);

            start_end_month[1] = local_time -> tm_mon;
            start_end_day[1] = local_time -> tm_mday;
            start_end_hour[1] = local_time -> tm_hour;
            start_end_minute[1] = local_time -> tm_min + 21;
            start_end_seconds[1] = local_time -> tm_sec;

            unsigned fewMonth = start_end_month[1] - start_end_month[0];
            unsigned fewDay = start_end_day[1] - start_end_day[0];
            unsigned fewHour = start_end_hour[1] - start_end_hour[0];
            unsigned fewMinutes = start_end_minute[1] - start_end_minute[0];
            double fewSeconds = start_end_seconds[1] - start_end_seconds[0];

            // добавляем тотальное время
            this -> fewMinutes += (fewSeconds / 60); // добавляем секунды
            this -> fewMinutes += fewMinutes; // добавляем минуты
            this -> fewMinutes += (fewHour * 60); // добаввляем часы
            this -> fewMinutes += (fewDay * 1440); // добавляем дни 
            this -> fewMinutes += (fewMonth * 43200); // добавляем месяца

            cout << "Всего прошло минут: " << this -> fewMinutes << endl;
        }

};

int main()
{
    Time test_task;
    std::string test {};

    test_task.startTime();
    std::cin >> test;
    test_task.endTime();
}