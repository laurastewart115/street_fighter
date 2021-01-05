#include "RoundResultState.h"
#include <iostream>
#include "Game.h"
#include "MainMenuState.h"
using namespace std;

void RoundResultState::Enter()
{
	m_vButtons.push_back(new Button("Img/UI/nextRound.png", { 0,0,190,50 }, { 312,300,400,100 }));
	m_vButtons.push_back(new Button("Img/UI/mainMenu.png", { 0,0,190,50 }, { 312,450,400,100 }));

	m_pFont = TTF_OpenFont("Font/LTYPE.TTF", 30);
}

void RoundResultState::Update()
{
	// Update buttons. Allows for mouseovers.
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Update();

	//close the round popup to continue to next round
	if (m_vButtons[btn::next]->Clicked())
	{
		Game::Instance()->GetFSM()->PopState();
	}
	//else if exit was clicked, we need to go back to main menu
	else if (m_vButtons[btn::quit]->Clicked())
	{
		Game::Instance()->GetFSM()->Clean(); // Clear all states, including GameState on bottom.
		//go back to main menu
		Game::Instance()->GetFSM()->ChangeState(new MainMenuState());
	}
}

void RoundResultState::Render()
{
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 51, 51, 51, 128);

	//draw background 
	SDL_Rect rect = { 256, 128, 512, 512 };
	SDL_RenderFillRect(Game::Instance()->GetRenderer(), &rect);

	//draw the buttons
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Render();

	// show result
	RenderFont(true, m_result.c_str(), 340, 200);

	ScreenState::Render();
}

void RoundResultState::Exit()
{
	for (int i = 0; i < (int)m_vButtons.size(); i++)
	{
		delete m_vButtons[i];
		m_vButtons[i] = nullptr;
	}
	m_vButtons.clear();
	m_vButtons.shrink_to_fit();
}