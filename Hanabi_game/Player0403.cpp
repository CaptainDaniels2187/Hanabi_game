#include "Player0403.h"

using namespace Hanabi;

Player0403::Player0403(View view, Log& log) : playerView(view), LOG(log)
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
		return "Player 1";
	}
	else
	{
		return "Player 0";
	}
}

void Hanabi::Player0403::receive(Id fromPlayerId, Color color, Mask mask)
{

}

void Hanabi::Player0403::receive(Id fromPlayerId, Number number, Mask mask)
{

}

Action Hanabi::Player0403::decide()
{
	return Action(Action::Play(0));
}
