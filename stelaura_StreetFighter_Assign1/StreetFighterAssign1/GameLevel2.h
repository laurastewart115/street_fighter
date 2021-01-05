#pragma once
#include "GameState.h"

class GameLevel2 : public GameState
{
private:
	int m_round = 1;
	int m_roundMax = 5;
	int m_winThreshold = 3;
public:
	GameLevel2() {}

	void Enter() override;

	void Update() override;
};

