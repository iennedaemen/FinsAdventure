#include "stdafx.h"
#include "Enemy.h"
#include "utils.h"


Enemy::Enemy(Point2f pos, float movePosMin, float movePosMax)
	: m_SpriteSheetLeft{0.0f}
	, m_SpriteSheetTop{0.0f}
	, m_SpriteSheetWidth{585.0f}
	, m_SpriteSheetHeight{455.0f}
	, m_Cols{10}
	, m_Rows{7}
	, m_FramesPerSec{10}
	, m_FrameTime{}
	, m_Scale{0.5f}
	, m_StartPosition{pos}

	, m_Texture{ "Resources/Fox.png" }

	, m_ActionState{ ActionState::idle}

	, m_GoLeft{false}
	, m_Velocity{0.0f, 0.0f}
	, m_TempVelocity{50.0f, 0.0f}
	, m_Acceleration{ 0,-981.0f }

	, m_MoveMin{movePosMin}
	, m_MoveMax{movePosMax}

	, m_SecSinceAttack{0.0f}
	, m_Attack{false}
	, m_AttackRange{180.0f}
	, m_AttackPerSec{1.0f}	

	, m_Hit{false}

	, m_MaxHp{5}

	, m_IsDead{false}

	, m_WaterAcceleration{ 0, -581.0f }
	, m_WaterHorSpeed{100.0f}
	, m_SecUnderwater{0.0f}

	, m_SecHurt{0.0f}

	, m_HurtSound{"Resources/Sound/Hurt.mp3"}

{
	m_Hp = m_MaxHp;

	m_Shape = { m_StartPosition.x, m_StartPosition.y, m_Texture.GetWidth() / 6 * m_Scale, m_Texture.GetHeight() / 3 * m_Scale};
	m_FrameTime = 1.0f / m_FramesPerSec;

	m_SrcRect.width = m_SpriteSheetWidth / m_Cols;
	m_SrcRect.height = m_SpriteSheetHeight / m_Rows;
}

void Enemy::Draw() const
{
	if (!m_IsDead)
	{
		for (size_t i = 0; i < m_Attacks.size(); i++)
		{
			m_Attacks[i]->Draw();
		}

		if (m_GoLeft)
		{
			glPushMatrix();
			glTranslatef(m_Shape.left + m_Shape.width / 2, m_Shape.bottom, 0);
			glScalef(-1.0f, 1.0f, 1.0f);
			glTranslatef(-m_Shape.left - m_Shape.width / 2, -m_Shape.bottom, 0);
			m_Texture.Draw(m_Shape, m_SrcRect);
			glPopMatrix();
		}
		else
		{
			glPushMatrix();
			glTranslatef(m_Shape.left + m_Shape.width / 2, m_Shape.bottom, 0);
			glScalef(1.0f, 1.0f, 1.0f);
			glTranslatef(-m_Shape.left - m_Shape.width / 2, -m_Shape.bottom, 0);
			m_Texture.Draw(m_Shape, m_SrcRect);
			glPopMatrix();
		}
	}
}

void Enemy::Update(float elapsedSec, const Rectf& actorShape, const Level& level, const Water& water)
{	
	if (!m_IsDead)
	{
		m_AccuSec += elapsedSec;

		if (m_AccuSec > m_FrameTime)
		{
			++m_CurFrame;
			m_AccuSec -= m_FrameTime;
		}

		for (size_t i = 0; i < m_Attacks.size(); i++)
		{
			m_Attacks[i]->Update(elapsedSec, m_GoLeft);
		}
		
		if(m_ActionState == ActionState::dead)
		{
			int frames = 5;
			UpdateSourceRect();
			if (m_CurFrame >= frames)
				m_IsDead = true;
			return;
		}

		Point2f leftBottom{ m_Shape.left, m_Shape.bottom };
		Point2f rightBottom{ m_Shape.left, m_Shape.bottom + m_Shape.width };

		Rectf attackRangeLeft{ m_Shape.left + m_Shape.width / 2 - m_AttackRange, m_Shape.bottom, m_AttackRange, m_Shape.height };
		Rectf attackRangeRight{ m_Shape.left + m_Shape.width / 2, m_Shape.bottom, m_AttackRange, m_Shape.height };

		m_Velocity.y += m_Acceleration.y * elapsedSec;
		int frames = 0;

		if (water.Drowning(m_Shape)) m_SecUnderwater += elapsedSec;
		else m_SecUnderwater = 0.0f;

		if (m_Hit && (m_ActionState != ActionState::dead || m_ActionState != ActionState::hurt))
		{
			m_Switch = true;
			--m_Hp;
			m_HurtSound.Play(0);
			if (m_Hp <= 0)
				m_ActionState = ActionState::dead;
			else
				m_ActionState = ActionState::hurt;
		}
		else if (m_SecUnderwater >= 5.0f)
		{
			m_Switch = true;
			m_ActionState = ActionState::dead;
		}
		else if (utils::IsOverlapping(actorShape, attackRangeLeft) || utils::IsOverlapping(actorShape, attackRangeRight))
		{
			if(m_ActionState != ActionState::hurt)
			m_ActionState = ActionState::attack;
		}

		if (!m_Attack)
		{
			if (m_Velocity.x > 0) m_GoLeft = false;
			else if (m_Velocity.x < 0) m_GoLeft = true;
		}

		switch (m_ActionState)
		{
		case ActionState::idle:
			UpdateSourceRect();
			m_SecSinceAttack += elapsedSec;
			if (m_SecSinceAttack >= 4)
			{
				m_SecSinceAttack = 0.0f;
				m_ActionState = ActionState::move;
			}
			break;

		case ActionState::move:
			UpdateSourceRect();

			if ((m_MoveMin >= leftBottom.x && m_Velocity.x < 0) || (m_MoveMax <= rightBottom.x && m_Velocity.x > 0))
			{
				m_TempVelocity.x = -m_TempVelocity.x;
			}
			m_Velocity.x = m_TempVelocity.x;

			if (utils::IsOverlapping(actorShape, attackRangeLeft))
			{
				m_ActionState = ActionState::attack;
			}
			else if (utils::IsOverlapping(actorShape, attackRangeRight))
			{
				m_ActionState = ActionState::attack;
			}

			break;

		case ActionState::dead:
			frames = 5;
			UpdateSourceRect();
			if(m_CurFrame >= frames)
				m_IsDead = true;
			break;

		case ActionState::attack:
			m_Velocity = { 0.0f, 0.0f };
			m_Attack = true;

			UpdateSourceRect();

			if (utils::IsOverlapping(actorShape, attackRangeLeft)) m_GoLeft = true;
			else if (utils::IsOverlapping(actorShape, attackRangeRight)) m_GoLeft = false;

			m_AttackPerSec += elapsedSec;
			if (m_AttackPerSec >= 1.0f)
			{
				CreateAttack();
				m_AttackPerSec = 0.0f;
			}
			if (!utils::IsOverlapping(actorShape, attackRangeLeft) && !utils::IsOverlapping(actorShape, attackRangeRight))
			{
				m_Attack = false;
				m_AttackPerSec = 0.3f;
				m_ActionState = ActionState::idle;
			}

			break;

		case ActionState::hurt:
			frames = 4;
			UpdateSourceRect();
			if (utils::IsOverlapping(actorShape, attackRangeLeft) && utils::IsOverlapping(actorShape, attackRangeRight))
			{
				if (m_CurFrame >= frames - 1)
				{
					--m_Hp;
					m_Hit = false;
					m_SecHurt = 0;
					m_ActionState = ActionState::attack;
				}
			}
			else if (m_CurFrame >= frames - 1)
			{
				--m_Hp;
				m_Hit = false;
				m_SecHurt = 0;
				m_ActionState = ActionState::idle;
			}
			
			break;
		}

		m_Shape.bottom += m_Velocity.y * elapsedSec;
		m_Shape.left += m_Velocity.x * elapsedSec;

		level.HandleCollision(m_Shape, m_Velocity);
	}
}

void Enemy::UpdateSourceRect()
{
	if (!m_IsDead)
	{
		if (m_Switch)
		{
			m_CurFrame = 0;
			m_Switch = false;
		}
		
		switch (m_ActionState)
		{
		case ActionState::move:
			m_SrcRect.left = m_CurFrame * m_SrcRect.width;
			m_SrcRect.bottom = m_SpriteSheetTop + m_SrcRect.height * 2;
			break;

		case ActionState::idle:
			m_SrcRect.left = m_CurFrame * m_SrcRect.width;
			m_SrcRect.bottom = m_SpriteSheetTop + m_SrcRect.height;
			break;

		case ActionState::dead:
			m_SrcRect.left = m_CurFrame * m_SrcRect.width;
			m_SrcRect.bottom = m_SpriteSheetTop + m_SrcRect.height * 3;
			break;

		case ActionState::attack:
			if (m_CurFrame >= 1) m_CurFrame = 0;
			m_SrcRect.left = m_CurFrame * m_SrcRect.width;
			m_SrcRect.bottom = m_SpriteSheetTop + m_SrcRect.height * 7;
			break;

		case ActionState::hurt:
			m_SrcRect.left = m_CurFrame * m_SrcRect.width;
			m_SrcRect.bottom = m_SpriteSheetTop + m_SrcRect.height * 6;
			break;
		}
		
	}
}

void Enemy::CreateAttack()
{
	m_Attacks.push_back(std::make_unique<Attack>(Point2f{ m_Shape.left + m_Shape.width / 2.0f - 10.0f, m_Shape.bottom + m_Shape.height / 2.0f }, Attack::AttackType::fire));
}

Rectf Enemy::GetShape() const
{
	return m_Shape;
}

void Enemy::SetHit(const bool& hit)
{
	m_Hit = hit;
}

bool Enemy::HitEnemy(const Rectf& rect)
{
	for (size_t i = 0; i < m_Attacks.size(); i++)
	{
		if (m_Attacks[i]->IsOverlapping(rect))
		{
			std::swap(m_Attacks.back(), m_Attacks[i]);
			m_Attacks.pop_back();
			return true;
		}
	}
	return false;
}

void Enemy::DeleteAttack(const Rectf& boundaries)
{
	for (size_t i = 0; i < m_Attacks.size(); i++)
	{
		if (!utils::IsOverlapping(m_Attacks[i]->GetShape(), boundaries))
		{
			std::swap(m_Attacks.back(), m_Attacks[i]);
			m_Attacks.pop_back();
		}
	}
}

bool Enemy::IsDead() const
{
	return m_IsDead;
}

void Enemy::Reset()
{
	m_IsDead = false;
	m_ActionState = ActionState::move;
	m_Hp = m_MaxHp;
	m_SecUnderwater = 0.0f;
	m_Shape = { m_StartPosition.x, m_StartPosition.y, m_Shape.width, m_Shape.height };
}

void Enemy::SetVolumeDown(const int& decrease)
{
	utils::SetVolumeDown(m_HurtSound, decrease);
}

void Enemy::SetVolumeUp(const int& increase)
{
	utils::SetVolumeUp(m_HurtSound, increase);
}