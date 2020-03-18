#include "stdafx.h"
#include "Game.h"
#include "utils.h"

Game::Game( const Window& window )
	:m_Window{ window }
	, m_Camera{360.0f, 360.f}
	, m_Water{5.0f, Point2f(m_Level.GetBoundaries().width,50)}
	, m_Enemy{ Point2f{500, 400}, 300, 550 }
	, m_Finnish{ {820.0f, 838.0f} }
	, m_IsFinnished{false}
	, m_UI{}
	, m_Titlescreen{}
	, m_MousePos{}
	, m_GameState{GameState::Menu}
	, m_GameOver{false}
	, m_LevelMusic{ "Resources/Sound/Level.mp3" }
	, m_VictoryMusic{"Resources/Sound/Victory.mp3"}
{	 
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	AddPowerUps( );
	m_Camera.SetBoundaries(m_Level.GetBoundaries());
}

void Game::Cleanup( )
{
}

void Game::Update( float elapsedSec )
{
	if(!m_GameOver)m_Titlescreen.IsGameOver(m_Avatar.GetCurrentHp());
	if (m_Avatar.GetCurrentHp() <= 0)
	{
		m_GameOver = true;
		m_GameState = GameState::Menu;
	}

	if (m_IsFinnished)
	{
		if (m_LevelMusic.IsPlaying()) m_LevelMusic.Stop();
		if (!m_VictoryMusic.IsPlaying()) m_VictoryMusic.Play(true);
	}

	if (m_GameOver)
	{
		m_Water.Reset();
		m_Avatar.Reset();
		m_Enemy.Reset();
		m_UI.Reset();
		m_PowerUpManager.Reset();
		m_Level.Reset();
		m_IsFinnished = false;
		m_GameOver = false;
	}

	if (m_Titlescreen.IsPlayPressed()) m_GameState = GameState::PlayGame;
	m_IsFinnished = m_Finnish.IsFinnished(m_Avatar.GetShape());
	if (m_GameState == GameState::PlayGame && !m_GameOver)
	{
		// Update game objects
		if (!m_IsFinnished)
		{
			if (!m_LevelMusic.IsPlaying()) m_LevelMusic.Play(true);
			m_Avatar.Update(elapsedSec, m_Level, m_Water);
			m_Water.Update(elapsedSec);
			m_Level.Update(elapsedSec, m_PowerUpManager.GetType());
			m_Enemy.Update(elapsedSec, m_Avatar.GetShape(), m_Level, m_Water);
			m_Finnish.Update(elapsedSec);

			m_UI.Update(elapsedSec, m_Avatar.GetCurrentHp(), m_Avatar.GetMaxHp(), m_Avatar.GetCurrentSecUnderwater(), m_Avatar.GetMaxSecUnderwater(), m_PowerUpManager.GetType());

			// Do collision
			DoCollisions();
		}
	}
	else if (m_GameState == GameState::Menu) m_Titlescreen.Update(elapsedSec);
}

void Game::Draw( ) const
{
	ClearBackground( );
	
	if (m_GameState == GameState::PlayGame)
	{
		glPushMatrix();
		glTranslatef(-m_Camera.GetPosition(m_Avatar.GetShape()).x, -m_Camera.GetPosition(m_Avatar.GetShape()).y, 0.0f);
		m_Level.DrawBackground();
		m_Finnish.Draw();
		m_PowerUpManager.Draw();
		m_Enemy.Draw();
		m_Avatar.Draw();
		m_Water.Draw();
		glPopMatrix();

		m_UI.Draw();
	}

	if (m_IsFinnished)
	{
		glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
		utils::FillRect({ 0.0f, 0.0f, m_Level.GetBoundaries().width, m_Level.GetBoundaries().height });
	}

	if (m_GameState == GameState::Menu) m_Titlescreen.Draw();

}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	int soundVol{ 5 };
	switch (e.keysym.sym)
	{
	case SDLK_u:
		m_Avatar.SetVolumeUp(soundVol);
		m_Enemy.SetVolumeUp(soundVol);
		utils::SetVolumeUp(m_LevelMusic, soundVol);
		utils::SetVolumeUp(m_VictoryMusic, soundVol);
		m_Titlescreen.SetVolumeUp(soundVol);
		m_PowerUpManager.SetVolumeUp(soundVol);
		break;

	case SDLK_d:
		m_Avatar.SetVolumeDown(soundVol);
		m_Enemy.SetVolumeDown(soundVol);
		utils::SetVolumeDown(m_LevelMusic, soundVol);
		utils::SetVolumeDown(m_VictoryMusic, soundVol);
		m_Titlescreen.SetVolumeDown(soundVol);
		m_PowerUpManager.SetVolumeDown(soundVol);
		break;
	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	m_MousePos.x = float(e.x);
	m_MousePos.y = 360.0f - float(e.y);

	if (m_GameState == GameState::Menu) m_Titlescreen.IsHoovering(m_MousePos);
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	if (m_GameState == GameState::Menu) m_Titlescreen.ProcessMouseDownEvent(e, m_MousePos);
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}




void Game::AddPowerUps( )
{
	m_PowerUpManager.AddItem( { 600.0f, 500 - 250.0f }, Type::red );
	m_PowerUpManager.AddItem( { 620.0f, 450 }, Type::blue );
	m_PowerUpManager.AddItem( { 200.0f, 870 }, Type::yellow );
	m_PowerUpManager.AddItem({ 570.0f, 650 }, Type::red);
	m_PowerUpManager.AddItem({ 650.0f, 750 }, Type::yellow);
}

void Game::DoCollisions( )
{
	m_PowerUpManager.HitItem(m_Avatar.GetShape());
	m_Enemy.SetHit(m_Avatar.HitEnemy(m_Enemy.GetShape(), m_Enemy.IsDead()));
	m_Avatar.SetHit(m_Enemy.HitEnemy(m_Avatar.GetShape()));
}

