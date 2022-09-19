#pragma once
#include "Hanabi.h"

namespace Hanabi {

	constexpr double dP = 0.001;

	constexpr double Half = 0.5;

	using StochasticMask = std::vector<double>;

	using AllCards = size_t(*)[NUMBERS_COUNT];

	class Player0403 : public PlayerAbstract {
		// Реализуйте необходимые методы абстрактного класса
		// Доступные типы и функции объявлены в заголовочном файле Hanabi.h

		View playerView;
		Log LOG;
		Id m_id;
		Index m_myHandSize;
		Mask ColoredPileMask[PLAYERS_COUNT][COLORS_COUNT];
		Mask NumericalPileMask[PLAYERS_COUNT][NUMBERS_COUNT];

		void InitAllOtherCards(AllCards& allCards);

		//Формирование карт в руке исходя из подсказок
		void ConstructHand(Pile& hand);
		void CalculateOtherCards(Pile* hands, AllCards& allCards);

		//Проверка условий игры для одного из ходов
		bool CanWePlay(Pile& hand);
		bool CanWePrompt();

		//Вычисление вероятностей ходов
		double pColorPlay(int id, int col, int index);
		double pNumPlay(int id, int num, int index);
		double pRandPlay(int id, int none, int index);
		double pColorVal(int id, int col, int index);
		double pNumVal(int id, int val, int index);
		double pRandVal(int id, int none, int index);

		//Вспомогательный метод для подсчета вероятности (частично)неизвестной карты
		void pUnknownCards(int index, StochasticMask& pMask, double(Player0403::* func1)(int, int, int), double(Player0403::* func2)(int, int, int),
			double(Player0403::* func3)(int, int, int));

		//Реализация ходов
		Action Play(Pile* hands);
		Action Prompt(Pile* hands);
		Action Discard(Pile* hands);
	public:
		//Конструктор объекта игрока с возможностью логирования для инициализации видимого стола для игрока и первоначальных данных о его руке
		Player0403(View view, Log& log);

		//Реализация метода выбора правильного действия игроком
		Action decide();

		//Вывод имени игрока
		const char* name() const;
		
		//Обработка получения подсказки типа "Цвет" от игрока fromPlayerId
		void receive(Id fromPlayerId, Color color, Mask mask);

		//Обработка получения подсказки типа "Число" от игрока fromPlayerId
		void receive(Id fromPlayerId, Number number, Mask mask);
	};
}
