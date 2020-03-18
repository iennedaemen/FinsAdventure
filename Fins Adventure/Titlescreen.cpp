#include "stdafx.h"
#include "Titlescreen.h"
#include "utils.h"

Titlescreen::Titlescreen()
	: m_TitlePart1 { "Fins","Resources/soupofjustice.ttf", 55 ,Color4f{ 0.1f, 0.3f, 1.0f, 1.0f } }
	, m_TitlePart2{ "Adventure!","Resources/soupofjustice.ttf", 55 ,Color4f{ 0.1f, 0.3f, 1.0f, 1.0f } }
	, m_InfoText{ "INFO","Resources/soupofjustice.ttf", 50 ,Color4f{ 1.0f, 1.0f, 1.0f, 1.0f } }
	, m_PlayText{ "PLAY","Resources/soupofjustice.ttf", 50 ,Color4f{ 1.0f, 1.0f, 1.0f, 1.0f } }
	, m_InfoButtonShape{105, 75, 150, 50}
	, m_PlayButtonShape{105, 150, 150, 50}
	, m_InfoHoovering{false}
	, m_PlayHoovering{false}
	, m_Background{"Resources/Titlescreen_Background.jpg"}
	, m_Infoscreen{}
	, m_GameOverScreen{}
	, m_MenuState{MenuState::Tiltlescreen}
	, m_SecAfterClick{0.0f}
	, m_MenuMusic{"Resources/Sound/Menu.mp3"}
	, m_GameOverMusic{ "Resources/Sound/GameOver.mp3" }
	, m_ClickSound{"Resources/Sound/Click.mp3"}
	, m_Pinguin{"Resources/Pinguin.png"}

{
}

void Titlescreen::Draw() const
{
	if (m_MenuState == MenuState::Tiltlescreen)
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

		// Title
		m_TitlePart1.Draw(Point2f{ 125, 260 });
		m_TitlePart2.Draw(Point2f{ 40, 210 });

		// play button
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		utils::FillRect(m_PlayButtonShape.left - 1, m_PlayButtonShape.bottom - 1, m_PlayButtonShape.width + 2, m_PlayButtonShape.height + 2);
		if (m_PlayHoovering) glColor4f(0.0f, 0.2f, 0.9f, 1.0f);
		else glColor4f(0.1f, 0.3f, 1.0f, 1.0f);
		utils::FillRect(m_PlayButtonShape);
		m_PlayText.Draw(Point2f{ m_PlayButtonShape.left + 21, m_PlayButtonShape.bottom - 3 });

		// info button
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		utils::FillRect(m_InfoButtonShape.left - 1, m_InfoButtonShape.bottom - 1, m_InfoButtonShape.width + 2, m_InfoButtonShape.height + 2);
		if (m_InfoHoovering) glColor4f(0.0f, 0.2f, 0.9f, 1.0f);
		else glColor4f(0.1f, 0.3f, 1.0f, 1.0f);
		utils::FillRect(m_InfoButtonShape);
		m_InfoText.Draw(Point2f{ m_InfoButtonShape.left + 22, m_InfoButtonShape.bottom - 3 });
	}

	else if (m_MenuState == MenuState::Infoscreen) m_Infoscreen.Draw();
	else if (m_MenuState == MenuState::GameOver) m_GameOverScreen.Draw();
}

void Titlescreen::Update(float elapsedSec)
{
	m_SecAfterClick += elapsedSec;
	if (!m_MenuMusic.IsPlaying() && m_MenuState != MenuState::GameOver && m_MenuState != MenuState::PlayingGame) m_MenuMusic.Play(true);
}

void Titlescreen::IsHoovering(const Point2f& mousePos)
{
	if (m_MenuState == MenuState::Tiltlescreen)
	{
		if (utils::IsPointInRect(mousePos, m_InfoButtonShape)) m_InfoHoovering = true;
		else m_InfoHoovering = false;

		if (utils::IsPointInRect(mousePos, m_PlayButtonShape)) m_PlayHoovering = true;
		else m_PlayHoovering = false;
	}

	else if (m_MenuState == MenuState::Infoscreen) m_Infoscreen.IsHoovering(mousePos);
	else if (m_MenuState == MenuState::GameOver) m_GameOverScreen.IsHoovering(mousePos);
}

void Titlescreen::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e, const Point2f& mousePos)
{
	if (e.button == SDL_BUTTON_LEFT)
	{
		if (utils::IsPointInRect(mousePos, m_InfoButtonShape) && m_MenuState == MenuState::Tiltlescreen && m_SecAfterClick > 0.2f)
		{
			m_ClickSound.Play(0);
			m_SecAfterClick = 0.0f;
			m_MenuState = MenuState::Infoscreen;
		}
		if (utils::IsPointInRect(mousePos, m_PlayButtonShape) && m_MenuState == MenuState::Tiltlescreen && m_SecAfterClick > 0.2f)
		{
			m_ClickSound.Play(0);
			if (m_MenuMusic.IsPlaying()) m_MenuMusic.Stop();
			m_SecAfterClick = 0.0f;
			m_MenuState = MenuState::PlayingGame;
		}
		if (utils::IsPointInRect(mousePos, m_Infoscreen.GetBackButtonShape()) && m_MenuState == MenuState::Infoscreen && m_SecAfterClick > 0.2f)
		{
			m_ClickSound.Play(0);
			m_SecAfterClick = 0.0f;
			m_MenuState = MenuState::Tiltlescreen;
		}
		if (utils::IsPointInRect(mousePos, m_GameOverScreen.GetMenuButtonShape()) && m_MenuState == MenuState::GameOver && m_SecAfterClick > 0.2f)
		{
			m_ClickSound.Play(0);
			if (m_GameOverMusic.IsPlaying()) m_GameOverMusic.Stop();
			m_SecAfterClick = 0.0f;
			m_MenuState = MenuState::Tiltlescreen;
		}
		if (utils::IsPointInRect(mousePos, m_GameOverScreen.GetTryAgainButtonShape()) && m_MenuState == MenuState::GameOver && m_SecAfterClick > 0.2f)
		{
			m_ClickSound.Play(0);
			if (m_GameOverMusic.IsPlaying()) m_GameOverMusic.Stop();
			m_SecAfterClick = 0.0f;
			m_MenuState = MenuState::PlayingGame;
		}
	}
}

bool Titlescreen::IsPlayPressed()
{
	if(m_MenuState == MenuState::PlayingGame) return true;
	return false;
}

void Titlescreen::IsGameOver(const int& hp)
{
	if (hp <= 0)
	{
		m_MenuState = MenuState::GameOver;
		m_GameOverMusic.Play(true);
	}
}

void Titlescreen::SetVolumeDown(const int& decrease)
{
	utils::SetVolumeDown(m_ClickSound, decrease);
	utils::SetVolumeDown(m_GameOverMusic, decrease);
	utils::SetVolumeDown(m_MenuMusic, decrease);
}

void Titlescreen::SetVolumeUp(const int& increase)
{
	utils::SetVolumeUp(m_ClickSound, increase);
	utils::SetVolumeUp(m_GameOverMusic, increase);
	utils::SetVolumeUp(m_MenuMusic, increase);
}