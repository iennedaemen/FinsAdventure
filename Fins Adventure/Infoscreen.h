#pragma once
#include "Texture.h"

class Infoscreen
{
public:
	Infoscreen();
	
	void Draw() const;
	void IsHoovering(const Point2f& mousePos);
	Rectf GetBackButtonShape() const;

private:
	Texture m_InfoMovement1;
	Texture m_InfoMovement2;
	Texture m_InfoAttack;

	Texture m_BackText;
	Rectf m_BackButtonShape;

	Texture m_Background;
	Texture m_Pinguin;

	bool m_Hoovering;

};

