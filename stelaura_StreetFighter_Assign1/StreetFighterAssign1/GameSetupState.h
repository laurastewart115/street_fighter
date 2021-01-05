#pragma once
#include "ScreenState.h"
#include "Button.h"
#include <vector>
#include <iostream>
using namespace std;

// level selection screen
class GameSetupState : public ScreenState
{
private:
	vector<Button*> m_vButtons;
	enum btn { level1, level2, level3 };

public:
	GameSetupState() { }

	void Enter();
	void Exit();
	void Update();
	void Render();
	void Resume() {}
};

