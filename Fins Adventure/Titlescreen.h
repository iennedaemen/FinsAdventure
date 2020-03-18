#pragma once
#include "Texture.h"
#include "structs.h"
#include "Infoscreen.h"
#include "GameOverScreen.h"
#include "SoundStream.h"
#include "SoundEffect.h"

class Titlescreen
{
public:
	enum class MenuState
	{
		Tiltlescreen,
		Infoscreen,
		GameOver,
		PlayingGame
	};

	Titlescreen();
	
	void Draw() const;
	void Update(float elapsedSec);

	void IsHoovering(const Point2f& mousePos);
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e, const Point2f& mousePos);

	bool IsPlayPressed();
	void IsGameOver(const int& hp);

	void SetVolumeDown(const int& decrease);
	void SetVolumeUp(const int& increase);

private:
	Infoscreen m_Infoscreen;
	GameOverScreen m_GameOverScreen;

	MenuState m_MenuState;

	Texture m_TitlePart1;
	Texture m_TitlePart2;
	Texture m_InfoText;
	Texture m_PlayText;

	Rectf m_InfoButtonShape;
	Rectf m_PlayButtonShape;

	Texture m_Background;
	Texture m_Pinguin;

	bool m_InfoHoovering;
	bool m_PlayHoovering;

	float m_SecAfterClick;

	SoundStream m_MenuMusic;
	SoundStream m_GameOverMusic;
	SoundEffect m_ClickSound;
};

