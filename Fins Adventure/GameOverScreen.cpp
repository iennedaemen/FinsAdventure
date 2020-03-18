#include "stdafx.h"
#include "GameOverScreen.h"
#include "utils.h"

GameOverScreen::GameOverScreen()
	: m_GameText{ "game","Resources/soupofjustice.ttf", 55, Color4f{ 0.1f, 0.3f, 1.0f, 1.0f } }
	, m_OverText{ "over","Resources/soupofjustice.ttf", 55, Color4f{ 0.1f, 0.3f, 1.0f, 1.0f } }
	, m_MenuText{ "MENU","Resources/soupofjustice.ttf", 50, Color4f{ 1.0f, 1.0f, 1.0f, 1.0f } }
	, m_TryAgainText{ "TRY AGAIN","Resources/soupofjustice.ttf", 30, Color4f{ 1.0f, 1.0f, 1.0f, 1.0f } }
	, m_MenuButtonShape{ 105, 75, 150, 50 }
	, m_TryAgainButtonShape{ 105, 150, 150, 50 }
	, m_MenuHoovering{false}
	, m_TryAgainHoovering{false}
	, m_Background{"Resources/Titlescreen_Background.jpg"}
{
}


void GameOverScreen::Draw() const
{
	// background
	glPushMatrix();
	glScalef(0.5f, 0.5f, 1.0f);
	m_Background.Draw();
	glPopMatrix();

	// Game Over text
	m_GameText.Draw(Point2f{ 110, 275 });
	m_OverText.Draw(Point2f{ 115, 225 });

	// Try again button
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	utils::FillRect(m_TryAgainButtonShape.left - 1, m_TryAgainButtonShape.bottom - 1, m_TryAgainButtonShape.width + 2, m_TryAgainButtonShape.height + 2);
	if (m_TryAgainHoovering) glColor4f(0.0f, 0.2f, 0.9f, 1.0f);
	else glColor4f(0.1f, 0.3f, 1.0f, 1.0f);
	utils::FillRect(m_TryAgainButtonShape);
	m_TryAgainText.Draw(Point2f{ m_TryAgainButtonShape.left + 12, m_TryAgainButtonShape.bottom+7 });

	// Menu button
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	utils::FillRect(m_MenuButtonShape.left - 1, m_MenuButtonShape.bottom - 1, m_MenuButtonShape.width + 2, m_MenuButtonShape.height + 2);
	if (m_MenuHoovering) glColor4f(0.0f, 0.2f, 0.9f, 1.0f);
	else glColor4f(0.1f, 0.3f, 1.0f, 1.0f);
	utils::FillRect(m_MenuButtonShape);
	m_MenuText.Draw(Point2f{ m_MenuButtonShape.left + 17, m_MenuButtonShape.bottom - 3 });
}

void GameOverScreen::IsHoovering(const Point2f& mousePos)
{
	if (utils::IsPointInRect(mousePos, m_MenuButtonShape)) m_MenuHoovering = true;
	else m_MenuHoovering = false;

	if (utils::IsPointInRect(mousePos, m_TryAgainButtonShape)) m_TryAgainHoovering = true;
	else m_TryAgainHoovering = false;
}

Rectf GameOverScreen::GetTryAgainButtonShape() const
{
	return m_TryAgainButtonShape;
}

Rectf GameOverScreen::GetMenuButtonShape() const
{
	return m_MenuButtonShape;
}