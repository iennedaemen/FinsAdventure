#include "stdafx.h"
#include "Avatar.h"
#include <algorithm>


Avatar::Avatar()
	: m_Shape{50,280,240.0f/4.0,233.0f/4.0f}
	, m_HorSpeed{200.0f}
	, m_WaterHorSpeed{100.0f}
	, m_JumpSpeed{600.0f}
	, m_Acceleration{0,-981.0f}
	, m_WaterAcceleration{0, -581.0f}
	, m_ActionState{ActionState::move}
	, m_MaxSecUnderwater{ 5.0f }
	, m_SecUnderwater{0.0f}
	, m_MaxHp{10}
	, m_SecDrowning{0.0f}
	, m_SecSinceSpikeHit{0.0f}

	, m_SpritesTexture{"Resources/Avatar_SpriteSheet.png"}
	, m_SpriteSheetLeft{ 0.0f }
	, m_SpriteSheetTop{ 0.0f }
	, m_SpriteSheetWidth{ 200.0f }
	, m_SpriteSheetHeight{ 272.0f }
	, m_Cols{ 4 }
	, m_Rows{ 4 }
	, m_FramesPerSec{ 5 }
	, m_Scale{ 1.0f }
	, m_AccuSec{}
	, m_CurFrame{}
	, m_SrcRect{}

	, m_AttackSec{0.0f}
	, m_GoLeft{false}
	, m_Hit{false}

	, m_HurtSound{"Resources/Sound/Hurt.mp3"}
	, m_JumpSound{"Resources/Sound/Jump.mp3"}
{
	m_Hp = m_MaxHp;

	m_FrameTime = 1.0f / m_FramesPerSec;

	m_SrcRect.width = m_SpriteSheetWidth / m_Cols;
	m_SrcRect.height = m_SpriteSheetHeight / m_Rows;
}

void Avatar::Update(float elapsedSec, const Level& level, const Water& water)
{
	Clamp(level);
	Point2f bottomLeftPos{ m_Shape.left, m_Shape.bottom };
	m_Velocity.y += m_Acceleration.y * elapsedSec;

	// Texture
	m_AccuSec += elapsedSec;
	if (m_AccuSec > m_FrameTime)
	{
		++m_CurFrame;
		m_AccuSec -= m_FrameTime;
		UpdateSourceRect(level);
	}


	// Water
	m_SecDrowning += elapsedSec;
	if (water.Drowning(m_Shape)) m_SecUnderwater += elapsedSec;
	else if(!water.Drowning(m_Shape) && m_SecUnderwater > 0) m_SecUnderwater -= elapsedSec;
	else m_SecUnderwater = 0.0f;

	if (m_Hp <= 0) m_ActionState = ActionState::dead;
	else if (m_Hit)
	{
		m_HurtSound.Play(0);
		m_ActionState = ActionState::hurt;
	}
	else if (m_SecUnderwater >= m_MaxSecUnderwater)
	{
		if (m_SecDrowning >= 1.0f)
		{
			m_SecDrowning = 0.0f;
			--m_Hp;
		}
	}


	// Spikes
	m_SecSinceSpikeHit += elapsedSec;
	if (level.IsOnSpikes(m_Shape, m_Velocity))
	{
		if (m_Hp > 0)
		{
			
			if (bottomLeftPos.y + m_Shape.height >= level.GetBoundaries().bottom + level.GetBoundaries().height)
			{
				m_Velocity.y = 0.0f;
				bottomLeftPos.y = level.GetBoundaries().bottom + level.GetBoundaries().height - m_Shape.height;
			}
			if (level.IsOnGround(m_Shape, m_Velocity)) m_Velocity.x = -m_Velocity.x * 10;
			else m_Velocity.y = m_JumpSpeed;
			if (m_SecSinceSpikeHit >= 1.0f)
			{
				m_SecSinceSpikeHit = 0.0f;
				m_HurtSound.Play(0);
				m_ActionState = ActionState::hurt;
			}
			
		}
	}

	// Attacks
	if (m_Velocity.x > 0) m_GoLeft = false;
	if (m_Velocity.x < 0) m_GoLeft = true;
	for (size_t i = 0; i < m_Attack.size(); i++)
	{
		m_Attack[i]->Update(elapsedSec, m_GoLeft);
	}
	DeleteAttack(level.GetBoundaries());

	// Actionstates
	switch (m_ActionState)
	{
	case ActionState::move:
		if (!SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_LEFT] && !SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_RIGHT] && level.IsOnGround(m_Shape, m_Velocity))
		{
			m_Velocity.x = 0;
			m_ActionState = ActionState::idle;
		}

		if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_LEFT] && SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_RIGHT])
		{
			m_Velocity.x = 0;
		}

		if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_LEFT])
		{
			if (bottomLeftPos.x <= level.GetBoundaries().left)
			{
				m_Velocity.x = 0.0f;
				bottomLeftPos.x = level.GetBoundaries().left;
			}
			else if (water.IsInWater(m_Shape)) m_Velocity.x = -m_WaterHorSpeed;
			else m_Velocity.x = -m_HorSpeed;
		}

		if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_RIGHT])
		{
			if (bottomLeftPos.x + m_Shape.width >= level.GetBoundaries().left + level.GetBoundaries().width)
			{
				m_Velocity.x = 0.0f;
				bottomLeftPos.x = level.GetBoundaries().left + level.GetBoundaries().width - m_Shape.width;
			}
			else if (water.IsInWater(m_Shape)) m_Velocity.x = m_WaterHorSpeed;
			else m_Velocity.x = m_HorSpeed;
		}

		if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_UP])
		{
			if (level.IsOnGround(m_Shape, m_Velocity) || level.IsOnColorPlatform(m_Shape, m_Velocity))
			{
				if (bottomLeftPos.y + m_Shape.height >= level.GetBoundaries().bottom + level.GetBoundaries().height)
				{
					m_Velocity.y = 0.0f;
					bottomLeftPos.y = level.GetBoundaries().bottom + level.GetBoundaries().height - m_Shape.height;
				}
				m_Velocity.y = m_JumpSpeed;
				m_JumpSound.Play(0);
				m_ActionState = ActionState::jump;
			}
		}

		// Attack
		if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_SPACE])
		{
			m_Velocity.x = 0.0f;
			m_ActionState = ActionState::attack;
		}

		break;

	case ActionState::idle:
		if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_LEFT])
		{
			if (bottomLeftPos.x <= level.GetBoundaries().left)
			{
				m_Velocity.x = 0.0f;
				bottomLeftPos.x = level.GetBoundaries().left;
			}
			else if (water.IsInWater(m_Shape)) m_Velocity.x -= m_WaterHorSpeed;
			else m_Velocity.x = -m_HorSpeed;
			m_ActionState = ActionState::move;
		}
		else if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_RIGHT])
		{
			if (bottomLeftPos.x + m_Shape.width >= level.GetBoundaries().left + level.GetBoundaries().width)
			{
				m_Velocity.x = 0.0f;
				bottomLeftPos.x = level.GetBoundaries().left + level.GetBoundaries().width - m_Shape.width;
			}
			else if (water.IsInWater(m_Shape)) m_Velocity.x = m_WaterHorSpeed;
			else m_Velocity.x = m_HorSpeed;
			m_ActionState = ActionState::move;
		}
		else if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_UP])
		{
			if (level.IsOnGround(m_Shape, m_Velocity))
			{
				if (bottomLeftPos.y + m_Shape.height >= level.GetBoundaries().bottom + level.GetBoundaries().height)
				{
					m_Velocity.y = 0.0f;
					bottomLeftPos.y = level.GetBoundaries().bottom + level.GetBoundaries().height - m_Shape.height;
				}
				m_Velocity.y = m_JumpSpeed;
				m_JumpSound.Play(0);
				m_ActionState = ActionState::jump;
			}
		}
		else if (level.IsOnColorPlatform(m_Shape, m_Velocity))
		{
			m_Velocity.x = level.GetColorPlatformVelocity(m_Shape, m_Velocity).x;
		}
		else m_Velocity.x = 0;


		// Attack
		if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_SPACE])
		{
			m_Velocity.x = 0.0f;
			m_ActionState = ActionState::attack;
		}	
		break;

	case ActionState::dead:
		m_Velocity = { 0.0f, 0.0f };
		UpdateSourceRect(level);
		std::cout << "dead\n";
		break;

	case ActionState::jump:
		if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_RIGHT])
		{
			if (bottomLeftPos.x + m_Shape.width >= level.GetBoundaries().left + level.GetBoundaries().width)
			{
				m_Velocity.x = 0.0f;
				bottomLeftPos.x = level.GetBoundaries().left + level.GetBoundaries().width - m_Shape.width;
			}
			else m_Velocity.x = m_HorSpeed;
		}
		if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_LEFT])
		{
			if (bottomLeftPos.x <= level.GetBoundaries().left)
			{
				m_Velocity.x = 0.0f;
				bottomLeftPos.x = level.GetBoundaries().left;
			}
			else m_Velocity.x = -m_HorSpeed;
		}
		if (level.IsOnGround(m_Shape, m_Velocity) || level.IsOnColorPlatform(m_Shape, m_Velocity))
		{
			if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_LEFT] || SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_RIGHT])
				m_ActionState = ActionState::move;
			else m_ActionState = ActionState::idle;
		}
		break;

	case ActionState::attack:
		
		m_AttackSec += elapsedSec;
		if (m_AttackSec >= 0.3f)
		{
			CreateAttack();
			if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_LEFT] || SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_RIGHT])
			{
				m_AttackSec = 0.3f;
				m_ActionState = ActionState::move;
			}
			else
			{
				m_AttackSec = 0.3f;
				m_ActionState = ActionState::idle;
			}
			m_AttackSec = 0.0f; 
		}

		break;
		

	case ActionState::hurt:
		--m_Hp;
		UpdateSourceRect(level);
		if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_LEFT] || SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_RIGHT])
			m_ActionState = ActionState::move;
		else m_ActionState = ActionState::idle;
		
		break;
	}

	m_Shape.bottom += m_Velocity.y * elapsedSec;
	m_Shape.left += m_Velocity.x * elapsedSec;
	level.HandleCollision(m_Shape, m_Velocity);
}

void Avatar::Draw() const
{
	for (size_t i = 0; i < m_Attack.size(); i++)
	{
		m_Attack[i]->Draw();
	}

	if ( m_GoLeft)
	{
		glPushMatrix();
		glTranslatef(m_Shape.left + m_Shape.width / 2, m_Shape.bottom, 0);
		glScalef(-1, 1, 1);
		glTranslatef(-m_Shape.left - m_Shape.width / 2, -m_Shape.bottom, 0);
		m_SpritesTexture.Draw(m_Shape, m_SrcRect);
		glPopMatrix();
	}
	else m_SpritesTexture.Draw(m_Shape, m_SrcRect);
}

Rectf Avatar::GetShape() const
{
	return m_Shape;
}

void Avatar::UpdateSourceRect(const Level& level)
{
	switch (m_ActionState)
	{
	case ActionState::move:
		m_SrcRect.left = m_CurFrame * m_SrcRect.width;
		m_SrcRect.bottom = m_SpriteSheetTop + m_SrcRect.height;
	break;

	case ActionState::idle:
		if (m_CurFrame >= 2) m_CurFrame = 0;
		m_SrcRect.left = m_CurFrame * m_SrcRect.width;
		m_SrcRect.bottom = m_SpriteSheetTop + m_SrcRect.height*2;
		break;

	case ActionState::jump:
		if (m_CurFrame >= 1) m_CurFrame = 0;
		m_SrcRect.left = m_CurFrame * m_SrcRect.width;
		m_SrcRect.bottom = m_SpriteSheetTop + m_SrcRect.height * 4;
		break;

	case ActionState::dead:
		if (m_CurFrame >= 1) m_CurFrame = 0;
		m_SrcRect.left = m_CurFrame * m_SrcRect.width;
		m_SrcRect.bottom = m_SpriteSheetTop + m_SrcRect.height * 3;
		break;

	case ActionState::attack:
		if (m_CurFrame >= 1) m_CurFrame = 0;
		m_SrcRect.left = m_CurFrame * m_SrcRect.width;
		m_SrcRect.bottom = m_SpriteSheetTop + m_SrcRect.height * 4;
		break;

	case ActionState::hurt:
		if (m_CurFrame >= 1) m_CurFrame = 0;
		m_SrcRect.left = m_CurFrame * m_SrcRect.width;
		m_SrcRect.bottom = m_SpriteSheetTop + m_SrcRect.height * 3;
		break;
	}
}

void Avatar::Clamp(const Level& level)
{
	Point2f bottomLeftPos{ m_Shape.left, m_Shape.bottom };

	if (bottomLeftPos.x <= level.GetBoundaries().left)
	{
		m_Velocity.x = 0.0f;
		bottomLeftPos.x = level.GetBoundaries().left;
	}
	if (bottomLeftPos.x + m_Shape.width > level.GetBoundaries().left + level.GetBoundaries().width)
	{
		m_Velocity.x = 0.0f;
		bottomLeftPos.x = level.GetBoundaries().left + level.GetBoundaries().width - m_Shape.width;
	}
	if (bottomLeftPos.y < level.GetBoundaries().bottom)
	{
		m_Velocity.y = 0.0f;
		bottomLeftPos.y = level.GetBoundaries().bottom;
	}
	if (bottomLeftPos.y + m_Shape.height > level.GetBoundaries().bottom + level.GetBoundaries().height)
	{
		m_Velocity.y = 0.0f;
		bottomLeftPos.y = level.GetBoundaries().bottom + level.GetBoundaries().height - m_Shape.height;
	}
}

void Avatar::CreateAttack()
{
	m_Attack.push_back(std::make_unique<Attack>( Point2f{ m_Shape.left + m_Shape.width / 2.0f - 10.0f, m_Shape.bottom + m_Shape.height / 2.0f }, Attack::AttackType::water));
}

bool Avatar::HitEnemy(const Rectf& rect, const bool& isDead)
{
	if (isDead) return false;

	for (size_t i = 0; i < m_Attack.size(); i++)
	{
		if (m_Attack[i]->IsOverlapping(rect))
		{
			std::swap(m_Attack.back(), m_Attack[i]);
			m_Attack.pop_back();
			return true;
		}
	}
	return false;
}

void Avatar::SetHit(const bool& hit)
{
	m_Hit = hit;
}

void Avatar::DeleteAttack(const Rectf& boundaries)
{
	for (size_t i = 0; i < m_Attack.size(); i++)
	{
		if (!utils::IsOverlapping(m_Attack[i]->GetShape(), boundaries))
		{
			std::swap(m_Attack.back(), m_Attack[i]);
			m_Attack.pop_back();
		}
	}
}

int Avatar::GetCurrentHp() const
{
	return m_Hp;
}

int Avatar::GetMaxHp() const
{
	return m_MaxHp;
}

float Avatar::GetMaxSecUnderwater() const
{
	return m_MaxSecUnderwater;
}

float Avatar::GetCurrentSecUnderwater() const
{
	return m_SecUnderwater;
}

void Avatar::Reset()
{
	m_ActionState = ActionState::move;
	m_Hp = m_MaxHp;
	m_SecUnderwater = 0.0f;
	m_Shape = { 50,280,240.0f / 4.0,233.0f / 4.0f };
	m_Velocity = { 0.0f, 0.0f };
}

void Avatar::SetVolumeDown(const int& decrease)
{
	utils::SetVolumeDown(m_HurtSound, decrease);
	utils::SetVolumeDown(m_JumpSound, decrease);
}

void Avatar::SetVolumeUp(const int& increase)
{
	utils::SetVolumeUp(m_HurtSound, increase);
	utils::SetVolumeUp(m_JumpSound, increase);
}