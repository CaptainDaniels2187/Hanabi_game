#pragma once
#include "Hanabi.h"

namespace Hanabi {

	constexpr double dP = 0.001;

	constexpr double Half = 0.5;

	using StochasticMask = std::vector<double>;

	using AllCards = size_t[COLORS_COUNT][NUMBERS_COUNT];

	class Player0403 : public PlayerAbstract {
		// Реализуйте необходимые методы абстрактного класса
		// Доступные типы и функции объявлены в заголовочном файле Hanabi.h

		View playerView;
		Log LOG;
		Id m_id;
		size_t m_myHandSize;
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
		double pColorPlay(Id id, int col, Index index);
		double pNumPlay(Id id, int num, Index index);
		double pRandPlay(Id id, int none, Index index);
		double pColorVal(Id id, int col, Index index);
		double pNumVal(Id id, int val, Index index);
		double pRandVal(Id id, int none, Index index);

		//Вспомогательный метод для подсчета вероятности (частично)неизвестной карты
		void pUnknownCards(Index index, StochasticMask& pMask, double(Player0403::* func1)(Id, int, Index), double(Player0403::* func2)(Id, int, Index),
			double(Player0403::* func3)(Id, int, Index));

		//Корректировка масок после разыгровки/сброса карты
		void CorrectMasks(Id id, Index index);

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

		//Обработка максок другого игрока во время игры/сброса его карты
		void beNotified(Id playerId, Action::Discard discard);
		void beNotified(Id playerId, Action::Play play);
	};
}
