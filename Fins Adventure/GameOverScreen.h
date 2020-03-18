#pragma once
#include "Texture.h"

class GameOverScreen
{
public:
	GameOverScreen();

	void Draw() const;
	void IsHoovering(const Point2f& mousePos);
	Rectf GetTryAgainButtonShape() const;
	Rectf GetMenuButtonShape() const;

private:
	Texture m_GameText;
	Texture m_OverText;
	Texture m_Background;

	Texture m_TryAgainText;
	Texture m_MenuText;

	Rectf m_TryAgainButtonShape;
	Rectf m_MenuButtonShape;

	bool m_TryAgainHoovering;
	bool m_MenuHoovering;
};

