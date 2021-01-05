#pragma once

#include "SpriteEx.h"
#include "Hadoken.h"
 #include <vector>
using namespace std;


class Player : public SpriteExAnimated
{
private:
	void MovePlayer(bool isFwd);
	void UpdatePlayer();

protected:
	double m_velX = 0,
		m_velY = 0,
		m_velMax = 10;
	double preJumpYVal;

	// stores the starting x and y coordinates so the sprite's position can be reset
	double m_startingX, m_startingY = 0;

	bool m_canMoveFwd = true; // prevent characters from moving forward when colliding with each other

	string m_currentAttack = "";

	vector<Hadoken*> m_hadokens;
	virtual void UpdateHadokens();
	bool spawningHadoken = false;
	void OnHadokenAnimationComplete();

	void Jump();

	void OnJumpAnimationComplete();

public :

	Player(SDL_Texture *tex, double x, double y);
	~Player();

	bool GetCanMoveForward() { return m_canMoveFwd; }
	void SetCanMoveForward(bool value) { m_canMoveFwd = value; }

	void ResetPosition();

	string GetCurrentAttack() { return m_currentAttack; }

	virtual	void Update();
	void Render();

	virtual void SpawnHadoken();

	vector<Hadoken*>& GetHadokens()
	{
		return m_hadokens;
	}

	bool IsDefending() 
	{ 
		if (currentState == "Crouch") 
			return true; 
		else 
			return false; 
	}

	virtual void OnAttackFinished();

	std::function<void()> onAttack;

	void RegisterOnAttackCallback(std::function<void()> cb)
	{
		onAttack = cb;
	}
 
};