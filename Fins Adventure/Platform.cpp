#include "stdafx.h"
#include "Platform.h"
#include "utils.h"

Platform::Platform(const Point2f& bottomLeft, PlatformSize size)
	: m_TextureBig{"Resources/Platform_Big.png"}
	, m_TextureSmall{ "Resources/Platform_Small.png" }
	, m_Size{size}
{
	switch (m_Size)
	{
	case PlatformSize::big:
		m_Shape = { bottomLeft.x, bottomLeft.y, m_TextureBig.GetWidth(), m_TextureBig.GetHeight() };
		break;
	case PlatformSize::small:
		m_Shape = { bottomLeft.x, bottomLeft.y, m_TextureSmall.GetWidth(), m_TextureSmall.GetHeight() };
		break;
	}
	
}

void Platform::Draw() const
{
	m_TextureBig.Draw(m_Shape);
}

void Platform::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
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

bool Platform::IsOnGround(const Rectf& actorShape, const Vector2f actorVelocity) const
{
	Rectf rect{ actorShape.left, actorShape.bottom, actorShape.width, -10 };
	if (actorVelocity.y <= 0.0f && utils::IsOverlapping(rect, m_Shape)) return true;
	return false;

}