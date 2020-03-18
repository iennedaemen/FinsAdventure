#include "stdafx.h"
#include "ColorPlatform.h"
#include "utils.h"


ColorPlatform::ColorPlatform(Type type, PlatformType move, Point2f boundaryMin, Point2f boundaryMax)
	: m_TexPlatforms{ "Resources/ColorPlatforms.png" }
	, m_TexDisapaired{"Resources/ColorPlatformsOpacity.png"}

	, m_Type{type}
	, m_PlatformType{ move }

	, m_MoveSpeed{30.0f}
	, m_Velocity{50.0f, 50.0f}
	, m_TempVelocity{ 50.0f, 50.0f }

	, m_BoundaryMax{boundaryMax}
	, m_BoundaryMin{boundaryMin}

	, m_IsActive{false}
{
	m_SrcRect = { (m_TexPlatforms.GetWidth() / 4) * (float(m_Type) - 1) , 0.0f , m_TexPlatforms.GetWidth() / 4, m_TexPlatforms.GetHeight() };
	m_Shape = { boundaryMax.x - boundaryMin.x / 2, boundaryMax.y - boundaryMin.y / 2, m_TexPlatforms.GetWidth() / 4, m_TexPlatforms.GetHeight() };
}

ColorPlatform::ColorPlatform(Type type, Point2f pos)
	: m_TexPlatforms{ "Resources/ColorPlatforms.png" }
	, m_TexDisapaired{ "Resources/ColorPlatformsOpacity.png" }

	, m_Type{ type }
	, m_PlatformType{ disapairing }

	, m_MoveSpeed{ 30.0f }
	, m_Velocity{ 50.0f, 50.0f }
	, m_TempVelocity{ 50.0f, 50.0f }

	, m_BoundaryMax{ pos }
	, m_BoundaryMin{ pos }
	
	, m_IsActive{ false }
{
	m_SrcRect = { (m_TexPlatforms.GetWidth() / 4) * (float(m_Type) - 1) , 0.0f , m_TexPlatforms.GetWidth() / 4, m_TexPlatforms.GetHeight() };
	m_Shape = { m_BoundaryMax.x - m_BoundaryMin.x / 2, m_BoundaryMax.y - m_BoundaryMin.y / 2, m_TexPlatforms.GetWidth() / 4, m_TexPlatforms.GetHeight() };
}

void ColorPlatform::Draw() const
{
	if(!m_IsActive && m_PlatformType == disapairing) m_TexDisapaired.Draw(m_Shape, m_SrcRect);
	else m_TexPlatforms.Draw(m_Shape, m_SrcRect);
}		

void ColorPlatform::Update(float elapsedSec,const Type& powerUp)
{
	Point2f leftBottom{ m_Shape.left, m_Shape.bottom };
	Point2f rightBottom{ m_Shape.left, m_Shape.bottom + m_Shape.width };;

	switch (m_PlatformType)
	{
	case horizontal:
		m_Velocity.y = 0.0f;

		if (powerUp == m_Type)
		{
			if ((m_BoundaryMin.x >= leftBottom.x && m_TempVelocity.x < 0) || (m_BoundaryMax.x <= rightBottom.x && m_TempVelocity.x > 0))
			{
				m_TempVelocity.x = -m_TempVelocity.x;
			}
			m_Velocity.x = m_TempVelocity.x;
		}
		else m_Velocity.x = 0.0f;

		break;

	case vertical:
		m_Velocity.x = 0.0f;

		if (powerUp == m_Type)
		{
			if ((m_BoundaryMin.y >= leftBottom.y && m_TempVelocity.y < 0) || (m_BoundaryMax.y <= rightBottom.y && m_TempVelocity.y > 0))
			{
				m_TempVelocity.y = -m_TempVelocity.y;
			}
			m_Velocity.y = m_TempVelocity.y;
		}
		else m_Velocity.y = 0.0f;

		break;

	case disapairing:
		m_Velocity = { 0.0f, 0.0f };
		if (powerUp == m_Type) m_IsActive = true;
		else m_IsActive = false;
	}

	m_Shape.bottom += m_Velocity.y * elapsedSec;
	m_Shape.left += m_Velocity.x * elapsedSec;
}

void ColorPlatform::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	if (!m_IsActive && m_PlatformType == disapairing) return;

	Rectf rect{ actorShape.left, actorShape.bottom, actorShape.width, -10 };
	if (actorVelocity.y <= 0.0f)
	{
		if (utils::IsOverlapping(rect, m_Shape))
		{
			actorVelocity.y = 0.0f;
			actorShape.bottom = m_Shape.bottom + m_Shape.height;
		}
	}
}

bool ColorPlatform::IsOnGround(const Rectf& actorShape, const Vector2f actorVelocity) const
{
	if (!m_IsActive && m_PlatformType == disapairing) return false;

	Rectf rect{ actorShape.left, actorShape.bottom-20, actorShape.width, actorShape.height };
	if (actorVelocity.y <= 0.0f && utils::IsOverlapping(rect, m_Shape)) return true;
	return false;

}

Vector2f ColorPlatform::GetVelocity() const
{
	return m_Velocity;
}

void ColorPlatform::Reset()
{
	m_Shape = { m_BoundaryMax.x - m_BoundaryMin.x / 2, m_BoundaryMax.y - m_BoundaryMin.y / 2, m_TexPlatforms.GetWidth() / 4, m_TexPlatforms.GetHeight() };
}