#pragma once
#include "Level.h"
#include "PowerUpManager.h"
#include "Avatar.h"
#include "Vector2f.h"
#include "Camera.h"
#include "Water.h"
#include "Enemy.h"
#include "Finnish.h"
#include "UI.h"
#include "Titlescreen.h"

class Game
{
public:
	enum class GameState
	{
		Menu,
		PlayGame
	};

	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game(Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// DATA MEMBERS
	const Window m_Window;

	Level m_Level;
	PowerUpManager m_PowerUpManager;
	Avatar m_Avatar;
	Camera m_Camera;
	Water m_Water;
	Enemy m_Enemy;
	Finnish m_Finnish;
	UI m_UI;
	Titlescreen m_Titlescreen;

	bool m_IsFinnished;
	bool m_GameOver;

	Point2f m_MousePos;
	GameState m_GameState;

	SoundStream m_LevelMusic;
	SoundStream m_VictoryMusic;

	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;
	void AddPowerUps( );
	void DoCollisions( );
};