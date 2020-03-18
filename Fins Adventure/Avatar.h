#pragma once
#include "structs.h"
#include "Vector2f.h"
#include "Level.h"
#include "Water.h"
#include "Attack.h"
#include <vector>
#include <memory>
#include "Enemy.h"
#include "SoundEffect.h"

class Avatar
{
public:
	Avatar();

	void Update(float elapsedSec, const Level& level, const Water& water);
	void Draw() const;
	void UpdateSourceRect(const Level& level);
	bool HitEnemy(const Rectf& rect, const bool& isDead);

	Rectf GetShape() const;
	int GetCurrentHp()const;
	int GetMaxHp()const;
	float GetMaxSecUnderwater() const;
	float GetCurrentSecUnderwater() const;

	void SetHit(const bool& hit);

	void Reset();

	void SetVolumeDown(const int& decrease);
	void SetVolumeUp(const int& increase);

private:
	// data members
	Rectf m_Shape;

	float m_HorSpeed;
	float m_JumpSpeed;
	Vector2f m_Velocity;
	Vector2f m_Acceleration;
	ActionState m_ActionState;

	int m_Hp;
	int m_MaxHp;

	float m_SecSinceSpikeHit;
	float m_AccuSec;

	Vector2f m_WaterAcceleration;
	float m_WaterHorSpeed;
	float m_SecUnderwater;
	float m_MaxSecUnderwater;
	float m_SecDrowning;

	Texture m_SpritesTexture;
	const float m_SpriteSheetLeft;
	const float m_SpriteSheetTop;
	const float m_SpriteSheetWidth;
	const float m_SpriteSheetHeight;
	const int m_Cols;
	const int m_Rows;
	int m_FramesPerSec;
	float m_FrameTime;
	float m_Scale;
	int m_CurFrame;
	Rectf m_SrcRect;

	std::vector<std::unique_ptr<Attack>> m_Attack;
	float m_AttackSec;

	bool m_Hit;
	bool m_GoLeft;

	SoundEffect m_HurtSound;
	SoundEffect m_JumpSound;

	// helper functions
	void Clamp(const Level& level);
	void CreateAttack();
	void DeleteAttack(const Rectf& boundaries);
};

