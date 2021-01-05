#pragma once
#include "GameState.h"

class GameLevel3 : public GameState
{
private:
	int m_round = 1;
	int m_roundMax = 7;
	int m_winThreshold = 4;
public:
	GameLevel3() {}

	void Enter() override;

	void Update() override;
};

