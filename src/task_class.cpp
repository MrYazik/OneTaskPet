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
        std::string name_task; // имя задачи
        bool isDone {false}; // выполненна задача или нет
        unsigned day_task, month_task, year_task_YYYY; // дата на какой день запланированная заметка

        std::atomic<unsigned> workSeconds {};
        std::atomic<unsigned> workMinutes {};

        std::atomic<unsigned> totalPausedSeconds {};
        std::atomic<unsigned> totalPausedMinutes {};

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
        std::string pause_and_stop_menu_point[3] {
            "\tПоставить на паузу выполнение",
            "\tВыйти из задачи и завершить задачу",
            "\tВыйти из задачи и не завершать задачу"
        };
        Menu pause_and_stop_menu_timer {pause_and_stop_menu_point, static_cast<unsigned>(std::size(pause_and_stop_menu_point))};

        // Меню для таймера в паузе
        std::string pause_points[2] {
            "\tПродолжить выполнение задачи",
            "\tПометить задачу как выполненную"
        };
        Menu pause_timer {pause_points, static_cast<unsigned>(std::size(pause_points))};

        static int time(Task* object)
        {
            while(true)
            {
                // Вывод информации о текущей сессии
                system("clear");
                cout << "Имя задачи: " << object->name_task << endl;
                cout << "Вы работаете уже: " << object -> workMinutes << " минут " << object -> workSeconds << " секунд " << endl;
                
                object->pause_and_stop_menu_timer.show();

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
                    cout << "Суммарно вы на паузе уже: " << object->totalPausedMinutes << " минут " << object->totalPausedSeconds << " секунд" << endl;
                    cout << "Вы на паузе уже: " << object->pausedMinutes << " минут " << object->pausedSeconds << " секунд" << endl;
                    
                    object -> pause_timer.show();

                    std::this_thread::sleep_for(std::chrono::seconds(1));

                    if (object-> pausedSeconds >= 59)
                    {
                        // Глобальное время паузы
                        object -> totalPausedMinutes.fetch_add(1);
                        object -> totalPausedSeconds.exchange(0);

                        // Локальное время на текущей сесии паузы
                        object -> pausedMinutes.fetch_add(1);
                        object -> pausedSeconds.exchange(0);
                    } else
                    {
                        // Глобальное время паузы
                        object -> totalPausedSeconds.fetch_add(1);

                        // Локальное время на текущей сесии паузы
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
            std::string select_point {};
            bool stop_request_input {false};

            std::thread time_th (time, this);
            time_th.detach(); // Запускем таймер

            // Выбор функции в менюшках
            while (true)
            {
                while (stop_request_input == false)
                {
                    // Выбор пункта
                    std::getline(std::cin, select_point);

                    if (select_point == "1") {
                        pause_work_func();
                        stop_request_input = true;
                    }
                    else if (select_point == "2") {
                        work_isdone();
                        stop_request_input = true;
                    }
                    else if (select_point == "3") {
                        unpause_work_func();
                        stop_request_input = true;
                    }
                    else {
                        cout << "Вы ввели неверный пункт меню." << endl;
                    }
                }

                stop_request_input = false;

                // Выбор функции в меню паузы
                while (pause_work == true)
                {
                    // Выбор пункта
                    std::getline(std::cin, select_point);

                    if (select_point == "1")
                    {
                        unpause_work_func();
                    } else if (select_point == "2")
                    {
                        work_isdone();
                    } else
                    {
                        cout << "Вы ввели неправильный пункт меню." << endl;
                    }
                }

                pause_work.exchange(false);

            }
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