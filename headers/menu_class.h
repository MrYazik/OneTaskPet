#include <string>

class Menu
{
	private:
		const static inline unsigned count_point {16}; // Максимальное количество пунктов меню
		std::string points[count_point] {}; // Текста пунктов меню
		int current_select_point {}; // Текущий выбранный пункт (нужен для функции select_point())
	public:
		void (*functions[count_point]) {};
		Menu(std::string points[], unsigned count_point);

		void show() const;
		void select_point();
};