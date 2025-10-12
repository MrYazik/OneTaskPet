#include <iostream>
#include <string>
#include "headers/menu_class.h"
#include <cstdlib>

using std::cout;
using std::endl;

int main()
{
    system("clear"); // очищаем консоль

    std::string points_menu[] {
        "📅\tУстановка задач на текущий день", 
        "🗓️\tУстановка задач на последующие дни",
        "⬇️\tЗадачи на текущий день",
        "↪️\tЗадачи на последующие дни",
        "✅\tВыполнение задач на текущий день",
        "🐶\tПитомец",
        "⚠️\tНастройка" 
    };
	Menu main_menu {points_menu, std::size(points_menu)};

    main_menu.show();
    main_menu.select_point();
}
