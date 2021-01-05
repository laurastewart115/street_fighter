#pragma once
#include "Player.h"
#include "Enemy.h"

enum class Attack { NONE, HADOKEN, PUNCH, KICK, ROUNDHOUSE };

class GameManager
{
private:
	int m_pHealth = 100;

	int m_eHealth = 100;

	int m_playerWins = 0; // number of rounds the player has won

	int CalculateDamage(Attack attack, bool defending);

public:
	static GameManager* Instance();

	int GetPlayerHealth() { return m_pHealth; }
	int GetEnemyHealth() { return m_eHealth; }

	int GetPlayerWins() { return m_playerWins; }

	void PlayerHitByAttack(Attack attack, bool defending);

	void EnemyHitByAttack(Attack attack, bool defending);

	bool PlayerWins();
	bool EnemyWins();

	void ResetRound();

	void ResetGame();
};

