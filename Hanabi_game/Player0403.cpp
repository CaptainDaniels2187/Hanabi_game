#include "Player0403.h"
#include <algorithm>

using namespace Hanabi;

Player0403::Player0403(View view, Log& log) : playerView(view), LOG(log), m_id(playerView.myId()), m_myHandSize(0)
{
}

Player0403::~Player0403()
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
	//�������� ��������� ����� � ����� � �������
	for (int i = 0; i < PLAYERS_COUNT; ++i)
	{
		for (int j = 0; j < hands[i].size(); ++j)
		{
			if (hands[i][j] != Card())
			{
				int cardColor = static_cast<int>(hands[i][j].color);
				int cardNum = static_cast<int>(hands[i][j].number) - 1;
				allCards[cardColor][cardNum] -= 1;
			}
		}
	}

	//�������� ��������� ����� � ������ ������
	Pile Trash = playerView.discardPile();
	for (auto const& card : Trash)
	{
		int cardColor = static_cast<int>(card.color);
		int cardNum = static_cast<int>(card.number) - 1;
		allCards[cardColor][cardNum] -= 1;
	}
	
	//�������� ��������� ����� �� �����
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

Action Player0403::Play(Pile* hands)
{
	//TODO
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
			bool isPrompt = false;
			for (int j = 0; i < COLORS_COUNT; ++j)
			{
				if (i < ColoredPileMask[m_id][j].size())
				{
					if (ColoredPileMask[m_id][j][i])
					{
						isPrompt = true;
						//���������� ����������� �� �����
						break;
					}
				}
			}
			for (int j = 0; j < NUMBERS_COUNT; ++j)
			{
				if (i < NumericalPileMask[m_id][j].size())
				{
					if (NumericalPileMask[m_id][j][i])
					{
						isPrompt = true;
						//���������� ����������� �� �����
						break;
					}
				}
			}
			if (!isPrompt)
			{
				//���������� ����������� ��� ��������� �����
			}
		}
	}
	//����� ����� ���������� ��������� ������������ ����, ������� �������� ��������� ���������
	return Action::Play(0);
}

Action Player0403::Prompt(Pile* hands)
{
	//TODO
	return Action::Prompt(PLAYERS_COUNT - m_id - 1, Number::One);
}

Action Player0403::Discard(Pile* hands)
{
	//TODO
	return Action::Discard(0);
}

Action Player0403::decide()
{
	//� ���� ����� �� ��������� � ������ ��������� ��� ���� �����, � ����� ����� ������ �������
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

	//����� ���� � ����� ������ ������ ����
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
