#pragma once
#include "ScreenState.h"
#include "Button.h"
#include <vector>
#include <iostream>
using namespace std;

class RoundResultState : public ScreenState
{
private:
	vector<Button*> m_vButtons;
	enum btn { next, quit };

	string m_result = "";

public:
	RoundResultState(string result) { m_result = result; }

	void Enter();
	void Exit();
	void Update();
	void Render();
	void Resume() {}
};

