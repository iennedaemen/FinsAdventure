#include "stdafx.h"
#include "Spikes.h"
#include "utils.h"


Spikes::Spikes(Point2f pos)
	: m_Texture{"Resources/Spikes.png"}
{
	m_Shape = { pos.x, pos.y, m_Texture.GetWidth(), m_Texture.GetHeight() };
}

void Spikes::Draw() const
{
	m_Texture.Draw(m_Shape);
}

void Spikes::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const
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

bool Spikes::IsOnSpikes(const Rectf& actorShape, const Vector2f actorVelocity) const
{
	if (utils::IsOverlapping(actorShape, m_Shape)) return true;
	return false;
}
