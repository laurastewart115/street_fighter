#include "Player.h"
#include "Game.h"
#include <algorithm> //for min/max functions

Player::Player(SDL_Texture *tex, double x, double y)
	: SpriteExAnimated(tex, x - 50, y - 50, 0, 1, 4, 0.1f)
{

	spriteSrcRect = { 0,0,70,80 };
	spriteDestRect = { (int)(m_X - 50),(int) (m_Y-50)  ,70,80 };

	m_startingX = m_X;
	m_startingY = m_Y;

	m_velX = 0.9f;
	m_velY = 5.5f;

	m_dRadius = 25;

	AddAnimState("Hadoken", AnimStateDefinition(0, 4, 120));
	AddAnimState("Idle",  AnimStateDefinition(1, 4, 120,true));
	AddAnimState("Punch", AnimStateDefinition(2, 3, 120));
	AddAnimState("Move", AnimStateDefinition(3, 5, 120,true));


	AddAnimState("Kick", AnimStateDefinition(6, 5, 120));
	AddAnimState("Roundhouse", AnimStateDefinition(7, 5, 120));
	AddAnimState("Jump", AnimStateDefinition(8, 7, 120));
	AddAnimState("Crouch", AnimStateDefinition(9, 1, 120));

	animStates["Jump"].RegisterOnAnimCompleteCallback
	(std::bind(&Player::OnJumpAnimationComplete,this));

	animStates["Hadoken"].RegisterOnAnimCompleteCallback(std::bind(&Player::OnHadokenAnimationComplete, this));

	animStates["Kick"].RegisterOnAnimCompleteCallback(std::bind(&Player::OnAttackFinished, this));
	animStates["Punch"].RegisterOnAnimCompleteCallback(std::bind(&Player::OnAttackFinished, this));
	animStates["Roundhouse"].RegisterOnAnimCompleteCallback(std::bind(&Player::OnAttackFinished, this));
}

void Player::OnAttackFinished()
{
	m_currentAttack = currentState;
	if (onAttack != nullptr)
		onAttack();
}

Player::~Player()
{
	for (int i = 0; i < (int)m_hadokens.size(); i++)
	{
		delete m_hadokens[i];
		m_hadokens[i] = nullptr;
	}
	m_hadokens.clear();
	m_hadokens.shrink_to_fit();

}

//For Lab 4
////don't forget to define in Player.h
//void Player::OnKickFrame3Done()
//{
//	cout << "detect kick here..";
//}

void Player::Render()
{
	// render hadokens
	for (int i = 0; i < (int)m_hadokens.size(); i++)
	{
		m_hadokens[i]->Render();
	}
 
	this->SpriteExAnimated::Render(); //invoke the base class's Render()

}


void Player::Update()
{
 
	this->UpdatePlayer();
	this->UpdateHadokens();
 }


void Player::Jump()
{
	if (m_iFrame < 3)
		m_Y -= m_velY;
	else if (m_iFrame > 3)
	{
		m_Y += m_velY;
		if (m_Y > preJumpYVal)
			m_Y = preJumpYVal;
		
	//the alternative to use callback, would have been to do the following:	
		//if (m_iFrame >= animStates[currentState].frames)
			//m_Y = preJumpYVal;

	}
}

//Engine must call this function when our jump animation is completed.
void Player::OnJumpAnimationComplete()
{

	if (m_Y > preJumpYVal)
		m_Y = preJumpYVal;

}

void Player::OnHadokenAnimationComplete()
{
	// allow more hadokens to spawn
	spawningHadoken = false;
}

void Player::MovePlayer(bool isFwd)
{

	if (isFwd && m_canMoveFwd)
		m_X += m_velX;
	else if (!isFwd)
		m_X -= m_velX;

	this->PlayState("Move");
}

void Player::ResetPosition()
{
	m_X = m_startingX;
	m_Y = m_startingY;
}

void Player::UpdatePlayer()
{


	if (Game::Instance()->KeyDown(SDL_SCANCODE_D)) 
	{
		this->MovePlayer(true);
		
	}
	else if (Game::Instance()->KeyDown(SDL_SCANCODE_A)) 
	{
		this->MovePlayer(false);
	}
	else if (Game::Instance()->KeyDown(SDL_SCANCODE_K)) {
		this->PlayState("Kick");
	}
	else if (Game::Instance()->KeyDown(SDL_SCANCODE_SPACE))
	{

		if (this->currentState != "Jump")
		{
			preJumpYVal = m_Y;
		
		}
		
	    this->PlayState("Jump");
	}
	else if (Game::Instance()->KeyDown(SDL_SCANCODE_P))
	{
		this->PlayState("Punch");
	}
	else if (Game::Instance()->KeyDown(SDL_SCANCODE_R))
	{
		this->PlayState("Roundhouse");
	}
	else if (Game::Instance()->KeyDown(SDL_SCANCODE_H))
	{
		if (!spawningHadoken)
		{
			spawningHadoken = true;
			this->SpawnHadoken();
		}
		
		this->PlayState("Hadoken");
	}
	else if (Game::Instance()->KeyDown(SDL_SCANCODE_C))
	{
		this->PlayState("Crouch");
	}
	else {
		this->PlayState("Idle");
	}
	 
	////compare and see if we are in jump state, and if yes, call Jump....
	if (currentState==("Jump"))
	{
		Jump();
 	}


	this->spriteSrcRect.x = spriteSrcRect.w * m_iFrame;
	this->UpdateDestRect();

}

void Player::UpdateHadokens()
{
	for (int i = 0; i < (int)m_hadokens.size(); i++)
	{
		m_hadokens[i]->Update();

		if (m_hadokens[i]->m_X < 0 || m_hadokens[i]->m_X >
			1024 ||
			m_hadokens[i]->m_Y < 0 || m_hadokens[i]->m_Y > 768)
		{
			delete m_hadokens[i];
			m_hadokens[i] = nullptr;
			m_hadokens.erase(m_hadokens.begin() + i);
		}
	}

	m_hadokens.shrink_to_fit();
}

void Player::SpawnHadoken()
{
	m_hadokens.push_back(new Hadoken(texture, m_X, m_Y, angle, true));
}