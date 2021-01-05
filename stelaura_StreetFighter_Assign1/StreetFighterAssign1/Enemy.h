#pragma once
#include "Player.h"
#include "GameManager.h"

class Enemy : public Player
{
private:
	bool m_canAttack = true;
	bool m_jumping = false;
	bool m_moving = false;

	int m_hadokenCooldown = 50;
	int m_attackCooldown = 25;

	void UpdateEnemy();

public:

	Enemy(SDL_Texture* tex, double x, double y);
	
	//Over-ride the base player class's Update functionality
	void Update() override;

	int GetHadokenCooldown() { return m_hadokenCooldown; }
	int GetAttackCooldown() { return m_attackCooldown; }

	// Enemy movement (will be opposite of player)
	void MoveEnemy(bool moveFwd);

	// Choose an attack based on whether the opponent is within melee range or not
	void SelectAttack(bool inMeleeRange);

	// Crouch or jump to avoid attacks
	void Defend(bool meleeAttack);

	// Spawn a Hadoken that flies towards the player
	void SpawnHadoken() override;

	void OnHadokenFinished();

	void OnAttackFinished() override;
};