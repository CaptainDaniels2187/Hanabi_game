#include "Player0403.h"

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

void Player0403::ConstructHand()
{
	for (int i = 0; i < COLORS_COUNT; ++i)
	{
		for (int j = 0; j < NUMBERS_COUNT; ++j)
		{
			for (int k = 0; k < m_myHandSize; ++k)
			{
				if (!ColoredPileMask[m_id][i].empty() && !NumericalPileMask[m_id][j].empty())
				{
					if (ColoredPileMask[m_id][i][k] == NumericalPileMask[m_id][j][k] == 1)
					{
						hands[m_id].push_back(Card(static_cast<Color>(i), static_cast<Number>(j + 1)));
					}
				}
			}
		}
	}
}

bool Player0403::CanWePlay()
{
	return false;
}

bool Player0403::CanWePrompt()
{
	return true;
}

Action Player0403::decide()
{
	//¬ этой части мы формируем в пам€ти известные нам свои карты, а также карты других игроков
	m_myHandSize = playerView.myHandSize();
	Mask ToDiscard(m_myHandSize);
	Mask ToPlay(m_myHandSize);

	ConstructHand();
	for (Id i = 0; i < PLAYERS_COUNT; ++i)
	{
		if (i != m_id)
		{
			hands[i] = playerView.hand(i);
		}
	}


	return Action(Action::Play(0));
}
