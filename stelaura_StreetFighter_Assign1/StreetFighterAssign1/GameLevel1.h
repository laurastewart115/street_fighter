#pragma once
#include "GameState.h"

class GameLevel1 : public GameState
{
private:
	int m_round = 1;
	int m_roundMax = 3;
	int m_winThreshold = 2;
public:
	GameLevel1() { m_round = 1; }

	void Enter() override;
	void Update() override;

};