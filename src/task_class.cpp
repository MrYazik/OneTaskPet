#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <thread>
#include <atomic>
#include <chrono>

#include "../headers/menu_class.h"

using std::cout;
using std::endl;

// класс для хранения информации о задаче (тотальное время её выполнения,
// имя события, на какую дату оно назначенно)

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

        unsigned totalTime; // сколько времени (в минутах) прошло на выполнение
        std::string name_task;
        bool isDone {false}; // выполненна задача или нет

        unsigned day_task, month_task, year_task_YYYY; // дата на какой день запланированная заметка

        std::atomic<unsigned> workSeconds {};
        std::atomic<unsigned> workMinutes {};

        std::atomic<unsigned> pausedSeconds {};
        std::atomic<unsigned> pausedMinutes {};

        // Bool
        std::atomic<bool> stop_work {false};
        std::atomic<bool> pause_work {false};

        // Функции

        // Постановка на паузу таймера
        void pause_work_func()
        {
            pause_work.exchange(true); // меняем значение на true
        }
        void unpause_work_func()
        {
            pause_work.exchange(false); // меняем значение на false
        }
        void work_isdone()
        {
            isDone = true;
        }


        // Меню для работающего таймера
        const static unsigned count_point_pause_and_stop_menu {3};
        std::string pause_and_stop_menu_point[count_point_pause_and_stop_menu] {
            "\tПоставить на паузу выполнение",
            "\tВыйти из задачи и завершить задачу",
            "\tВыйти из задачи и не завершать задачу"
        };
        Menu pause_and_stop_menu_timer {pause_and_stop_menu_point, count_point_pause_and_stop_menu};

        // Меню для таймера в паузе
        const static unsigned count_pause_menu_point {2};
        std::string pause_points[count_pause_menu_point] {
            "\tПродолжить выполнение задачи",
            "\tПометить задачу как выполненную"
        };
        Menu pause_timer {pause_points, count_pause_menu_point};

        static int time(Task* object)
        {
            while(true)
            {
                // Вывод информации о текущей сессии
                system("clear");
                cout << "\tИмя задачи: " << object->name_task << endl;
                cout << "\tВы работаете уже: " << object -> workMinutes << " минут " << object -> workSeconds << " секунд " << endl;
                
                // object -> pause_timer.show();

                // Прибавление к переменной 1 каждую секунду
                std::this_thread::sleep_for(std::chrono::seconds(1));

                // остановка работы
                if (object->stop_work == true)
                {
                    return 0;
                    object->endTime();
                }

                // если науступила пауза
                while (object -> pause_work == true)
                {
                    system("clear");
                    cout << "Вы на паузе уже: " << object->pausedMinutes << " минут " << object->pausedSeconds << " секунд" << endl;
                    
                    //object -> pause_timer.show();

                    std::this_thread::sleep_for(std::chrono::seconds(1));

                    if (object->workSeconds >= 59)
                    {
                        object -> pausedMinutes.fetch_add(1);
                        object -> pausedSeconds.exchange(0);
                    } else
                    {
                        object -> pausedSeconds.fetch_add(1);
                    }
                }

                if (object->workSeconds >= 59)
                {
                    object -> workMinutes.fetch_add(1);
                    object -> workSeconds.exchange(0);
                } else
                {
                    object -> workSeconds.fetch_add(1);
                }
            }

            return 0;
        }
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
            std::thread time_th (time, this);
            time_th.detach();
            std::string noneText {"test"};

            std::getline(std::cin, noneText); // ожидаем пока пользователь нажмёт Enter
            stop_work = true;
        }
        void endTime()
        {
            stop_work = false;
            cout << "Работа таймера приостановленна" << endl;
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
            cout << "✅\tВы уже выполняете эту задачу: " << workMinutes << " минут " << workSeconds << " секунд " << endl;
        }
};

int main()
{
    Task test_task("Тестовое", 1, 10, 2025);
    std::string test {};

    test_task.startTime();
    std::getline(std::cin, test);

    test_task.printInfoTask();
}