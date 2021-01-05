#include "GameLevel3.h"
#include "Game.h"
#include "RoundResultState.h"
#include "LevelResultState.h"

void GameLevel3::Enter()
{
	bgSpriteTex = Game::Instance()->LoadTexture("Img/Levels/bgLevel3.jpg");

	mainSpriteTex = Game::Instance()->LoadTexture("Img/Players/PlayerKenSprite2.png");

	// Load a different coloured enemy sprite
	enemySpriteTex = Game::Instance()->LoadTexture("Img/Players/EnemyKenSprite2.png");

	m_pFont = TTF_OpenFont("Font/LTYPE.TTF", 30);

	SDL_Rect bgSrcRect;
	bgSrcRect.x = bgSrcRect.y = 0;
	SDL_QueryTexture(bgSpriteTex, NULL, NULL, &bgSrcRect.w, &bgSrcRect.h);

	SDL_Rect bgDestRect;
	bgDestRect.x = bgDestRect.y = 0;
	SDL_GetWindowSize(Game::Instance()->GetWindow(), &bgDestRect.w, &bgDestRect.h);

	bg = new SpriteEx(bgSpriteTex, bgSrcRect, bgDestRect);


	player = new Player(mainSpriteTex, bgDestRect.w * 0.33, bgDestRect.h - 50);

	enemy = new Enemy(enemySpriteTex, bgDestRect.w * 0.66, bgDestRect.h - 50);

	player->RegisterOnAttackCallback(std::bind(&GameState::OnPlayerAttack, this));
	enemy->RegisterOnAttackCallback(std::bind(&GameState::OnEnemyAttack, this));
}

void GameLevel3::Update()
{
	// detect win or loss
	if (GameManager::Instance()->PlayerWins() || GameManager::Instance()->EnemyWins())
	{
		m_round++;

		bool playerWinsRound = GameManager::Instance()->PlayerWins();

		GameManager::Instance()->ResetRound();

		// reset player and enemy positions
		player->ResetPosition();
		enemy->ResetPosition();

		if (m_round > m_roundMax)
		{
			// transition to level result state
			string result = "";
			if (GameManager::Instance()->GetPlayerWins() >= m_winThreshold)
			{
				result = "You win the level!";
			}
			else
			{
				result = "You lose the level...";
			}
			Game::Instance()->GetFSM()->PushState(new LevelResultState(result));
			return;
		}
		else
		{
			// transition to round result state
			string roundResult = "";
			if (playerWinsRound)
			{
				roundResult = "You win the round!";
			}
			else
			{
				roundResult = "You lose the round...";
			}

			Game::Instance()->GetFSM()->PushState(new RoundResultState(roundResult));
			return;
		}
	}

	GameState::Update();
}