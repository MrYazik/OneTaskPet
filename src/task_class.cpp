#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <thread>
#include <atomic>
#include <chrono>
#include <fstream>

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
        unsigned day_task {0}, month_task {0}, year_task_YYYY {0}; // дата на какой день запланированная заметка

        std::atomic<unsigned> totalWorkSeconds {};
        std::atomic<unsigned> totalWorkMinutes {};

        std::atomic<unsigned> workSeconds {};
        std::atomic<unsigned> workMinutes {};

        bool stopWorkNotDown {false};
        bool stopWork {false}; // Завершение выполнение функций класса

        std::atomic<unsigned> totalPausedSeconds {};
        std::atomic<unsigned> totalPausedMinutes {};

        std::atomic<unsigned> pausedSeconds {};
        std::atomic<unsigned> pausedMinutes {};

        // Bool
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
        // Выход с отмечением задачи как выполненной
        void work_isdone()
        {
            isDone = true;
            stopWork = true;
        }

        // Выход из задачи, не отмечая, что её выполнил
        void exit_work_not_down()
        {
            stopWorkNotDown = true;
        };

        // Меню для работающего таймера
        std::string pause_and_stop_menu_point[3] {
            "\tПоставить на паузу выполнение",
            "\tВыйти из задачи и завершить задачу",
            "\tВыйти из задачи и не завершать задачу",
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
                cout << "В текущей сессии вы работаете уже: " << object -> workMinutes << " минут " << object -> workSeconds << " секунд " << endl;
                cout << "Сумарно вы работаете уже: " << object -> totalWorkMinutes << " минут " << object -> totalWorkSeconds << " секунд " << endl;

                object->pause_and_stop_menu_timer.show();

                // Прибавление к переменной 1 каждую секунду
                std::this_thread::sleep_for(std::chrono::seconds(1));

                // если науступила пауза
                while (object -> pause_work == true)
                {
                    system("clear");
                    cout << "На текущей паузе вы уже: " << object->pausedMinutes << " минут " << object->pausedSeconds << " секунд" << endl;
                    cout << "Суммарно вы на паузе уже: " << object->totalPausedMinutes << " минут " << object->totalPausedSeconds << " секунд" << endl;
                    
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

                    // Обнуляем локальный таймер таймера работы 
                    object -> workMinutes.exchange(0);
                    object -> workSeconds.exchange(0);
                }

                // Обнуление локального таймера паузы
                object -> pausedMinutes.exchange(0);
                object -> pausedSeconds.exchange(0);

                if (object->workSeconds >= 59)
                {
                    object -> totalWorkMinutes.fetch_add(1);
                    object -> totalWorkSeconds.exchange(0);

                    object -> workMinutes.fetch_add(1);
                    object -> workSeconds.exchange(0);
                } else
                {
                    object -> totalWorkSeconds.fetch_add(1);

                    object -> workSeconds.fetch_add(1);
                }

                // Если пользователь завершил работу с отмеченным выполненным 
                if (object -> isDone == true)
                {

                    return 0;
                }

                // Если пользователь завершил работу без отмечения выполненным
                if (object -> stopWorkNotDown == true)
                {
                    return 0;
                }
            }

            return 0;
        }
    public:
        Task() 
        {

        }

        void startTime()
        {
            std::string select_point {};
            bool stop_request_input {false};

            std::thread time_th (time, this);
            time_th.detach(); // Запускем таймер

            // Выбор функции в менюшках
            while (stopWork == false)
            {
                while (stop_request_input == false && stopWork == false)
                {
                    // Выбор пункта
                    std::getline(std::cin, select_point);

                    if (select_point == "1") {
                        pause_work_func();
                        stop_request_input = true; // Выход из текузей менюшки
                    }
                    else if (select_point == "2") {
                        work_isdone();
                        stopWork = true;
                    }
                    else if (select_point == "3") {
                        exit_work_not_down();
                        stopWork = true;
                    }
                    else {
                        cout << "Вы ввели неверный пункт меню." << endl;
                    }
                }

                stop_request_input = false;

                // Выбор функции в меню паузы
                while (pause_work == true && stopWork == false)
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

        // Функция для добавления N дней к дате назначения заметки (на какой день она заплонированна)
        void changeDateToNNext()
        {
            // сделать
        }

        void printInfoTask()
        {
            cout << "👤\tИмя задачи: " << name_task << endl;
            cout << "🗓️\tЗапланированная дата, в которую вы хотите выполнить эту задачу: " << day_task << "/" << month_task << "/" << year_task_YYYY << endl;
            cout << "✅\tВы уже выполняете эту задачу: " << totalWorkMinutes << " минут " << totalWorkSeconds << " секунд " << endl;
            cout << "📳\tОтдыхаете вы: " << totalPausedMinutes << " минут " << totalPausedSeconds << " секунд" << endl;
        }

        void installTasks()
        {
            bool stop_current_input {false}; // Остановка текущего запроса
            std::string yesOrNo {""}; // Ввод подверждения
            bool error {false}; // Ошибка, например: неправильно введено имя задачи
            std::string errorText {""}; // Текст, который ввёл пользователь и который вывел ошибку

            // меню месяцев
            std::string point_select_month[12] {
                "Январь",
                "Февраль",
                "Март",
                "Апрель",
                "Май",
                "Июнь",
                "Июль",
                "Август",
                "Сентябрь",
                "Октябрь",
                "Ноябрь",
                "Декабрь"
            };

            Menu select_month {point_select_month, std::size(point_select_month)};

            // Формат даты
            std::time_t current_time = std::time(nullptr);


            // Называем задачу
            while (stop_current_input == false)
            {
                // Очищаем консоль
                system("clear");

                bool confirmName {false};

                cout << "🖊️  Назовите задачу: ";
                std::getline(std::cin, name_task);

                system("clear");

                cout << "👤  Название задачи: " << name_task << "\tПодвердить: (Y/n): ";
                std::getline(std::cin, yesOrNo);

                bool yes {
                    yesOrNo == "Y" ||
                    yesOrNo == "y" ||
                    yesOrNo == "Д" ||
                    yesOrNo == "д"
                };

                cout << endl;

                if (yes)
                {
                    system("clear");

                    cout << "👤 Ваше имя задачи: " <<
                    name_task << endl;

                    stop_current_input = true; // Завершаем выполнение текущего while
                }
            }

            stop_current_input = false;

            // Задаём день
            while (stop_current_input == false)
            {
                system("clear");

                // Проверка на ошибку в предыдущем цикле
                if (error == true)
                {
                    cout << """\033[31mВы ввели неправильный день месяца\033[0m: """ << errorText << """\
                        Введите день от 1 до 31""" << endl;
                    error = false;
                }

                std::string dayMonth {};
                cout << "Введите день месяца, в который вы хотите выполнить задачу (например, 01): ";
                std::getline(std::cin, dayMonth);

                try
                {
                    std::tm* local_time = std::localtime(&current_time);

                    if (std::stoi(dayMonth) <= 31 && std::stoi(dayMonth) > 0)
                    {
                        day_task = std::stoi(dayMonth);
                        stop_current_input = true;
                    } else
                    {
                        error = true;
                        errorText = dayMonth;
                    }
                } catch (...)
                {
                    error = true;
                    errorText = dayMonth;
                }
            }

            stop_current_input = false;
            
            // Задаём месяц
            while (stop_current_input == false)
            {
                system("clear");

                if (error == true)
                {
                    cout << "\033[31mВы ввели неправильный номер месяца: \033[0m  " << errorText << 
                        "  Введите номер месяца от 1 до 12: " << endl;
                    error = false;
                }

                std::string select_month_input {""};

                select_month.show();

                cout << "Введите месяц в который вы хотите выполнить эту задачу: ";
                std::getline(std::cin, select_month_input);
            
                try 
                {
                    if (std::stoi(select_month_input) > 0 && std::stoi(select_month_input) <= 12)
                    {
                        month_task = std::stoi(select_month_input);
                        stop_current_input = true;
                    } else
                    {
                        error = true;
                        errorText = select_month_input;
                    }
                } catch (...)
                {
                    error = true;
                    errorText = select_month_input;
                }
            }

            stop_current_input = false;

            // Выбор года
            while (stop_current_input == false)
            {
                system("clear");

                if (error == true)
                {
                    cout << """\033[31mВы ввели неправильный год\033[0m: """ << errorText << """\
                        Введите день от 1 до 31""" << endl;
                    error = false;
                }

                std::string year_input;
                std::tm* local_time = std::localtime(&current_time);

                cout << "Введите год в который вы планируете выполнить эту задачу: ";
                std::getline(std::cin, year_input);

                try {
                    if (std::stoi(year_input) >= local_time->tm_year + 1900)
                    {
                        year_task_YYYY = std::stoi(year_input);
                        stop_current_input = true;
                    } else
                    {
                        error = true;
                        errorText = year_input;
                    }
                } catch (...)
                {
                    error = true;
                    errorText = year_input;
                }
            }

            stop_current_input = false;

            // Добавление задачи в файл
            while (stop_current_input == false)
            {
                try {
                    std::ofstream task_file {"tasks.json", std::ios::app};

                    // Получение текущего времени
                    

                    task_file << "task: " << endl;
                    task_file << "\t name: " << name_task << endl;
                    task_file << "\t completed: " << isDone << endl;
                    task_file << "\t date create: " << endl;
                } catch (...)
                {
                    std::string getExit {};

                    cout << "Файл для работы с задачами открыт в какой-то другой программе, закройте его и нажмите любую клавишу и Enter" << endl;
                    std::getline(std::cin, getExit);
                }
            }
        }
};

int main()
{
    Task test_task;
    std::string test {};

    test_task.installTasks();
}