#include "GameSetupState.h"
#include "Game.h"
#include "GameLevel1.h"
#include "GameLevel2.h"
#include "GameLevel3.h"

void GameSetupState::Enter()
{
	cout << "Entering GameSetup...\n";
	Game::Instance()->SetLeftMouse(false);

	m_vButtons.push_back(new Button("Img/UI/btnLevel1.png", { 0,0,190,50 }, { 312,150,400,100 }));
	m_vButtons.push_back(new Button("Img/UI/btnLevel2.png", { 0,0,190,50 }, { 312,300,400,100 }));
	m_vButtons.push_back(new Button("Img/UI/btnLevel3.png", { 0,0,190,50 }, { 312,450,400,100 }));
}

void GameSetupState::Update()
{

	// Update buttons. Allows for mouseovers.
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Update();

	if (m_vButtons[btn::level1]->Clicked())
	{
		// Go to Level 1
		Game::Instance()->GetFSM()->ChangeState(new GameLevel1());
	}
	else if (m_vButtons[btn::level2]->Clicked())
	{
		// Go to Level 2
		Game::Instance()->GetFSM()->ChangeState(new GameLevel2());
	}
	else if (m_vButtons[btn::level3]->Clicked())
	{
		// Go to Level 3
		Game::Instance()->GetFSM()->ChangeState(new GameLevel3());
	}

}


void GameSetupState::Render()
{
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 255, 128, 0, 255);
	SDL_RenderClear(Game::Instance()->GetRenderer());
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Render();

	ScreenState::Render();
}

void GameSetupState::Exit()
{
	cout << "Exiting GameSetup...\n";

	for (int i = 0; i < (int)m_vButtons.size(); i++)
	{
		delete m_vButtons[i];
		m_vButtons[i] = nullptr;
	}
	m_vButtons.clear();
	m_vButtons.shrink_to_fit();

}
