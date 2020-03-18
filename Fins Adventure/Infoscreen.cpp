#include "stdafx.h"
#include "Infoscreen.h"
#include "utils.h"

Infoscreen::Infoscreen()
	: m_InfoMovement1{"Use the ARROW KEYS to move","Resources/soupofjustice.ttf", 25, Color4f{ 0.1f, 0.3f, 1.0f, 1.0f }}
	, m_InfoMovement2{ "left, right and to jump","Resources/soupofjustice.ttf", 25, Color4f{ 0.1f, 0.3f, 1.0f, 1.0f } }
	, m_InfoAttack{ "Press SPACEBAR to attack","Resources/soupofjustice.ttf", 25, Color4f{ 0.1f, 0.3f, 1.0f, 1.0f } }
	, m_BackText{ "BACK","Resources/soupofjustice.ttf", 50, Color4f{ 1.0f, 1.0f, 1.0f, 1.0f } }
	, m_BackButtonShape{ 105, 75, 150, 50 }
	, m_Hoovering{false}
	, m_Background{ "Resources/Titlescreen_Background.jpg" }
	, m_Pinguin{ "Resources/Pinguin.png" }
{
}

void Infoscreen::Draw() const
{
	// Background
	glPushMatrix();
	glScalef(0.5f, 0.5f, 1.0f);
	m_Background.Draw();
	glPopMatrix();

	// Pinguin
	glPushMatrix();
	glScalef(0.2f, 0.2f, 1.0f);
	m_Pinguin.Draw(Point2f{ 1300, 5 });
	glPopMatrix();

	// Info
	m_InfoMovement1.Draw(Point2f{ 20, 300 });
	m_InfoMovement2.Draw(Point2f{ 50, 270 });
	m_InfoAttack.Draw(Point2f{ 30, 200 });

	// Back button
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	utils::FillRect(m_BackButtonShape.left - 1, m_BackButtonShape.bottom - 1, m_BackButtonShape.width + 2, m_BackButtonShape.height + 2);
	if (m_Hoovering) glColor4f(0.0f, 0.2f, 0.9f, 1.0f);
	else glColor4f(0.1f, 0.3f, 1.0f, 1.0f);
	utils::FillRect(m_BackButtonShape);
	m_BackText.Draw(Point2f{ m_BackButtonShape.left + 20, m_BackButtonShape.bottom - 3 });

	
}

void Infoscreen::IsHoovering(const Point2f& mousePos)
{
	if (utils::IsPointInRect(mousePos, m_BackButtonShape)) m_Hoovering = true;
	else m_Hoovering = false;
}

Rectf Infoscreen::GetBackButtonShape() const
{
	return m_BackButtonShape;
}