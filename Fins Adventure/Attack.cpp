#include "stdafx.h"
#include "Attack.h"
#include "utils.h"

std::unique_ptr<Texture> Attack::m_pTexWater;
std::unique_ptr<Texture> Attack::m_pTexFire;

Attack::Attack(Point2f playerPos, AttackType attackType)
	: m_AttackType{ attackType }
	, m_CurFrame{ 0 }
	, m_FrameTime{ 1.0f / 6.0f }
	, m_AccuSec{ 0.0f }
	, m_VelocityX{ 50 }
	, m_Attack{ false }

{
	if(m_pTexWater == nullptr) m_pTexWater = { std::make_unique<Texture>("Resources/Attack.png") };
	if (m_pTexFire == nullptr) m_pTexFire = { std::make_unique<Texture>("Resources/Attack2.png") };

	switch (m_AttackType)
	{
	case AttackType::water:
		m_Shape = { playerPos.x, playerPos.y, m_pTexWater->GetWidth() / 2, m_pTexWater->GetHeight() };
		m_SrcRect = { 0.0f , 0.0f , m_pTexWater->GetWidth() / 2, m_pTexWater->GetHeight() };
		break;
	case AttackType::fire:
		m_Shape = { playerPos.x, playerPos.y, m_pTexFire->GetWidth() / 8, m_pTexFire->GetHeight() };
		m_SrcRect = { 0.0f , 0.0f , m_pTexFire->GetWidth() / 8, m_pTexFire->GetHeight() };
		break;
	}
}


void Attack::Draw() const
{
	switch (m_AttackType)
	{
	case AttackType::water:
		if (m_VelocityX > 0 && m_Attack)
		{
			glPushMatrix();
			glTranslatef(m_Shape.left + m_Shape.width / 2, m_Shape.bottom, 0);
			glScalef(-1, 1, 1);
			glTranslatef(-m_Shape.left - m_Shape.width / 2, -m_Shape.bottom, 0);
			m_pTexWater->Draw(m_Shape, m_SrcRect);
			glPopMatrix();
		}
		else if (m_Attack ) m_pTexWater->Draw(m_Shape, m_SrcRect);
		break;

	case AttackType::fire:
		if (m_VelocityX < 0 && m_Attack)
		{
			glPushMatrix();
			glTranslatef(m_Shape.left + m_Shape.width / 2, m_Shape.bottom, 0);
			glScalef(-1, 1, 1);
			glTranslatef(-m_Shape.left - m_Shape.width / 2, -m_Shape.bottom, 0);
			m_pTexFire->Draw(m_Shape, m_SrcRect);
			glPopMatrix();
		}
		else if (m_Attack) m_pTexFire->Draw(m_Shape, m_SrcRect);
		break;
	}

}

void Attack::Update(float elapsedSec,  const bool& left)
{
	m_AccuSec += elapsedSec;
	if (m_AccuSec > m_FrameTime)
	{
		++m_CurFrame;
		m_AccuSec -= m_FrameTime;
		m_SrcRect.left = m_CurFrame * m_SrcRect.width;
	}

	if (!m_Attack)
	{
		m_Attack = true;
		if (!left) m_VelocityX = 150.0f;
		else m_VelocityX = -150.0f;
	}

	m_Shape.left += m_VelocityX * elapsedSec;
}

Rectf Attack::GetShape() const
{
	return m_Shape;
}

bool Attack::IsOverlapping(const Rectf& rect) const
{
	if (utils::IsOverlapping(rect, m_Shape)) return true;
	return false;
}
