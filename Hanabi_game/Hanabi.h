#pragma once
#include <vector>
#include <ostream>

namespace Hanabi {
	// constexpr требует C++11.

	// количество игроков
	constexpr int PLAYERS_COUNT = 2;
	// количество номеров карт
	constexpr int NUMBERS_COUNT = 5;
	// количество цветов карт
	constexpr int COLORS_COUNT = 5;
	// количество карт в игре
	constexpr int CARDS_COUNT = 50;
	// количество карт в руке после первой раздачи
	constexpr int INITIAL_HAND_SIZE = 5;
	// количество жетонов подсказки в игре (синих жетонов)
	constexpr int MAX_PROMPT_TOKENS = 8;
	// количество жетонов ошибки в игре (красных жетонов)
	constexpr int MAX_MISTAKE_TOKENS = 3;
	// максимально возможный счет в игре
	constexpr int MAX_SCORE = NUMBERS_COUNT * COLORS_COUNT;
	// количество карт в колоде по номерам (CARDS_COUNT_BY_NUMBER[0] - количество карт с номером Number::One и т.д.
	constexpr int CARDS_COUNT_BY_NUMBER[] = { 3, 2, 2, 2, 1 };

	//Цвет карты
	enum class Color : char {
		White = 0,
		Red,
		Blue,
		Yellow,
		Green
	};

	// Номер карты
	enum class Number : char {
		One = 1,
		Two = 2,
		Three = 3,
		Four = 4,
		Five = 5
	};

	// Признак, о котором сообщают в подсказке: общий цвет или номер карт
	struct Attribute {
		// Возможный тип признака карты
		enum class Type : char {
			Color, // цвет
			Number // номер
		};

		/**
		 * @brief Конвертирующий (converting) конструктор из Color в Attribute.
		 *
		 * Пример:
		 *
		 * Attribute a = Color::White; // то же, что Attribute(Color::White)
		 *
		 * @param color цвет карты
		*/
		Attribute(Color color);
		/**
		 * @brief Конвертирующий (converting) конструктор из Number в Attribute.
		 *
		 * Пример:
		 *
		 * Attribute b = Number::One; // то же, что Attribute(Number::One)
		 *
		 * @param number номер карты
		*/
		Attribute(Number number);

		// сравнение признаков на равенство (по значению)
		bool operator==(const Attribute& other) const;
		bool operator!=(const Attribute& other) const;

		Type type;
		union {
			Color color;
			Number number;
		};
	}; // struct Attribute

	// Карта
	struct Card {
		Card() = default;
		/**
		 * @brief Карта
		 * @param color цвет
		 * @param number номер
		*/
		Card(Color color, Number number);
		
		// сравнение на равенство (по значению)
		bool operator==(const Card& other) const;
		bool operator!=(const Card& other) const;


		/**
		 * @brief имеет ли карта данный признак (цвет/номер)
		 * @param attribute признак карты (см. конструктор Attribute)
		*/
		bool satisfy(Attribute attribute) const;

		// цвет карты
		Color color;
		// номер карты
		Number number;
	}; // struct Card

	// Индекс карты в руке игрока
	using Index = unsigned short;

	// id игрока
	// возможные значения: 0 <= x < PLAYERS_COUNT
	using Id = unsigned short;

	// Действие
	struct Action {
		// Возможный тип действия
		enum class Type : char {
			Prompt, // подсказать карту/карты
			Discard, // cбросить карту
			Play // разыграть карту
		};

		// Подсказать карту/карты
		struct Prompt {
			/**
			 * @brief Действие: Подсказка другому игроку.
			 *
			 * Примеры:
			 *
			 * // подсказать игроку 0 позиции всех его синих карт:
			 * return Action::Prompt(0, Color::Blue); // см. конструктор Attribute
			 * // подсказать игроку 1 позиции всех его четверок:
			 * return Action::Prompt(1, Number::Four);
			 *
			 * @param receiverId id игрока, которому сообщается информация о его картах
			 * @param attribute общий цвет или номер некоторых карт
			*/
			explicit Prompt(Id receiverId, Attribute attribute);

			// сравнение на равенство (по значению)
			bool operator==(const Prompt& other) const;
			bool operator!=(const Prompt& other) const;

			Id receiverId; // id игрока, которому сообщается информация о его картах
			Attribute attribute; // общий цвет или номер некоторых карт
		};
		// Сбросить карту
		struct Discard {
			/**
			 * @param cardIndex индекс сбрасываемой карты в руке игрока
			*/
			explicit Discard(Index cardIndex);

			// сравнение на равенство (по значению)
			bool operator==(const Discard& other) const;
			bool operator!=(const Discard& other) const;

			Index cardIndex;
		};
		// Разыграть карту
		struct Play {
			/**
			 * @param cardIndex индекс разыгрываемой карты в руке игрока
			*/
			explicit Play(Index cardIndex);

			// сравнение на равенство (по значению)
			bool operator==(const Play& other) const;
			bool operator!=(const Play& other) const;

			Index cardIndex;
		};

		/**
		 * @brief Конвертирующий (converting) конструктор из Prompt в Action.
		 *
		 * Пример:
		 *
		 * Action a = Action::Prompt(0, Color::Blue); // подсказать нулевому игроку позиции всех синих карт
		 * // то же, что Action(Action::Prompt(0, Color::Blue))
		*/
		Action(Prompt prompt);
		/**
		 * @brief Конвертирующий (converting) конструктор из Discard в Action.
		 *
		 * Пример:
		 *
		 * Action b = Action::Discard(0); // сбросить нулевую карту
		*/
		Action(Discard discard);
		/**
		 * @brief Конвертирующий (converting) конструктор из Play в Action.
		 *
		 * Пример:
		 *
		 * Action с = Action::Play(0); // разыграть нулевую карту
		*/
		Action(Play play);

		// сравнение действий на равенство (по значению)
		bool operator==(const Action& other) const;
		bool operator!=(const Action& other) const;

		Type type;
		union {
			Prompt prompt;
			Discard discard;
			Play play;
		};
	}; // struct Action

	// Набор карт: Стопка сброса/колода/карты в руке
	using Pile = std::vector<Card>;
	// Маска, задает подмножество карт в руке
	using Mask = std::vector<bool>;

	struct BoardImplemenation; // forward declaration для Output::operator<<, View

	/**
	 * @brief Позволяет выводить сообщения в поток std::ostream (например, в std::cout)
	 *        в зависимости от уровня (см. Log::Level).
	 *
	 * //Выводить сообщения от уровня debug() и выше на консоль:
	 * Log LOG{ Log::Level::Debug, std:cout };
	 * LOG.debug() << "x=" << 42 << std::endl; // выведет: [Debug] x=42
	 *
	 * //Выводить сообщения от уровня info() и выше в файл out.txt:
	 * std::ofstream out{ "out.txt" }; // #include <fstream>
	 * Log LOG{ Log::Level::Info, out }; // инициализация
	 * LOG.debug() << "some debug info" << std::endl; // ничего не выведет
	 * LOG.info() << "y=" << 69 << std::endl; // выведет: [Info] y=69
	 *
	 * Замечание: это некопируемый тип (non-copyable)
	*/
	class Log {
	public:
		// Уровень логирования
		enum class Level {
			Trace = 0, // детальный вывод
			Debug = 1, // выводить отладочные сообщения
			Info = 2, // выводить только необходимые сообщения
			Silent = 0x7F, // не выводить ничего
		};
		/**
		 * @brief Конструктор
		 * @param level уровень логирования
		 * @param base конкретный поток, в который выводятся сообщения
		*/
		explicit Log(Level level, std::ostream& base);

		inline std::ostream& trace() {
			return m_trace << "[Trace] ";
		}
		inline std::ostream& debug() {
			return m_debug << "[Debug] ";
		}
		inline std::ostream& info() {
			return m_info << "[Info] ";
		}
	private:
		std::ostream& m_trace;
		std::ostream& m_debug;
		std::ostream& m_info;
	};

	// Видимая часть стола board для игрока с id=myId
	class View {
	public:
		View(const BoardImplemenation& board, Id myId);

		// id игрока, которому предоставили доступ к столу
		Id myId() const;
		/**
		 * @brief карты в руке игрока
		 *
		 * Игрок не должен видеть свои карты,
		 * поэтому вызов с playerId=myId() вернет пустой Pile,
		 * а в Debug-версии прекратит исполнение программы с возможностью отладки.
		 *
		 * @param playerId id игрока
		*/
		const Pile& hand(Id playerId) const;
		// количество карт в собственной руке (myId)
		Index myHandSize() const;
		// количество карт в колоде
		size_t deckSize() const;
		// стопка сброса
		const Pile& discardPile() const;
		// количество карт в цепочке фейерверка данного цвета (color)
		int firework(Color color) const;
		// можно ли успешно разыграть карту (card)
		bool canPlay(Card card) const;
		// количество жетонов подсказки (синих жетонов)
		int promptTokens() const;
		// количество жетонов ошибки (красных жетонов)
		int mistakeTokens() const;
		// игровой счет
		int score() const;
	private:
		const BoardImplemenation& m_board;
		const Id m_myId;
	}; // class View

	// Базовый абстрактный класс игрока
	class PlayerAbstract {
	public:
		// деструктор
		virtual ~PlayerAbstract() = default;

		/**
		 * @brief принять решение о ходе
		 *
		 * Нельзя подсказывать (Action::Prompt):
		 *  - самому себе (View::myId)
		 *  - если жетонов подсказки 0 (View::promptTokens)
		 * Нельзя сбрасывать карту (Action::Discard):
		 *  - если количество жетонов подсказки = MAX_PROMPT_TOKENS
		*/
		virtual Action decide() = 0;
		
		// имя игрока, используется при выводе
		virtual const char* name() const = 0;
		
		/**
		 * @brief получить подсказку (о цвете)
		 * @param fromPlayerId id игрока, который подсказывает
		 * @param color цвет карт
		 * @param mask о каких картах идет речь (размер маски равен количеству карт в руке,
		 *        if (mask[i]) - карта с индексом i в руке цвета color)
		*/
		virtual void receive(Id fromPlayerId, Color color, Mask mask) = 0;
		
		/**
		 * @brief получить подсказку (о номере)
		 * @param fromPlayerId id игрока, который подсказывает
		 * @param number номер карт
		 * @param mask о каких картах идет речь (размер маски равен количеству карт в руке,
		 *        if (mask[i]) - карта с индексом i в руке имеет номер number)
		*/
		virtual void receive(Id fromPlayerId, Number number, Mask mask) = 0;
		
		/**
		 * @brief получить уведомление о выборе другим игроком действия для хода (подсказки цвета)
		 *
		 * (необязательно переопределять, но можно)
		 *
		 * @param playerId id игрока
		 * @param promptReceiverId id получателя подсказки
		 * @param promptColor цвет в подсказке
		*/
		virtual void beNotified(Id playerId, Id promptReceiverId, Color promptColor) {};
		
		/**
		 * @brief получить уведомление о выборе другим игроком действия для хода (подсказки номера)
		 *
		 * (необязательно переопределять, но можно)
		 *
		 * @param playerId id игрока
		 * @param promptReceiverId id получателя подсказки
		 * @param promptNumber номер в подсказке
		*/
		virtual void beNotified(Id playerId, Id promptReceiverId, Number promptNumber) {};
		
		/**
		 * @brief получить уведомление о выборе другим игроком действия для хода (сброса карты)
		 *
		 * (необязательно переопределять, но можно)
		 *
		 * @param playerId id игрока
		 * @param discard действие
		*/
		virtual void beNotified(Id playerId, Action::Discard discard) {};
		
		/**
		 * @brief получить уведомление о выборе другим игроком действия для хода (розыгрыша карты)
		 *
		 * (необязательно переопределять, но можно)
		 *
		 * @param playerId id игрока
		 * @param play действие
		*/
		virtual void beNotified(Id playerId, Action::Play play) {};
	}; // class PlayerAbstract

	// Функция по указателю типа PlayerFactory должна создавать нового игрока с помощью вызова конкретного конструктора оператором new.
	// функция должна принимать один параметр типа View и возвращать указатель на PlayerAbstract
	//
	// Замечание: тип View - некопируемый, так как содержит ссылку. Определяя конструктор класса игрока,
	//            используйте список инициализации, чтобы сохранить переменную типа View
	using PlayerFactory = PlayerAbstract * (*)(View);

	/**
	 * @brief запустить игру
	 * @param factories игроки создаются вызовом "фабрик" PlayerFactory.
	 * @param gameIndex номер игры. От него зависит, кто ходит первым
	 * @param log позволяет выводить игры на экран/в файл, см. Log
	 * @return игровой счет в конце игры
	*/
	int game(const PlayerFactory(&factories)[PLAYERS_COUNT], int gameIndex, Log& log);

	// функции для вывода в поток для определенных здесь типов
	namespace Output {
		std::ostream& operator<<(std::ostream& os, const Mask& mask);
		std::ostream& operator<<(std::ostream& os, const Color& color);
		std::ostream& operator<<(std::ostream& os, const Number& number);
		std::ostream& operator<<(std::ostream& os, const Card& card);
		std::ostream& operator<<(std::ostream& os, const Attribute& attribute);
		std::ostream& operator<<(std::ostream& os, const Action& action);
	}
} // namespace Hanabi
