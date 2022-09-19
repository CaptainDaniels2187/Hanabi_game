#pragma once
#include "Hanabi.h"

namespace Hanabi {
	class Player0403 : public PlayerAbstract {
		// Реализуйте необходимые методы абстрактного класса
		// Доступные типы и функции объявлены в заголовочном файле Hanabi.h

		View playerView;
		Log LOG;
		Id m_id;
		Index m_myHandSize;
		Pile hands[PLAYERS_COUNT];
		Mask ColoredPileMask[PLAYERS_COUNT][COLORS_COUNT];
		Mask NumericalPileMask[PLAYERS_COUNT][NUMBERS_COUNT];

		//Формирование карт в руке исходя из подсказок
		void ConstructHand();

		//Проверка условий игры для одного из ходов
		bool CanWePlay();
		bool CanWePrompt();

		//Реализация ходов
		Action Play();
		Action Prompt();
		Action Discard();
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
