#pragma once
#include "structs.h"
#include "Texture.h"
#include <memory>

class Attack
{
public:
	enum class AttackType
	{
		water,
		fire
	};

	Attack(Point2f playerPos, AttackType attackType);
	
	void Draw() const;
	void Update(float elapsedSec, const bool& left);
	Rectf GetShape() const;
	bool IsOverlapping(const Rectf& rect) const;
	
	static std::unique_ptr<Texture> m_pTexWater;
	static std::unique_ptr<Texture> m_pTexFire;

private:
	AttackType m_AttackType;

	Rectf m_Shape;
	Rectf m_SrcRect;

	int m_CurFrame;
	float m_FrameTime;
	float m_AccuSec;

	float m_VelocityX;

	bool m_Attack;;
};

