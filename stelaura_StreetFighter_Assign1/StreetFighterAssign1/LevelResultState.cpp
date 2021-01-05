#include "LevelResultState.h"
#include "Game.h"
#include "MainMenuState.h"

void LevelResultState::Enter()
{
	m_menu = new Button("Img/UI/mainMenu.png", { 0,0,190,50 }, { 312,350,400,100 });

	m_pFont = TTF_OpenFont("Font/LTYPE.TTF", 30);
}

void LevelResultState::Update()
{
	m_menu->Update();

	if (m_menu->Clicked())
	{
		Game::Instance()->GetFSM()->Clean(); // Clear all states, including GameState on bottom.
		//go back to main menu
		Game::Instance()->GetFSM()->ChangeState(new MainMenuState());
	}
}

void LevelResultState::Render()
{
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 51, 51, 51, 128);

	//draw background 
	SDL_Rect rect = { 256, 128, 512, 512 };
	SDL_RenderFillRect(Game::Instance()->GetRenderer(), &rect);

	//draw the buttons
	m_menu->Render();

	// show result
	RenderFont(true, m_result.c_str(), 340, 200);

	ScreenState::Render();
}

void LevelResultState::Exit()
{
	delete m_menu;
	m_menu = nullptr;
}