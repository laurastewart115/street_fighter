#include "GameManager.h"

GameManager* GameManager::Instance()
{
	static GameManager* instance = new GameManager();
	return instance;
}

void GameManager::PlayerHitByAttack(Attack attack, bool defending)
{
	int dmg = CalculateDamage(attack, defending);

	m_pHealth -= dmg;
	if (m_pHealth < 0)
		m_pHealth = 0;
}

void GameManager::EnemyHitByAttack(Attack attack, bool defending)
{
	int dmg = CalculateDamage(attack, defending);

	m_eHealth -= dmg;
	if (m_eHealth < 0)
		m_eHealth = 0;
}

int GameManager::CalculateDamage(Attack attack, bool defending)
{
	float dmg = 0;

	switch (attack)
	{
	case Attack::HADOKEN:
		dmg = 30;
		break;
	case Attack::PUNCH:
		dmg = 5;
		break;
	case Attack::KICK:
		dmg = 10;
		break;
	case Attack::ROUNDHOUSE:
		dmg = 15;
		break;
	default:
		dmg = 0;
		break;
	}

	if (defending)
	{
		dmg /= 2;
	}

	return (int)dmg;
}

bool GameManager::PlayerWins()
{
	if (m_pHealth > 0 && m_eHealth <= 0)
	{
		m_playerWins++;
		return true;
	}
	else
	{
		return false;
	}
}

bool GameManager::EnemyWins()
{
	if (m_eHealth > 0 && m_pHealth <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GameManager::ResetRound()
{
	m_pHealth = 100;
	m_eHealth = 100;
}

void GameManager::ResetGame()
{
	m_pHealth = 100;
	m_eHealth = 100;
	m_playerWins = 0;
}