#include <iostream>
#include <string>
#include "headers/menu_class.h"


using std::cout;
using std::endl;

int main()
{
    std::string points_menu[] {
        "📅\tУстановка задач на текущий день", 
        "🗓️\tУстановка задач на последующие дни",
        "🐶\tПитомец",
        "⚠️\tНастройка" 
    };
	Menu main_menu {points_menu, std::size(points_menu)};

    main_menu.show();
    main_menu.select_point();
}
