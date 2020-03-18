#include "stdafx.h"
#include "UI.h"


UI::UI()
	: m_HpBarShape{45, 330, 150, 20}
	, m_HpText{ "HP","Resources/soupofjustice.ttf", 25 ,Color4f{0.0f, 0.0f, 0.0f, 1.0f} }
	, m_OxygenBarShape{55, 315, 130, 7}
	, m_OxygenText{ "Air","Resources/soupofjustice.ttf", 17 ,Color4f{ 0.0f, 0.0f, 0.0f, 1.0f } }
	, m_PowerUp{ "Resources/PowerUps.png" }
	, m_Type{Type::none}
	, m_PowerUpText{ "power:","Resources/soupofjustice.ttf", 25 ,Color4f{ 0.0f, 0.0f, 0.0f, 1.0f } }

{
	m_HpBarFill = m_HpBarShape;
	m_OxygenBarFill = m_OxygenBarShape;
	m_TextClip = { (m_PowerUp.GetWidth() / 4) * (float(m_Type) - 1) , 0.0f , m_PowerUp.GetWidth() / 4, m_PowerUp.GetHeight() };
}


void UI::Draw() const
{
	// health
	m_HpText.Draw(Point2f{ 10,325 });

	glColor4f(1.0f, 0.2f, 0.2f, 1.0f);
	utils::FillRect(m_HpBarShape);

	glColor4f(0.2f, 1.0f, 0.2f, 1.0f);
	utils::FillRect(m_HpBarFill);

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	utils::DrawRect(m_HpBarShape, 1.0f);

	// oxygen
	m_OxygenText.Draw(Point2f{ 25, 308 });

	glColor4f(0.7f, 0.7f, 1.0f, 1.0f);
	utils::FillRect(m_OxygenBarShape);

	glColor4f(0.2f, 0.5f, 1.0f, 1.0f);
	utils::FillRect(m_OxygenBarFill);

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	utils::DrawRect(m_OxygenBarShape, 1.0f);

	// power up
	m_PowerUpText.Draw(Point2f{ 220,325 });
	if(m_Type != Type::none) m_PowerUp.Draw(Rectf{ 305, 330, 20, 20 }, m_TextClip);
}

void UI::Update(float elapsedSec, const int& currentHp, const int& maxHp, const float& secUnderWater, const float& maxSecUnderWater, Type type)
{
	if (m_HpBarFill.width <= 0) m_HpBarFill.width = 0;
	else m_HpBarFill.width = (m_HpBarShape.width/maxHp)*currentHp;

	if (secUnderWater >= maxSecUnderWater) m_OxygenBarFill.width = 0;
	else m_OxygenBarFill.width = (m_OxygenBarShape.width / maxSecUnderWater)*(maxSecUnderWater - secUnderWater);

	m_Type = type;
	m_TextClip = { (m_PowerUp.GetWidth() / 4) * (float(m_Type) - 1) , 0.0f , m_PowerUp.GetWidth() / 4, m_PowerUp.GetHeight() };
}

void UI::Reset()
{
	m_HpBarFill.width = m_HpBarShape.width;
	m_OxygenBarFill.width = m_OxygenBarShape.width;
}