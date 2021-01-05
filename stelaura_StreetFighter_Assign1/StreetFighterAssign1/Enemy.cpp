#include "Enemy.h"

Enemy::Enemy(SDL_Texture* tex, double x, double y) : Player(tex, x, y)
{
	isFlipped = true;
	spriteSrcRect = { 0,0,70,80 };
	spriteDestRect = { (int)(m_X - 50),(int)(m_Y - 50)  ,70,80 };

	m_dRadius = 25;

	AddAnimState("Hadoken", AnimStateDefinition(0, 4, 120));
	AddAnimState("Idle", AnimStateDefinition(1, 4, 120, true));
	AddAnimState("Punch", AnimStateDefinition(2, 3, 120));
	AddAnimState("Move", AnimStateDefinition(3, 5, 120, true));

	AddAnimState("Kick", AnimStateDefinition(6, 5, 120));
	AddAnimState("Roundhouse", AnimStateDefinition(7, 5, 120));
	AddAnimState("Jump", AnimStateDefinition(8, 7, 120));
	AddAnimState("Crouch", AnimStateDefinition(9, 1, 120));

	animStates["Jump"].RegisterOnAnimCompleteCallback(std::bind(&Enemy::OnJumpAnimationComplete, this));

	animStates["Hadoken"].RegisterOnAnimCompleteCallback(std::bind(&Enemy::OnHadokenFinished, this));
	animStates["Punch"].RegisterOnAnimCompleteCallback(std::bind(&Enemy::OnAttackFinished, this));
	animStates["Kick"].RegisterOnAnimCompleteCallback(std::bind(&Enemy::OnAttackFinished, this));
	animStates["Roundhouse"].RegisterOnAnimCompleteCallback(std::bind(&Enemy::OnAttackFinished, this));

}

void Enemy::OnAttackFinished()
{
	m_canAttack = true;
	m_attackCooldown = 50;
	m_currentAttack = currentState;
	if (onAttack != nullptr)
		onAttack();
}

void Enemy::OnHadokenFinished()
{
	m_canAttack = true;
	m_hadokenCooldown = 250;
}

void Enemy::Update()
{
	this->UpdateEnemy();
	this->UpdateHadokens();
}

void Enemy::UpdateEnemy()
{
	// Jump
	if (currentState == ("Jump"))
	{
		Jump();
	}

	if (m_hadokenCooldown > 0)
		m_hadokenCooldown--;
	if (m_attackCooldown > 0)
		m_attackCooldown--;

	this->spriteSrcRect.x = spriteSrcRect.w * m_iFrame;

	this->UpdateDestRect();
}

void Enemy::MoveEnemy(bool moveFwd)
{

	if (moveFwd && m_canMoveFwd)
		m_X -= m_velX;
	else if (!moveFwd)
		m_X += m_velX;

	this->PlayState("Move");

}

void Enemy::SelectAttack(bool inMeleeRange)
{
	// select the attack to do (punch, kick, roundhouse, hadoken)
	if (!m_moving)
	{
		if (!inMeleeRange)
		{
			// do a hadoken if they are far away
			this->PlayState("Hadoken");
			if (m_canAttack)
			{
				this->SpawnHadoken();
			}
		}
		else
		{
			// randomly select a melee attack
			int result = rand() % 3;

			switch (result)
			{
			case 0:
				this->PlayState("Punch");
				break;
			case 1:
				this->PlayState("Kick");
				break;
			case 2:
				this->PlayState("Roundhouse");
				break;
			default:
				break;
			}
		}

		m_canAttack = false;
	}
}

void Enemy::SpawnHadoken()
{
	m_hadokens.push_back(new Hadoken(texture, m_X, m_Y, angle, false));
}

void Enemy::Defend(bool meleeAttack)
{
	// jump to defend against a hadoken, crouch to defend in melee
	if (meleeAttack)
	{
		this->PlayState("Crouch");
	}
	else
	{
		if (!m_jumping)
		{
			m_jumping = true;
			preJumpYVal = m_Y;
			
		}
		this->PlayState("Jump");
	}
}

