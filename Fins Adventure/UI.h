#pragma once
#include "Texture.h"
#include "structs.h"
#include "utils.h"

class UI
{
public:
	UI();
	
	void Draw() const;
	void Update(float elapsedSec, const int& currentHp, const int& maxHp, const float& secUnderWater, const float& maxSecUnderWater, Type type);
	void Reset();

private:
	// health
	Rectf m_HpBarShape;
	Rectf m_HpBarFill;
	Texture m_HpText;

	// oxygen
	Rectf m_OxygenBarShape;
	Rectf m_OxygenBarFill;
	Texture m_OxygenText;

	// power up
	Texture m_PowerUpText;
	Texture m_PowerUp;
	Rectf m_TextClip;
	Type m_Type;

};

