#pragma once
#include "Hanabi.h"

namespace Hanabi {

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

		//Реализация ходов
		Action Play(Pile* hands);
		Action Prompt(Pile* hands);
		Action Discard(Pile* hands);
	public:
		//Конструктор объекта игрока с возможностью логирования для инициализации видимого стола для игрока и первоначальных данных о его руке
		Player0403(View view, Log& log);

		//Перегрузка деструктора для корректного очищения полей класса Player0403
		~Player0403();

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
