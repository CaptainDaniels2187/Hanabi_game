#include "Player0403.h"
#include <algorithm>

using namespace Hanabi;

Player0403::Player0403(View view, Log& log) : playerView(view), LOG(log), m_id(playerView.myId()), m_myHandSize(0)
{
}

//Wrong realization!!!!!!!!!!!!!!!!!!!!!
const char* Player0403::name() const
{
	if (playerView.myId())
	{
		return "Danya";
	}
	else
	{
		return "Nastya";
	}
}

void Player0403::receive(Id fromPlayerId, Color color, Mask mask)
{
	ColoredPileMask[m_id][static_cast<int>(color)] = mask;
}

void Player0403::receive(Id fromPlayerId, Number number, Mask mask)
{
	NumericalPileMask[m_id][static_cast<int>(number) - 1] = mask;
}

void Player0403::ConstructHand(Pile& hand)
{
	for (int i = 0; i < COLORS_COUNT; ++i)
	{
		for (int j = 0; j < NUMBERS_COUNT; ++j)
		{
			int n = std::min(ColoredPileMask[m_id][i].size(), NumericalPileMask[m_id][j].size());
			for (int k = 0; k < n; ++k)
			{
				if (ColoredPileMask[m_id][i][k] == NumericalPileMask[m_id][j][k] && ColoredPileMask[m_id][i][k] == 1)
				{
					hand[k] = Card(static_cast<Color>(i), static_cast<Number>(j + 1));
				}
			}
		}
	}
}

void Player0403::InitAllOtherCards(AllCards& allCards)
{
	for (int i = 0; i < COLORS_COUNT; ++i)
	{
		for (int j = 0; j < NUMBERS_COUNT; ++j)
		{
			allCards[i][j] = CARDS_COUNT_BY_NUMBER[j];
		}
	}
}

void Player0403::CalculateOtherCards(Pile* hands, AllCards& allCards)
{
	//Вычиатем известные карты в руках у игроков
	for (int i = 0; i < PLAYERS_COUNT; ++i)
	{
		int n = hands[i].size();
		for (int j = 0; j < n; ++j)
		{
			if (hands[i][j] != Card())
			{
				int cardColor = static_cast<int>(hands[i][j].color);
				int cardNum = static_cast<int>(hands[i][j].number) - 1;
				allCards[cardColor][cardNum] -= 1;
			}
		}
	}

	//Вычитаем известные карты в стопке сброса
	Pile Trash = playerView.discardPile();
	for (auto const& card : Trash)
	{
		int cardColor = static_cast<int>(card.color);
		int cardNum = static_cast<int>(card.number) - 1;
		allCards[cardColor][cardNum] -= 1;
	}
	
	//Вычитаем известные карты на столе
	for (int i = 0; i < COLORS_COUNT; ++i)
	{
		int num = playerView.firework(static_cast<Color>(i));
		for (int j = 0; j < num; ++j)
		{
			allCards[i][j] -= 1;
		}
	}
}

bool Player0403::CanWePlay(Pile& hand)
{
	for (auto const& card : hand)
	{
		if (playerView.canPlay(card))
		{
			return true;
		}
	}
	return false;
}

bool Player0403::CanWePrompt()
{
	return playerView.promptTokens() > 0;
}

double Player0403::pColorPlay(int id, int col, int index)
{
	//TODO
	//Вычисление вероятности для игры по цвету
	return 0;
}

double Player0403::pNumPlay(int id, int num, int index)
{
	//TODO
	//Вычисление вероятности для игры по числу
	return 0;
}

double Player0403::pRandPlay(int id, int none, int index)
{
	//TODO
	//Вычисление вероятности для игры рандомной карты
	return 0;
}

double Player0403::pColorVal(int id, int col, int index)
{
	//TODO
	//Вычисление вероятности полезности по цвету
	return 0;
}

double Player0403::pNumVal(int id, int num, int index)
{
	//TODO
	//Вычисление вероятности полезности по числу
	return 0;
}

double Player0403::pRandVal(int id, int none, int index)
{
	//TODO
	//Вычисление вероятности полезности для рандомной карты
	return 0;
}

void Player0403::pUnknownCards(int index, StochasticMask& pMask, double(Player0403::* func1)(int, int, int), double(Player0403::* func2)(int, int, int),
	double(Player0403::* func3)(int, int, int))
{
	bool isPrompt = false;
	for (int j = 0; index < COLORS_COUNT; ++j)
	{
		if (index < ColoredPileMask[m_id][j].size())
		{
			if (ColoredPileMask[m_id][j][index])
			{
				isPrompt = true;
				pMask[index] = (this->*func1)(m_id, j, index);
				break;
			}
		}
	}
	for (int j = 0; j < NUMBERS_COUNT; ++j)
	{
		if (index < NumericalPileMask[m_id][j].size())
		{
			if (NumericalPileMask[m_id][j][index])
			{
				isPrompt = true;
				pMask[index] = (this->*func2)(m_id, j, index);
				break;
			}
		}
	}
	if (!isPrompt)
	{
		pMask[index] = (this->*func3)(m_id, 0, index);
	}
}

Action Player0403::Play(Pile* hands)
{
	StochasticMask playableCards(m_myHandSize);
	for (int i = 0; i < m_myHandSize; ++i)
	{
		if (hands[m_id][i] != Card())
		{
			if (playerView.canPlay(hands[m_id][i]))
			{
				playableCards[i] = 1;
			}
			else
			{
				playableCards[i] = 0;
			}
		}
		else
		{
			pUnknownCards(i, playableCards, pColorPlay, pNumPlay, pRandPlay);
		}
	}

	//Поиск карты наибольшей вероятной безошибочной игры, которая обладает наивысшим значением
	double maxProbability = playableCards[0];
	Number maxNum = hands[m_id][0].number;
	Index maxIt = 0;
	for (int i = 1; i < m_myHandSize; ++i)
	{
		if (playableCards[i] > maxProbability + dP)
		{
			maxProbability = playableCards[i];
			maxNum = hands[m_id][i].number;
			maxIt = i;
		}
		else
		{
			if ((playableCards[i] >= maxProbability - dP && playableCards[i] <= maxProbability + dP) && (hands[m_id][i].number > maxNum))
			{
				maxNum = hands[m_id][i].number;
				maxIt = i;
			}
		}
	}

	//TODO
	//Вызов функции корректировки всех масок
	return Action::Play(maxIt);
}

Action Player0403::Prompt(Pile* hands)
{
	//TODO
	return Action::Prompt(PLAYERS_COUNT - m_id - 1, Number::One);
}

Action Player0403::Discard(Pile* hands)
{
	//TODO
	StochasticMask ValuableMask(m_myHandSize);
	for (int i = 0; i < m_myHandSize; ++i)
	{
		if (hands[m_id][i] != Card())
		{
			bool isTrash = false;
			for (int j = 0; j < COLORS_COUNT; ++j)
			{
				int num = static_cast<int>(hands[m_id][i].number);
				if (num <= playerView.firework(static_cast<Color>(i)))
				{
					isTrash = true;
					ValuableMask[i] = 0;
				}
			}
			for (int j = i; j < m_myHandSize; ++j)
			{
				if (hands[m_id][i] == hands[m_id][j])
				{
					isTrash = true;
					ValuableMask[i] = 0;
				}
			}
			if (!isTrash)
			{
				//TODO
				//Вычисление вероятности полезности для известной карты
			}
		}
		else
		{
			pUnknownCards(i, ValuableMask, pColorVal, pNumVal, pRandVal);
		}
	}


	return Action::Discard(0);
}

Action Player0403::decide()
{
	//В этой части мы формируем в памяти известные нам свои карты, а также карты других игроков
	AllCards AllOtherCards;
	InitAllOtherCards(AllOtherCards);

	m_myHandSize = playerView.myHandSize();

	Pile Hands[PLAYERS_COUNT];
	Hands[m_id] = Pile(m_myHandSize);

	ConstructHand(Hands[m_id]);
	for (Id i = 0; i < PLAYERS_COUNT; ++i)
	{
		if (i != m_id)
		{
			Hands[i] = playerView.hand(i);
		}
	}
	CalculateOtherCards(Hands, AllOtherCards);

	//Выбор хода и вызов метода самого хода
	if (CanWePlay(Hands[m_id]))
	{
		return Play(Hands);
	}
	else
	{
		if (CanWePrompt())
		{
			return Prompt(Hands);
		}
		else
		{
			return Discard(Hands);
		}
	}
}
