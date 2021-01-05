#include "GameState.h"
#include "Game.h"
#include "PauseMenuPopupState.h"
#include "GameManager.h"
#include "RoundResultState.h"

void GameState::Enter()
{
//	bgSpriteTex = Game::Instance()->LoadTexture("background.png");
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

	  
	player = new Player(mainSpriteTex, bgDestRect.w*0.33, bgDestRect.h - 100);

	enemy = new Enemy(enemySpriteTex, bgDestRect.w*0.66, bgDestRect.h - 100);

	player->RegisterOnAttackCallback(std::bind(&GameState::OnPlayerAttack, this));
	enemy->RegisterOnAttackCallback(std::bind(&GameState::OnEnemyAttack, this));
}

void GameState::OnPlayerAttack()
{
	// check that the player is colliding with the enemy
	if (CircleCollisionTest(player->GetX(), player->GetY(), enemy->GetX(), enemy->GetY(), player->GetRadius(), enemy->GetRadius()))
	{
		// get the current attack
		Attack a = Attack::NONE;
		string aString = player->GetCurrentAttack();
		if (aString == "Punch")
			a = Attack::PUNCH;
		else if (aString == "Kick")
			a = Attack::KICK;
		else if (aString == "Roundhouse")
			a = Attack::ROUNDHOUSE;
		
		GameManager::Instance()->EnemyHitByAttack(a, enemy->IsDefending());
	}
}

void GameState::OnEnemyAttack()
{
	// check that the player is colliding with the enemy
	if (CircleCollisionTest(player->GetX(), player->GetY(), enemy->GetX(), enemy->GetY(), player->GetRadius(), enemy->GetRadius()))
	{
		// get the current attack
		Attack a = Attack::NONE;
		string aString = enemy->GetCurrentAttack();
		if (aString == "Punch")
			a = Attack::PUNCH;
		else if (aString == "Kick")
			a = Attack::KICK;
		else if (aString == "Roundhouse")
			a = Attack::ROUNDHOUSE;

		GameManager::Instance()->PlayerHitByAttack(a, player->IsDefending());
	}
}

void GameState::Update()
{
	if (Game::Instance()->KeyDown(SDL_SCANCODE_ESCAPE) == 1)
	{
		Game::Instance()->GetFSM()->PushState(new PauseMenuPopupState());
		return; //to make sure we don't run the game in background while pause menu shows
	}

	if (enemy)
	{
		//std::cout << "under enemy...\n";
		enemy->Update();
	}
	if (player) {
		//std::cout << "under player...\n";
		player->Update();
	}

	if (player) this->CheckCollision();
}


void GameState::Render()
{
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 0, 0, 255); //set background window color
	SDL_RenderClear(Game::Instance()->GetRenderer()); //clear the screen

	bg->Render();

	if (player) player->Render();

	if (enemy) enemy->Render();

	// Render health UI
	string pHealth = "Player Health: " + to_string(GameManager::Instance()->GetPlayerHealth());
	RenderFont(true, pHealth.c_str(), 32, 50);

	string eHealth = "Enemy Health: " + to_string(GameManager::Instance()->GetEnemyHealth());
	RenderFont(true, eHealth.c_str(), 650, 50);

	ScreenState::Render();

}


void GameState::Exit()
{
	//to-do:: de-allocate memory, texture, surface etc, sound 

}

void GameState::CheckCollision()
{
	// Check if the enemy and player are colliding
	if (CircleCollisionTest(player->GetX(), player->GetY(), enemy->GetX(), enemy->GetY(), player->GetRadius(), enemy->GetRadius()))
	{
		// prevent characters from moving through each other
		player->SetCanMoveForward(false);
		enemy->SetCanMoveForward(false);

		// have the enemy attack
		ChooseEnemyAction(true);
	}
	else if (enemy->GetX() < player->GetX())
	{
		// move the enemy backwards toward the player if they somehow got past
		enemy->MoveEnemy(false);
		enemy->PlayState("Move");
	}
	else 
	{
		// allow characters to move towards each other again
		player->SetCanMoveForward(true);
		enemy->SetCanMoveForward(true);

		// have the enemy move forward or hadoken
		ChooseEnemyAction(false);
	}

	// check if the player collided with the enemy hadoken
	for (int e = 0; e < (int)enemy->GetHadokens().size(); e++)
	{
		Hadoken* hadoken = enemy->GetHadokens()[e];

		if (CircleCollisionTest(hadoken->GetX(), hadoken->GetY(),
			player->GetX(), player->GetY(), hadoken->GetRadius(),
			player->GetRadius()))
		{

			// subtract health
			GameManager::Instance()->PlayerHitByAttack(Attack::HADOKEN, player->IsDefending());

			// destroy hadoken
			delete hadoken;
			enemy->GetHadokens()[e] = nullptr;
			enemy->GetHadokens().erase(enemy->GetHadokens().begin() + e);

		}
	}

	// check if the enemmy collided with the player hadoken
	for (int e = 0; e < (int)player->GetHadokens().size(); e++)
	{
		Hadoken* hadoken = player->GetHadokens()[e];

		if (CircleCollisionTest(hadoken->GetX(), hadoken->GetY(),
			enemy->GetX(), enemy->GetY(), hadoken->GetRadius(),
			enemy->GetRadius()))
		{

			// subtract health
			GameManager::Instance()->EnemyHitByAttack(Attack::HADOKEN, enemy->IsDefending());

			// destroy hadoken
			delete hadoken;
			player->GetHadokens()[e] = nullptr;
			player->GetHadokens().erase(player->GetHadokens().begin() + e);

		}
	}

}

void GameState::ChooseEnemyAction(bool inMeleeRange)
{
	if (inMeleeRange)
	{
		// Select an attack, idle, or defend
		int action = rand() % 3;
		switch (action)
		{
		case 0:
			// attack
			if (enemy->GetAttackCooldown() <= 0)
				enemy->SelectAttack(inMeleeRange);
			enemy->PlayState("Idle");
			break;
		case 1:
			// defend
			enemy->Defend(true);
			break;
		default:
			//idle
			enemy->PlayState("Idle");
			break;
		}

	}
	else
	{
		// at long range, so move forward, do a hadoken, or defend
		if (enemy->GetCanMoveForward() && enemy->GetHadokenCooldown() > 0)
		{
			if (player->GetHadokens().size() > 0)
			{
				bool defending = false;
				//loop through hadokens to check that they are past the enemy
				for (int i = 0; i < (int)player->GetHadokens().size(); i++)
				{
					if (player->GetHadokens()[i]->GetX() < enemy->GetX())
					{
						enemy->Defend(false); // defend against ranged attacks by jumping
						defending = true;
						break;
					}
				}

				if (!defending)
				{
					enemy->MoveEnemy(true);
				}
			}
			else
			{
				enemy->MoveEnemy(true);
			}

		}
		else
		{
			// attack
			enemy->SelectAttack(inMeleeRange);
		}
	}
}
