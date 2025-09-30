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

// класс для хранения информации о задачи (тотальное время её выполнения,
// имя события, на какую дату оно назначенно
class Task
{
    private:
        // хранение времени (например, 20 или 18) начала и следующей датой конец
        unsigned start_end_month[2];
        unsigned start_end_day[2];
        unsigned start_end_hour[2];
        unsigned start_end_minute[2];
        unsigned start_end_seconds[2];
        unsigned all_time_work_minutes;

        double totalTime; // сколько времени (в минутах) прошло на выполнение
        std::string name_task;
        unsigned day_task, month_task, year_task_YYYY; // дата на какой день запланированная заметка
    public:

        Task(std::string name_task = "None name", unsigned day_task = 0, unsigned month_task = 12, unsigned year_task_YYYY = 0)
        {
            // присваиваиваем знаения полям
            this -> name_task = name_task;
            this -> day_task = day_task;
            this -> month_task = month_task;
            this -> year_task_YYYY = year_task_YYYY;
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
            start_end_minute[1] = local_time -> tm_min;
            start_end_seconds[1] = local_time -> tm_sec;

            unsigned fewMonth = start_end_month[1] - start_end_month[0];
            unsigned fewDay = start_end_day[1] - start_end_day[0];
            unsigned fewHour = start_end_hour[1] - start_end_hour[0];
            unsigned fewMinutes = start_end_minute[1] - start_end_minute[0];
            unsigned fewSeconds = start_end_seconds[1] - start_end_seconds[0];

            // добавляем тотальное время
            this -> totalTime += (fewSeconds / 60); // добавляем секунды
            this -> totalTime += fewMinutes; // добавляем минуты
            this -> totalTime += (fewHour * 60); // добаввляем часы
            this -> totalTime += (fewDay * 1440); // добавляем дни 
            this -> totalTime += (fewMonth * 43200); // добавляем месяца

            cout << "Всего прошло минут: " << this -> totalTime << endl;
        }

        // Функция для добавления N дней к дате назначения заметки (на какой день она заплонированна)
        void changeDateToNNext()
        {
            // сделать
        }

        void printInfoTask()
        {
            cout << "👤\tИмя задачи: " << name_task << endl;
            cout << "🗓️\tЗапланированная дата, в которую вы хотите выполнить эту задачу: " << day_task << "/" << month_task << year_task_YYYY << endl;
            cout << "✅\tВы уже выполняете эту задачу: " << totalTime << " минут" << endl;
        }
};

int main()
{
    Task test_task("Тестовое", 1, 10, 2025);
    std::string test {};

    test_task.startTime();
    std::cin >> test;
    test_task.endTime();

    test_task.printInfoTask();
}