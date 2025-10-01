#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <thread>
#include <atomic>
#include <chrono>

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
        unsigned day_task, month_task, year_task_YYYY; // дата на какой день запланированная заметка

        std::atomic<unsigned> workSeconds {};
        std::atomic<unsigned> workMinutes {};

        std::atomic<bool> pause_work {false};
    public:
        Task(std::string name_task = "None name", unsigned day_task = 0, unsigned month_task = 12, unsigned year_task_YYYY = 0)
        {
            // присваиваиваем знаения полям
            this -> name_task = name_task;
            this -> day_task = day_task;
            this -> month_task = month_task;
            this -> year_task_YYYY = year_task_YYYY;
        }

        static int time(Task* object)
        {
            while(true)
            {
                // Вывод информации о текущей сессии
                system("clear");
                cout << "Имя задачи: " << object->name_task << endl;
                cout << "Вы работаете уже: " << object -> workMinutes << " минут " << object -> workSeconds << " секунд " << endl;
                cout << endl;
                cout << "Нажмите Enter чтоб приостановить" << endl;

                std::this_thread::sleep_for(std::chrono::seconds(1));

                // остановка работы
                if (object->pause_work == true)
                {
                    return 0;
                    object->endTime();
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

        void startTime()
        {
            std::thread time_th (time, this);
            time_th.detach();
            std::string noneText {"test"};

            std::getline(std::cin, noneText); // ожидаем пока пользователь нажмёт Enter
            pause_work = true;
        }
        void endTime()
        {
            pause_work = false;
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