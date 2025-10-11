#include <iostream>
#include <cstdlib>
#include <string>
#include "../headers/menu_class.h"
#include <sys/ioctl.h>
#include <unistd.h>

using std::string;
using std::cout;
using std::endl; 

// Данный класс нужен для создания менюшек
// В объект класса нужно передавать: 1. Массив с текстом пунктов (максимум 16 пунктов) 2. Количесвто тукстов пунктов

// Также если необходимо, вызывать какие-то функции по выбору пункта с помощью функции класса Menu select_point(), то необходимо передать функцию в переменную объекта functions по индексу, который на 1 меньше чем номер пункта
	
Menu::Menu(std::string points[], unsigned count_point)
{
	// system("clear"); // очищаем консоль

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

void Menu::show() const // Функция для отображения всех пунктов и их номеров
{
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);  
	unsigned color {31};
	cout << endl;

	for (unsigned i {}; this -> points[i] != "Null"; i++)
	{
		if (color >= 37)
		{
			color = 31;
		} else
		{
			color++;
		}

		// Делаем отступы от левого края меню
		for (unsigned i {}; i < (w.ws_col / 35); i++)
		{
			cout << " ";
		}

		// Для изменения цветов текста
		cout << "\033[1;" << color << "m" << (i + 1) << "\033[0m" << ". " << points[i] << endl;
	
	}

	// Сбрасываем счётчик цветов для пунктов меню
	color = 31;

	// Отступ от низа меню
	for (unsigned i {}; i < (w.ws_row / 3); i++)
	{
		cout << endl;
	}
}

void Menu::select_point() // Функция, которая выводит поле для выбора пункта меню и потом вызывает функцию, которая соответсвет пункту меню
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
		cout << "Вы ввели неправильный пункт меню." << endl;
		cout << endl;

		show();
		select_point(); // Вызываем выбор по новому
	}

	current_select_point = 0; // Обнуляем выбор
};

// void test_hello(Menu &object)
// {
// 	cout << "Hello" << endl;
// };

// int main()
// {
// 	string point[8] {"Привет", "как дела", "зачем"};
// 	Menu test_menu {point, 3};
// 	test_menu.functions[0] = reinterpret_cast<void*>(test_hello);

// 	test_menu.show();
// 	test_menu.select_point();
// }
