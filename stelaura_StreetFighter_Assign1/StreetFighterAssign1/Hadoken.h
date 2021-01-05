#pragma once
#include "SpriteEx.h"

class Hadoken : public SpriteExAnimated
{
public:
	friend class Player;

	bool m_right = true;

	// constructor
	Hadoken(SDL_Texture* tex, double x, double y, double a, bool right)
		:
		SpriteExAnimated(tex, x, y, a, 0, 0, 20.0f)
	{
		m_DX = 0.2;
		//m_DY = dy;
		spriteSrcRect = { 0,0,70,80 };
		spriteDestRect = { (int)(m_X - 70),(int)(m_Y - 80)  ,70,80 };
		m_dRadius = 35;

		// determine the direction the hadoken will travel
		m_right = right;
		isFlipped = !m_right; // flip the sprite if it is travelling left

		AddAnimState("Idle", AnimStateDefinition(4, 2, 120, true));
		currentState = "Idle";
		this->PlayState("Idle");

	}

	void Update()
	{
		if (m_right)
			m_X += (m_DX * m_dSpeed);
		else
			m_X -= (m_DX * m_dSpeed);

		this->UpdateDestRect();
	}

};

