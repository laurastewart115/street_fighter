#pragma once
#include "ScreenState.h"
#include "Button.h"
#include <vector>
#include <iostream>
using namespace std;

class LevelResultState : public ScreenState
{
private:
	Button* m_menu;
	string m_result;
public:
	LevelResultState(std::string result) { m_result = result; }

	void Enter();
	void Exit();
	void Update();
	void Render();
	void Resume() {}
};

