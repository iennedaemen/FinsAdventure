#pragma once
#include "Texture.h"
#include "structs.h"
#include "Vector2f.h"
#include "utils.h"
#include "Attack.h"
#include <memory>
#include "Level.h"
#include "Avatar.h"
#include "Water.h"
#include "SoundEffect.h"

class Enemy
{
public:
	Enemy(Point2f posLeft, float movePosMin, float movePosMax);
	
	void Draw() const;
	void Update(float elapsedSec, const Rectf& actorShape, const Level& level, const Water& water);
	bool HitEnemy(const Rectf& rect);
	bool IsDead() const;

	Rectf GetShape() const;
	void SetHit(const bool& hit);

	void Reset();

	void SetVolumeDown(const int& decrease);
	void SetVolumeUp(const int& increase);

private:
	Texture m_Texture;
	Rectf m_Shape;
	Rectf m_SrcRect;
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

	float m_AccuSec;
	float m_SecHurt;

	bool m_GoLeft;
	Vector2f m_Velocity;
	Vector2f m_TempVelocity;
	ActionState m_ActionState;
	Vector2f m_Acceleration;
	float m_MoveMax;
	float m_MoveMin;

	Vector2f m_WaterAcceleration;
	float m_WaterHorSpeed;
	float m_SecUnderwater;

	std::vector<std::unique_ptr<Attack>> m_Attacks;
	float m_SecSinceAttack;
	bool m_Attack;
	float m_AttackRange;
	float m_AttackPerSec;

	int m_Hp;
	int m_MaxHp;
	bool m_Hit;

	bool m_IsDead;

	Point2f m_StartPosition;

	SoundEffect m_HurtSound;

	bool m_Switch{false };

	// helper functions
	void UpdateSourceRect();
	void CreateAttack();
	void DeleteAttack(const Rectf& boundaries);

};

