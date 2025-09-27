#include <iostream>
#include <cstdlib>
#include <string>

using std::string;
using std::cout;
using std::endl; 

// Данный класс нужен для создания менюшек
// В объект класса нужно передавать: 1. Массив с текстом пунктов (максимум 16 пунктов) 2. Количесвто тукстов пунктов

// Также если необходимо, вызывать какие-то функции по выбору пункта с помощью функции класса Menu select_point(), то необходимо передать функцию в переменную объекта functions по индексу, который на 1 меньше чем номер пункта

class Menu
{
	private:
		const static inline unsigned count_point {16}; // Максимальное количество пунктов меню
	
		std::string points[count_point] {}; // Текста пунктов меню

		int current_select_point {}; // Текущий выбранный пункт (нужен для функции select_point())
	public:
		void (*functions[count_point]) {};
	
		Menu(std::string points[], unsigned count_point)
		{
			system("clear"); // очищаем консоль

			// Заполняем все поля с пунктами
			for (unsigned i {}; i < this -> count_point; i++)
			{
				this -> points[i] = "Null";
			}

			// Заполняем все поля, переданными текстами
			for (unsigned i {}; i < count_point; i++)
			{
				this -> points[i] = points[i];
			}
		}

		void show() const // Функция для отображения всех пунктов и их номеров
		{
			for (unsigned i {}; this -> points[i] != "Null"; i++)
			{
				cout << (i + 1) << ". " << points[i] << endl;
			}
		}

		void select_point() // Функция, которая выводит поле для выбора пункта меню и потом вызывает функцию, которая соответсвет пункту меню
		{
			cout << endl;

			cout << "Выберите пункт: ";
			std::cin >> current_select_point;

			// проверка на верность пункта
			if (current_select_point != 0 && current_select_point > 0 && current_select_point < count_point)
			{
				(reinterpret_cast<void(*)()>(functions[current_select_point - 1]))(); // Вызываем функцию
			} else
			{
				system("clear");
				show();

				select_point(); // Вызываем выбор по новому
			}

			current_select_point = 0; // Обнуляем выбор
		};
};

void test_hello(Menu &object)
{
	cout << "Hello" << endl;
};

int main()
{
	string point[8] {"Привет", "как дела", "зачем"};
	Menu test_menu {point, 3};
	test_menu.functions[0] = reinterpret_cast<void*>(test_hello);

	test_menu.show();
	test_menu.select_point();
}
